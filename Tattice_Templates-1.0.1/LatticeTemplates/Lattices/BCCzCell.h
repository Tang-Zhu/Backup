#pragma once
#include "Cylinder.h"
#include "BCCCell.h"
#include "SCCell.h"

namespace lattice
{
	//BCCz包含了BCC结构的框体和SC结构的内部结构，因此使用继承直接将两种结构组合即可
	class BCCzCell : private BCCCell, SCCell
	{
	public:
		static TopoDS_Shape BCCzWithRoundedCylinder(Standard_Real R, Standard_Real H);
	private:
		class BCCzCellImpl;
		BCCzCell() = delete;
		~BCCzCell() = delete;
		BCCzCell(const BCCCell&) = delete;
		BCCzCell& operator = (const BCCzCell&) = delete;
	};
}