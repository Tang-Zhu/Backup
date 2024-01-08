#include "Generator.h"
#include <igl/voxel_grid.h>
#include <igl/copyleft/cgal/SelfIntersectMesh.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <filesystem>
#include <functional>
#include <future>
#include "TPMS.h"
#include "../Model/Model.h"
#include "../SDF/SDF.h"
#include "../Util/ProgressBar.hpp"
#include "../Secure/Validate.h"

namespace heat_ex
{
	template class Generator<float>;
	template class Generator<double>;

	template<class T>
	class Generator<T>::GeneratorImpl
	{
	public:
		static void Generate(const std::string& domainMesh, const std::string& IOMeshOne, const std::string& IOMeshTwo, 
					  const std::string& shellMeshOne, const std::string& shellMeshTwo, const T scale, const std::string& surfaceType, 
					  const int elementsNum,  const int padCount, const T period, const T thickness, igl::SignedDistanceType signedType, const bool modelCache,
					  const bool SDFCache, const bool show);
		static void Generate(const std::string& domainMesh, const std::string& TPMSMesh, const std::string& hotFluidMesh,
					const std::string& coldFluidMesh, const T scale, const std::string& surfaceType, const int elementsNum,
					const int padCount, const T period, const T thickness, igl::SignedDistanceType signedType, const bool modelCache,
					const bool SDFCache, const bool show);
		static void Generate(const std::string& domainMesh, const std::string& hotFluidMesh, const std::string& coldFluidMesh, const std::string & surfaceType, 
			const int elementsNum, const T elementSize, const T TPMSThickness, const T shellThickness, const T scale = 1., const int padCount = 2,
			igl::SignedDistanceType signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT, const bool modelCache = false, const bool SDFCache = false, const bool show = false);
		//~GeneratorImpl();

	private:
		static void MeshRepair(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces);
		static void MeshSelfIntersectionRepair(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces);
		static Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> MeshSelfIntersectionCheck(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces);

		GeneratorImpl() = delete;
		~GeneratorImpl() = delete;
		GeneratorImpl(const GeneratorImpl&) = delete;
		GeneratorImpl& operator = (const GeneratorImpl&) = delete;
	};

	//template <class T>
	//void Generator<T>::GeneratorImpl::Generate(const std::string& domainMesh, const std::string& IOMeshOne,
	//										   const std::string& IOMeshTwo, const std::string& shellMeshOne, 
	//										   const std::string& shellMeshTwo, const T scale, const std::string& surfaceType, 
	//										   const int elementsNum,  const int padCount, const T period, 
	//										   const T thickness, igl::SignedDistanceType signedType,
	//									       const bool modelCache, const bool SDFCache, const bool show)
	//{
	//	std::cout << "[.....] Starting..." << std::endl;

	//	std::cout << "[#....] Generating signed distance filed domain..." << std::endl;
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices;
	//	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces;
	//	model::Model<T> model(domainMesh, modelCache);
	//	vertices = model.GetVertices() / scale;
	//	faces = model.GetFaces();

	//	Eigen::RowVector<T, 3> shellMax;
	//	Eigen::RowVector<T, 3> shellMin;
	//	shellMax = vertices.colwise().maxCoeff();
	//	shellMin = vertices.colwise().minCoeff();
	//	Eigen::RowVector<T, 3> meshCenter = (shellMax + shellMin) / 2;

	//	Eigen::AlignedBox<T, 3> SDFDomain(shellMin, shellMax);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
	//	Eigen::RowVector3i volNums;
	//	igl::voxel_grid(SDFDomain, (shellMax - shellMin).maxCoeff()* period * elementsNum, padCount, hrNodes, volNums);

	//	/*T DX = (shellMax - shellMin)(0) / (std::round((shellMax - shellMin)(0) * period * elementsNum) - 1);
	//	T DY = (shellMax - shellMin)(1) / (std::round((shellMax - shellMin)(1) * period * elementsNum) - 1);
	//	T DZ = (shellMax - shellMin)(2) / (std::round((shellMax - shellMin)(2) * period * elementsNum) - 1);

