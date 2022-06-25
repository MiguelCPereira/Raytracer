#include "Scene.h"

#include <iostream>

#include "Geometry.h"
#include "Camera.h"
#include "BaseLight.h"

Scene::Scene()
	: m_Geometry()
	, m_Cameras()
	, m_Lights()
	, m_CurrentCameraIdx(0)
{
}

Scene::~Scene()
{
	ClearGeometry();
	ClearCameras();
	ClearLights();
}


void Scene::AddGeometry(Geometry* newGeometry)
{
	m_Geometry.push_back(newGeometry);
}

void Scene::ClearGeometry()
{
	for (auto* geometry : m_Geometry)
		delete geometry;
	
	m_Geometry.clear();
}

void Scene::AddCamera(Camera* newCamera)
{
	m_Cameras.push_back(newCamera);
}

void Scene::ClearCameras()
{
	for (auto* camera : m_Cameras)
		delete camera;

	m_Cameras.clear();
}


void Scene::ChangeCamera(bool changeToNextCam)
{
	if (changeToNextCam)
	{
		if (m_CurrentCameraIdx + 1 >= m_Cameras.size())
			m_CurrentCameraIdx = 0;
		else
			m_CurrentCameraIdx++;
	}
	else
	{
		if (m_CurrentCameraIdx <= 0)
			m_CurrentCameraIdx = m_Cameras.size() - 1;
		else
			m_CurrentCameraIdx--;
	}
}

void Scene::AddLight(BaseLight* newLight)
{
	//std::cout << "Light added\n";
	m_Lights.push_back(newLight);
}

void Scene::ClearLights()
{
	//std::cout << "Clear lights called\n";
	for (auto* light : m_Lights)
		delete light;

	m_Lights.clear();
}

void Scene::Update(float elapsedTime, float totalTime)
{
	for (auto* geometry : m_Geometry)
		geometry->Update(elapsedTime, totalTime);;
}








