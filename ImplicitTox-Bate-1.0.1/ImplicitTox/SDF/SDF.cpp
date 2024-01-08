#include "SDF.h"
#include <filesystem>
#include <igl/per_vertex_normals.h>
#include <igl/per_edge_normals.h>
#include <igl/signed_distance.h>
#include <igl/WindingNumberAABB.h>
#include <cryptopp/blake2.h>
#include <cryptopp/hex.h>
#include <fstream>

namespace sdf {

	template class SDF<float>;
	template class SDF<double>;
	template std::tuple<Eigen::Tensor<float, 3, Eigen::RowMajor>, Eigen::Tensor<float, 3, Eigen::RowMajor>, Eigen::Tensor<float, 3, Eigen::RowMajor>>
		MeshGrid<float>(const float& XStart, const float& XEnd, const float& XStep,
			const float& YStart, const float& YEnd, const float& YStep,
			const float& ZStart, const float& ZEnd, const float& ZStep);
	template std::tuple<Eigen::Tensor<double, 3, Eigen::RowMajor>, Eigen::Tensor<double, 3, Eigen::RowMajor>, Eigen::Tensor<double, 3, Eigen::RowMajor>>
		MeshGrid<double>(const double& XStart, const double& XEnd, const double& XStep,
			const double& YStart, const double& YEnd, const double& YStep,
			const double& ZStart, const double& ZEnd, const double& ZStep);
	template std::tuple<Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::RowVector3i>
		MeshGridFlatten<float>(const float& XStart, const float& XEnd, const float& XStep,
			const float& YStart, const float& YEnd, const float& YStep,
			const float& ZStart, const float& ZEnd, const float& ZStep);
	template std::tuple<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::RowVector3i>
		MeshGridFlatten<double>(const double& XStart, const double& XEnd, const double& XStep,
			const double& YStart, const double& YEnd, const double& YStep,
			const double& ZStart, const double& ZEnd, const double& ZStep);

	template<class T>
	class SDF<T>::SDFImpl
	{
	public:
		SDFImpl(Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints, 
				Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
				const Eigen::Matrix<int,Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices,
				igl::SignedDistanceType signType, bool cache);

		~SDFImpl();

		Eigen::Vector<T, Eigen::Dynamic> GetSmallestSignedDistances() const;
		Eigen::VectorXi GetSmallestDistancesIndices() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetClosestPoints() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetClosestNormals() const;

		
		static const std::tuple<std::string, std::string, std::string, std::string>
			GenerateCacheSignature(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints,
									Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
									Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices);
	private:
		bool ImportFromCache(const std::string& cacheFile, const std::string& queryPointsSignature, 
							const std::string& meshVerticesSignature, const std::string& meshIndicesSignature);
		void CalculateSDF(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints,
							Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
							Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices,
							igl::SignedDistanceType signType);
		void ExportDataCache(const std::string& cacheFile, const std::string& queryPointsSignature,
			const std::string& meshVerticesSignature, const std::string& meshIndicesSignature) const;
		Eigen::Vector<T, Eigen::Dynamic> smallestSignedDistances;
		Eigen::VectorXi smallestDistancesIndices;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> closestPoints;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> closestNormals;
		Eigen::RowVector3i resolution;
		static std::filesystem::path cacheFolder;
	};

	template <class T>
	std::filesystem::path SDF<T>::SDFImpl::cacheFolder = std::filesystem::current_path() / "Cache" / "SDFs";

	template<class T>
	SDF<T>::SDFImpl::SDFImpl(Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints, 
							Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
							const Eigen::Matrix<int,Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices, 
							igl::SignedDistanceType signType, bool cache)
	{
		bool cacheExist = false;
		std::string cacheFile, queryPointsSignature, meshVerticesSignature, meshIndicesSignature;
		if (cache)
		{
			std::tie(cacheFile, queryPointsSignature, meshVerticesSignature, meshIndicesSignature) 
				= GenerateCacheSignature(queryPoints, meshVertices, meshIndices);

			cacheExist = ImportFromCache(cacheFile, queryPointsSignature, meshVerticesSignature, meshIndicesSignature);
		}

		if (!cache || !cacheExist)
		{
			CalculateSDF(queryPoints, meshVertices, meshIndices, signType);

			if (cache && !cacheExist)
				ExportDataCache(cacheFile, queryPointsSignature, meshVerticesSignature, meshIndicesSignature);
		}
	}

	template<class T>
	SDF<T>::SDFImpl::~SDFImpl() = default;

	template<class T>
	Eigen::Vector<T, Eigen::Dynamic> SDF<T>::SDFImpl::GetSmallestSignedDistances() const
	{
		return smallestSignedDistances;
	}

