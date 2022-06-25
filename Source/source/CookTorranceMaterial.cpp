#include "CookTorranceMaterial.h"

CookTorranceMaterial::CookTorranceMaterial(const Elite::RGBColor& color, float roughness, bool metalness)
	: m_Albedo(color)
	, m_Roughness(roughness)
	, m_Metalness(metalness)
{
}

Elite::RGBColor CookTorranceMaterial::GetColor() const
{
	return m_Albedo;
}

Elite::RGBColor CookTorranceMaterial::Shade(const HitRecord& hitRecord, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir) const
{
	// Set up reflectivity
	Elite::RGBColor reflectivity{ 0.04f, 0.04f, 0.04f };
	if (m_Metalness) reflectivity = m_Albedo;

	// Calculate the CookTorranceBRDF
	Elite::RGBColor fresnel{};
	Elite::RGBColor cookTorranceBRDF = CookTorranceBRDF(hitRecord.normal, incomingLightDir, viewDir, m_Roughness, reflectivity, fresnel);

	// Calculate the diffuse
	auto kd = 1.f - fresnel.r;
	if (m_Metalness)
		kd = 0;
	const auto diffuse = LambertBRDF(m_Albedo, kd);

	// And return the sum of the 2
	return diffuse + cookTorranceBRDF;
}