//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include <iostream>
#include "BaseLight.h"
#include "ERGBColor.h"
#include "Scene.h"
#include "Geometry.h"
#include "Camera.h"
#include "BaseMaterial.h"

Elite::Renderer::Renderer(SDL_Window * pWindow)
	: m_Futures()
	, m_Cores(std::thread::hardware_concurrency())
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

// The multithreading implementation is inspired by the following:
//https://medium.com/@phostershop/solving-multithreaded-raytracing-issues-with-c-11-7f018ecd76fa

void Elite::Renderer::Render(Scene* pScene, bool shadowsOn, int lightEquationTerms)
{
	SDL_LockSurface(m_pBackBuffer);

	// Get all the needed camera info
	Camera* pCamera = pScene->GetCurrentCamera();
	FMatrix4 cameraLookAt = pCamera->LookAt();
	const auto aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	const auto fovAngle = ToRadians(pCamera->GetFovAngle());
	const auto fov = tan(fovAngle / 2.f);

	// Get the lights
	auto lightsVec = pScene->GetLights();

	// Loop over all the pixels
	// And calculate the rendering of each one asynchronously
	for (std::size_t i(0); i < m_Cores; ++i)
	{
		m_Futures.emplace_back(std::async([=]()
			{
				for (std::size_t index(i); index < std::size_t(m_Height) * std::size_t(m_Width); index += m_Cores)
				{
					std::size_t c = index % m_Width;
					std::size_t r = index / m_Width;

					// Cast a ray from the origin (the camera position)
					// with a direction to the center of the intended pixel (calculated with the aspect ratio and lookAt)
					const auto screenSpaceOrigin = pCamera->GetPosition();
					const auto pixelWorldPos = FPoint4(-(2.f * (c + 0.5f) / m_Width - 1.f) * aspectRatio * fov,
						(1.f - 2.f * (r + 0.5f) / m_Height) * fov, 1.f, 1.f);
					const auto transfPixelWorldPos = cameraLookAt * pixelWorldPos;
					const FVector3 direction = GetNormalized(transfPixelWorldPos.xyz - pCamera->GetPosition());
					Ray ray{ screenSpaceOrigin, direction };
					
					RenderPixel(pScene, shadowsOn, lightEquationTerms, ray, direction, lightsVec, c, r);
				}
			}));
	}

	// Wait for each thread to be complete
	// And then clean the future vector
	for (auto& th : m_Futures)
		th.wait();
	m_Futures.clear();
	
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}
void Elite::Renderer::RenderPixel(Scene* pScene, bool shadowsOn, int lightEquationTerms, const Ray& ray, const Elite::FVector3& direction, const std::vector<BaseLight*>& lightsVec, size_t c, size_t r) const
{
	// Check each geometry object in the scene for hits with the ray
	// And save the hit record of the closest one
	auto sceneGeometry = pScene->GetGeometry();
	HitRecord closestObjHitRec{};
	bool objectFound = false;
	for (auto* object : sceneGeometry)
	{		
		HitRecord hitRecord{};

		// If the ray has hit an object
		if (object->Hit(ray, hitRecord))
		{
			if (objectFound == false) // If it's the first object, just take its color
			{
				objectFound = true;
				closestObjHitRec = hitRecord;
			}
			else // If an object has been hit before, only change the color if the new obj is closer to the camera
			{
				if (hitRecord.tValue < closestObjHitRec.tValue)
					closestObjHitRec = hitRecord;
			}
		}
	}

	// Set the final color to a preemptive black
	Elite::RGBColor finalColor{ 0,0,0 };

	// If an object has been hit, go over each light in the scene and calculate the actual pixel color
	if (objectFound)
	{
		for (const auto* light : lightsVec)
		{
			if (light->GetActive())
			{
				// Get a ray from the hitpoint to the light source
				Elite::FVector3 lightVector = light->GetPosition() - closestObjHitRec.point;
				Ray objLightRay{ closestObjHitRec.point, Elite::GetNormalized(lightVector), 0.01f, Magnitude(lightVector) };
				HitRecord shadowHitRecord{};

				// Check if any other object is standing between the hitpoint and the light
				bool pointShadowed = false;
				if (shadowsOn)
				{
					for (auto* otherObject : sceneGeometry)
					{
						if (otherObject->Hit(objLightRay, shadowHitRecord))
						{
							pointShadowed = true;
							break;
						}
					}
				}

				// If not, shade it
				if (pointShadowed == false)
				{
					auto observedArea = Dot(light->GetDirection(closestObjHitRec.point), closestObjHitRec.normal);
					if (observedArea <= 0.f)
					{
						continue;
					}

					auto radiance = light->Irradiance(closestObjHitRec.point);

					// lightEquationTerms = 1 means only irradiance is intended; = 2 means it's only the BRDF; and = 0 means it's both
					switch (lightEquationTerms)
					{
					case 0:
						finalColor += radiance * closestObjHitRec.material->Shade(closestObjHitRec, light->GetDirection(closestObjHitRec.point), -direction)
							* observedArea;
						break;
					case 1:
						finalColor += radiance * Elite::Clamp(Dot(light->GetDirection(closestObjHitRec.point), closestObjHitRec.normal), 0.f, 1.f);
						break;
					case 2:
						finalColor += closestObjHitRec.material->Shade(closestObjHitRec, light->GetDirection(closestObjHitRec.point), -direction) * observedArea;
						break;
					default:
						finalColor += radiance * closestObjHitRec.material->Shade(closestObjHitRec, light->GetDirection(closestObjHitRec.point), -direction) * observedArea;
						break;
					}
				}
			}
		}
	}

	// Cap the color value at 1.f
	finalColor.MaxToOne();

	// Apply the color to the BackBuffer
	m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
		static_cast<uint8_t>(finalColor.r * 255.f),
		static_cast<uint8_t>(finalColor.g * 255.f),
		static_cast<uint8_t>(finalColor.b * 255.f));
}
bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}