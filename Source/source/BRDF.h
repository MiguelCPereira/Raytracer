#pragma once
#include "EMath.h"
#include "ERGBColor.h"

inline Elite::RGBColor LambertBRDF(const Elite::RGBColor& diffuseColor, float difuseReflectance)
{
	return diffuseColor * difuseReflectance / E_PI;
}

inline Elite::RGBColor PhongBRDF(float specularReflectance, int phongExponent, const Elite::FVector3& hitRecordNormal, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir)
{
	const Elite::FVector3 reflect = Reflect(incomingLightDir, hitRecordNormal);
	const auto specularReflection = specularReflectance * powf(std::max(0.0f, Dot(reflect, viewDir)), phongExponent);
	return { specularReflection, specularReflection, specularReflection };
}

inline Elite::RGBColor CookTorranceBRDF(const Elite::FVector3& hitRecordNormal, const Elite::FVector3& incomingLightDir, const Elite::FVector3& viewDir, float roughness, Elite::RGBColor reflectivity, Elite::RGBColor& fresnelRef)
{
	// Calculate fresnel
	Elite::FVector3 halfVector = viewDir + incomingLightDir;
	Normalize(halfVector);
	fresnelRef = reflectivity + (Elite::RGBColor{ 1.f, 1.f, 1.f } - reflectivity) * powf(1.f - Dot(halfVector, viewDir), 5.f);

	// Calculate geometry
	const float alpha = powf(roughness, 2.f);
	const float dirRemappedAlpha = powf(alpha + 1.f, 2.f) / 8.f;
	const float geometryView = std::max(Dot(hitRecordNormal, viewDir), 0.f) / (std::max(Dot(hitRecordNormal, viewDir), 0.f) * (1.f - dirRemappedAlpha) + dirRemappedAlpha);
	const float geometryLight = std::max(Dot(hitRecordNormal, incomingLightDir), 0.f) / (std::max(Dot(hitRecordNormal, incomingLightDir), 0.f) * (1.f - dirRemappedAlpha) + dirRemappedAlpha);
	const float geometry = geometryView * geometryLight;

	// Calculate normal distribution
	const float normalDistribution = powf(alpha, 2.f) / (E_PI *
		powf(powf(std::max(Dot(hitRecordNormal, halfVector), 0.f), 2.f) * (powf(alpha, 2) - 1.f) + 1.f, 2.f));

	return Elite::RGBColor{ fresnelRef * normalDistribution * geometry /
		(4.f * std::max(Dot(viewDir, hitRecordNormal), 0.f) * std::max(Dot(incomingLightDir, hitRecordNormal), 0.f)) };
}