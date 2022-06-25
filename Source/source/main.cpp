//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include <string>

#include "ETimer.h"
#include "ERenderer.h"
#include "Plane.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"
#include "LambertMaterial.h"
#include "LambertPhongMaterial.h"
#include "CookTorranceMaterial.h"
#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include <fstream>
#include <sstream>

TriangleMesh* ParseOBJFile(BaseMaterial* material)
{
	const Elite::RGBColor color{ 242.f / 255.f, 248.f / 255.f, 187.f / 255.f };
	
	std::vector<Elite::FPoint3> vertexBuffer{};
	std::vector<Elite::IPoint3> indexBuffer{};
	Elite::FMatrix4 transformation;
	transformation[0][0] = 1.f;
	transformation[0][1] = 0.f;
	transformation[0][2] = 0.f;
	transformation[0][3] = 0.f;
	transformation[1][0] = 0.f;
	transformation[1][1] = 1.f;
	transformation[1][2] = 0.f;
	transformation[1][3] = 0.f;
	transformation[2][0] = 0.f;
	transformation[2][1] = 0.f;
	transformation[2][2] = 1.f;
	transformation[2][3] = 0.f;
	transformation[3][0] = 0.f;
	transformation[3][1] = 0.f;
	transformation[3][2] = 0.f;
	transformation[3][3] = 1.f;

	// Open the file
	std::ifstream in("lowpoly_bunny.obj", std::ios::in);
	if (!in)
	{
		std::cout << "An error occurred while opening the obj file." << std::endl;
		return new TriangleMesh(new LambertMaterial(color), vertexBuffer, indexBuffer, transformation);
	}

	// Go over every line
	std::string line;
	while (std::getline(in, line))
	{
		// Save the vertexes
		if (line[0] == 'v')
		{
			std::istringstream v(line.substr(2));
			float x, y, z;
			v >> x;
			v >> y;
			v >> z;
			Elite::FPoint3 vertex(x, y, z);
			//std::cout << " v - " << x << ", " << y << ", " << z << '\n';
			vertexBuffer.push_back(vertex);
		}

		// Save the faces
		else if (line[0] == 'f')
		{
			std::istringstream f(line.substr(2));
			int a, b, c;
			f >> a;
			f >> b;
			f >> c;
			//std::cout << " f - " << a << ", " << b << ", " << c << '\n';
			indexBuffer.push_back(Elite::IPoint3(a-1, b - 1, c - 1));
		}

	}	
	
	return new TriangleMesh(material, vertexBuffer, indexBuffer, transformation);
}

