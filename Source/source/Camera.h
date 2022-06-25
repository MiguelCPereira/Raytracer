#pragma once
#include "EMath.h"

class Camera
{
public:
	Camera(const Elite::FPoint3& position = { 0.f, 0.f, 0.f }, float fovAngle = 0);
	~Camera() = default;

	Camera(const Camera& other) = delete;
	Camera(Camera&& other) noexcept = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera& operator=(Camera&& other) noexcept = delete;

	Elite::FPoint3 GetPosition() const { return m_Position; }
	float GetFovAngle() const { return m_FovAngle; }
	void SetPosition(const Elite::FPoint3& position) { m_Position = position; }
	void SetFovAngle(float fovAngle) { m_FovAngle = fovAngle; }

	void RotateAroundXAxis(float angle);
	void RotateAroundYAxis(float angle);

	void MoveAround(float moveAmount, int direction); // 1 left/right, 2 up/down, 3 forward/backwards

	void Reset();
	Elite::FMatrix4 LookAt() const;
	

private:
	Elite::FPoint3 m_Position;
	float m_FovAngle;
	Elite::FVector3 m_Forward;
	const Elite::FPoint3 m_InitialPosition;
	const float m_InitialFovAngle;
	const Elite::FVector3 m_InitialForward;
};
