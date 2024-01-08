#pragma once
#include <iostream>
#include <Eigen/Core>

namespace debug
{
	template<typename T>
	void ExportData(Eigen::Vector<T, Eigen::Dynamic> data, const std::string& pFile)
	{
		std::ofstream outputFile(pFile);

		if (outputFile.is_open()) {
			for (int i = 0; i < data.size(); i++) {
				outputFile << data[i] << "\n"; 
			}
			outputFile.close(); 
			std::cout << "Result saved to " + pFile + "\n";
		}
		else {
			std::cout << "Unable to open the file\n";
		}
	}
	template<typename T>
	void ExportData(Eigen::Matrix<T, Eigen::Dynamic, 3, Eigen::RowMajor> data, const std::string& pFile)
	{
		std::ofstream outputFile(pFile);

		if (outputFile.is_open()) {
			for (int i = 0; i < data.rows(); i++) {
				outputFile << data(i, 0) << ' ' << data(i, 1) << ' ' << data(i, 2) << "\n"; 
			}
			outputFile.close(); 
			std::cout << "Result saved to " + pFile + "\n";
		}
		else {
			std::cout << "Unable to open the file\n";
		}
	}
};
