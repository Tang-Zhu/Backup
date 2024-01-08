#include "HeatEx/Generator.h"
#include "Secure/Validate.h"
#include "Model/Model.h"
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/voxel_grid.h>
#include "SDF/SDF.h"
#include <iostream>

int main(int argc, char* argv[])
{
	//if (argc == 2 && std::string(argv[1]) == "help")
	//{
	//	std::cout << "参数请按顺序填写，可选参数不可跨参数填写，参考值仅供参考不为最优解，计算单位: 米" << std::endl;
	//	std::cout << "程序结果位于程序根目录下的Results文件夹中" << std::endl;
	//	std::cout << "目录输入'\\'时需要使用'\\\\'或'/'" << std::endl;
	//	std::cout << "1. 数据精度: 低精度-float, 高精度-double" << std::endl;
	//	std::cout << "2. 主容器网格文件" << std::endl;
	//	std::cout << "3. 热流体容器网格文件" << std::endl;
	//	std::cout << "4. 冷流体容器网格文件" << std::endl;
	//	std::cout << "5. TPMS类型: P, G, D, IWP" << std::endl;
	//	std::cout << "6. 单晶胞划分网格数（参考值: 8-12）" << std::endl;
	//	std::cout << "7. 各晶胞单元尺寸（参考值: 最低0.01, 内存不足优先调大该值）" << std::endl;
	//	std::cout << "8. TPMS厚度（参考值: 0.0001-0.002）" << std::endl;
	//	std::cout << "9. （可选）进出口处封闭流体结构的厚度（默认同TPMS厚度）" << std::endl;
	//	std::cout << "10. （可选）模型缩放比（默认不缩放）" << std::endl;
	//	std::cout << "11. （可选）SDF网格扩展（默认值: 2）" << std::endl;
	//	std::cout << "12. （可选）SDF类型（默认值: default, 可选: default, winding, pseudonormal）" << std::endl;
	//	std::cout << "13. （可选）网格缓存（默认值: false, false开启, true关闭，开启后同网格优先使用缓存，提高计算速度）" << std::endl;
	//	std::cout << "14. （可选）SDF缓存 （默认值: false, false开启, true关闭，开启后同SDF优先使用缓存，提高计算速度）" << std::endl;
	//	std::cout << "15. （可选）可视化（默认值: false, false开启, true关闭，将结果模型可视化，性能消耗较高）" << std::endl;
	//	std::exit(0);
	//}

	//if (argc < 8)
	//{
	//	std::cout << "输入参数太少" << std::endl;
	//	std::exit(0);
	//}

	////validate::LicenseValidate::Validate();

	//std::cout << "* Welcome to the software! *" << std::endl;

	//const std::string precision(argv[1]);
	//const std::string domainMesh(argv[2]);
	//const std::string hotFluidMesh(argv[3]);
	//const std::string coldFluidMesh(argv[4]);
	//std::string surfaceType(argv[5]);
	//int elementsNum = std::stoi(argv[6]);
	//double elementSize = std::stod(argv[7]);
	//double TPMSThickness = std::stod(argv[8]);
	//double shellThickness = TPMSThickness;
	//double scale = 1.;
	//int padCount = 2;
	//igl::SignedDistanceType signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT;
	//bool modelCache = false;
	//bool SDFCache = false;
	//bool show = false;

	//if (argc > 9)
	//	shellThickness = std::stod(argv[9]);
	//if (argc > 10)
	//	scale = std::stod(argv[10]);
	//if (argc > 11)
	//	padCount = std::stoi(argv[11]);
	//if (argc > 12)
	//{
	//	if (std::string(argv[12]) == "winding")
	//		signedType = igl::SIGNED_DISTANCE_TYPE_WINDING_NUMBER;
	//	if (std::string(argv[12]) == "pseudonormal")
	//		signedType = igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL;
	//	if (std::string(argv[12]) == "default")
	//		signedType = igl::SIGNED_DISTANCE_TYPE_DEFAULT;
	//}
	//if (argc > 13)
	//	if (std::string(argv[13]) == "true")
	//		modelCache = true;
	//if (argc > 14)
	//	if (std::string(argv[14]) == "true")
	//		SDFCache = true;
	//if (argc > 15)
	//	if (std::string(argv[15]) == "true")
	//		show = true;


	//std::cout << "精度：" << precision << std::endl;
	//std::cout << "主容器网格文件：" << domainMesh << std::endl;
	//std::cout << "热流体容器网格文件：" << hotFluidMesh << std::endl;
	//std::cout << "冷流体容器网格文件：" << coldFluidMesh << std::endl;
	//std::cout << "TPMS类型：" << surfaceType << std::endl;
	//std::cout << "单晶胞划分网格数：" << elementsNum << std::endl;
	//std::cout << "各晶胞单元尺寸：" << elementSize << std::endl;
	//std::cout << "TPMS厚度：" << TPMSThickness << std::endl;
	//std::cout << "进出口处封闭流体结构厚度：" << shellThickness << std::endl;
	//std::cout << "模型缩放比：" << scale << std::endl;
	//std::cout << "SDF网格扩展：" << padCount << std::endl;
	//std::cout << "SDF类型：" << signedType << std::endl;
	//std::cout << "网格缓存：" << (modelCache == false ? "False" : "True") << std::endl;
	//std::cout << "SDF缓存：" << (SDFCache == false ? "False" : "True") << std::endl;
	//std::cout << "可视化：" << (show == false ? "False" : "True") << std::endl;

	//if(precision == "float")
	//	heat_ex::Generator<float>::Generate(domainMesh, hotFluidMesh, coldFluidMesh, surfaceType, elementsNum, elementSize,
	//		TPMSThickness, shellThickness, scale, padCount, signedType, modelCache, SDFCache, show);
	//else
	//	heat_ex::Generator<double>::Generate(domainMesh, hotFluidMesh, coldFluidMesh, surfaceType, elementsNum, elementSize,
	//		TPMSThickness, shellThickness, scale, padCount, signedType, modelCache, SDFCache, show);


	const std::string file1 = "E:\\Projects\\Models\\Domain.stl"; 
	const std::string file2 = "E:\\Projects\\Models\\Domain_TPMS.stl";
	const std::string file3 = "E:\\Projects\\Models\\Hot.stl";
	const std::string file4 = "E:\\Projects\\Models\\Cold.stl";
	heat_ex::Generator<double>::Generate(file1, file2, file3, file4, 1000, "IWP", 12, 2, 100, 0.0006, igl::SIGNED_DISTANCE_TYPE_WINDING_NUMBER, true, true);
	//heat_ex::Generator<double>::Generate(file1, file3, file4, "D", 12, 0.01, 0.0006, 0.0001, 1000, 2);

	//model::Model<double> model1("G:\\DLUT\\assets\\models\\FluidOne.stl");
	//model::Model<double> model2("G:\\DLUT\\assets\\models\\FluidTwo.stl");
	//Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> v1 = model1.GetVertices();
	//Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> f1 = model1.GetFaces();
	//Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> v2 = model2.GetVertices();
	//Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> f2 = model2.GetFaces();
	//igl::copyleft::cgal::mesh_boolean(v1, f1, v2, f2, igl::MESH_BOOLEAN_TYPE_RESOLVE, v1, f1);
	//model::Model<double>::Export("test.stl", v1, f1);


	//model::Model<double> model(file3);
	//Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> vertices = model.GetVertices() / 1000;
	//Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> faces = model.GetFaces();

	//Eigen::RowVector<double, 3> shellMax;
	//Eigen::RowVector<double, 3> shellMin;
	//shellMax = vertices.colwise().maxCoeff();
	//shellMin = vertices.colwise().minCoeff();
	//Eigen::RowVector<double, 3> meshCenter = (shellMax + shellMin) / 2;

	//Eigen::AlignedBox<double, 3> SDFDomain(shellMin, shellMax);
	//Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> hrNodes;
	//Eigen::RowVector3i volNums;
	//igl::voxel_grid(SDFDomain, (shellMax - shellMin).maxCoeff() * 12 * 100, 2, hrNodes, volNums);
	//-sdf::SDF<double>(hrNodes, model.GetVertices() / 1000, model.GetFaces(), igl::SIGNED_DISTANCE_TYPE_WINDING_NUMBER).GetSmallestSignedDistances();
}


