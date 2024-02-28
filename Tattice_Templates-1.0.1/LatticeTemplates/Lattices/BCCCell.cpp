#include "BCCCell.h"
#include <opencascade/BRepAlgoAPI_BuilderAlgo.hxx>

namespace lattice
{
	class BCCCell::BCCCellImpl
	{
	public:
		BCCCellImpl(const Standard_Real R, const Standard_Real L, BCCCell& base);
		~BCCCellImpl();
		BCCCellImpl(const BCCCellImpl& other);
		BCCCellImpl& operator = (const BCCCellImpl& other);

	private:
		BCCCellImpl() = delete;
	};

	BCCCell::BCCCellImpl::BCCCellImpl(const Standard_Real R, const Standard_Real L, BCCCell& base)
	{
		Standard_Real LL = Sqrt(3) * L;
		TopoDS_ListOfShape cylinderList;

		cylinderList.Append(lattice::RoundedCylinder(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(L, L, L)), R, LL).GetShape());
		cylinderList.Append(lattice::RoundedCylinder(gp_Ax2(gp_Pnt(L, 0, 0), gp_Dir(-L, L, L)), R, LL).GetShape());
		cylinderList.Append(lattice::RoundedCylinder(gp_Ax2(gp_Pnt(0, L, 0), gp_Dir(L, -L, L)), R, LL).GetShape());
		cylinderList.Append(lattice::RoundedCylinder(gp_Ax2(gp_Pnt(L, L, 0), gp_Dir(-L, -L, L)), R, LL).GetShape());

		BRepAlgoAPI_BuilderAlgo builder;
		builder.SetArguments(cylinderList);
		builder.Build();
		base.SetShape(builder.Shape());
	}

	BCCCell::BCCCellImpl::~BCCCellImpl() = default;

	BCCCell::BCCCellImpl::BCCCellImpl(const BCCCellImpl& other) = default;

	BCCCell::BCCCellImpl& BCCCell::BCCCellImpl::operator=(const BCCCellImpl& other) = default;


	BCCCell::BCCCell(const Standard_Real R, const Standard_Real L) : RoundedCylinder(R, L)
	{
		try
		{
			pImpl = std::make_unique<BCCCellImpl>(R, L, *this);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Generating BCC Cell error!" << std::endl;
			throw(e);
		}
	}
	BCCCell::~BCCCell() = default;

	BCCCell::BCCCell(const BCCCell& other) : RoundedCylinder(other)
	{
		if (this != &other)
		{
			if (other.pImpl)
			{
				try
				{
					pImpl = std::make_unique<BCCCellImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Generating BCC Cell error!");
				}
			}
			else
				pImpl = nullptr;
		}
	}

	BCCCell& BCCCell::operator=(const BCCCell& other)
	{

		if (this != &other)
		{
			RoundedCylinder::operator=(other);
			if (!other.pImpl)
				pImpl = nullptr;

			else
			{
				if (pImpl)
					*pImpl = *other.pImpl;
				else
					try
				{
					pImpl = std::make_unique<BCCCellImpl>(*other.pImpl);
				}
				catch (const std::exception& e)
				{
					throw("{Error} Generating rounded cylinder error!");
				}
			}
		}
		return *this;
	}
}