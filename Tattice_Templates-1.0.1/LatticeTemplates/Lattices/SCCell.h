#pragma once
#include "Cylinder.h"

namespace lattice
{

	class SCCell : private RoundedCylinder
	{
	public:
		static TopoDS_Shape SCWithRoundedCylinder(Standard_Real R, Standard_Real H);
	private:
		class SCCellImpl;
		SCCell() = delete;
		~SCCell() = delete;
		SCCell(const SCCell&) = delete;
		SCCell& operator = (const SCCell&) = delete;
	};
}