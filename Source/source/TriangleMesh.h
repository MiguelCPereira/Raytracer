#pragma once
#include <vector>
#include "EMath.h"
#include "Geometry.h"

class Triangle;

class TriangleMesh final : public Geometry
{
public:
	TriangleMesh(BaseMaterial* material, const std::vector<Elite::FPoint3>& vertexBuffer, const std::vector<Elite::IPoint3>& idxBuffer, const Elite::FMatrix4& transformation);
	~TriangleMesh();

	TriangleMesh(const  TriangleMesh& other) = delete;
	TriangleMesh(TriangleMesh&& other) = delete;
	TriangleMesh& operator=(const  TriangleMesh& other) = delete;
	TriangleMesh& operator=(TriangleMesh&& other) = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

private:
	BaseMaterial* m_Material;
	const std::vector<Elite::FPoint3> m_VertexBuffer;
	const std::vector<Elite::IPoint3> m_IdxBuffer;
	const Elite::FMatrix4 m_Transformation;
	std::vector<Triangle*> m_TriangleVector;
	Elite::FPoint3 m_Bounds[2];
	
};
