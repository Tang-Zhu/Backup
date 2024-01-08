#pragma once
#include <memory>
#include <string>
#include <igl/signed_distance.h>

namespace heat_ex
{
	template<class T = float>
	class Generator
	{
	public:
		static void Generate(const std::string& domainMesh, const std::string& IOMeshOne, const std::string& IOMeshTwo,
				const std::string& shellMeshOne, const std::string& shellMeshTwo, const T scale = 0., 
				const std::string& surfaceType = "P",	const int elementsNum = 2, const int padCount = 2,
				const T period = 40, const T thickness = 0.001, igl::SignedDistanceType signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT,
				const bool modelCache = false, const bool SDFCache = false, const bool show = false);
		static void Generate(const std::string& domainMesh, const std::string& TPMSMesh, const std::string& hotFluidMesh,
				const std::string& coldFluidMesh, const T scale = 0., const std::string& surfaceType = "P",
				const int elementsNum = 2, const int padCount = 2, const T period = 40, const T thickness = 0.001, 
				igl::SignedDistanceType signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT,
				const bool modelCache = false, const bool SDFCache = false, const bool show = false);
		static void Generate(const std::string& domainMesh, const std::string& hotFluidMesh, const std::string& coldFluidMesh, 
				const std::string & surfaceType, const int elementsNum, const T elementSize, const T TPMSThickness, 
				const T shellThickness = TPMSThickness, const T scale = 0., const int padCount = 2,
				igl::SignedDistanceType signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT, const bool modelCache = false,
				const bool SDFCache = false, const bool show = false);
		//~Generator();
	private:
		class GeneratorImpl;
		Generator() = delete;
		~Generator() = delete;
		Generator(const Generator&) = delete;
		Generator& operator = (const Generator&) = delete;
		//std::unique_ptr<GeneratorImpl> pImpl;
	};
}