	template<class T>
	Eigen::VectorXi SDF<T>::SDFImpl::GetSmallestDistancesIndices() const
	{
		return smallestDistancesIndices;
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> SDF<T>::SDFImpl::GetClosestPoints() const
	{
		return closestPoints;
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> SDF<T>::SDFImpl::GetClosestNormals() const
	{
		return closestNormals;
	}

	template <class T>
	const std::tuple<std::string, std::string, std::string, std::string> SDF<T>::SDFImpl::
		GenerateCacheSignature(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints,
								Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices, 
								Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices)
	{
		CryptoPP::BLAKE2b hash;
		std::string queryPointsSignature;

		CryptoPP::ArraySource(reinterpret_cast<const CryptoPP::byte*>(queryPoints.data()),
			queryPoints.size() * sizeof(T), true,
			new CryptoPP::HashFilter(hash,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(queryPointsSignature))));

		std::string meshVerticesSignature;

		CryptoPP::ArraySource(reinterpret_cast<const CryptoPP::byte*>(meshVertices.data()),
			meshVertices.size() * sizeof(T), true,
			new CryptoPP::HashFilter(hash,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(meshVerticesSignature))));

		std::string meshIndicesSignature;

		CryptoPP::ArraySource(reinterpret_cast<const CryptoPP::byte*>(meshIndices.data()),
			meshIndices.size() * sizeof(int), true,
			new CryptoPP::HashFilter(hash,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(meshIndicesSignature))));

