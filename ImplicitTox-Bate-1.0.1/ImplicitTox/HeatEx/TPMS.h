#pragma once
#include <memory>
#include <string>
#include <tuple>
#include <Eigen/Core>

namespace heat_ex
{
	template<class T>
	class TPMS
	{
	public:
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		GenerateTPMS(const std::string& surfaceType, const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
			Eigen::Vector<T, 3> center, const T period, const T thicknesses, 
			Eigen::Vector<T, Eigen::Dynamic> TDF,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, 
			Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		GenerateTPMS(const std::string& surfaceType, const T L,
			const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
			Eigen::Vector<T, 3> center, const T period, T thicknesses,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
			GenerateTPMSModified(const std::string& surfaceType,
				const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
				Eigen::Vector<T, 3> center, const T period, const T thicknesses,
				Eigen::Vector<T, Eigen::Dynamic> TDF,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		//~GenerateTPMS();

		//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
		//	Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>> GetValues() const;

	private:
		class TPMSImpl;
		TPMS() = delete;
		~TPMS() = delete;
		TPMS(const TPMS&) = delete;
		TPMS& operator = (const TPMS&) = delete;
		//class TPMSImpl;
		//std::unique_ptr<TPMSImpl> pImpl;
	};
}