	//	Eigen::RowVector<T, 3> nodeSteps(DX,DY,DZ);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
	//	Eigen::RowVector3i volNums;
	//	std::tie(hrNodes, volNums) = sdf::MeshGridFlatten(shellMin(0), shellMax(0), nodeSteps(0),
	//													shellMin(1), shellMax(1), nodeSteps(1),
	//													shellMin(2), shellMax(2), nodeSteps(2));*/

	//	std::cout << "[##...] Calculating signed distance filed..." << std::endl;
	//	Eigen::Vector<T, Eigen::Dynamic> domainMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	model = model::Model<T>(IOMeshOne, modelCache);
	//	vertices = model.GetVertices() / scale;
	//	faces = model.GetFaces();
	//	Eigen::Vector<T, Eigen::Dynamic> IOSDFOne = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	model = model::Model<T>(IOMeshTwo, modelCache);
	//	vertices = model.GetVertices() / scale;
	//	faces = model.GetFaces();
	//	Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();
	//
	//	model = model::Model<T>(shellMeshOne, modelCache);
	//	vertices = model.GetVertices() / scale;
	//	faces = model.GetFaces();
	//	Eigen::Vector<T, Eigen::Dynamic> shellSDFOne = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();
	//
	//	model = model::Model<T>(shellMeshTwo, modelCache);
	//	vertices = model.GetVertices() / scale;
	//	faces = model.GetFaces();
	//	Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	std::cout << "[###..] Filling with TPMS-" + surfaceType + " structure..." << std::endl;
	//	const T L = 100.;
	//	auto [temPhiOne, temPhiTwo, _, PhiStruct] = TPMS<T>::GenerateTPMS(surfaceType, L, hrNodes, meshCenter, period, thickness, IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);

	//	std::cout << "[####.] Saving results to \\Results folder..." << std::endl;
	//	const std::filesystem::path savePath = std::filesystem::current_path() / "Results";
	//	if (!std::filesystem::exists(savePath))
	//		std::filesystem::create_directories(savePath);
	//	const T isoValue = 0.;

	//	secure::validate::LicenseValidate::Validate();

	//	model::Model<T>::ExportFromSDF((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(PhiStruct), hrNodes, volNums, isoValue);
	//	model::Model<T>::ExportFromSDF((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(temPhiOne), hrNodes, volNums, isoValue);
	//	model::Model<T>::ExportFromSDF((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(temPhiTwo), hrNodes, volNums, isoValue);
	//	std::cout << "[#####] Accomplished!" << std::endl;
	//	if(show)
	//	{
	//		model::Model<T>((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
	//		model::Model<T>((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
	//		model::Model<T>((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
	//	}
	//	std::cout << "Press any key to exit...";
	//	std::cin.ignore();
	//	std::exit(0);
	//}

