#pragma once
#include "EMath.h"
#include "Geometry.h"

class Triangle final : public Geometry
{
public:
	Triangle(BaseMaterial* material, const Elite::FPoint3& vertex01, const Elite::FPoint3& vertex02, const Elite::FPoint3& vertex03, int cullMode = 2, bool spinning = false);
	~Triangle() = default;

	Triangle(const Triangle& other) = delete;
	Triangle(Triangle&& other) = delete;
	Triangle& operator=(const Triangle& other) = delete;
	Triangle& operator=(Triangle&& other) = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void Update(float elapsedTime, float totalTime) override;

private:
	BaseMaterial* m_Material;
	const int m_CullMode; // 0 is Back-Face, 1 is Front-Face, 2 is NoCulling
	Elite::FPoint3 m_V0;
	Elite::FPoint3 m_V1;
	Elite::FPoint3 m_V2;
	const bool m_Spinning;
	float m_RotationSpeed;
	const Elite::FVector3 m_TriangleCenter;
};