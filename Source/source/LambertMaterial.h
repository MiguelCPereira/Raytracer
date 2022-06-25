#pragma once
#include "BaseMaterial.h"

class LambertMaterial final : public BaseMaterial
{
public:
	LambertMaterial(const Elite::RGBColor& color, float difuseReflectance = 1.f);
	~LambertMaterial() = default;

	LambertMaterial(const LambertMaterial& other) = delete;
	LambertMaterial(LambertMaterial&& other) = delete;
	LambertMaterial& operator=(const LambertMaterial& other) = delete;
	LambertMaterial& operator=(LambertMaterial&& other) = delete;

	Elite::RGBColor GetColor() const override;

	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const override;

private:
	const Elite::RGBColor m_DiffuseColor;
	const float m_DifuseReflectance;
};
