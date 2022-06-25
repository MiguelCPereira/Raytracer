#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity)
	: m_Active(true)
	, m_Direction(direction)
	, m_DifuseColor(color)
	, m_Intensity(intensity)
{
}

Elite::RGBColor DirectionalLight::Irradiance(const Elite::FPoint3&) const
{
	// Since there's no area, just multiply the color by the intensity
	return m_DifuseColor * m_Intensity;
}

void DirectionalLight::SetActive(bool newActive)
{
	m_Active = newActive;
}

bool DirectionalLight::GetActive() const
{
	return m_Active;
}

Elite::FVector3 DirectionalLight::GetDirection(const Elite::FPoint3&) const
{
	return m_Direction;
}





