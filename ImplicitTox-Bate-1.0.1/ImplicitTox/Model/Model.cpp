#include "Model.h"
#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <igl/write_triangle_mesh.h>
#include <igl/marching_cubes.h>
#include <igl/remove_duplicate_vertices.h>
#include <igl/opengl/glfw/Viewer.h>
#include <cryptopp/blake2.h>
#include <cryptopp/hex.h>

namespace model
{
	template class Model<float>;
	template class Model<double>;

	template<class T>
	class Model<T>::ModelImpl
	{
	public:
		ModelImpl(const std::string& pFile, bool cache);
		ModelImpl(const ModelImpl& other);
		~ModelImpl();

		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetVertices() const;
		Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> GetFaces() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetNormals() const;
		void ClearCache();
		void Export(const std::string& pFile, igl::FileEncoding encoding);
		void Show();
		static const std::string GenerateCacheSignature(const std::string& pFile);
		static void ClearCache(const std::string& pFile);
		static void Show(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, 
			Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces);
		static void Export(const std::string& pFile, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, 
			Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces, igl::FileEncoding encoding);
		static void ExportFromSDF(const std::string& pFile, Eigen::Vector<T, Eigen::Dynamic> SDF, 
			Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, 
			T isoValue, igl::FileEncoding encoding);
		static std::tuple< Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>>
			GenerateISOSurfaces(Eigen::Vector<T, Eigen::Dynamic> SDF,
			Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution,	T isoValue);
	private:
		bool ImportFromCache(const std::string& cacheFile);
		void ImportFromFile();
		void ExportDataCache(const std::string& cacheFile) const;
		std::string pFile;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices;
		Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> normals;
		static std::filesystem::path cacheFolder; 
	};

	template <class T>
	std::filesystem::path Model<T>::ModelImpl::cacheFolder = std::filesystem::current_path() / "Cache" / "Models";

	template<class T>
	Model<T>::ModelImpl::ModelImpl(const std::string& pFile, bool cache)
	{
		this->pFile = pFile;
		bool cacheExist = false;
		std::string cacheFile;
		if (cache)
		{
			cacheFile = GenerateCacheSignature(pFile);
			cacheExist = ImportFromCache(cacheFile);
		}
	
		if(!cache || !cacheExist)
		{
			ImportFromFile();

			if (cache && !cacheExist)
				ExportDataCache(cacheFile);
		}
	}

	template<class T>
	Model<T>::ModelImpl::ModelImpl(const ModelImpl& other)
	{
		if (this != &other)
		{
			vertices = other.vertices;
			faces = other.faces;
			normals = other.normals;
		}
	}

	template<class T>
	Model<T>::ModelImpl::~ModelImpl() = default;

	template <class T>
	const std::string Model<T>::ModelImpl::GenerateCacheSignature(const std::string& pFile)
	{
		const std::string creatTime = std::to_string(std::filesystem::last_write_time(pFile).time_since_epoch().count());
		const std::string modelType = typeid(T).name();
		const std::string encodeStr = pFile + modelType + creatTime;

		CryptoPP::BLAKE2b hash;
		std::string signature;
		CryptoPP::StringSource(encodeStr, true, 
			new CryptoPP::HashFilter(hash, 
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(signature))));

		if (signature == "")
			throw("[Error] Generate model cache signature faild!");

		std::string cacheFile = signature + ".dat";



