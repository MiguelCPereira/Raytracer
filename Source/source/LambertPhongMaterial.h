#pragma once
#include "BaseMaterial.h"

class LambertPhongMaterial final : public BaseMaterial
{
public:
	LambertPhongMaterial(const Elite::RGBColor& color, float difuseReflectance = 1.f, float specularReflectance = 1.f, int phongExponent = 10);
	~LambertPhongMaterial() = default;

	LambertPhongMaterial(const LambertPhongMaterial& other) = delete;
	LambertPhongMaterial(LambertPhongMaterial&& other) = delete;
	LambertPhongMaterial& operator=(const LambertPhongMaterial& other) = delete;
	LambertPhongMaterial& operator=(LambertPhongMaterial&& other) = delete;

	Elite::RGBColor GetColor() const override;

	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const override;

private:
	const Elite::RGBColor m_DiffuseColor;
	const float m_DifuseReflectance;
	const float m_SpecularReflectance;
	const int m_PhongExponent;
};