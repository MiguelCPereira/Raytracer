#pragma once
#include "BaseLight.h"

class DirectionalLight final : public BaseLight
{
public:
	DirectionalLight(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity);
	~DirectionalLight() = default;

	DirectionalLight(const DirectionalLight& other) = delete;
	DirectionalLight(DirectionalLight&& other) = delete;
	DirectionalLight& operator=(const DirectionalLight& other) = delete;
	DirectionalLight& operator=(DirectionalLight&& other) = delete;

	Elite::RGBColor Irradiance(const Elite::FPoint3& pointToShade) const override;
	void SetActive(bool newActive) override;
	bool GetActive() const override;

	Elite::FVector3 GetDirection(const Elite::FPoint3& hitPoint) const override;

	Elite::FPoint3 GetPosition() const override { return Elite::FPoint3{ 0.f,0.f,0.f }; }

private:
	bool m_Active;
	const Elite::FVector3 m_Direction;
	const Elite::RGBColor m_DifuseColor;
	const float m_Intensity;
};