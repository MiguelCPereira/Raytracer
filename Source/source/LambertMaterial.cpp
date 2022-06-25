#include "LambertMaterial.h"

LambertMaterial::LambertMaterial(const Elite::RGBColor& color, float difuseReflectance)
	: m_DiffuseColor(color)
	, m_DifuseReflectance(difuseReflectance)
{
}

Elite::RGBColor LambertMaterial::GetColor() const
{
	return m_DiffuseColor;
}

Elite::RGBColor LambertMaterial::Shade(const HitRecord&, const Elite::FVector3&, const Elite::FVector3&) const
{
	return LambertBRDF(m_DiffuseColor, m_DifuseReflectance);
}