//#include <iostream>
//#include <fstream>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Polyhedron_3.h>
//#include <CGAL/Polygon_mesh_processing/corefinement.h>
//#include <CGAL/Boolean_set_operations_2.h>
//#include <CGAL/boost/graph/IO/STL.h>
//
//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
//typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
//
//int main() {
//    // 定义两个多边形网格对象
//    Polyhedron mesh1, mesh2;
//
//    // 使用CGAL的STL读取函数读取STL模型
//    const std::string filename1 = "G:\\DLUT\\assets\\models\\kitten_ascii.stl"; // 根据实际模型文件名修改
//    const std::string filename2 = "G:\\DLUT\\assets\\models\\FluidTwo.stl"; // 根据实际模型文件名修改
//
//    //std::ifstream input1(filename1, std::ios::binary);
//    //if (!input1) {
//    //    std::cerr << "无法打开文件：" << filename1 << std::endl;
//    //    return 1;
//    //}
//
//    //std::ifstream input2(filename2, std::ios::binary);
//    //if (!input2) {
//    //    std::cerr << "无法打开文件：" << filename2 << std::endl;
//    //    return 1;
//    //}
//
//    if (!CGAL::IO::read_STL(filename1, mesh1, CGAL::parameters::verbose(true))) {
//        std::cerr << "无法读取模型：" << filename1 << std::endl;
//        return 1;
//    }
//
//    if (!CGAL::IO::read_STL(filename2, mesh2)) {
//        std::cerr << "无法读取模型：" << filename2 << std::endl;
//        return 1;
//    }
//
//    // 执行布尔减法运算
//    Polyhedron result;
//    //CGAL::Polygon_mesh_processing::corefine_faces(mesh1);
//    //CGAL::Polygon_mesh_processing::corefine_faces(mesh2);
//   
//
//    // 将结果保存为STL模型
//    const std::string outputFilename = "result.stl"; // 根据实际需要修改输出文件名
//    //std::ofstream output(outputFilename, std::ios::binary);
//    //if (!output) {
//    //    std::cerr << "无法创建文件：" << outputFilename << std::endl;
//    //    return 1;
//    //}
//
//    CGAL::IO::write_STL(outputFilename, mesh1, CGAL::parameters::verbose(true));
// //CGAL::Polygon_mesh_processing::corefine_and_compute_difference(mesh1, mesh2, result);
//    std::cout << "布尔减法结果已保存为STL模型：" << outputFilename << std::endl;
//
//    return 0;
//}