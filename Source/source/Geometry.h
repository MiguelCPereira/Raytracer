#pragma once
#include "ExtraStructs.h"

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

	Geometry(const Geometry& other) = delete;
	Geometry(Geometry&& other) noexcept = delete;
	Geometry& operator=(const Geometry& other) = delete;
	Geometry& operator=(Geometry&& other) noexcept = delete;
	
	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;

	virtual void Update(float elapsedTime, float totalTime){}
};

