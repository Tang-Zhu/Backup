#include "RoundedCylinder.h"
#include <opencascade/BRepPrimAPI_MakeCylinder.hxx>
#include <opencascade/BRepAlgoAPI_Fuse.hxx>
#include <opencascade/BRepBuilderAPI_Transform.hxx>
#include <opencascade/BRepPrimAPI_MakeSphere.hxx>

namespace lattice
{
	class RoundedCylinder::RoundedCylinderImpl
	{
	public:
		RoundedCylinderImpl(const Standard_Real R, const Standard_Real L, RoundedCylinder& base);
		RoundedCylinderImpl(const gp_Ax2& ax, Standard_Real R, Standard_Real L, RoundedCylinder& base);
		~RoundedCylinderImpl();
		RoundedCylinderImpl(const RoundedCylinderImpl& other);
		RoundedCylinderImpl& operator = (const RoundedCylinderImpl& other);
		
		Standard_Real GetRadius();
		Standard_Real GetLength();
;
	private:
		RoundedCylinderImpl() = delete;

	private:
		Standard_Real R;
		Standard_Real L;

	};

	RoundedCylinder::RoundedCylinderImpl::RoundedCylinderImpl(const gp_Ax2& ax, const Standard_Real R, const Standard_Real L, RoundedCylinder& base)
	{
		this->R = R;
		this->L = L;
		 base.SetShape(BRepAlgoAPI_Fuse(BRepAlgoAPI_Fuse(BRepPrimAPI_MakeCylinder(ax, R, L).Shape(), BRepPrimAPI_MakeSphere(ax, R).Shape()),
			BRepPrimAPI_MakeSphere(gp_Ax2(ax.Location().Translated(gp_Vec(ax.Direction()) * L), ax.Direction()), R).Shape()).Shape());
	}

	RoundedCylinder::RoundedCylinderImpl::RoundedCylinderImpl(const Standard_Real R, const Standard_Real L, RoundedCylinder& base)
	{
		this->R = R;
		this->L = L;
		gp_Ax2 ax(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
		base.SetShape(BRepAlgoAPI_Fuse(BRepAlgoAPI_Fuse(BRepPrimAPI_MakeCylinder(ax, R, L).Shape(), BRepPrimAPI_MakeSphere(ax, R).Shape()),
			BRepPrimAPI_MakeSphere(gp_Ax2(ax.Location().Translated(gp_Vec(ax.Direction()) * L), ax.Direction()), R).Shape()).Shape());
	}

	RoundedCylinder::RoundedCylinderImpl::~RoundedCylinderImpl() = default;

	RoundedCylinder::RoundedCylinderImpl::RoundedCylinderImpl(const RoundedCylinderImpl& other)
	{
		R = other.R;
		L = other.L;
	}

	RoundedCylinder::RoundedCylinderImpl& RoundedCylinder::RoundedCylinderImpl::operator= (const RoundedCylinderImpl& other)
	{
		R = other.R;
		L = other.L;
	}

	Standard_Real RoundedCylinder::RoundedCylinderImpl::GetRadius()
	{
		return R;
	}

	Standard_Real RoundedCylinder::RoundedCylinderImpl::GetLength()
	{
		return L;
	}

	RoundedCylinder::RoundedCylinder(const Standard_Real R, const Standard_Real L) : Base()
	{
		try
		{
			pImpl = std::make_unique<RoundedCylinderImpl>(R, L, *this);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Generating rounded cylinder error!" << std::endl;
			throw(e);
		}
	}

	RoundedCylinder::RoundedCylinder(const gp_Ax2& ax, const Standard_Real R, const Standard_Real L) : Base()
	{
		try
		{
			pImpl = std::make_unique<RoundedCylinderImpl>(ax, R, L, *this);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Generating rounded cylinder error!" << std::endl;
			throw(e);
		}
	}

	RoundedCylinder::~RoundedCylinder() = default;

	RoundedCylinder::RoundedCylinder(const RoundedCylinder& other) : Base(other)
	{
		if (this != &other)
		{
			if (other.pImpl)
			{
				try
				{
					pImpl = std::make_unique<RoundedCylinderImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Generating rounded cylinder error!");
				}
			}
			else
				pImpl = nullptr;
		}
	}

	RoundedCylinder& RoundedCylinder::operator=(const RoundedCylinder& other) 
	{
		if (this != &other)
		{
			Base::operator=(other);
			if (!other.pImpl)
				pImpl = nullptr;

			else
			{
				if (pImpl)
					*pImpl = *other.pImpl;
				else
					try
				{
					pImpl = std::make_unique<RoundedCylinderImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Generating rounded cylinder error!");
				}
			}
		}
		return *this;
	}

	Standard_Real RoundedCylinder::GetRadius()
	{
		return pImpl->GetRadius();
	}

	Standard_Real RoundedCylinder::GetLength()
	{
		return pImpl->GetLength();
	}

}
