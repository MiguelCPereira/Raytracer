#include "PointLight.h"

PointLight::PointLight(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity)
	: m_Active(true)
	, m_Position(position)
	, m_DifuseColor(color)
	, m_Intensity(intensity)
{
}

Elite::RGBColor PointLight::Irradiance(const Elite::FPoint3& pointToShade) const
{
	// First, calculate the radius (from the position of the light to the point to shade)
	const float radiusSqr =SqrMagnitude(m_Position - pointToShade);

	// Then we calculate the final irradiance value
	const float finalIrr = m_Intensity / radiusSqr;

	// And finally, we just multiply the color by this value
	return m_DifuseColor * finalIrr;
}

void PointLight::SetActive(bool newActive)
{
	m_Active = newActive;
}

bool PointLight::GetActive() const
{
	return m_Active;
}

Elite::FVector3 PointLight::GetDirection(const Elite::FPoint3& hitPoint) const
{
	return GetNormalized(Elite::FVector3(m_Position - hitPoint));
}


