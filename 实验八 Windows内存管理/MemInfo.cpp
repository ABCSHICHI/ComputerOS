#include <windows.h>//使用windows.API
#include <iostream>//标准输出流
#include <Shlwapi.h>//使用了strFormatByteSize
#include <iomanip>//使用io控制

using namespace std;
#pragma comment(lib, "shlwapi.lib")//链接库Shlwapi.lib

void main() {
	//首先，获取系统信息
	SYSTEM_INFO si;//SYSTEM_INFO结构体中包含计算机系统的相关信息
	ZeroMemory(&si, sizeof(si));//结构体清零
	GetSystemInfo(&si);//获取当前系统信息

	//使用外壳辅助程序对一些尺寸进行格式化
	TCHAR szPageSize[MAX_PATH];//字符串，用于保存页面大小，此处字符串长度其实和最大路径长度MAX_PATH没有关系
	StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);//将页面字节si.dwPageSize转换成字符串存于szPageSize
	DWORD dwMenSize = (DWORD)si.lpMaximumApplicationAddress -
		(DWORD)si.lpMinimumApplicationAddress; //计算内存可访问的虚拟地址空间大小
	TCHAR szMenSize[MAX_PATH];//字符串，用于保存虚拟地址空间大小，此处字符串长度其实和最大路径长度MAX_PATH没有关系
	StrFormatByteSize(dwMenSize, szMenSize, MAX_PATH);//将虚拟地址空间大小si.dwMenSize转换成字符串存于szMenSize

	TCHAR szAllocationGranularity[MAX_PATH];//字符串，用于保存虚拟地址空间大小，此处字符串长度其实和最大路径长度MAX_PATH没有关系
	StrFormatByteSize(si.dwAllocationGranularity, szAllocationGranularity, MAX_PATH);//将虚拟地址空间大小si.dwMenSize转换成字符串存于szMenSize

	//将内存信息显示出
	cout << "虚拟地址空间大小:" << szPageSize << endl;
	cout.fill('0');//将填充字符设置为‘0’；
	cout << "最小应用程序地址：0x" << hex << setw(8)
		<< (DWORD)si.lpMinimumApplicationAddress
		<< endl;
	cout << "最大应用程序地址：0x" << hex << setw(8)
		<< (DWORD)si.lpMaximumApplicationAddress
		<< endl;

	cout << "总共可用的虚拟内存：" << szMenSize << endl;

	cout << "内存分配粒度：" << szAllocationGranularity << endl;
	cout << "CPU核心数：" << si.dwNumberOfProcessors << endl;
}