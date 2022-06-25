#pragma once
#include "BaseMaterial.h"

class CookTorranceMaterial final : public BaseMaterial
{
public:
	CookTorranceMaterial(const Elite::RGBColor& color, float roughness = 0.6f, bool metalness = true);
	~CookTorranceMaterial() = default;

	CookTorranceMaterial(const CookTorranceMaterial& other) = delete;
	CookTorranceMaterial(CookTorranceMaterial&& other) = delete;
	CookTorranceMaterial& operator=(const CookTorranceMaterial& other) = delete;
	CookTorranceMaterial& operator=(CookTorranceMaterial&& other) = delete;

	Elite::RGBColor GetColor() const override;

	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const override;

private:
	const Elite::RGBColor m_Albedo;
	const float m_Roughness;
	const bool m_Metalness;
};