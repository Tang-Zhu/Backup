#pragma once
#include <memory>
#include <string>
#include <Eigen/Core>
#include <igl/FileEncoding.h>

namespace model
{
	template<class T = float>
	class Model
	{
	public:
		Model(const std::string& pFile, bool cache = false);
		Model(const Model<T>& other);
		~Model();
		Model<T>& operator = (const Model<T>& other);

		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetVertices() const;
		Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> GetFaces() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetNormals() const;

		void ClearCache() const;
		static void ClearCache(const std::string& pFile);
		void Export(const std::string& pFile, igl::FileEncoding encoding = igl::FileEncoding::Ascii);
		void Show();
		static void Show(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> faces);
		static void Export(const std::string& pFile, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> vertices, Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces, igl::FileEncoding encoding = igl::FileEncoding::Ascii);
		static void ExportFromSDF(const std::string& pFile, Eigen::Vector<T, Eigen::Dynamic> SDF, Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue, igl::FileEncoding encoding = igl::FileEncoding::Ascii);
		static std::tuple< Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>>
			GenerateISOSurfaces(Eigen::Vector<T, Eigen::Dynamic> SDF,
				Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> domain, Eigen::RowVector3i resolution, T isoValue);
	private:
		class ModelImpl;
		std::unique_ptr<ModelImpl> pImpl;
	};
	void ClearAllCache();
}

