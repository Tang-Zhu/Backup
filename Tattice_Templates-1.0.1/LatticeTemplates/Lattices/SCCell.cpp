#include "SCCell.h"
#include <opencascade/BRepAlgoAPI_BuilderAlgo.hxx>

namespace lattice
{
	class SCCell::SCCellImpl
	{
	public:
		static TopoDS_Shape SCWithRoundedCylinderImpl(Standard_Real R, Standard_Real H);
	private:
		SCCellImpl() = delete;
		~SCCellImpl() = delete;
		SCCellImpl(const SCCellImpl&) = delete;
		SCCellImpl& operator = (const SCCellImpl&) = delete;
	};

	TopoDS_Shape SCCell::SCCellImpl::SCWithRoundedCylinderImpl(Standard_Real R, Standard_Real H)
	{
		TopoDS_ListOfShape cylinderList;
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, H, 0), gp_Dir(1, 0, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(H, 0, 0), gp_Dir(0, 1, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(H, 0, 0), gp_Dir(0, 0, 1)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, H), gp_Dir(1, 0, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, H), gp_Dir(0, 1, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, H, 0), gp_Dir(0, 0, 1)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(0, H, H), gp_Dir(1, 0, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(H, 0, H), gp_Dir(0, 1, 0)), R, H));
		cylinderList.Append(lattice::RoundedCylinder::RoundedCylinder(gp_Ax2(gp_Pnt(H, H, 0), gp_Dir(0, 0, 1)), R, H));

		BRepAlgoAPI_BuilderAlgo builder;
		builder.SetArguments(cylinderList);
		builder.Build();
		return builder.Shape();
	}

	TopoDS_Shape SCCell::SCWithRoundedCylinder(Standard_Real R, Standard_Real H)
	{
		try
		{
			return SCCellImpl::SCWithRoundedCylinderImpl(R, H);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Generating SC cell with rounded cylinder error!" << std::endl;
			throw(e);
		}
	}
}