#pragma once
#include "Base.h"
#include <opencascade/gp_Ax2.hxx>

namespace lattice
{
	class RoundedCylinder : public Base
	{
	public:
		RoundedCylinder(const Standard_Real R, const Standard_Real L);
		RoundedCylinder(const gp_Ax2& ax, const Standard_Real R, const Standard_Real L);
		~RoundedCylinder();
		RoundedCylinder(const RoundedCylinder& other);
		RoundedCylinder& operator = (const RoundedCylinder& other);

		Standard_Real GetRadius();
		Standard_Real GetLength();

	private:
		class RoundedCylinderImpl;
		std::unique_ptr<RoundedCylinderImpl> pImpl;
		RoundedCylinder() = delete;		
	};
}
