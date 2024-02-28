#include "Base.h"
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/Poly_Triangulation.hxx>
#include <opencascade/BRepBuilderAPI_MakeShapeOnMesh.hxx>

namespace lattice
{
	class Base::BaseImpl
	{
	public:
        BaseImpl();
        ~BaseImpl();

		BRepMesh_IncrementalMesh GenerateMesh(const Standard_Real lineDeflection,
										    const Standard_Boolean isRelative,
										    const Standard_Real    angleDeflection,
										    const Standard_Boolean isInParallel); 

        TopoDS_Shape Triangulation(const Standard_Real lineDeflection,
                                    const Standard_Boolean isRelative,
                                    const Standard_Real    angleDeflection,
                                    const Standard_Boolean isInParallel);

        TopoDS_Shape GetShape();
        void SetShape(const TopoDS_Shape& shape);
        BaseImpl(const BaseImpl& other);
	private:
        TopoDS_Shape shape;
	};

    BRepMesh_IncrementalMesh Base::BaseImpl::GenerateMesh(const Standard_Real lineDeflection,
										                const Standard_Boolean isRelative,
										                const Standard_Real    angleDeflection,
										                const Standard_Boolean isInParallel)
	{
		BRepMesh_IncrementalMesh mesh(shape, lineDeflection, isRelative, angleDeflection, isInParallel);
        if (!mesh.IsDone())
            throw("[Error] Generate mesh faild!");
        shape = mesh.Shape();
        return mesh;
	}


    TopoDS_Shape Base::BaseImpl::Triangulation(const Standard_Real lineDeflection,
                                                const Standard_Boolean isRelative,
                                                const Standard_Real angleDeflection,
                                                const Standard_Boolean isInParallel)
	{
        GenerateMesh(lineDeflection, isRelative, angleDeflection, isInParallel);
        Standard_Integer nodes = 0;
        Standard_Integer triangles = 0;

        // calculate total number of the nodes and triangles
        for (TopExp_Explorer ExpSF(shape, TopAbs_FACE); ExpSF.More(); ExpSF.Next())
        {
            TopLoc_Location location;
            Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(TopoDS::Face(ExpSF.Current()), location);
            if (!triangulation.IsNull())
            {
                nodes += triangulation->NbNodes();
                triangles += triangulation->NbTriangles();
            }
        }

        if (triangles == 0)
        {
            // No triangulation on the shape
            TopoDS_Shape tmp;
            tmp.Nullify();
            return tmp;
        }

        // create temporary triangulation
        Handle(Poly_Triangulation) polys = new Poly_Triangulation(nodes, triangles, Standard_False);
        // count faces missing triangulation
        Standard_Integer facesNoTri = 0;
        // fill temporary triangulation
        Standard_Integer nodeOffset = 0;
        Standard_Integer triangleOffet = 0;
        for (TopExp_Explorer ExpSF(shape, TopAbs_FACE); ExpSF.More(); ExpSF.Next())
        {
            const TopoDS_Shape& face = ExpSF.Current();
            TopLoc_Location location;
            Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(TopoDS::Face(face), location);
            if (triangulation.IsNull())
            {
                ++facesNoTri;
                continue;
            }

            // copy nodes
            gp_Trsf transformation = location.Transformation();
            for (Standard_Integer aNodeIter = 1; aNodeIter <= triangulation->NbNodes(); ++aNodeIter)
            {
                gp_Pnt point = triangulation->Node(aNodeIter);
                point.Transform(transformation);
                polys->SetNode(aNodeIter + nodeOffset, point);
            }

            // copy triangles
            const TopAbs_Orientation orientation = ExpSF.Current().Orientation();
            for (Standard_Integer triIter = 1; triIter <= triangulation->NbTriangles(); ++triIter)
            {
                Poly_Triangle triangle = triangulation->Triangle(triIter);

                Standard_Integer index[3];
                triangle.Get(index[0], index[1], index[2]);
                if (orientation == TopAbs_REVERSED)
                {
                    // Swap 1, 2.
                    Standard_Integer tmpIdx = index[1];
                    index[1] = index[2];
                    index[2] = tmpIdx;
                }

                // Update nodes according to the offset.
                index[0] += nodeOffset;
                index[1] += nodeOffset;
                index[2] += nodeOffset;

                triangle.Set(index[0], index[1], index[2]);
                polys->SetTriangle(triIter + triangleOffet, triangle);
            }

            nodeOffset += triangulation->NbNodes();
            triangleOffet += triangulation->NbTriangles();
        }

        BRepBuilderAPI_MakeShapeOnMesh converter(polys);
        converter.Build();
        return converter.Shape();
	}

    Base::BaseImpl::BaseImpl()
    {
        shape.Nullify();
    }

    Base::BaseImpl::~BaseImpl() = default;

    TopoDS_Shape Base::BaseImpl::GetShape()
    {
        return shape;
    }

    void Base::BaseImpl::SetShape(const TopoDS_Shape& shape)
    {
        this->shape = shape;
    }

    Base::BaseImpl::BaseImpl(const BaseImpl& other)
    {
        if (this != &other)
        {
            shape = other.shape;
        }
    }

    BRepMesh_IncrementalMesh Base::GenerateMesh(const Standard_Real lineDeflection,
									            const Standard_Boolean isRelative,
									            const Standard_Real    angleDeflection,
									            const Standard_Boolean isInParallel)
	{
		try
		{
			return pImpl->GenerateMesh(lineDeflection, isRelative, angleDeflection, isInParallel);
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Error] Triangularize shape faild!" << std::endl;
			throw(e);
		}
	}

    TopoDS_Shape Base::Triangulation(const Standard_Real lineDeflection,
                                    const Standard_Boolean isRelative,
                                    const Standard_Real    angleDeflection,
                                    const Standard_Boolean isInParallel)
    {
        try
        {
            return pImpl->Triangulation(lineDeflection, isRelative, angleDeflection, isInParallel);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Error] Triangularize shape faild!" << std::endl;
            throw(e);
        }
    }

    TopoDS_Shape Base::GetShape()
    {
        return pImpl->GetShape();
    }

    void Base::SetShape(const TopoDS_Shape& shape)
    {
        pImpl->SetShape(shape);
    }

    Base::Base()
    {
        try
        {
            pImpl = std::make_unique<BaseImpl>();
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Error] Base error!" << std::endl;
            throw(e);
        }
    }

    Base::~Base() = default;

    Base::Base(const Base& other)
    {
        if (this != &other)
        {
            if (other.pImpl)
            {
                try
                {
                    pImpl = std::make_unique<BaseImpl>(*other.pImpl);
                }
                catch (const std::exception& e)
                {
                    throw("{Error} Base error!");
                }
            }
            else
                pImpl = nullptr;
        }
    }

    Base& Base::operator=(const Base& other)
    {
        if (this != &other)
        {
            if (!other.pImpl)
                pImpl = nullptr;

            else
            {
                if (pImpl)
                    *pImpl = *other.pImpl;
                else
                    try
                    {
                        pImpl = std::make_unique<BaseImpl>(*other.pImpl);
                    }
                    catch (const std::exception& e)
                    {
                        throw("{Error} Base error!");
                    }
            }
        }
        return *this;
    }

}