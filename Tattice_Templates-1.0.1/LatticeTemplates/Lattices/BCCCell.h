#pragma once
#include "RoundedCylinder.h"

namespace lattice
{
	class BCCCell : public RoundedCylinder
	{
	public:
		BCCCell(const Standard_Real R, const Standard_Real L);
		~BCCCell();
		BCCCell(const BCCCell& other);
		BCCCell& operator = (const BCCCell& other);

	private:
		class BCCCellImpl;
		std::unique_ptr<BCCCellImpl> pImpl;
		BCCCell() = delete;
	};
}