#pragma once
#include "EMath.h"
#include "Geometry.h"

class Sphere final : public Geometry
{
public:
	Sphere(BaseMaterial* material, const Elite::FPoint3& center, float radius);
	~Sphere() = default;

	Sphere(const Sphere& other) = delete;
	Sphere(Sphere&& other) = delete;
	Sphere& operator=(const Sphere& other) = delete;
	Sphere& operator=(Sphere&& other) = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

private:
	BaseMaterial* m_Material;
	const Elite::FPoint3 m_Center;
	const float m_Radius;
};