void InitializeGeometricScene(Scene* scene)
{
	scene->AddCamera(new Camera(Elite::FPoint3(0.f, 3.f, 9.f), 45));

	// Materials
	auto* roughMetal = new CookTorranceMaterial(Elite::RGBColor(0.972f, 0.96f, 0.915f), 1.f, true);
	auto* mediumMetal = new CookTorranceMaterial(Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.6f, true);
	auto* smoothMetal = new CookTorranceMaterial(Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.1f, true);
	auto* roughPlastic = new CookTorranceMaterial(Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.f, false);
	auto* mediumPlastic = new CookTorranceMaterial(Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.4f, false);
	auto* smoothPlastic = new CookTorranceMaterial(Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.1f, false);
	auto* lambertGreyBlue = new LambertMaterial(Elite::RGBColor{ 0.49f, 0.57f, 0.57f }, 1.f);
	auto* lambertWhite = new LambertMaterial(Elite::RGBColor{ 1.f, 1.f, 1.f }, 1.f);

	// Planes
	scene->AddGeometry(new Plane(lambertGreyBlue, Elite::FPoint3(0.f, 0.f, 0.f), Elite::FVector3(0.f, 1.f, 0.f)));
	scene->AddGeometry(new Plane(lambertGreyBlue, Elite::FPoint3(0.f, 0.f, -10.f), Elite::FVector3(0.f, 0.f, 1.f)));
	scene->AddGeometry(new Plane(lambertGreyBlue, Elite::FPoint3(0.f, 10.f, 0.f), Elite::FVector3(0.f, -1.f, 0.f)));
	scene->AddGeometry(new Plane(lambertGreyBlue, Elite::FPoint3(5.f, 0.f, 0.f), Elite::FVector3(-1.f, 0.f, 0.f)));
	scene->AddGeometry(new Plane(lambertGreyBlue, Elite::FPoint3(-5.f, 0.f, 0.f), Elite::FVector3(1.f, 0.f, 0.f)));

	// Spheres
	//scene->AddGeometry(new Sphere(new LambertPhongMaterial(Elite::RGBColor{ 1.f, 0.f, 0.f }) , Elite::FPoint3(-1.75f, 1.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(roughMetal, Elite::FPoint3(-1.75f, 1.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(mediumMetal, Elite::FPoint3(0.f, 1.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(smoothMetal, Elite::FPoint3(1.75f, 1.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(roughPlastic, Elite::FPoint3(-1.75f, 3.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(mediumPlastic, Elite::FPoint3(0.f, 3.f, 0.f), 0.75f));
	scene->AddGeometry(new Sphere(smoothPlastic, Elite::FPoint3(1.75f, 3.f, 0.f), 0.75f));

	// Triangles
	
	Elite::FPoint3 v1MS{ -0.75f, 1.5f, 0.f };
	Elite::FPoint3 v2MS{ -0.75f, 0.f, 0.f };
	Elite::FPoint3 v3MS{ 0.75f, 0.f, 0.f };
	scene->AddGeometry(new Triangle(lambertWhite, Elite::FPoint3{ -1.75f + v1MS.x, 4.5f + v1MS.y,  v1MS.z },
		Elite::FPoint3{ -1.75f + v2MS.x, 4.5f + v2MS.y,  v2MS.z }, Elite::FPoint3{ -1.75f + v3MS.x, 4.5f + v3MS.y,  v3MS.z }, 0, true));
	scene->AddGeometry(new Triangle(lambertWhite, Elite::FPoint3{ v1MS.x, 4.5f + v1MS.y,  v1MS.z },
		Elite::FPoint3{ v2MS.x, 4.5f + v2MS.y,  v2MS.z }, Elite::FPoint3{ v3MS.x, 4.5f + v3MS.y,  v3MS.z }, 1, true));
	scene->AddGeometry(new Triangle(lambertWhite, Elite::FPoint3{ 1.75f + v1MS.x, 4.5f + v1MS.y,  v1MS.z },
		Elite::FPoint3{ 1.75f + v2MS.x, 4.5f + v2MS.y,  v2MS.z }, Elite::FPoint3{ 1.75f + v3MS.x, 4.5f + v3MS.y,  v3MS.z }, 2, true));

	// Lights
	scene->AddLight(new PointLight(Elite::FPoint3(0.f, 5.f, -5.f), Elite::RGBColor(1.f, 0.61f, 0.45f), 50.f));
	scene->AddLight(new PointLight(Elite::FPoint3(-2.5f, 5.f, 5.f), Elite::RGBColor(1.f, 0.8f, 0.45f), 70.f));
	scene->AddLight(new PointLight(Elite::FPoint3(2.5f, 2.5f, 5.f), Elite::RGBColor(0.34f, 0.47f, 0.68f), 50.f));
}