		return cacheFile;
	}

	template <class T>
	void model::Model<T>::ModelImpl::Show()
	{
		igl::opengl::glfw::Viewer viewer;
		viewer.data().set_mesh(vertices.cast<double>(), faces);
		viewer.launch();
	}

	template <class T>
	bool Model<T>::ModelImpl::ImportFromCache(const std::string& cacheFile)
	{
		bool cacheExist = false;

		if (!std::filesystem::exists(cacheFolder))
				std::filesystem::create_directories(cacheFolder);

		else
		{
			for (const auto& entry : std::filesystem::directory_iterator(cacheFolder)) {
				if (entry.is_regular_file()) {
					std::string fileName = entry.path().filename().string();
					if (fileName == cacheFile)
						cacheExist = true;
				}
			}
		}
		if (cacheExist)
		{
			std::ifstream dataFile(cacheFolder / cacheFile, std::ios::binary);
			if (!dataFile)
				throw("[Error] Failed to open model cache file!");

			std::vector<Eigen::Index> dataInfo = { 0, 0, 0, 0, 0, 0 };
			dataFile.read(reinterpret_cast<char*>(dataInfo.data()), dataInfo.size() * sizeof(Eigen::Index));

			vertices.resize(dataInfo[0], dataInfo[1]);
			dataFile.read(reinterpret_cast<char*>(vertices.data()), vertices.size() * sizeof(T));

			faces.resize(dataInfo[2], dataInfo[3]);
			dataFile.read(reinterpret_cast<char*>(faces.data()), faces.size() * sizeof(int));

			normals.resize(dataInfo[4], dataInfo[5]);
			dataFile.read(reinterpret_cast<char*>(normals.data()), normals.size() * sizeof(T));

			dataFile.close();
		}

		return cacheExist;
	}

	template <class T>
	void Model<T>::ModelImpl::ImportFromFile()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(pFile, aiProcess_ValidateDataStructure |
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
		if (!scene)
			throw("[Error] " + std::string(importer.GetErrorString()));

		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			throw("[Error] The model was loaded, but the scene is incomplete!");

		vertices.resize(scene->mMeshes[0]->mNumVertices, 3);
		faces.resize(scene->mMeshes[0]->mNumFaces, 3);
		normals.resize(scene->mMeshes[0]->mNumVertices, 3);

#pragma omp parallel for
		for (int n = 0; n < vertices.rows(); ++n)
		{
			vertices(n, 0) = scene->mMeshes[0]->mVertices[n].x;
			vertices(n, 1) = scene->mMeshes[0]->mVertices[n].y;
			vertices(n, 2) = scene->mMeshes[0]->mVertices[n].z;

			normals(n, 0) = scene->mMeshes[0]->mNormals[n].x;
			normals(n, 1) = scene->mMeshes[0]->mNormals[n].y;
			normals(n, 2) = scene->mMeshes[0]->mNormals[n].z;
		}
#pragma omp parallel for
		for (int n = 0; n < faces.rows(); ++n)
		{
			faces(n, 0) = scene->mMeshes[0]->mFaces[n].mIndices[0];
			faces(n, 1) = scene->mMeshes[0]->mFaces[n].mIndices[1];
			faces(n, 2) = scene->mMeshes[0]->mFaces[n].mIndices[2];
		}
	}

	template <class T>
	void model::Model<T>::ModelImpl::Export(const std::string& pFile, igl::FileEncoding encoding)
	{
		igl::write_triangle_mesh(pFile, vertices, faces, encoding);
	}

	template <class T>
	void Model<T>::ModelImpl::ExportDataCache(const std::string& cacheFile) const
	{
		std::ofstream dataFile(cacheFolder / cacheFile, std::ios::binary);
		if (!dataFile)
			throw("[Error] Failed to cache model file!");

		std::vector<Eigen::Index> dataInfo = { vertices.rows(), vertices.cols(), faces.rows(), faces.cols(), normals.rows(), normals.cols() };

		dataFile.write(reinterpret_cast<const char*>(dataInfo.data()), dataInfo.size() * sizeof(Eigen::Index));
		dataFile.write(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(T));
		dataFile.write(reinterpret_cast<const char*>(faces.data()), faces.size() * sizeof(int));
		dataFile.write(reinterpret_cast<const char*>(normals.data()), normals.size() * sizeof(T));
		dataFile.close();
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Model<T>::ModelImpl::GetVertices() const
	{
		return vertices;
	}

	template<class T>
	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Model<T>::ModelImpl::GetFaces() const
	{
		return faces;
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Model<T>::ModelImpl::GetNormals() const
	{
		return normals;
	}

	template<class T>
	void Model<T>::ModelImpl::ClearCache()
	{
		std::string cacheFile = GenerateCacheSignature(pFile);

		if (!std::filesystem::remove(cacheFolder / cacheFile))
			throw("[Error] Clear file faild!");
	}

	template<class T>
	void Model<T>::ModelImpl::ClearCache(const std::string& pFile)
	{
		std::string cacheFile = GenerateCacheSignature(pFile);

		if (!std::filesystem::remove(cacheFolder / cacheFile))
			throw("[Error] Clear file faild!");
	}

	template <class T>
	void model::Model<T>::ModelImpl::Show(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices,
		Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces)
	{
		igl::opengl::glfw::Viewer viewer;
		viewer.data().set_mesh(vertices.cast<double>(), faces);
		viewer.launch();
	}

	template<class T>
	void Model<T>::ModelImpl::Export(const std::string& pFile, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces, igl::FileEncoding encoding)
	{
		igl::write_triangle_mesh(pFile, vertices, faces, encoding);
	}

	template<class T>
	void Model<T>::ModelImpl::ExportFromSDF(const std::string& pFile, Eigen::Vector<T, Eigen::Dynamic> SDF, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue, igl::FileEncoding encoding)
	{
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> V;
		Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> F;
		std::tie(V, F) = GenerateISOSurfaces(SDF, domain, resolution, isoValue);
		Export(pFile, V, F, encoding);
	}

	template<class T>
	std::tuple< Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>>
		Model<T>::ModelImpl::GenerateISOSurfaces(Eigen::Vector<T, Eigen::Dynamic> SDF, 
			Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue)
	{
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> V;
		Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> F;
		igl::marching_cubes(SDF, domain, resolution(0), resolution(1), resolution(2), isoValue, V, F);
		//Eigen::VectorXi I;
		//Eigen::VectorXi J;
		//igl::remove_duplicate_vertices(V, F, 0, V, I, J, F);
		return std::make_tuple(V, F);
	}

	template<class T>
	Model<T>::Model(const std::string& pFile, bool cache)
	{
		if (!std::is_floating_point<T>::value)
		{
			throw("[Error] Mesh data must be float/double type!");
		}

		if (!std::filesystem::exists(pFile))
		{
			throw("[Error] File " + pFile + " doesn't exist!");
		}

		try
		{
			pImpl = std::make_unique<ModelImpl>(pFile, cache);
		}
		catch (const std::exception& e)
		{
			throw("[Error] Import model failed!");
		}
	}

	template<class T>
	Model<T>::Model(const Model<T>& other)
	{
		if (this != &other)
		{
			if (other.pImpl)
			{
				try
				{
					pImpl = std::make_unique<ModelImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Import model failed!");
				}
			}
			else
				pImpl = nullptr;
		}	
	}

	template<class T>
	Model<T>::~Model() = default;

	template<class T>
	Model<T>& Model<T>::operator=(const Model<T>& other)
	{
		if (this != &other)
		{
			if (!other.pImpl)
				pImpl = nullptr;

			else
			{
				if (pImpl)
					*pImpl = *other.pImpl;
				else
					try
				{
					pImpl = std::make_unique<ModelImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Import model failed!");
				}
			}
		}
		return *this;
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Model<T>::GetVertices() const
	{
		return pImpl->GetVertices();
	}

	template<class T>
	Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Model<T>::GetFaces() const
	{
		return pImpl->GetFaces();
	}

	template<class T>
	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Model<T>::GetNormals() const
	{
		return pImpl->GetNormals();
	}

	template<class T>
	void Model<T>::ClearCache() const
	{
		try
		{
			pImpl->ClearCache();
		}
		catch (std::exception& e)
		{
			throw("[Error] Clear cache faild!");
		}
	}

	template<class T>
	void Model<T>::ClearCache(const std::string& pFile)
	{
		try
		{
			ModelImpl::ClearCache(pFile);
		}
		catch (std::exception& e)
		{
			throw("[Error] Clear cache faild!");
		}
	}

	template <class T>
	void model::Model<T>::Show()
	{
		pImpl->Show();
	}

	template <class T>
	void model::Model<T>::Show(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices,
		Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces)
	{
		try
		{
			ModelImpl::Show(vertices, faces);
		}
		catch (std::exception& e)
		{
			throw("[Error] Show model faild!");
		}
	}

	template<class T>
	void Model<T>::Export(const std::string& pFile, igl::FileEncoding encoding)
	{
		try
		{
			pImpl->Export(pFile, encoding);
		}
		catch (std::exception& e)
		{
			throw("[Error] Export model faild!");
		}
	}

	template<class T>
	void Model<T>::Export(const std::string& pFile, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces, igl::FileEncoding encoding)
	{
		try
		{
			ModelImpl::Export(pFile, vertices, faces, encoding);
		}
		catch (std::exception& e)
		{
			throw("[Error] Export model faild!");
		}
	}

	template<class T>
	void Model<T>::ExportFromSDF(const std::string& pFile, Eigen::Vector<T, Eigen::Dynamic> SDF, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue, igl::FileEncoding encoding)
	{
		try
		{
			ModelImpl::ExportFromSDF(pFile, SDF, domain, resolution, isoValue, encoding);
		}
		catch (std::exception& e)
		{
			throw("[Error] Export model from SDF faild!");
		}
	}

	template<class T>
	std::tuple<Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>>
		Model<T>::GenerateISOSurfaces(Eigen::Vector<T, Eigen::Dynamic> SDF, 
			Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue)
	{
		try
		{
			return ModelImpl::GenerateISOSurfaces(SDF, domain, resolution, isoValue);
		}
		catch (std::exception& e)
		{
			throw("[Error] Generate ISO Surfaces faild!");
		}
	}

	void ClearAllCache()
	{
		try
		{
			const std::filesystem::path cachePath = std::filesystem::current_path() / "Cache" / "Models";
			if (std::filesystem::exists(cachePath))
				if (!std::filesystem::remove_all(cachePath))
					throw("[Error] Clear file faild!");
		}
		catch (std::exception& e)
		{
			throw("[Error] Clear cache files faild!");
		}
	
	}
}