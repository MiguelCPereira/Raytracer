#pragma once
#include "EMath.h"
#include "Geometry.h"

class Plane final : public Geometry
{
public:
	Plane(BaseMaterial* material, const Elite::FPoint3& center, const Elite::FVector3& normal);
	~Plane() = default;

	Plane(const Plane& other) = delete;
	Plane(Plane&& other) = delete;
	Plane& operator=(const Plane& other) = delete;
	Plane& operator=(Plane&& other) = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

private:
	BaseMaterial* m_Material;
	const Elite::FPoint3 m_Center;
	const Elite::FVector3 m_Normal;
};

