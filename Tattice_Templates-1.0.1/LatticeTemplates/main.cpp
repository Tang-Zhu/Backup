#include "Lattices/RoundedCylinder.h"
//#include "Lattices/SCCell.h"
#include "Lattices/BCCCell.h"
//#include "Lattices/BCCzCell.h"
#include "Lattices/Base.h"
#include <opencascade/StlAPI.hxx>
#include <opencascade/BRepAlgoAPI_Fuse.hxx>
#include <opencascade/RWStl.hxx>
#include <opencascade/BRepMesh_IncrementalMesh.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/BRep_Tool.hxx>
#include <opencascade/OSD_Path.hxx>
#include <opencascade/BRepPrimAPI_MakeCylinder.hxx>
#include <opencascade/BRepBuilderAPI_Transform.hxx>
#include <opencascade/BRepPrimAPI_MakeSphere.hxx>
#include <opencascade/DE_Wrapper.hxx>
#include <opencascade/BRepTools.hxx>
#include <opencascade/BRepBuilderAPI_MakeShapeOnMesh.hxx>
#include <opencascade/BOPAlgo_Tools.hxx>
#include <opencascade/STEPControl_Writer.hxx>
#include <opencascade/BRepAlgoAPI_Cut.hxx>
#include <opencascade/BRepAlgoAPI_Common.hxx>
#include <opencascade/ShapeFix_Shape.hxx>
#include <opencascade/BRepCheck_Analyzer.hxx>
//#include <igl/read_triangle_mesh.h>
//#include <igl/write_triangle_mesh.h>
//#include <igl/copyleft/cgal/mesh_boolean.h>
//#include <Eigen/Core>
#include <string>
#include <ctime>

using namespace std;
//using namespace Eigen;

int main() {

    
    
    // 导出到STP文件，STP格式是3D打印格式可下载freeCAD查看
    /*STEPControl_Writer writer;
    writer.Transfer(lattices::SCCell::SCWithRoundedCylinder(R, H), STEPControl_AsIs);
    IFSelect_ReturnStatus status = writer.Write("SC.stp");*/


    //导出状态检测
    //if (status == IFSelect_RetDone) {
    //    std::cout << "Export successful." << std::endl;
    //}
    //else {
    //    std::cout << "Export failed." << std::endl;
    //}

    Standard_Real R = 2., H = 30;

    StlAPI writer;    
    double start = clock();
    lattice::BCCCell cy(R, H);
    lattice::BCCCell cy2 = cy;
    TopoDS_Shape inner = cy2.GetShape();
    cout << cy2.GetLength() << endl;
    cout << cy2.GetRadius() << endl;


   /* Handle(Poly_Triangulation) aMesh = RWStl::ReadFile("D:\\Projects\\Models\\Domain.stl");
    BRepBuilderAPI_MakeShapeOnMesh aConverter(aMesh);
    aConverter.Build();
    TopoDS_Shape outer = aConverter.Shape();

    Handle(Poly_Triangulation) aMesh2 = RWStl::ReadFile("inner.stl");
    BRepBuilderAPI_MakeShapeOnMesh aConverter2(aMesh2);
    aConverter2.Build();
    inner = aConverter2.Shape();*/


    //inner = lattice::Base::TriangulationDeTop(inner, 0.01);

    //TopoDS_Shape res = BRepAlgoAPI_Fuse(inner, outer).Shape();
    BRepMesh_IncrementalMesh mesh2(inner, 0.01);
    //res = mesh2.Shape();
    
    writer.Write(inner,"cy.stl");


    double end = clock();
    cout << end - start << "ms" << endl;

    //double start = clock();
    //MatrixX3d V1;
    //MatrixX3i F1;
    //igl::read_triangle_mesh("inner.stl", V1, F1);
    //MatrixX3d V2;
    //MatrixX3i F2;
    //igl::read_triangle_mesh("D:\\Projects\\Models\\Domain.stl", V2, F2);
    //igl::copyleft::cgal::mesh_boolean(V1, F1, V2, F2, igl::MESH_BOOLEAN_TYPE_XOR, V1, F1);
    //igl::write_triangle_mesh("mix2.stl", V1, F1);
    //double end = clock();
    //cout << end - start << "ms" << endl;
    return 0;
}

