#include "Sphere.h"

Sphere::Sphere(BaseMaterial* material, const Elite::FPoint3& center, float radius)
	: m_Material(material)
	, m_Center(center)
	, m_Radius(radius)
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// First calculate the vector between the origin of the ray and the center of the sphere
	const Elite::FVector3 raySphereCenterVec = m_Center - ray.origin;

	// Then, the distance between the ray origin and the perpendicular of said ray to the sphere center
	const float intercRayLength = Dot(raySphereCenterVec, ray.direction);

	// With said length, now calculate the distance between said interception and the actual sphere center
	// AKA, between the halfway point of the first and second hits in the sphere surface, and the sphere center
	const float halfwayIntercDist = Magnitude(Reject(raySphereCenterVec, ray.direction));

	// If the distance is equal to or less then the sphere radius, there was a hit
	if (halfwayIntercDist <= m_Radius)
	{
		// The distance between the hit points and the previously referred interception is easy to calculate now, considering the radius is given
		const float hitsHalfwayDist = sqrtf(m_Radius * m_Radius - halfwayIntercDist * halfwayIntercDist);

		// And now we only need to calculate the t value
		float t = intercRayLength - hitsHalfwayDist;

		if (t > ray.tMin && t < ray.tMax)
		{
			hitRecord.point = ray.origin + t * ray.direction;
			hitRecord.tValue = t;
			hitRecord.didItHit = true;
			hitRecord.normal = GetNormalized(Elite::FVector3(hitRecord.point - m_Center));
			hitRecord.material = m_Material;
			return true;
		}
	}

	hitRecord.didItHit = false;
	return false;
}


