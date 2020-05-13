#include <windows.h>
#include <iostream>
#include <Shlwapi.h>
#include <iomanip>
using namespace std;
#pragma comment(lib, "Shlwapi.lib")

//�Կɶ���ʽ���û���ʾ�����ĸ���
//������Ǳ�ʾ����Ӧ�ó�����ڴ���з��ʵ�����
//�Լ�����ϵͳǿ�Ʒ��ʵ�����
inline bool TestSet(DWORD dwTarget, DWORD dwMask) {
	return((dwTarget&dwMask) == dwMask);
}

#define SHOWMASK(dwTarget, type)\
if (TestSet(dwTarget, PAGE_##type))\
	{	cout << "," << #type;	}//�궨�壨#����ת��Ϊ�ַ�����##���������ã�
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

//�������������ڴ沢���û���ʾ������
void WalkVM(HANDLE hProcess) {
	// ���ȣ����ϵͳ��Ϣ
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	// ����Ҫ�����Ϣ�Ļ�����
	MEMORY_BASIC_INFORMATION mbi;  // MEMORY_BASIC_INFORMATION�ṹ������ڴ��������Ϣ
	ZeroMemory(&mbi, sizeof(mbi));

	// ѭ����������Ӧ�ó����ַ�ռ�
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		// �����һ�������ڴ��������Ϣ(������ͬ���Ե������ڴ�ҳ�湹��һ������)
		if (VirtualQueryEx(
			hProcess,					// ��ؽ��̵ľ��
			pBlock,                     // ��ʼλ��
			&mbi,                       // ���ڴ����Ϣ�Ľṹ��ָ��
			sizeof(mbi)) == sizeof(mbi))	// VirtualQueryEx����ֵΪ��ȡ��Ϣ�Ĵ�С,���ڽṹ���С���ȡ�ɹ�
		{
			// ��������Ľ�β�������С
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];				// ���ڴ�������С���ַ���
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);

			// ��ʾ������ֹ��ַ�ʹ�С
			cout.fill('0');					// ����ַ�Ϊ'0'
			cout
				<< hex << setw(8) << (DWORD)pBlock // ������ʼ��ַ
				<< "-"
				<< hex << setw(8) << (DWORD)pEnd	// ������ֹ��ַ
				<< " (" << szSize << ") ";			// �����С

			// ��ʾ�����״̬
			switch (mbi.State)
			{
			case MEM_COMMIT:					// �ѷ����ڴ�
				cout << "Committed";
				break;
			case MEM_FREE:						// �����ڴ�
				cout << "Free";
				break;
			case MEM_RESERVE:					// ������
				cout << "Reserved";
				break;
			}

			// ��ʾ������Ϣ(�Կ������޶���)
			ShowProtection(mbi.Protect);

			// ��ʾ��������
			switch (mbi.Type) {
			case MEM_IMAGE: // ������������ַԭ�����ڴ�ӳ���ӳ���ļ�����.exe��DLL�ļ�����֧�֡�
				cout << ", Image";
				break;
			case MEM_MAPPED: // ������������ַԭ�������ڴ�ӳ��������ļ���֧�֡�
				cout << ", Mapped";
				break;
			case MEM_PRIVATE: // ˽����
				cout << ", Private";
				break;
			}

			// �����ִ�е�Ӱ��
			TCHAR szFilename[MAX_PATH];
			if (GetModuleFileName(	// ��ȡģ���ļ�������ȡ�ɹ��򷵻�ֵΪ��ȡ���ַ������ȣ���ȡʧ�ܷ���0
				(HMODULE)pBlock,	// ʵ�������ڴ��ģ����(�����ʵ����ָ��)
				szFilename,         // ���ڱ��������ļ�·�������ַ�������
				MAX_PATH) > 0)      // �ַ���������С
			{
				// ��ȥ·������ʾ
				PathStripPath(szFilename);
				cout << ", Module: " << szFilename;
			}

			cout << endl;

			// �ƶ�����ָ���Ի����һ�¸�����
			pBlock = pEnd;
		}
	}
}

void main() {
	//������ǰ����
	WalkVM(GetCurrentProcess());
}