	template<class T>
	void Generator<T>::GeneratorImpl::Generate(const std::string& domainMesh, const std::string& TPMSMesh,
		const std::string& hotFluidMesh, const std::string& coldFluidMesh, const T scale,
		const std::string& surfaceType, const int elementsNum, const int padCount, const T period,
		const T thickness, igl::SignedDistanceType signedType, const bool modelCache, const bool SDFCache, const bool show)
	{
		std::cout << "[.....] Starting..." << std::endl;
		
		std::cout << "[#....] Generating signed distance filed domain..." << std::endl;
		model::Model<T> model(domainMesh, modelCache);
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices = model.GetVertices() / scale;
		Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces = model.GetFaces();

		Eigen::RowVector<T, 3> shellMax;
		Eigen::RowVector<T, 3> shellMin;
		shellMax = vertices.colwise().maxCoeff();
		shellMin = vertices.colwise().minCoeff();
		Eigen::RowVector<T, 3> meshCenter = (shellMax + shellMin) / 2;
				
		Eigen::AlignedBox<T, 3> SDFDomain(shellMin, shellMax);
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
		Eigen::RowVector3i volNums;
		igl::voxel_grid(SDFDomain, (shellMax - shellMin).maxCoeff() * period * elementsNum, padCount, hrNodes, volNums);
		/*T DX = (shellMax - shellMin)(0) / (std::round((shellMax - shellMin)(0) * period * elementsNum) - 1);
		T DY = (shellMax - shellMin)(1) / (std::round((shellMax - shellMin)(1) * period * elementsNum) - 1);
		T DZ = (shellMax - shellMin)(2) / (std::round((shellMax - shellMin)(2) * period * elementsNum) - 1);
		
		Eigen::RowVector<T, 3> nodeSteps(DX,DY,DZ);
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
		Eigen::RowVector3i volNums;
		std::tie(hrNodes, volNums) = sdf::MeshGridFlatten(shellMin(0), shellMax(0), nodeSteps(0),
														shellMin(1), shellMax(1), nodeSteps(1),
														shellMin(2), shellMax(2), nodeSteps(2));*/


		//Eigen::Vector<T, Eigen::Dynamic> domainMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

		//model = model::Model<T>(TPMSMesh, modelCache);
		//vertices = model.GetVertices() / scale;
		//faces = model.GetFaces();
		//Eigen::Vector<T, Eigen::Dynamic> TPMSMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

		//model = model::Model<T>(hotFluidMesh, modelCache);
		//vertices = model.GetVertices() / scale;
		//faces = model.GetFaces();
		//Eigen::Vector<T, Eigen::Dynamic> hotFluidMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

		//model = model::Model<T>(coldFluidMesh, modelCache);
		//vertices = model.GetVertices() / scale;
		//faces = model.GetFaces();
		//Eigen::Vector<T, Eigen::Dynamic> coldFluidMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

		std::cout << "[##...] Calculating signed distance filed..." << std::endl;
		std::future<Eigen::Vector<T, Eigen::Dynamic>> domainMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
			{
				return -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();
			});
		std::future<Eigen::Vector<T, Eigen::Dynamic>>TPMSMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
			{
				model::Model<T> model(TPMSMesh, modelCache);
				return -sdf::SDF<T>(hrNodes, model.GetVertices() / scale, model.GetFaces(), signedType, SDFCache).GetSmallestSignedDistances();
			});

		std::future<Eigen::Vector<T, Eigen::Dynamic>>hotFluidMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
			{
				model::Model<T> model(hotFluidMesh, modelCache);
				return -sdf::SDF<T>(hrNodes, model.GetVertices() / scale, model.GetFaces(), signedType, SDFCache).GetSmallestSignedDistances();

			});

		std::future<Eigen::Vector<T, Eigen::Dynamic>>coldFluidMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
			{
				model::Model<T> model(coldFluidMesh, modelCache);
				return -sdf::SDF<T>(hrNodes, model.GetVertices() / scale, model.GetFaces(), signedType, SDFCache).GetSmallestSignedDistances();
			});

		Eigen::Vector<T, Eigen::Dynamic> domainMeshSDF = domainMeshSDFFuture.get();
		Eigen::Vector<T, Eigen::Dynamic> TPMSMeshSDF = TPMSMeshSDFFuture.get();
		Eigen::Vector<T, Eigen::Dynamic>  hotFluidMeshSDF = hotFluidMeshSDFFuture.get();
		Eigen::Vector<T, Eigen::Dynamic> coldFluidMeshSDF = coldFluidMeshSDFFuture.get();

	
	
		std::cout << "[###..] Filling with TPMS-" + surfaceType + " structure..." << std::endl;
		Eigen::Vector<T, Eigen::Dynamic> IOSDFOne = hotFluidMeshSDF.cwiseMin(-TPMSMeshSDF);
		
		Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo = coldFluidMeshSDF.cwiseMin(-TPMSMeshSDF);


		//std::ofstream file1("domain.dat");
		//file1.write(reinterpret_cast<const char*>(domainMeshSDF.data()), domainMeshSDF.size() * sizeof(double));
		//file1.close();
		//std::ofstream file2("one.dat");
		//file2.write(reinterpret_cast<const char*>(IOSDFOne.data()), IOSDFOne.size() * sizeof(double));
		//file2.close();
		//std::ofstream file3("two.dat");
		//file3.write(reinterpret_cast<const char*>(IOSDFTwo.data()), IOSDFTwo.size() * sizeof(double));
		//file3.close();
		//std::ofstream file4("hot.dat");
		//file4.write(reinterpret_cast<const char*>(hotFluidMeshSDF.data()), hotFluidMeshSDF.size() * sizeof(double));
		//file4.close();
		//std::ofstream file5("cold.dat");
		//file5.write(reinterpret_cast<const char*>(coldFluidMeshSDF.data()), coldFluidMeshSDF.size() * sizeof(double));
		//file5.close();
		//std::ofstream file6("hrnode.dat");
		//file6.write(reinterpret_cast<const char*>(hrNodes.data()), hrNodes.size() * sizeof(double));
		//file6.close();
		//std::ofstream file7("center.dat");
		//file7.write(reinterpret_cast<const char*>(meshCenter.data()), meshCenter.size() * sizeof(double));
		//file7.close();

		auto [temPhiOne, temPhiTwo, phiOnly, phiStruct] = TPMS<T>::GenerateTPMSModified(surfaceType, hrNodes, meshCenter, period, thickness, domainMeshSDF, IOSDFOne, IOSDFTwo, hotFluidMeshSDF, coldFluidMeshSDF);

		std::cout << "[####.] Saving results to .\\Results folder..." << std::endl;
		const std::filesystem::path savePath = std::filesystem::current_path() / "Results";
		if (!std::filesystem::exists(savePath))
			std::filesystem::create_directories(savePath);
		const T isoValue = 0.;

		//secure::validate::LicenseValidate::Validate();

		model::Model<T>::ExportFromSDF((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(phiStruct), hrNodes, volNums, isoValue);
		model::Model<T>::ExportFromSDF((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(temPhiOne), hrNodes, volNums, isoValue);
		model::Model<T>::ExportFromSDF((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string(), domainMeshSDF.cwiseMin(temPhiTwo), hrNodes, volNums, isoValue);
		std::cout << "[#####] Accomplished!" << std::endl;
		if (show)
		{
			model::Model<T>((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
			model::Model<T>((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
			model::Model<T>((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(period) + std::to_string(thickness) + ".stl")).string()).Show();
		}
		std::cout << "Press any key to exit...";
		std::cin.ignore();
		std::exit(0);
	}

	//template <class T>
	//void Generator<T>::GeneratorImpl::Generate(const std::string& domainMesh, const std::string& hotFluidMesh, 
	//	const std::string& coldFluidMesh, const std::string & surfaceType, const int elementsNum, const T elementSize, 
	//	const T TPMSThickness, const T shellThickness, const T scale, const int padCount, 
	//	igl::SignedDistanceType signedType, const bool modelCache, const bool SDFCache, const bool show)
	//{
	//	std::cout << "[......] Starting..." << std::endl;

	//	std::cout << "[#.....] Generating signed distance filed domain..." << std::endl;
	//	model::Model<T> model(domainMesh, modelCache);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices = model.GetVertices() / scale;
	//	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces = model.GetFaces();
	//
	//	Eigen::RowVector<T, 3> shellMax;
	//	Eigen::RowVector<T, 3> shellMin;
	//	shellMax = vertices.colwise().maxCoeff();
	//	shellMin = vertices.colwise().minCoeff();
	//	Eigen::RowVector<T, 3> meshCenter = (shellMax + shellMin) / 2;

	//	Eigen::AlignedBox<T, 3> SDFDomain(shellMin, shellMax);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
	//	Eigen::RowVector3i volNums;

	//	igl::voxel_grid(SDFDomain, (shellMax - shellMin).maxCoeff() * (1./ elementSize) * elementsNum, padCount, hrNodes, volNums);
	//	
	//	/*T DX = (shellMax - shellMin)(0) / (std::round((shellMax - shellMin)(0) * period * elementsNum) - 1);
	//	T DY = (shellMax - shellMin)(1) / (std::round((shellMax - shellMin)(1) * period * elementsNum) - 1);
	//	T DZ = (shellMax - shellMin)(2) / (std::round((shellMax - shellMin)(2) * period * elementsNum) - 1);

	//	Eigen::RowVector<T, 3> nodeSteps(DX,DY,DZ);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
	//	Eigen::RowVector3i volNums;
	//	std::tie(hrNodes, volNums) = sdf::MeshGridFlatten(shellMin(0), shellMax(0), nodeSteps(0),
	//													shellMin(1), shellMax(1), nodeSteps(1),
	//													shellMin(2), shellMax(2), nodeSteps(2));*/


	//	//Eigen::Vector<T, Eigen::Dynamic> domainMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	//model = model::Model<T>(TPMSMesh, modelCache);
	//	//vertices = model.GetVertices() / scale;
	//	//faces = model.GetFaces();
	//	//Eigen::Vector<T, Eigen::Dynamic> TPMSMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	//model = model::Model<T>(hotFluidMesh, modelCache);
	//	//vertices = model.GetVertices() / scale;
	//	//faces = model.GetFaces();
	//	//Eigen::Vector<T, Eigen::Dynamic> hotFluidMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	//model = model::Model<T>(coldFluidMesh, modelCache);
	//	//vertices = model.GetVertices() / scale;
	//	//faces = model.GetFaces();
	//	//Eigen::Vector<T, Eigen::Dynamic> coldFluidMeshSDF = -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();

	//	std::cout << "[##....] Calculating signed distance filed..." << std::endl;
	//	std::future<Eigen::Vector<T, Eigen::Dynamic>> domainMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
	//		{
	//			return -sdf::SDF<T>(hrNodes, vertices, faces, signedType, SDFCache).GetSmallestSignedDistances();
	//		});

	//	std::future<Eigen::Vector<T, Eigen::Dynamic>>hotFluidMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
	//		{
	//			model::Model<T> model(hotFluidMesh, modelCache);
	//			return -sdf::SDF<T>(hrNodes, model.GetVertices() / scale, model.GetFaces(), signedType, SDFCache).GetSmallestSignedDistances();

	//		});

	//	std::future<Eigen::Vector<T, Eigen::Dynamic>>coldFluidMeshSDFFuture = std::async(std::launch::deferred, [=]()->Eigen::Vector<T, Eigen::Dynamic>
	//		{
	//			model::Model<T> model(coldFluidMesh, modelCache);
	//			return -sdf::SDF<T>(hrNodes, model.GetVertices() / scale, model.GetFaces(), signedType, SDFCache).GetSmallestSignedDistances();
	//		});

	//	Eigen::Vector<T, Eigen::Dynamic> domainMeshSDF = domainMeshSDFFuture.get();
	//	Eigen::Vector<T, Eigen::Dynamic> hotFluidMeshSDF = hotFluidMeshSDFFuture.get();
	//	Eigen::Vector<T, Eigen::Dynamic> coldFluidMeshSDF = coldFluidMeshSDFFuture.get();

	//	Eigen::Vector<T, Eigen::Dynamic> TPMSMeshSDF = hotFluidMeshSDF.cwiseMin(coldFluidMeshSDF).array() + shellThickness;

	//	std::cout << "[###...] Filling with TPMS-" + surfaceType + " structure..." << std::endl;
	//	Eigen::Vector<T, Eigen::Dynamic> IOSDFOne = hotFluidMeshSDF.cwiseMin(-TPMSMeshSDF);

	//	Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo = coldFluidMeshSDF.cwiseMin(-TPMSMeshSDF);

	//	auto [temPhiOne, temPhiTwo, phiOnly, phiStruct] = TPMS<T>::GenerateTPMS(surfaceType, hrNodes, meshCenter, (1. / elementSize), TPMSThickness, domainMeshSDF, IOSDFOne, IOSDFTwo, hotFluidMeshSDF, coldFluidMeshSDF);

	//	std::cout << "[####..] Generating models..." << std::endl;

	//	const T isoValue = 0.;
	//	//Eigen::MatrixXi IF; 
	//	//Eigen::VectorXi J; 
	//	//Eigen::VectorXi IM;
	//	//Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedDomainV;
	//	//Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedDomainF;
	//	//std::tie(reconstructedDomainV, reconstructedDomainF) = model::Model<T>::GenerateISOSurfaces(domainMeshSDF.cwiseMin(phiStruct), hrNodes, volNums, isoValue);
	//	//igl::copyleft::cgal::remesh_self_intersections(reconstructedDomainV, reconstructedDomainF,
	//	//	igl::copyleft::cgal::RemeshSelfIntersectionsParam::RemeshSelfIntersectionsParam(true, false, true),
	//	//	reconstructedDomainV, reconstructedDomainF, IF, J, IM);

	//	//std::cout << IF.size() << std::endl;


	//	////igl::remove_duplicate_vertices(reconstructedDomainV, 0, reconstructedDomainV, IF, J);
	//	//igl::remove_unreferenced(reconstructedDomainV, reconstructedDomainF, reconstructedDomainV, reconstructedDomainF, IF);

	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedFluidOneV;
	//	Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedFluidOneF;
	//	std::tie(reconstructedFluidOneV, reconstructedFluidOneF) = model::Model<T>::GenerateISOSurfaces(-domainMeshSDF.cwiseMin(temPhiOne), hrNodes, volNums, isoValue);
	//	//igl::copyleft::cgal::remesh_self_intersections(reconstructedFluidOneV, reconstructedFluidOneF,
	//	//	igl::copyleft::cgal::RemeshSelfIntersectionsParam::RemeshSelfIntersectionsParam(true, false, true),
	//	//	reconstructedFluidOneV, reconstructedFluidOneF, IF, J, IM);
	//	//igl::remove_unreferenced(reconstructedFluidOneV, reconstructedFluidOneF, reconstructedFluidOneV, reconstructedFluidOneF, IF);

	//	igl::copyleft::cgal::mesh_boolean(vertices, faces, reconstructedFluidOneV, reconstructedFluidOneF,
	//		igl::MESH_BOOLEAN_TYPE_MINUS, vertices, faces);

	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedFluidTwoV;
	//	Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> reconstructedFluidTwoF;
	//	std::tie(reconstructedFluidTwoV, reconstructedFluidTwoF) = model::Model<T>::GenerateISOSurfaces(-domainMeshSDF.cwiseMin(temPhiTwo), hrNodes, volNums, isoValue);
	//	//igl::copyleft::cgal::remesh_self_intersections(reconstructedFluidTwoV, reconstructedFluidTwoF,
	//	//	igl::copyleft::cgal::RemeshSelfIntersectionsParam::RemeshSelfIntersectionsParam(false, false, false),
	//	//	reconstructedFluidTwoV, reconstructedFluidTwoF, IF, J, IM);
	//	//igl::remove_unreferenced(reconstructedFluidTwoV, reconstructedFluidTwoF, reconstructedFluidTwoV, reconstructedFluidTwoF, IF);

	//	igl::copyleft::cgal::mesh_boolean(vertices, faces, reconstructedFluidTwoV, reconstructedFluidTwoF,
	//		igl::MESH_BOOLEAN_TYPE_MINUS, vertices, faces);



	//	std::cout << "[#####.] Saving results to .\\Results folder..." << std::endl;
	//	const std::filesystem::path savePath = std::filesystem::current_path() / "Results";
	//	if (!std::filesystem::exists(savePath))
	//		std::filesystem::create_directories(savePath);

	//	//secure::validate::LicenseValidate::Validate();

	//	model::Model<T>::Export((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" + std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + ".stl")).string(), vertices, faces);
	//	model::Model<T>::Export((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + "-.stl")).string(), reconstructedFluidOneV, reconstructedFluidOneF);
	//	model::Model<T>::Export((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + "-.stl")).string(), reconstructedFluidTwoV, reconstructedFluidTwoF);
	//	 
	//	//model::Model<T>::ExportFromSDF((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + "-.stl")).string(), -domainMeshSDF.cwiseMin(phiStruct), hrNodes, volNums, isoValue);
	//	//model::Model<T>::ExportFromSDF((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + "-.stl")).string(), -domainMeshSDF.cwiseMin(temPhiOne), hrNodes, volNums, isoValue);
	//	//model::Model<T>::ExportFromSDF((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + "-.stl")).string(), -domainMeshSDF.cwiseMin(temPhiTwo), hrNodes, volNums, isoValue);
	//	std::cout << "[######] Accomplished!" << std::endl;
	//	if (show)
	//	{
	//		//model::Model<T>((savePath / ("Struct_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + ".stl")).string()).Show();
	//		//model::Model<T>((savePath / ("FluidOne_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + ".stl")).string()).Show();
	//		//model::Model<T>((savePath / ("FluidTwo_" + surfaceType + "_" + std::to_string(elementsNum) + "_" +  std::to_string(elementSize) + "_" + std::to_string(TPMSThickness) + "_" + std::to_string(shellThickness) + ".stl")).string()).Show();
	//	}
	//	std::cout << "Press any key to exit...";
	//	std::cin.ignore();
	//	std::exit(0);
	//}

	////template<class T>
	////Generator<T>::GeneratorImpl::~GeneratorImpl() = default;

	//template<class T>
	//void Generator<T>::Generate(const std::string& domainMesh, const std::string& IOMeshOne, const std::string& IOMeshTwo,
	//						const std::string& shellMeshOne, const std::string& shellMeshTwo, const T scale, const std::string& surfaceType,
	//						const int elementsNum, const int padCount, const T period, const T thickness, igl::SignedDistanceType signedType,
	//						const bool modelCache,	const bool SDFCache, const bool show)
	//{
	//	try
	//	{
	//		if (surfaceType != "P" && surfaceType != "G" && surfaceType != "D" && surfaceType != "IWP")
	//		{
	//			std::cerr << "[Error] Unsupport TPMS type!" << std::endl;
	//			exit(EXIT_FAILURE);
	//		}
	//		GeneratorImpl::Generate(domainMesh, IOMeshOne, IOMeshTwo, shellMeshOne, shellMeshTwo, scale, surfaceType, 
	//								elementsNum, padCount, period, thickness, signedType, modelCache, SDFCache, show);
	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << e.what() << std::endl;
	//		std::cerr << "[Error] Heat-EX generated error!" << std::endl;
	//		exit(EXIT_FAILURE);
	//	}
	//}

	template<class T>
	void Generator<T>::Generate(const std::string& domainMesh, const std::string& TPMSMesh, const std::string& hotFluidMesh,
							const std::string& coldFluidMesh, const T scale, const std::string& surfaceType, const int elementsNum,
							const int padCount, const T period, const T thickness, igl::SignedDistanceType signedType, const bool modelCache,
							const bool SDFCache, const bool show)
	{
		try
		{
			if (surfaceType != "P" && surfaceType != "G" && surfaceType != "D" && surfaceType != "IWP")
			{
				std::cerr << "[Error] Unsupport TPMS type!" << std::endl;
				exit(EXIT_FAILURE);
			}
			GeneratorImpl::Generate(domainMesh, TPMSMesh, hotFluidMesh, coldFluidMesh, scale, surfaceType,
									elementsNum, padCount, period, thickness, signedType, modelCache, SDFCache, show);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "[Error] Heat-EX generated error!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	//template<class T>
	//void Generator<T>::Generate(const std::string& domainMesh, const std::string& hotFluidMesh,
	//	const std::string& coldFluidMesh, const std::string & surfaceType, const int elementsNum, const T elementSize,
	//	const T TPMSThickness, const T shellThickness, const T scale, const int padCount, igl::SignedDistanceType signedType,
	//	const bool modelCache, const bool SDFCache, const bool show)
	//{
	//	try
	//	{
	//		if (surfaceType != "P" && surfaceType != "G" && surfaceType != "D" && surfaceType != "IWP")
	//		{
	//			std::cerr << "[Error] Unsupport TPMS type!" << std::endl;
	//			exit(EXIT_FAILURE);
	//		}
	//		GeneratorImpl::Generate(domainMesh, hotFluidMesh, coldFluidMesh, surfaceType, elementsNum, elementSize, 
	//					TPMSThickness, shellThickness, scale, padCount, signedType, modelCache, SDFCache, show);
	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << e.what() << std::endl;
	//		std::cerr << "[Error] Heat-EX generated error!" << std::endl;
	//		exit(EXIT_FAILURE);
	//	}
	//}

	////template<class T>
	////Generator<T>::~Generator() = default;
}