#include "LambertPhongMaterial.h"

LambertPhongMaterial::LambertPhongMaterial(const Elite::RGBColor& color, float difuseReflectance, float specularReflectance, int phongExponent)
	: m_DiffuseColor(color)
	, m_DifuseReflectance(difuseReflectance)
	, m_SpecularReflectance(specularReflectance)
	, m_PhongExponent(phongExponent)
{
}

Elite::RGBColor LambertPhongMaterial::GetColor() const
{
	return m_DiffuseColor;
}

Elite::RGBColor LambertPhongMaterial::Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const
{
	return LambertBRDF(m_DiffuseColor, m_DifuseReflectance) + PhongBRDF(m_SpecularReflectance, m_PhongExponent, hitRecord.normal, -incomingLightDir, viewDir);
}
