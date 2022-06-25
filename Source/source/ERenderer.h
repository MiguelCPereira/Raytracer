/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include <future>
#include <thread>
#include <vector>
#include "EMath.h"

struct Ray;
class BaseLight;
class Scene;
struct SDL_Window;
struct SDL_Surface;

namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(Scene* pScene, bool shadowsOn, int lightEquationTerms); // lightEquationTerms -> 0 = both; 1 = only irradiance; 2 = only BRDF
		bool SaveBackbufferToImage() const;

		void RenderPixel(Scene* pScene, bool shadowsOn, int lightEquationTerms, const Ray& ray, const FVector3& direction, const std::vector<BaseLight*>& lightsVec, size_t c, size_t r) const;

	private:
		std::vector<std::future<void>> m_Futures;
		std::size_t m_Cores;
		SDL_Window* m_pWindow = nullptr;
		SDL_Surface* m_pFrontBuffer = nullptr;
		SDL_Surface* m_pBackBuffer = nullptr;
		uint32_t* m_pBackBufferPixels = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}

#endif