		std::string signature;
		const std::string modelType = typeid(T).name();
		CryptoPP::StringSource(modelType + queryPointsSignature + meshVerticesSignature + meshIndicesSignature, true,
			new CryptoPP::HashFilter(hash,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(signature))));

		if (signature == "")
			throw("[Error] Generate model cache signature faild!");

		std::string cacheFile = signature + ".dat";

		return std::make_tuple(cacheFile, queryPointsSignature, meshVerticesSignature, meshIndicesSignature);
	}

	template <class T>
	bool SDF<T>::SDFImpl::ImportFromCache(const std::string& cacheFile, const std::string& queryPointsSignature,
										const std::string& meshVerticesSignature, const std::string& meshIndicesSignature)
	{
		bool cacheExist = false;
		std::ifstream dataFile;

		if (!std::filesystem::exists(cacheFolder))
			std::filesystem::create_directories(cacheFolder);

		else
		{
			for (const auto& entry : std::filesystem::directory_iterator(cacheFolder)) {
				if (entry.is_regular_file()) {
					std::string fileName = entry.path().filename().string();
					if (fileName == cacheFile)
					{
						dataFile.open(cacheFolder / cacheFile, std::ios::binary);
						if (!dataFile)
							throw("[Error] Failed to open model cache file!");

						std::string validQueryPointsSignature(queryPointsSignature);
						std::string validMeshVerticessSignature(meshVerticesSignature);
						std::string validMeshIndicesSignature(meshIndicesSignature);
						dataFile.read(validQueryPointsSignature.data(), validQueryPointsSignature.size());
						dataFile.read(validMeshVerticessSignature.data(), validMeshVerticessSignature.size());
						dataFile.read(validMeshIndicesSignature.data(), validMeshIndicesSignature.size());
						
						if(validQueryPointsSignature == queryPointsSignature
							&& validMeshVerticessSignature == meshVerticesSignature
							&& validMeshIndicesSignature == meshIndicesSignature)
							cacheExist = true;
					}
				}
			}
		}
		if (cacheExist)
		{
			std::vector<Eigen::Index> dataInfo = { 0, 0, 0, 0, 0, 0 };
			dataFile.read(reinterpret_cast<char*>(dataInfo.data()), dataInfo.size() * sizeof(Eigen::Index));

			smallestSignedDistances.resize(dataInfo[0]);
			dataFile.read(reinterpret_cast<char*>(smallestSignedDistances.data()), smallestSignedDistances.size() * sizeof(T));

			smallestDistancesIndices.resize(dataInfo[1]);
			dataFile.read(reinterpret_cast<char*>(smallestDistancesIndices.data()), smallestDistancesIndices.size() * sizeof(int));

			closestPoints.resize(dataInfo[2], dataInfo[3]);
			dataFile.read(reinterpret_cast<char*>(closestPoints.data()), closestPoints.size() * sizeof(T));

			closestNormals.resize(dataInfo[4], dataInfo[5]);
			dataFile.read(reinterpret_cast<char*>(closestNormals.data()), closestNormals.size() * sizeof(T));

			dataFile.read(reinterpret_cast<char*>(resolution.data()), resolution.size() * sizeof(int));
		}
		if(dataFile.is_open())
			dataFile.close();

		return cacheExist;
	}

	template <class T>
	void SDF<T>::SDFImpl::CalculateSDF(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints, 
										Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
										Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices, 
										igl::SignedDistanceType signType)
	{
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> faceNormals, vertexNormals, edgeNormals;
		Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> edges;
		Eigen::Matrix<int, Eigen::Dynamic, 1> edgeMap;
		igl::AABB<Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, 3> tree;
		igl::WindingNumberAABB<Eigen::Matrix<T, 1, 3>, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>> hierarchy;

		if (meshIndices.rows() > 0) {
			switch (meshVertices.cols()) {
			case 2: {
				igl::signed_distance(queryPoints, meshVertices, meshIndices, signType, smallestSignedDistances, smallestDistancesIndices, closestPoints, closestNormals);
				break;
			}
			case 3: {
				tree.deinit();
				tree.init(meshVertices, meshIndices);
				if (signType == igl::SIGNED_DISTANCE_TYPE_DEFAULT || signType == igl::SIGNED_DISTANCE_TYPE_WINDING_NUMBER) {
					hierarchy.set_mesh(meshVertices, meshIndices);
					hierarchy.grow();
				}
				else if (signType == igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL) {
					igl::per_face_normals(meshVertices, meshIndices, faceNormals);
					igl::per_vertex_normals(meshVertices, meshIndices, igl::PER_VERTEX_NORMALS_WEIGHTING_TYPE_ANGLE, faceNormals, vertexNormals);
					igl::per_edge_normals(meshVertices, meshIndices, igl::PER_EDGE_NORMALS_WEIGHTING_TYPE_UNIFORM, faceNormals, edgeNormals, edges, edgeMap);
				}

				closestNormals.resize(queryPoints.rows(), 3);
				smallestSignedDistances.resize(queryPoints.rows());
				smallestDistancesIndices.resize(queryPoints.rows());
				closestPoints.resize(queryPoints.rows(), 3);

				igl::parallel_for(queryPoints.rows(), [&](const int p) {
					const Eigen::Matrix<T, 1, 3> point(queryPoints(p, 0), queryPoints(p, 1), queryPoints(p, 2));
					T signedDistance, squaredDistance;
					Eigen::Matrix<T, 1, 3> closestPoint;
					int index;

					switch (signType) {
					case igl::SIGNED_DISTANCE_TYPE_DEFAULT:
					case igl::SIGNED_DISTANCE_TYPE_WINDING_NUMBER:
						igl::signed_distance_winding_number(tree, meshVertices, meshIndices, hierarchy, point, signedDistance, squaredDistance, index, closestPoint);
						break;
					case igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL: {
						Eigen::Matrix<T, 1, 3> normal(0, 0, 0);
						igl::signed_distance_pseudonormal(tree, meshVertices, meshIndices, faceNormals, vertexNormals, edgeNormals, edgeMap, point, signedDistance, squaredDistance, index, closestPoint, normal);
						closestNormals.row(p) = normal;
						break;
					}
					default:
						throw("[Error] Unsupported Signed Distance Field type!");
					}

					smallestDistancesIndices(p) = index;
					smallestSignedDistances(p) = signedDistance * std::sqrt(squaredDistance);
					closestPoints.row(p) = closestPoint;
					}, 10000);

				break;
			}
			default:
				throw("[Error] face format error!");
			}
		}
	}


	template <class T>
	void SDF<T>::SDFImpl::ExportDataCache(const std::string& cacheFile, const std::string& queryPointsSignature,
		const std::string& meshVerticesSignature, const std::string& meshIndicesSignature) const
	{
		std::ofstream dataFile(cacheFolder / cacheFile, std::ios::binary);
		if (!dataFile)
			throw("[Error] Failed to cache model file!");

		dataFile.write(queryPointsSignature.data(), queryPointsSignature.size());
		dataFile.write(meshVerticesSignature.data(), meshVerticesSignature.size());
		dataFile.write(meshIndicesSignature.data(), meshIndicesSignature.size());

		std::vector<Eigen::Index> dataInfo = { smallestSignedDistances.size(), smallestDistancesIndices.size(), 
												closestPoints.rows(), closestPoints.cols(),
												closestNormals.rows(), closestNormals.cols()};
		
		dataFile.write(reinterpret_cast<const char*>(dataInfo.data()), dataInfo.size() * sizeof(Eigen::Index));
		dataFile.write(reinterpret_cast<const char*>(smallestSignedDistances.data()), smallestSignedDistances.size() * sizeof(T));
		dataFile.write(reinterpret_cast<const char*>(smallestDistancesIndices.data()), smallestDistancesIndices.size() * sizeof(int));
		dataFile.write(reinterpret_cast<const char*>(closestPoints.data()), closestPoints.size() * sizeof(T));
		dataFile.write(reinterpret_cast<const char*>(closestNormals.data()), closestNormals.size() * sizeof(T));
		dataFile.write(reinterpret_cast<const char*>(resolution.data()), resolution.size() * sizeof(int));

		dataFile.close();
	}

	template<class T>
	SDF<T>::SDF(Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints, 
				Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices,
				Eigen::Matrix<int,Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices, 
				igl::SignedDistanceType signType, bool cache)
	{
		try
		{
			pImpl = std::make_unique<SDFImpl>(queryPoints, meshVertices, meshIndices, signType, cache);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "[Error] Generate signed distance field faild!" << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}

	template<class T>
	SDF<T>::~SDF() = default;

	template<class T>
	Eigen::Vector<T, Eigen::Dynamic> SDF<T>::GetSmallestSignedDistances() const
	{
		return pImpl->GetSmallestSignedDistances();
	}

	template<class T>
	Eigen::VectorXi SDF<T>::GetSmallestDistancesIndices() const
	{
		return pImpl->GetSmallestDistancesIndices();
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> SDF<T>::GetClosestPoints() const
	{
		return pImpl->GetClosestPoints();
	}
	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> SDF<T>::GetClosestNormals() const
	{
		return pImpl->GetClosestNormals();
	}

	void ClearAllCache()
	{
		const std::filesystem::path cachePath = std::filesystem::current_path() / "Cache" / "SDFs";
		if (std::filesystem::exists(cachePath))
			if (!std::filesystem::remove_all(cachePath))
				std::cerr << "[Error] Clear file faild!" << std::endl;
	}

	template<typename T>
	std::tuple<Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::RowVector3i>
		MeshGridFlatten(const T& XStart, const T& XEnd, const T& XStep,
			const T& YStart, const T& YEnd, const T& YStep,
			const T& ZStart, const T& ZEnd, const T& ZStep)
	{
		const int sizeX = std::round((XEnd - XStart) / XStep) + 1;
		const int sizeY = std::round((YEnd - YStart) / YStep) + 1;
		const int sizeZ = std::round((ZEnd - ZStart) / ZStep) + 1;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> mesh(sizeX * sizeY * sizeZ, 3);

#pragma omp parallel for collapse(3)
		for (int k = 0; k < sizeZ; ++k)
		{
			for (int i = 0; i < sizeX; ++i)
			{
				for (int j = 0; j < sizeY; ++j)
				{
					mesh(j + i * sizeY + k * sizeX * sizeY, 0) = XStart + i * XStep;
					mesh(j + i * sizeY + k * sizeX * sizeY, 1) = YStart + j * YStep;
					mesh(j + i * sizeY + k * sizeX * sizeY, 2) = ZStart + k * ZStep;
				}
			}
		}
		return std::make_tuple(mesh, Eigen::Vector3i(sizeX, sizeY, sizeZ));
	}

	template<typename T>
	std::tuple<Eigen::Tensor<T, 3, Eigen::RowMajor>, Eigen::Tensor<T, 3, Eigen::RowMajor>, Eigen::Tensor<T, 3, Eigen::RowMajor>> 
		MeshGrid(const T& XStart, const T& XEnd, const T& XStep,
				const T& YStart, const T& YEnd, const T& YStep, 
				const T& ZStart, const T& ZEnd, const T& ZStep)
	{
		const int sizeX = std::round((XEnd - XStart) / XStep) + 1;
		const int sizeY = std::round((YEnd - YStart) / YStep) + 1;
		const int sizeZ = std::round((ZEnd - ZStart) / ZStep) + 1;
		Eigen::Tensor<T, 3, Eigen::RowMajor> gx(sizeX, sizeY, sizeZ);
		Eigen::Tensor<T, 3, Eigen::RowMajor> gy(sizeX, sizeY, sizeZ);
		Eigen::Tensor<T, 3, Eigen::RowMajor> gz(sizeX, sizeY, sizeZ);

#pragma omp parallel for collapse(3)
		for (int k = 0; k < sizeZ; ++k)
		{	
			for (int j = 0; j < sizeY; ++j)
			{
				for (int i = 0; i < sizeX;++i)
				{
					gx(i, j, k) = XStart + i * XStep;
					gy(i, j, k) = YStart + j * YStep;
					gz(i, j, k) = ZStart + k * ZStep; 
				}
			}
		}
		
		return std::make_tuple(gx, gy, gz);
	}
}