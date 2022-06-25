#include "Plane.h"

Plane::Plane(BaseMaterial* material, const Elite::FPoint3& center, const Elite::FVector3& normal)
	: m_Material(material)
	, m_Center(center)
	, m_Normal(normal)
{
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// Calculate the distance between the ray origin and the collision point (if there is one)
	const float t = Dot(m_Center - ray.origin, m_Normal) / Dot(m_Normal, ray.direction);

	// If there's a hit
	if (t > ray.tMin && t < ray.tMax)
	{
		hitRecord.point = ray.origin + t * ray.direction; // Calculate the pos of the actual hit by adding the distance to the origin, in the direction of the ray
		hitRecord.tValue = t;
		hitRecord.didItHit = true;
		hitRecord.normal = m_Normal;
		hitRecord.material = m_Material;
		return true;
	}

	hitRecord.didItHit = false;
	return false;
}


