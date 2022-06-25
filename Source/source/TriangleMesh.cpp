#include "TriangleMesh.h"
#include "Triangle.h"

TriangleMesh::TriangleMesh(BaseMaterial* material, const std::vector<Elite::FPoint3>& vertexBuffer, const std::vector<Elite::IPoint3>& idxBuffer, const Elite::FMatrix4& transformation)
	: m_Material(material)
	, m_VertexBuffer(vertexBuffer)
	, m_IdxBuffer(idxBuffer)
	, m_Transformation(transformation)
	, m_TriangleVector()
	, m_Bounds{}
{
	// Fill the triangle vector according to the buffers
	for (const Elite::IPoint3& face : m_IdxBuffer)
	{
		const Elite::FPoint3 v0 = m_VertexBuffer[face.x];
		const Elite::FPoint3 v1 = m_VertexBuffer[face.y];
		const Elite::FPoint3 v2 = m_VertexBuffer[face.z];
		const Elite::FPoint4 tV0 = m_Transformation * Elite::FPoint4(v0.x, v0.y, v0.z, 1.f);
		const Elite::FPoint4 tV1 = m_Transformation * Elite::FPoint4(v1.x, v1.y, v1.z, 1.f);
		const Elite::FPoint4 tV2 = m_Transformation * Elite::FPoint4(v2.x, v2.y, v2.z, 1.f);
		m_TriangleVector.push_back(new Triangle(material, Elite::FPoint3(tV0.x, tV0.y, tV0.z), 
			Elite::FPoint3(tV1.x, tV1.y, tV1.z), Elite::FPoint3(tV2.x, tV2.y, tV2.z), 0));
	}

	// Calculate the bounding box min and max values (for the AABB intersection in the hit)
	m_Bounds[0] = m_VertexBuffer[0];
	m_Bounds[1] = m_VertexBuffer[0];
	for(const auto& vertex : m_VertexBuffer)
	{
		if (vertex.x < m_Bounds[0].x) m_Bounds[0].x = vertex.x;
		if (vertex.x > m_Bounds[1].x) m_Bounds[1].x = vertex.x;
		if (vertex.y < m_Bounds[0].y) m_Bounds[0].y = vertex.y;
		if (vertex.y > m_Bounds[1].y) m_Bounds[1].y = vertex.y;
		if (vertex.z < m_Bounds[0].z) m_Bounds[0].z = vertex.z;
		if (vertex.z > m_Bounds[1].z) m_Bounds[1].z = vertex.z;
	}
}

TriangleMesh::~TriangleMesh()
{
	for (auto* triangle : m_TriangleVector)
	{
		delete triangle;
		triangle = nullptr;
	}
}


bool TriangleMesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// Rule out  some hit checks according to the intersection of the bounding box (AABB)
	// The following implementation is based on this documentation:
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

	//// Calculate the signs
	auto invDir = ray.direction;
	invDir.x = 1.f / invDir.x;
	invDir.y = 1.f / invDir.y;
	invDir.z = 1.f / invDir.z;
	int sign0 = invDir.x < 0.f;
	int sign1 = invDir.y < 0.f;
	int sign2 = invDir.z < 0.f;

	//// Calculate the points where the ray might intercept the planes of the bounding box in X and Y
	float tmin = (m_Bounds[sign0].x - ray.origin.x) * invDir.x;
	float tmax = (m_Bounds[1 - sign0].x - ray.origin.x) * invDir.x;
	float tymin = (m_Bounds[sign1].y - ray.origin.y) * invDir.y;
	float tymax = (m_Bounds[1 - sign1].y - ray.origin.y) * invDir.y;

	//// Return false if the hit is not inside the actual box
	if (tmin > tymax || tymin > tmax)
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	//// Calculate the points where the ray might intercept the planes of the bounding box in Z
	float tzmin = (m_Bounds[sign2].z - ray.origin.z) * invDir.z;
	float tzmax = (m_Bounds[1 - sign2].z - ray.origin.z) * invDir.z;

	//// Return false if the hit is not inside the actual box
	if (tmin > tzmax || tzmin > tmax)
		return false;


	// Go over every remaining triangle and check if there's a hit
	// If so, return the closest one
	
	bool atLeastOneHit = false;
	HitRecord closestHitRecord{};
	
	for(auto* triangle : m_TriangleVector)
	{
		if (triangle->Hit(ray, hitRecord))
		{
			if(atLeastOneHit)
			{
				if (hitRecord.tValue < closestHitRecord.tValue)
					closestHitRecord = hitRecord;
			}
			else
			{
				atLeastOneHit = true;
				closestHitRecord = hitRecord;
			}
		}
	}

	if(atLeastOneHit)
	{
		hitRecord = closestHitRecord;
		return true;
	}

	hitRecord.didItHit = false;
	return false;
}


