#pragma once
#include <vector>

class BaseLight;
class Camera;
class Geometry;

class Scene
{
public:
	Scene();
	~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) noexcept = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) noexcept = delete;

	std::vector<Geometry*> GetGeometry() const { return m_Geometry; }
	void AddGeometry(Geometry* newGeometry);
	void ClearGeometry();

	void AddCamera(Camera* newCamera);
	void ClearCameras();
	void ChangeCamera(bool changeToNextCam = true);
	Camera* GetCurrentCamera() const { if (m_Cameras.empty() == false) { return m_Cameras[m_CurrentCameraIdx]; } return nullptr; }

	std::vector<BaseLight*> GetLights() const { return m_Lights; }
	void AddLight(BaseLight* newLight);
	void ClearLights();

	void Update(float elapsedTime, float totalTime);

private:
	std::vector<Geometry*> m_Geometry;
	std::vector <Camera*> m_Cameras;
	std::vector <BaseLight*> m_Lights;
	int m_CurrentCameraIdx;
};

