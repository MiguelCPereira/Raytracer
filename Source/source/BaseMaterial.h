#pragma once
#include "ExtraStructs.h"
#include "ERGBColor.h"
#include "BRDF.h"

class BaseMaterial
{
public:
	BaseMaterial();
	virtual ~BaseMaterial();

	BaseMaterial(const BaseMaterial& other) = delete;
	BaseMaterial(BaseMaterial&& other) noexcept = delete;
	BaseMaterial& operator=(const BaseMaterial& other) = delete;
	BaseMaterial& operator=(BaseMaterial&& other) noexcept = delete;
	
	virtual Elite::RGBColor GetColor() const = 0;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const = 0;
};

