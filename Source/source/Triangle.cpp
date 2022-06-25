#include "Triangle.h"
#include <iostream>
#include <vector>

Triangle::Triangle(BaseMaterial* material, const Elite::FPoint3& vertex01, const Elite::FPoint3& vertex02, const Elite::FPoint3& vertex03, int cullMode, bool spinning)
	: m_Material(material)
	, m_CullMode(cullMode)
	, m_V0(vertex01)
	, m_V1(vertex02)
	, m_V2(vertex03)
	, m_Spinning(spinning)
	, m_RotationSpeed(E_PI / 11.f)
	, m_TriangleCenter((vertex01.x + vertex02.x + vertex03.x) / 3.f, (vertex01.y + vertex02.y + vertex03.y) / 3.f, (vertex01.z + vertex02.z + vertex03.z) / 3.f )
{
}


bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// Calculate the triangle edges
	const Elite::FVector3 edge1 = m_V1 - m_V0;
	const Elite::FVector3 edge2 = m_V2 - m_V0;

	// Calculate the triangle's normal
	const Elite::FVector3 normal = GetNormalized(Cross(edge1, edge2));
	const Elite::FVector3 q = Cross(ray.direction, edge2);
	const float a = Dot(edge1, q);

	// Return false if the ray and the triangle are (nearly) parallel
	if (abs(a) <= ray.tMin)
	{
		hitRecord.didItHit = false;
		return false;
	}

	// Return false according to culling mode
	if (m_CullMode != 2) //// If it's NoCull, jump this portion of code
	{
		if (Dot(normal, ray.direction) > 0) //// If the ray hits from the back
		{
			if (m_CullMode == 0) //// If it's BackFacing Mode, return false
			{
				hitRecord.didItHit = false;
				return false;
			}
		}
		else //// If the ray hits from the front
		{
			if (m_CullMode == 1) //// If it's FrontFacing Mode, return false
			{
				hitRecord.didItHit = false;
				return false;
			}
		}
	}

	const Elite::FVector3 s = (ray.origin - m_V0) / a;
	const Elite::FVector3 r = Cross(s, edge1);

	Elite::FVector3 intersectionPoint;
	intersectionPoint.x = Dot(s, q);
	intersectionPoint.y = Dot(r, ray.direction);
	intersectionPoint.z = 1.0f - intersectionPoint.x - intersectionPoint.y;

	// If the intersection is outside the triangle, return false
	if ((intersectionPoint.x < 0.0f) || (intersectionPoint.y < 0.0f) || (intersectionPoint.z < 0.0f))
	{
		hitRecord.didItHit = false;
		return false;
	}

	// And finally, we calculate the t distance
	float t = Dot(edge2, r);

	// And return a true hit if t is inside limits
	if (t > ray.tMin && t < ray.tMax)
	{
		hitRecord.point = Elite::FPoint3(ray.origin + ray.direction * t);
		hitRecord.tValue = t;
		hitRecord.didItHit = true;
		hitRecord.normal = GetNormalized(Cross(edge1, edge2));
		hitRecord.material = m_Material;
		return true;
	}
	hitRecord.didItHit = false;
	return false;
}

void Triangle::Update(float elapsedTime, float totalTime)
{
	if(m_Spinning)
	{
		// Calculate the rotation angle for the current frame
		// through the cosine function using totalTime
		float frameRotation = m_RotationSpeed * cosf(totalTime);
		
		// Build the rotation matrix around y axis
		Elite::FMatrix4 rotationMatrix;
		rotationMatrix[0][0] = cosf(frameRotation);
		rotationMatrix[0][1] = 0.f;
		rotationMatrix[0][2] = -sinf(frameRotation);
		rotationMatrix[0][3] = 0.f;
		rotationMatrix[1][0] = 0.f;
		rotationMatrix[1][1] = 1.f;
		rotationMatrix[1][2] = 0.f;
		rotationMatrix[1][3] = 0.f;
		rotationMatrix[2][0] = sinf(frameRotation);
		rotationMatrix[2][1] = 0.f;
		rotationMatrix[2][2] = cosf(frameRotation);
		rotationMatrix[2][3] = 0.f;
		rotationMatrix[3][0] = 0.f;
		rotationMatrix[3][1] = 0.f;
		rotationMatrix[3][2] = 0.f;
		rotationMatrix[3][3] = 1.f;

		// First translate the point to the origin
		m_V0 -= m_TriangleCenter;
		m_V1 -= m_TriangleCenter;
		m_V2 -= m_TriangleCenter;
		
		// Then rotate it
		Elite::FVector4 tV0 = rotationMatrix * Elite::FVector4(m_V0.x, m_V0.y, m_V0.z, 1.f);
		Elite::FVector4 tV1 = rotationMatrix * Elite::FVector4(m_V1.x, m_V1.y, m_V1.z, 1.f);
		Elite::FVector4 tV2 = rotationMatrix * Elite::FVector4(m_V2.x, m_V2.y, m_V2.z, 1.f);
		m_V0.x = tV0.x; m_V0.y = tV0.y; m_V0.z = tV0.z;
		m_V1.x = tV1.x; m_V1.y = tV1.y; m_V1.z = tV1.z;
		m_V2.x = tV2.x; m_V2.y = tV2.y; m_V2.z = tV2.z;

		// And finally translate it back
		m_V0 += m_TriangleCenter;
		m_V1 += m_TriangleCenter;
		m_V2 += m_TriangleCenter;
	}
}
