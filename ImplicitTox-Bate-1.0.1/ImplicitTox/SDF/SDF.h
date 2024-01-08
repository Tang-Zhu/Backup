#pragma once
#include <memory>
#include <Eigen/Core>
#include <igl/signed_distance.h>
#include <unsupported/Eigen/CXX11/Tensor>
namespace sdf {
   
	template<class T = float>
	class SDF
	{
	public:
		SDF(Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> queryPoints, 
			Eigen::Matrix<T,Eigen::Dynamic, 3, Eigen::RowMajor> meshVertices, 
			Eigen::Matrix<int,Eigen::Dynamic, 3, Eigen::RowMajor> meshIndices,
			igl::SignedDistanceType signType, bool cache = false);

		~SDF();

		Eigen::Vector<T, Eigen::Dynamic> GetSmallestSignedDistances() const;
		Eigen::VectorXi GetSmallestDistancesIndices() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetClosestPoints() const;
		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> GetClosestNormals() const;
		
	private:
		class SDFImpl;
		std::unique_ptr<SDFImpl> pImpl;
	};
	void ClearAllCache();

	template<typename T>
	std::tuple<Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>, Eigen::RowVector3i>
		MeshGridFlatten(const T& XStart, const T& XEnd, const T& XStep,
			const T& YStart, const T& YEnd, const T& YStep,
			const T& ZStart, const T& ZEnd, const T& ZStep);

	template<typename T>
	std::tuple<Eigen::Tensor<T, 3, Eigen::RowMajor>, Eigen::Tensor<T, 3, Eigen::RowMajor>, Eigen::Tensor<T, 3, Eigen::RowMajor>>
		MeshGrid(const T& XStart, const T& XEnd, const T& XStep,
				const T& YStart, const T& YEnd, const T& YStep,
				const T& ZStart, const T& ZEnd, const T& ZStep);
}
