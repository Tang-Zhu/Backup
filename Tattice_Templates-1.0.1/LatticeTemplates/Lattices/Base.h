#pragma once
#include <memory>
#include <opencascade/TopoDS.hxx>
#include <opencascade/BRepMesh_IncrementalMesh.hxx>

namespace lattice
{

	class Base
	{
	public:
		BRepMesh_IncrementalMesh GenerateMesh(const Standard_Real lineDeflection = 0.001,
											const Standard_Boolean isRelative = Standard_False,
											const Standard_Real    angleDeflection = 0.5,
											const Standard_Boolean isInParallel = Standard_True);
		TopoDS_Shape Triangulation(const Standard_Real lineDeflection = 0.001,
									const Standard_Boolean isRelative = Standard_False,
									const Standard_Real    angleDeflection = 0.5,
									const Standard_Boolean isInParallel = Standard_True);
		TopoDS_Shape GetShape();
		void SetShape(const TopoDS_Shape& shape);
		Base();
		~Base();
		Base(const Base& other);
		Base& operator = (const Base& other);
	private:
		class BaseImpl;
		std::unique_ptr<BaseImpl> pImpl;
	};
}