#pragma once
#include "Cylinder.h"
#include "BCCCell.h"
#include "SCCell.h"

namespace lattice
{
	//BCCz������BCC�ṹ�Ŀ����SC�ṹ���ڲ��ṹ�����ʹ�ü̳�ֱ�ӽ����ֽṹ��ϼ���
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