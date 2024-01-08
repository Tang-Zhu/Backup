#include "TPMS.h"
#include <iostream>
#include <cmath>

namespace heat_ex
{
	template class TPMS<float>;
	template class TPMS<double>;

	template<class T>
	class TPMS<T>::TPMSImpl
	{
	public:
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		GenerateTPMSImpl(const std::string& surfaceType, 
			    const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
				Eigen::Vector<T, 3> center, const T period, const T thicknesses, 
			    Eigen::Vector<T, Eigen::Dynamic> TDF, 
				Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
			    Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
				Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, 
				Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		GenerateTPMSImpl(const std::string& surfaceType, const T L,
				const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
				Eigen::Vector<T, 3> center, const T period, T thicknesses,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		static std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
			Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
			GenerateTPMSModifiedImpl(const std::string& surfaceType,
				const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
				Eigen::Vector<T, 3> center, const T period, const T thicknesses,
				Eigen::Vector<T, Eigen::Dynamic> TDF,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
				Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo);
		//~TPMSImpl();

		//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
		//	       Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>> GetValues() const;

	private:
		//Eigen::Vector<T, Eigen::Dynamic> phiFuncOne;
		//Eigen::Vector<T, Eigen::Dynamic> phiFuncTwo;
		//Eigen::Vector<T, Eigen::Dynamic> phiOnly;
		//Eigen::Vector<T, Eigen::Dynamic> phiTDF;
		TPMSImpl() = delete;
		~TPMSImpl() = delete;
		TPMSImpl(const TPMSImpl&) = delete;
		TPMSImpl& operator = (const TPMSImpl&) = delete;
	};

	//template<class T>
	//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
	//	Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
	//TPMS<T>::TPMSImpl::GenerateTPMSImpl(const std::string& surfaceType, const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
	//							Eigen::Vector<T, 3> center, const T period, T thicknesses, 
	//							Eigen::Vector<T, Eigen::Dynamic> TDF,
	//							Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, 
	//							Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//							Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, 
	//							Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//	auto Fun = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x, 
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& y, 
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& z, 
	//					const T period, const std::string& surfaceType)
	//					-> Eigen::Array<T, Eigen::Dynamic, 1>
	//	{
	//		const T PI = Eigen::numext::acos(-1.0);
	//		if (surfaceType == "P")
	//			return (x * 2 * PI * period).cos() + (y * 2 * PI * period).cos() + (z * 2 * PI * period).cos();
	//		else if (surfaceType == "G")
	//			return (x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() + (z * 2 * PI * period).sin()
	//			       * (x * 2 * PI * period).cos() + (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos();
	//		else if (surfaceType == "D")
	//			return (x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos() -
	//			       (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin();
	//		else if (surfaceType == "IWP")
	//			return 2 * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() + (y * 2 * PI * period).cos()
	//				   * (z * 2 * PI * period).cos() + (z * 2 * PI * period).cos() * (x * 2 * PI * period).cos())
	//			       - ((2 * x * 2 * PI * period).cos() + (2 * y * 2 * PI * period).cos() + (2 * z * 2 * PI * period).cos());
	//		return Eigen::Array<T, Eigen::Dynamic, 1>::Zero(x.size());
	//	};

	//	auto DFun = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& y,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& z,
	//					const T period, const std::string& surfaceType)
	//					-> Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>
	//	{
	//		const T PI = Eigen::numext::acos(-1.0);
	//		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> res(x.size(), 3);
	//		if (surfaceType == "P")
	//			res << (-2 * PI * period * (x * 2 * PI * period).sin()),
	//			(-2 * PI * period * (y * 2 * PI * period).sin()),
	//			(-2 * PI * period * (z * 2 * PI * period).sin());
	//		else if (surfaceType == "G")
	//			res << 2 * PI * period * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() - (z * 2 * PI * period).sin() * (x * 2 * PI * period).sin()),
	//			2 * PI * period * ((y * 2 * PI * period).cos() * (z * 2 * PI * period).cos() - (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin()),
	//			2 * PI * period * ((z * 2 * PI * period).cos() * (x * 2 * PI * period).cos() - (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin());
	//		else if (surfaceType == "D")
	//			res << -2 * PI * period * ((x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()
	//				+ (x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()),
	//			-2 * PI * period * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos()
	//				+ (x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()),
	//			-2 * PI * period * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()
	//				+ (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos());
	//		else if (surfaceType == "IWP")
	//			res << -2 * PI * period * (2 * ((x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() + (z * 2 * PI * period).cos() * (x * 2 * PI * period).sin())
	//				- 2 * (2 * x * 2 * PI * period).sin()),
	//			-2 * PI * period * (2 * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() + (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos())
	//				- 2 * (2 * y * 2 * PI * period).sin()),
	//			-2 * PI * period * (2 * ((y * 2 * PI * period).cos() * (z * 2 * PI * period).sin() + (z * 2 * PI * period).sin() * (x * 2 * PI * period).cos())
	//				- 2 * (2 * z * 2 * PI * period).sin());
	//		else
	//			res.fill(0);
	//		return res;
	//	};

	//	auto JFunx = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& y,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& z,
	//					const T period, const std::string& surfaceType)
	//					-> Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>
	//	{
	//		const T PI = Eigen::numext::acos(-1.0);
	//		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> res(x.size(), 3);
	//		if (surfaceType == "P")
	//			res << -std::pow(2 * PI * period, 2) * (x * 2 * PI * period).cos(), 0 * x, 0 * x;
	//		else if (surfaceType == "G")
	//			res << -std::pow(2 * PI * period, 2) * ((x * 2 * PI * period).sin() * (y * 2 * PI * period).cos()
	//				+ (z * 2 * PI * period).sin() * (x * 2 * PI * period).cos()),
	//			-std::pow(2 * PI * period, 2) * (x * 2 * PI * period).cos() * (y * 2 * PI * period).sin(),
	//			-std::pow(2 * PI * period, 2) * (z * 2 * PI * period).cos() * (x * 2 * PI * period).sin();
	//		else if (surfaceType == "D")
	//			res << -std::pow(2 * PI * period, 2) * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()
	//				- (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()),
	//			-std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos()
	//				+ (x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()),
	//			-std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()
	//				+ (x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos());
	//		else if (surfaceType == "IWP")
	//			res << -std::pow(2 * PI * period, 2) * (2 * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos()
	//				+ (z * 2 * PI * period).cos() * (x * 2 * PI * period).cos()) - 4 * (2 * x * 2 * PI * period).cos()),
	//				-2 * (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin(),
	//				-2 * (z * 2 * PI * period).sin() * (x * 2 * PI * period).sin();
	//		else
	//			res.fill(0);

	//		return res;
	//	};

	//	auto JFuny = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& y,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& z,
	//					const T period, const std::string& surfaceType)
	//					-> Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>
	//	{
	//		const T PI = Eigen::numext::acos(-1.0);
	//		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> res(x.size(), 3);
	//		if (surfaceType == "P")
	//			res << 0 * y, -std::pow(2 * PI * period, 2) * (y * 2 * PI * period).cos(), 0 * y;
	//		else if (surfaceType == "G")
	//			res << -std::pow(2 * PI * period, 2) * (x * 2 * PI * period).cos() * (y * 2 * PI * period).sin(),
	//			-std::pow(2 * PI * period, 2) * ((y * 2 * PI * period).sin() * (z * 2 * PI * period).cos()
	//				+ (x * 2 * PI * period).sin() * (y * 2 * PI * period).cos()),
	//			-std::pow(2 * PI * period, 2) * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin();
	//		else if (surfaceType == "D")
	//			res << -std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos()
	//			   + (x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()),
	//		   -std::pow(2 * PI * period, 2) * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()
	//			   - (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()),
	//		   -std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()
	//			   + (x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos());
	//		else if (surfaceType == "IWP")
	//			res << -std::pow(2 * PI * period, 2) * (-2 * (x * 2 * PI * period).sin() * (y * 2 * PI * period).sin()),
	//			-std::pow(2 * PI * period, 2) * (2 * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() + (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos())
	//				- 4 * (2 * y * 2 * PI * period).cos()),
	//			-std::pow(2 * PI * period, 2) * (-2 * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin());
	//		else
	//			res.fill(0);

	//		return res;
	//	};
	//	
	//	auto JFunz = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& y,
	//					const Eigen::Array<T, Eigen::Dynamic, 1>& z,
	//					const T period, const std::string& surfaceType)
	//					-> Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor>
	//	{
	//		const T PI = Eigen::numext::acos(-1.0);
	//		Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> res(x.size(), 3);
	//		if (surfaceType == "P")
	//			res << 0 * z, 0 * z, -std::pow(2 * PI * period, 2) * (z * 2 * PI * period).cos();
	//		else if (surfaceType == "G")
	//			res << -std::pow(2 * PI * period, 2) * (z * 2 * PI * period).cos() * (x * 2 * PI * period).sin(),
	//			-std::pow(2 * PI * period, 2) * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin(),
	//			-std::pow(2 * PI * period, 2) * ((z * 2 * PI * period).sin() * (x * 2 * PI * period).cos()
	//				+ (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos());
	//		else if (surfaceType == "D")
	//			res << -std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).sin()
	//				+ (x * 2 * PI * period).cos() + (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos()),
	//			-std::pow(2 * PI * period, 2) * (-(x * 2 * PI * period).cos() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()
	//				+ (x * 2 * PI * period).sin() + (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()),
	//			-std::pow(2 * PI * period, 2) * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()
	//				- (x * 2 * PI * period).sin() + (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin());
	//		else if (surfaceType == "IWP")
	//			res << -std::pow(2 * PI * period, 2) * (-2 * (z * 2 * PI * period).sin() * (x * 2 * PI * period).sin()),
	//			-std::pow(2 * PI * period, 2) * (-2 * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin()),
	//			-std::pow(2 * PI * period, 2) * (2 * ((y * 2 * PI * period).cos() * (z * 2 * PI * period).cos()
	//				+ (z * 2 * PI * period).cos() * (x * 2 * PI * period).cos()) - 4 * (2 * z * 2 * PI * period).cos());
	//		else
	//			res.fill(0);

	//		return res;
	//	};
	//	
	//	Eigen::Vector<T, Eigen::Dynamic> f = Fun(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, surfaceType);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Df = DFun(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, surfaceType);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Jfx = JFunx(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, surfaceType);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Jfy = JFuny(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, surfaceType);
	//	Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> Jfz = JFunz(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, surfaceType);

	//	Eigen::Vector<T, Eigen::Dynamic> fk = (Df.col(0).array() * (Jfx.array() * Df.array()).rowwise().sum()
	//															+ Df.col(1).array() * (Jfy.array() * Df.array()).rowwise().sum()
	//															+ Df.col(2).array() * (Jfz.array() * Df.array()).rowwise().sum())
	//															/ Df.array().square().rowwise().sum();
	//	Eigen::Vector<T, Eigen::Dynamic> fmid = (Df.array().square().rowwise().sum() - 2 * f.array() * fk.array()).unaryExpr([&](T x) { return x < 0 ? 0 : std::sqrt(x);});
	//	Eigen::Vector<T, Eigen::Dynamic> flevel = 2 * f.array() / (Df.array().square().rowwise().sum().sqrt() + fmid.array());

	//	Eigen::Vector<T, Eigen::Dynamic> phiFuncOne;
	//	Eigen::Vector<T, Eigen::Dynamic> phiFuncTwo;
	//	Eigen::Vector<T, Eigen::Dynamic> phiOnly;
	//	Eigen::Vector<T, Eigen::Dynamic> phiTDF;

	//	phiFuncOne = -(flevel.array() + thicknesses);
	//	phiFuncTwo = flevel.array() - thicknesses;

	//	if (TDF.size() == 0)
	//	{
	//		phiOnly = shellSDFOne.cwiseMin(shellSDFTwo).cwiseMin((-phiFuncOne).cwiseMin(-phiFuncTwo));
	//		Eigen::Vector<T, Eigen::Dynamic> phiTDFOne = shellSDFOne.cwiseMin(phiFuncOne);
	//		phiFuncOne = phiTDFOne.cwiseMax(IOSDFOne);
	//		Eigen::Vector<T, Eigen::Dynamic> phiTDFTwo = shellSDFTwo.cwiseMin(phiFuncTwo);
	//		phiFuncTwo = phiTDFTwo.cwiseMax(IOSDFTwo);
	//		phiTDF = -phiFuncOne.cwiseMax(phiFuncTwo);
	//	}
	//	else
	//	{
	//		phiOnly = shellSDFOne.cwiseMin(shellSDFTwo).cwiseMin((-phiFuncOne).cwiseMin(-phiFuncTwo));
	//		phiFuncOne = phiFuncOne.cwiseMin(shellSDFOne);
	//		phiFuncOne = TDF.cwiseMin(phiFuncOne.cwiseMax(IOSDFOne));
	//		phiFuncTwo = phiFuncTwo.cwiseMin(shellSDFTwo);
	//		phiFuncTwo = TDF.cwiseMin(phiFuncTwo.cwiseMax(IOSDFTwo));
	//		phiTDF = TDF.cwiseMin(-(phiFuncOne.cwiseMax(phiFuncTwo)));
	//	}

	//	return std::make_tuple(phiFuncOne, phiFuncTwo, phiOnly, phiTDF);
	//}

	//template <class T>
	//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
	//	Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
	//TPMS<T>::TPMSImpl::GenerateTPMSImpl(const std::string& surfaceType, const T L,
	//							const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
	//							Eigen::Vector<T, 3> center, const T period, T thicknesses,
	//							Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//							Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//		std::unordered_map<std::string, T> defaultCoefValues = {
	//		{"P", T(0.1863)},
	//		{"G", T(0.1907)},
	//		{"D", T(0.2516)},
	//		{"IWP", T(0.06468)}
	//	};
	//	thicknesses = period * thicknesses / defaultCoefValues[surfaceType];
	//
	//	auto Fun = [&](const Eigen::Array<T, Eigen::Dynamic, 1>& x,
	//		const Eigen::Array<T, Eigen::Dynamic, 1>& y,
	//		const Eigen::Array<T, Eigen::Dynamic, 1>& z,
	//		const T period, const T thicknesses, const std::string& surfaceType)
	//		-> Eigen::Array<T, Eigen::Dynamic, 1>
	//		{
	//			const T PI = Eigen::numext::acos(-1.0);
	//			if (surfaceType == "P")
	//				return (x * 2 * PI * period).cos() + (y * 2 * PI * period).cos() + (z * 2 * PI * period).cos() + thicknesses;
	//			else if (surfaceType == "G")
	//				return (x * 2 * PI * period).sin() * (y * 2 * PI * period).cos() + (z * 2 * PI * period).sin()
	//				* (x * 2 * PI * period).cos() + (y * 2 * PI * period).sin() * (z * 2 * PI * period).cos() + thicknesses;
	//			else if (surfaceType == "D")
	//				return (x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() * (z * 2 * PI * period).cos() -
	//				(x * 2 * PI * period).sin() * (y * 2 * PI * period).sin() * (z * 2 * PI * period).sin() + thicknesses;
	//			else if (surfaceType == "IWP")
	//				return 2 * ((x * 2 * PI * period).cos() * (y * 2 * PI * period).cos() + (y * 2 * PI * period).cos()
	//					* (z * 2 * PI * period).cos() + (z * 2 * PI * period).cos() * (x * 2 * PI * period).cos())
	//				- ((2 * x * 2 * PI * period).cos() + (2 * y * 2 * PI * period).cos() + (2 * z * 2 * PI * period).cos()) + thicknesses;
	//			return Eigen::Array<T, Eigen::Dynamic, 1>::Zero(x.size());
	//		};

	//	Eigen::Vector<T, Eigen::Dynamic> phiFuncOne;
	//	Eigen::Vector<T, Eigen::Dynamic> phiFuncTwo;
	//	Eigen::Vector<T, Eigen::Dynamic> phiOnly;
	//	Eigen::Vector<T, Eigen::Dynamic> phiTDF;
	//	
	//	phiFuncOne = -(Fun(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, thicknesses, surfaceType));
	//	phiFuncTwo = Fun(hrNodes.col(0).array() - center(0), hrNodes.col(1).array() - center(1), hrNodes.col(2).array() - center(2), period, -thicknesses, surfaceType);

	//	phiOnly = shellSDFOne.cwiseMin(shellSDFTwo).cwiseMin((-phiFuncOne).cwiseMin(-phiFuncTwo));
	//	Eigen::Vector<T, Eigen::Dynamic> phiTDFOne = -((-L * shellSDFOne.array()).exp() + (-L * phiFuncOne.array()).exp()).log() / L;
	//	phiFuncOne = ((L * phiTDFOne.array()).exp() + (L * IOSDFOne.array()).exp()).log() / L;
	//	Eigen::Vector<T, Eigen::Dynamic> phiTDFTwo = -((-L * shellSDFTwo.array()).exp() + (-L * phiFuncTwo.array()).exp()).log() / L;
	//	phiFuncTwo = ((L * phiTDFTwo.array()).exp() + (L * IOSDFTwo.array()).exp()).log() / L;
	//	phiTDF = -((L * phiFuncOne.array()).exp() + (L * phiFuncTwo.array())).log() / L;

	//	return std::make_tuple(phiFuncOne, phiFuncTwo, phiOnly, phiTDF);
	//}

	template <class T>
	std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
		Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		TPMS<T>::TPMSImpl::GenerateTPMSModifiedImpl(const std::string& surfaceType,
			const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
			Eigen::Vector<T, 3> center, const T period, const T thicknesses,
			Eigen::Vector<T, Eigen::Dynamic> TDF,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	{
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Tgxyz;
		Tgxyz.resize(hrNodes.rows(), hrNodes.cols());
		Tgxyz.col(0) = hrNodes.col(0).array() - center(0);
		Tgxyz.col(1) = hrNodes.col(1).array() - center(1);
		Tgxyz.col(2) = hrNodes.col(2).array() - center(2);

		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> a1;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> p;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> h1;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> h2;
		
		const T PI = Eigen::numext::acos(-1.0);

		if (surfaceType == "P")
		{
			a1.resize(1, 2);
			a1 << 0.5, 0.5;
			h1.resize(1, 3);
			h1 << 1., 0., 0.;
			p.resize(1, 1);
			p << 0.;
		}
		else if (surfaceType == "G")
		{
			a1.resize(1, 2);
			a1 << 0.5, 0.5;
			h1.resize(1, 3);
			h1 << 1., 1., 0.;
			p.resize(1, 1);
			p << -PI / 2.;
		}
		else if (surfaceType == "D")
		{
			a1.resize(1, 2);
			a1 << sqrt(2.) / 12., sqrt(2.) / 4.;
			h1.resize(1, 3);
			h1 << 1., 1., 1.;
			p.resize(1, 1);
			p << -PI / 4.;
		}
		else if (surfaceType == "IWP")
		{
			a1.resize(1, 4);
			a1 << 1., 1., -0.5, -0.5;
			h1.resize(1, 3);
			h1 << 1., 1., 0.;
			p.resize(2, 1);
			p << 0., 0.;
			h2.resize(1, 3);
			h2 << 2., 0., 0.;
		}
		else if (surfaceType == "FRD")
		{
			a1.resize(1, 4);
			a1 << 2. / 3., 2., -0.5, -0.5;
			h1.resize(1, 3);
			h1 << 1., 1., 1.;
			p.resize(2, 1);
			p << 0., 0.;
			h2.resize(1, 3);
			h2 << 2., 2., 0.;
		}
		else
		{
			std::cerr << "[ERROR] Unsupport SDF Type!" << std::endl;
			exit(-1);
		}

		Eigen::Matrix<T, 3, 3> M2;
		M2<< 0., 0., 1., 
			 1., 0., 0., 
			 0., 1., 0.;
		Eigen::Matrix<T, 3, 3> M3;
		M3<< 0., 1., 0., 
			 0., 0., 1., 
			 1., 0., 0.;
		Eigen::Matrix<T, 3, 3> S2;
		S2<< -1., 0., 0.,
			 0., 1., 0.,
			 0., 0., -1.;
		
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> hBase;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> hBaseNorm;
		T hNorm1, hNorm2;
		std::cout << ("here1");
		if (h2.size() == 0)
		{
			hBase.resize(6, 3);
			hBase.block<1, 3>(0, 0) = h1;
			hBase.block<1, 3>(1, 0) = h1 * M2;
			hBase.block<1, 3>(2, 0) = h1 * M3;
			hBase.block<1, 3>(3, 0) = h1 * S2;
			std::cout << ("here2");
			hBase.block<1, 3>(4, 0) = h1 * S2 * M2;
			hBase.block<1, 3>(5, 0) = h1 * S2 * M3;
			hNorm1 = h1.norm();
			hNorm2 = 0.;
			hBaseNorm = hBase / hNorm1;
		}
		else
		{
			std::cout << ("here3");
			hBase.resize(12, 3);
			hBase.block<1, 3>(0, 0) = h1;
			hBase.block<1, 3>(1, 0) = h1 * M2;
			hBase.block<1, 3>(2, 0) = h1 * M3;
			hBase.block<1, 3>(3, 0) = h1 * S2;
			hBase.block<1, 3>(4, 0) = h1 * S2 * M2;
			hBase.block<1, 3>(5, 0) = h1 * S2 * M3;
			hBase.block<1, 3>(6, 0) = h2;
			hBase.block<1, 3>(7, 0) = h2 * M2;
			hBase.block<1, 3>(8, 0) = h2 * M3;
			hBase.block<1, 3>(9, 0) = h2 * S2;
			hBase.block<1, 3>(10, 0) = h2 * S2 * M2;
			hBase.block<1, 3>(11, 0) = h2 * S2 * M3;

			hNorm1 = h1.norm();
			hNorm2 = h2.norm();
			
			Eigen::Matrix<T, 12, 3> hNorm;
			hNorm << Eigen::Matrix<T, 6, 3>::Constant(hNorm1), Eigen::Matrix<T, 6, 3>::Constant(hNorm2);

			hBaseNorm = hBase.array() / hNorm.array();
		}
		std::cout << ("here4");
		Tgxyz = Tgxyz * period;
		Tgxyz.transposeInPlace();
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> kron;
		kron.resize(hBase.rows(), hBase.cols());
		if (p.cols() == 1)
		{
			kron.setConstant(p(0));
		}
		else
		{
			kron << Eigen::Matrix<T, 6, 3>::Constant(p(0)), Eigen::Matrix<T, 6, 3>::Constant(p(1));
		}
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> baseVector = 2 * PI * hBase * Tgxyz + kron;
		kron.resize(1, baseVector.rows());
		if (a1.cols() == 2)
		{
			kron << Eigen::Matrix<T, 1, 3>::Constant(a1(0)), Eigen::Matrix<T, 1, 3>::Constant(a1(1));
		}
		else
		{
			kron << Eigen::Matrix<T, 1, 3>::Constant(a1(0)), Eigen::Matrix<T, 1, 3>::Constant(a1(1)), 
				Eigen::Matrix<T, 1, 3>::Constant(a1(2)), Eigen::Matrix<T, 1, 3>::Constant(a1(3));
		}
		std::cout << ("here5");
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> fg = kron.array() * baseVector.array().cos();
		fg.transposeInPlace();
		std::cout << ("here6");
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dfgX = -kron.array() * hBaseNorm.col(0).transpose().array() * 2 * PI * period * baseVector.array().sin();
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dfgY = -kron.array() * hBaseNorm.col(1).transpose().array() * 2 * PI * period * baseVector.array().sin();
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dfgZ = -kron.array() * hBaseNorm.col(2).transpose().array() * 2 * PI * period * baseVector.array().sin();
		std::cout << ("here7");
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dfg = (dfgX.array().pow(2) + dfgY.array().pow(2) + dfgZ.array().pow(2)).sqrt();
		dfg.transposeInPlace();

		T C = period * std::tan(2 * PI * (hNorm1 + hNorm2) / 2 * thicknesses / period) / 2 / PI;

		//Eigen::Vector<T, Eigen::Dynamic> phiFuncOne;
		//Eigen::Vector<T, Eigen::Dynamic> phiFuncTwo;
		//Eigen::Vector<T, Eigen::Dynamic> phiOnly;
		//Eigen::Vector<T, Eigen::Dynamic> phiTDF;
		 
		Eigen::Vector<T, Eigen::Dynamic> phiFuncOne = -(fg + dfg * C);
		Eigen::Vector<T, Eigen::Dynamic> phiFuncTwo = fg - dfg * C;
		Eigen::Vector<T, Eigen::Dynamic> phiOnly;
		Eigen::Vector<T, Eigen::Dynamic> phiTDF;

		if (IOSDFOne.size() == 0)
		{
			phiOnly = (dfg * C).array().pow(2) - fg.array().pow(2);
			std::cout << ("here8");
		}
		else
		{
			phiOnly = shellSDFOne.cwiseMin(shellSDFTwo).cwiseMin(Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>((dfg * C).array().pow(2) - fg.array().pow(2)));
			phiFuncOne = shellSDFOne.cwiseMin(phiFuncOne);
			phiFuncTwo = shellSDFTwo.cwiseMin(phiFuncTwo);
			std::cout << ("here9");
		}

		if (TDF.size() == 0)
		{
			if (IOSDFOne.size() == 0)
			{
				phiTDF = -phiFuncOne.cwiseMax(phiFuncTwo);
				std::cout << ("here10");
			}
			else
			{
				phiFuncOne = IOSDFOne.cwiseMax(phiFuncOne);
				phiFuncTwo = IOSDFTwo.cwiseMax(phiFuncTwo);
				phiTDF = -phiFuncOne.cwiseMax(phiFuncTwo);
				std::cout << ("here11");
			}
		}
		else
		{
			std::cout << ("here12");
			phiFuncOne = IOSDFOne.cwiseMax(phiFuncOne).cwiseMin(TDF);
			std::cout << ("here13");
			phiFuncTwo = IOSDFTwo.cwiseMax(phiFuncTwo).cwiseMin(TDF);
			std::cout << ("here14");
			phiTDF = (-phiFuncOne.cwiseMax(phiFuncTwo)).cwiseMin(TDF);
			std::cout << ("here15");
		}

		return std::make_tuple(phiFuncOne, phiFuncTwo, phiOnly, phiTDF);
}

	////template<class T>
	////TPMS<T>::TPMSImpl::~TPMSImpl() = default;

	////template<class T>
	////std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>> TPMS<T>::TPMSImpl::GetValues() const
	////{
	////	return std::make_tuple(phiFuncOne, phiFuncTwo, phiOnly, phiTDF);
	////}

	//template<class T>
	//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
	//	Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
	//TPMS<T>::GenerateTPMS(const std::string& surfaceType, const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
	//			  Eigen::Vector<T, 3> center, const T period, const T thicknesses,
	//			  Eigen::Vector<T, Eigen::Dynamic> TDF,
	//			  Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, 
	//			  Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//			  Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, 
	//			  Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//	try
	//	{
	//		return TPMSImpl::GenerateTPMSImpl(surfaceType, hrNodes, center, period, thicknesses,
	//										   TDF, IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);
	//	
	//	}
	//	catch(const std::exception& e)
	//	{
	//		std::cerr << "[Error] TPMS calculated error!" << std::endl;
	//		throw(e);
	//	}
	//}

	//template<class T>
	//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
	//	Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
	//TPMS<T>::GenerateTPMS(const std::string& surfaceType, const T L, 
	//			const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
	//			Eigen::Vector<T, 3> center, const T period, T thicknesses, 
	//			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//	try
	//	{
	//		return TPMSImpl::GenerateTPMSImpl(surfaceType, L, hrNodes, center, period, thicknesses,
	//											IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);

	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << "[Error] TPMS calculated error!" << std::endl;
	//		throw(e);
	//	}
	//}


	template<class T>
	std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>,
		Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>>
		TPMS<T>::GenerateTPMSModified(const std::string& surfaceType,
			const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes,
			Eigen::Vector<T, 3> center, const T period, const T thicknesses,
			Eigen::Vector<T, Eigen::Dynamic> TDF,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne,
			Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	{
		try
		{
			return TPMSImpl::GenerateTPMSModifiedImpl(surfaceType, hrNodes, center, period, thicknesses,
													TDF, IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);

		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] TPMS calculated error!" << std::endl;
			throw(e);
		}
	}

	//template<class T>
	//TPMS<T>::TPMS(const std::string& surfaceType, const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
	//			  Eigen::Vector<T, 3> center, const T period, const T thicknesses,
	//			  Eigen::Vector<T, Eigen::Dynamic> TDF,
	//			  Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, 
	//			  Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//			  Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, 
	//		      Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//	try
	//	{
	//		pImpl = std::make_unique<TPMSImpl>(surfaceType, hrNodes, center, period, thicknesses,
	//										   TDF, IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);
	//		
	//	}
	//	catch(const std::exception& e)
	//	{
	//		std::cerr << "[Error] TPMS calculated error!" << std::endl;
	//		throw(e);
	//	}
	//}

	//template<class T>
	//TPMS<T>::TPMS(const std::string& surfaceType, const T L, 
	//			const Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes, 
	//			Eigen::Vector<T, 3> center, const T period, T thicknesses, 
	//			Eigen::Vector<T, Eigen::Dynamic> IOSDFOne, Eigen::Vector<T, Eigen::Dynamic> IOSDFTwo, 
	//			Eigen::Vector<T, Eigen::Dynamic> shellSDFOne, Eigen::Vector<T, Eigen::Dynamic> shellSDFTwo)
	//{
	//	try
	//	{
	//		pImpl = std::make_unique<TPMSImpl>(surfaceType, L, hrNodes, center, period, thicknesses,
	//											IOSDFOne, IOSDFTwo, shellSDFOne, shellSDFTwo);

	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << "[Error] TPMS calculated error!" << std::endl;
	//		throw(e);
	//	}
	//}


	//template<class T>
	//TPMS<T>::~TPMS() = default;

	//template<class T>
	//std::tuple<Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>, Eigen::Vector<T, Eigen::Dynamic>> TPMS<T>::GetValues() const
	//{
	//	return pImpl->GetValues();
	//}

}