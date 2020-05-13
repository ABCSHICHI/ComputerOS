#include <windows.h>
#include <iostream>
#include <Shlwapi.h>
#include <iomanip>
using namespace std;
#pragma comment(lib, "Shlwapi.lib")

//以可读方式对用户显示保护的辅助
//保护标记表示允许应用程序对内存进行访问的类型
//以及操作系统强制访问的类型
inline bool TestSet(DWORD dwTarget, DWORD dwMask) {
	return((dwTarget&dwMask) == dwMask);
}

#define SHOWMASK(dwTarget, type)\
if (TestSet(dwTarget, PAGE_##type))\
	{	cout << "," << #type;	}//宏定义（#代表转化为字符串，##起连接作用）
void ShowProtection(DWORD dwTarget)
{
	SHOWMASK(dwTarget, READONLY);
	SHOWMASK(dwTarget, GUARD);
	SHOWMASK(dwTarget, NOCACHE);
	SHOWMASK(dwTarget, READWRITE);
	SHOWMASK(dwTarget, WRITECOPY);
	SHOWMASK(dwTarget, EXECUTE);
	SHOWMASK(dwTarget, EXECUTE_READ);
	SHOWMASK(dwTarget, EXECUTE_READWRITE);
	SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
	SHOWMASK(dwTarget, NOACCESS);
}

//遍历整个虚拟内存并对用户显示其属性
void WalkVM(HANDLE hProcess) {
	// 首先，获得系统信息
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	// 分配要存放信息的缓冲区
	MEMORY_BASIC_INFORMATION mbi;  // MEMORY_BASIC_INFORMATION结构体包含内存区域的信息
	ZeroMemory(&mbi, sizeof(mbi));

	// 循环遍历整个应用程序地址空间
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		// 获得下一个虚拟内存区域的信息(具有相同属性的相邻内存页面构成一个区域)
		if (VirtualQueryEx(
			hProcess,					// 相关进程的句柄
			pBlock,                     // 开始位置
			&mbi,                       // 用于存放信息的结构体指针
			sizeof(mbi)) == sizeof(mbi))	// VirtualQueryEx返回值为获取信息的大小,等于结构体大小则获取成功
		{
			// 计算区域的结尾及区域大小
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];				// 用于存放区域大小的字符串
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);

			// 显示区域起止地址和大小
			cout.fill('0');					// 填充字符为'0'
			cout
				<< hex << setw(8) << (DWORD)pBlock // 区域起始地址
				<< "-"
				<< hex << setw(8) << (DWORD)pEnd	// 区域终止地址
				<< " (" << szSize << ") ";			// 区域大小

			// 显示区域的状态
			switch (mbi.State)
			{
			case MEM_COMMIT:					// 已分配内存
				cout << "Committed";
				break;
			case MEM_FREE:						// 空闲内存
				cout << "Free";
				break;
			case MEM_RESERVE:					// 保留区
				cout << "Reserved";
				break;
			}

			// 显示保护信息(对空闲区无定义)
			ShowProtection(mbi.Protect);

			// 显示区域类型
			switch (mbi.Type) {
			case MEM_IMAGE: // 该区域的虚拟地址原先受内存映射的映像文件（如.exe或DLL文件）的支持。
				cout << ", Image";
				break;
			case MEM_MAPPED: // 该区域的虚拟地址原先是受内存映射的数据文件的支持。
				cout << ", Mapped";
				break;
			case MEM_PRIVATE: // 私有区
				cout << ", Private";
				break;
			}

			// 检验可执行的影像
			TCHAR szFilename[MAX_PATH];
			if (GetModuleFileName(	// 获取模块文件名，获取成功则返回值为获取的字符串长度，获取失败返回0
				(HMODULE)pBlock,	// 实际虚拟内存的模块句柄(句柄其实就是指针)
				szFilename,         // 用于保存完整文件路径名的字符缓冲区
				MAX_PATH) > 0)      // 字符缓冲区大小
			{
				// 除去路径并显示
				PathStripPath(szFilename);
				cout << ", Module: " << szFilename;
			}

			cout << endl;

			// 移动区域指针以获得下一下个区域
			pBlock = pEnd;
		}
	}
}

void main() {
	//遍历当前进程
	WalkVM(GetCurrentProcess());
}