void InitializeBunnyScene(Scene* scene)
{
	scene->AddCamera(new Camera(Elite::FPoint3(0.f, 3.f, 12.f), 45));

	// Materials
	auto* lambertWhite = new LambertMaterial(Elite::RGBColor{ 1.f, 1.f, 1.f }, 1.f);
	auto* lambertLemon = new LambertMaterial(Elite::RGBColor{ 1.f,250.f / 255.f,205.f / 255.f }, 1.f);

	// Planes
	scene->AddGeometry(new Plane(lambertWhite, Elite::FPoint3(0.f, 0.f, 0.f), Elite::FVector3(0.f, 1.f, 0.f)));
	scene->AddGeometry(new Plane(lambertWhite, Elite::FPoint3(0.f, 0.f, -6.f), Elite::FVector3(0.f, 0.f, 1.f)));
	scene->AddGeometry(new Plane(lambertWhite, Elite::FPoint3(0.f, 6.f, 0.f), Elite::FVector3(0.f, -1.f, 0.f)));
	scene->AddGeometry(new Plane(lambertWhite, Elite::FPoint3(5.f, 0.f, 0.f), Elite::FVector3(-1.f, 0.f, 0.f)));
	scene->AddGeometry(new Plane(lambertWhite, Elite::FPoint3(-5.f, 0.f, 0.f), Elite::FVector3(1.f, 0.f, 0.f)));

	// Bunny
	scene->AddGeometry(ParseOBJFile(lambertLemon));

	// Lights
	scene->AddLight(new PointLight(Elite::FPoint3(0.f, 3.f, -3.f), Elite::RGBColor(1.f, 196.f / 255.f, 207.f / 255.f), 20.f));
	scene->AddLight(new PointLight(Elite::FPoint3(0.f, 3.f, 4.f), Elite::RGBColor(1.f, 206.f / 255.f, 217.f / 255.f), 30.f));
}

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - Miguel Pereira",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	//Initialize scene
	auto* bunnyScene = new Scene();
	InitializeBunnyScene(bunnyScene);
	auto* geometricScene = new Scene();
	InitializeGeometricScene(geometricScene);
	auto* currentScene = geometricScene;

	//Print extra commands
	std::cout << "Extra Commands:\n\n";
	std::cout << "  SPACE -> Switch between scenes\n";
	std::cout << "  R -----> Restart the camera to its original position and rotation\n";
	std::cout << "  X -----> Take a screenshot\n\n";
	std::cout << "------------------------------------\n\n";
	
	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	bool shadowsOn = true;
	int lightEquationTerms = 0;

	// Set up the bools for the mouse buttons, as well as the movement and rotation amounts
	//const float posMovAmount = 0.5f;
	const float movementSpeed = 0.6f;
	const float rotationSpeed = E_PI / 640.f;
	bool leftMouseDown = false;
	bool rightMouseDown = false;
	
	while (isLooping)
	{
		// Get the Camera
		Camera* pCamera = currentScene->GetCurrentCamera();

		// Get the input events for non-continuous actions
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				// Take screenshot with X
				case SDLK_x:
					takeScreenshot = true;
					break;
				// Restart camera with R
				case SDLK_r:
					pCamera->Reset();
					break;
				// Change scene with SPACE
				case SDLK_SPACE:
					if (currentScene != bunnyScene)
						currentScene = bunnyScene;
					else
						currentScene = geometricScene;
					break;
				// Toggle shadows with Q
				case SDLK_q:
					shadowsOn = !shadowsOn;
					break;
				// Change light-equation terms with E
				case SDLK_e:
					if (lightEquationTerms + 1 > 2)
						lightEquationTerms = 0;
					else
						lightEquationTerms++;
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					leftMouseDown = true;
					break;
				case SDL_BUTTON_RIGHT:
					rightMouseDown = true;
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					leftMouseDown = false;
					break;
				case SDL_BUTTON_RIGHT:
					rightMouseDown = false;
					break;
				}
				break;
			default:
				break;
			}
		}

		// Get the keyboard and mouse state
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		int mouseX = 0, mouseY = 0;
		const auto mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

		// Get the keyboard input for continuous actions		
		//// Position Horizontal		
		if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
			pCamera->MoveAround(-movementSpeed, 1);
		if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
			pCamera->MoveAround(movementSpeed, 1);
		//// Position Vertical	
		if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
			pCamera->MoveAround(-movementSpeed, 3);
		if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
			pCamera->MoveAround(movementSpeed, 3);

		// Get the mouse input for continuous actions
		if (leftMouseDown && rightMouseDown)
		{
			//// Position Vertical
			if (mouseY > 0)
				pCamera->MoveAround(-movementSpeed, 2);
			else if (mouseY < 0)
				pCamera->MoveAround(movementSpeed, 2);
		}
		else if (rightMouseDown)
		{
			//// Horizontal Rotation
			if(mouseX != 0.f)
				pCamera->RotateAroundYAxis(-rotationSpeed * mouseX);

			//// Vertical Rotation
			if (mouseY != 0.f)
				pCamera->RotateAroundXAxis(-rotationSpeed * mouseY);
		}
		else if (leftMouseDown)
		{
			//// Horizontal Rotation
			if (mouseX != 0.f)
				pCamera->RotateAroundYAxis(-rotationSpeed * mouseX);

			//// Move Forward/Backwards
			if (mouseY > 0)
				pCamera->MoveAround(movementSpeed, 3);
			else if (mouseY < 0)
				pCamera->MoveAround(-movementSpeed, 3);
		}
		
		// Update Scene
		currentScene->Update(pTimer->GetElapsed(), pTimer->GetTotal());
		
		// Render
		pRenderer->Render(currentScene, shadowsOn, lightEquationTerms);

		// Timer
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
			//std::cout << "#lights:"<<scene->GetLights().size()<<std::endl;
		}

		// Save Screenshot After Full Render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}