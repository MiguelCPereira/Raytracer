#pragma once
#include "EMath.h"
#include "ERGBColor.h"

class BaseLight
{
public:
	BaseLight();
	virtual ~BaseLight();

	BaseLight(const BaseLight& other) = delete;
	BaseLight(BaseLight&& other) noexcept = delete;
	BaseLight& operator=(const BaseLight& other) = delete;
	BaseLight& operator=(BaseLight&& other) noexcept = delete;

	virtual Elite::RGBColor Irradiance(const Elite::FPoint3& pointToShade) const = 0;
	virtual void SetActive(bool newActive) = 0;
	virtual bool GetActive() const = 0;

	virtual Elite::FVector3 GetDirection(const Elite::FPoint3& hitPoint) const = 0;

	virtual Elite::FPoint3 GetPosition() const = 0;
};
