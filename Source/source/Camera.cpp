#include "Camera.h"

Camera::Camera(const Elite::FPoint3& position, float fovAngle)
	: m_Position(position)
	, m_FovAngle(fovAngle)
	, m_Forward(0.f, 0.f, -1.f)
	, m_InitialPosition(position)
	, m_InitialFovAngle(fovAngle)
	, m_InitialForward(0.f, 0.f, -1.f )
{
}

Elite::FMatrix4 Camera::LookAt() const
{	// Calculate the 'right' and 'up' vectors
	Elite::FVector3 right = GetNormalized(Cross(Elite::FVector3{ 0.f, 1.f, 0.f }, m_Forward));
	Elite::FVector3 up = GetNormalized(Cross(m_Forward, right));

	// Create the ONB
	Elite::FMatrix4 lookAtMatrix{};
	lookAtMatrix[0][0] = right.x;
	lookAtMatrix[0][1] = right.y;
	lookAtMatrix[0][2] = right.z;
	lookAtMatrix[0][3] = 0.f;
	lookAtMatrix[1][0] = up.x;
	lookAtMatrix[1][1] = up.y;
	lookAtMatrix[1][2] = up.z;
	lookAtMatrix[1][3] = 0.f;
	lookAtMatrix[2][0] = m_Forward.x;
	lookAtMatrix[2][1] = m_Forward.y;
	lookAtMatrix[2][2] = m_Forward.z;
	lookAtMatrix[2][3] = 0.f;
	lookAtMatrix[3][0] = m_Position.x;
	lookAtMatrix[3][1] = m_Position.y;
	lookAtMatrix[3][2] = m_Position.z;
	lookAtMatrix[3][3] = 1.f;

	return lookAtMatrix;
}

void Camera::RotateAroundXAxis(float angle) 
{
	Elite::FMatrix4 rotationMatrix;
	rotationMatrix[0][0] = 1.f;
	rotationMatrix[0][1] = 0.f;
	rotationMatrix[0][2] = 0.f;
	rotationMatrix[1][0] = 0.f;
	rotationMatrix[1][1] = cosf(angle);
	rotationMatrix[1][2] = sinf(angle);
	rotationMatrix[2][0] = 0.f;
	rotationMatrix[2][1] = -sinf(angle);
	rotationMatrix[2][2] = cosf(angle);

	const Elite::FVector4 rotatedForward = GetNormalized(rotationMatrix * Elite::FVector4(m_Forward.x, m_Forward.y, m_Forward.z, 1.f));
	m_Forward.x = rotatedForward.x;
	m_Forward.y = rotatedForward.y;
	m_Forward.z = rotatedForward.z;
}

void Camera::RotateAroundYAxis(float angle)
{
	Elite::FMatrix4 rotationMatrix;
	rotationMatrix[0][0] = cosf(angle);
	rotationMatrix[0][1] = 0.f;
	rotationMatrix[0][2] = -sinf(angle);
	rotationMatrix[1][0] = 0.f;
	rotationMatrix[1][1] = 1.f;
	rotationMatrix[1][2] = 0.f;
	rotationMatrix[2][0] = sinf(angle);
	rotationMatrix[2][1] = 0.f;
	rotationMatrix[2][2] = cosf(angle);
	
	const Elite::FVector4 rotatedForward = GetNormalized(rotationMatrix * Elite::FVector4(m_Forward.x, m_Forward.y, m_Forward.z, 1.f));
	m_Forward.x = rotatedForward.x;
	m_Forward.y = rotatedForward.y;
	m_Forward.z = rotatedForward.z;
}

void Camera::MoveAround(float moveAmount, int direction)
{
	Elite::FVector3 transformedMoveAmount{};

	switch (direction)
	{
	case 1:
		transformedMoveAmount = Cross(GetNormalized(m_Forward), Elite::FVector3(0.f, 1.f, 0.f)) * moveAmount;
		m_Position += transformedMoveAmount;
		break;
	case 2:
		transformedMoveAmount = -Cross(GetNormalized(m_Forward), Elite::FVector3(1.f, 0.f, 1.f)) * moveAmount;
		m_Position += transformedMoveAmount;
		break;
	case 3:
		transformedMoveAmount = -GetNormalized(m_Forward) * moveAmount;
		m_Position += transformedMoveAmount;
		break;
	default:
		break;
	}
}


void Camera::Reset()
{
	m_Position = m_InitialPosition;
	m_FovAngle = m_InitialFovAngle;
	m_Forward = m_InitialForward;
}



