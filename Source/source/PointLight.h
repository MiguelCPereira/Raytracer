#pragma once
#include "BaseLight.h"

class PointLight final : public BaseLight
{
public:
	PointLight(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity);
	~PointLight() = default;

	PointLight(const PointLight& other) = delete;
	PointLight(PointLight&& other) = delete;
	PointLight& operator=(const PointLight& other) = delete;
	PointLight& operator=(PointLight&& other) = delete;

	Elite::RGBColor Irradiance(const Elite::FPoint3& pointToShade) const override;
	void SetActive(bool newActive) override;
	bool GetActive() const override;

	Elite::FVector3 GetDirection(const Elite::FPoint3& hitPoint) const override;

	Elite::FPoint3 GetPosition() const override { return m_Position; }

private:
	bool m_Active;
	const Elite::FPoint3 m_Position;
	const Elite::RGBColor m_DifuseColor;
	const float m_Intensity;
};
