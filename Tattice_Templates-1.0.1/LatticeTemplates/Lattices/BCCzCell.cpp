#include "BCCzCell.h"
#include <opencascade/BRepAlgoAPI_Fuse.hxx>

namespace lattice
{
	class BCCzCell::BCCzCellImpl
	{
	public:
		static TopoDS_Shape BCCzWithRoundedCylinderImpl(Standard_Real R, Standard_Real H);
	private:
		BCCzCellImpl() = delete;
		~BCCzCellImpl() = delete;
		BCCzCellImpl(const BCCzCellImpl&) = delete;
		BCCzCellImpl& operator = (const BCCzCellImpl&) = delete;
	};

	TopoDS_Shape BCCzCell::BCCzCellImpl::BCCzWithRoundedCylinderImpl(Standard_Real R, Standard_Real H)
	{
		return BRepAlgoAPI_Fuse(BCCWithRoundedCylinder(R, H), SCWithRoundedCylinder(R, H));
	}

	TopoDS_Shape BCCzCell::BCCzWithRoundedCylinder(Standard_Real R, Standard_Real H)
	{
		try
		{
			return BCCzCellImpl::BCCzWithRoundedCylinderImpl(R, H);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Generating SC cell with rounded cylinder error!" << std::endl;
			throw(e);
		}
	}
}