#include <windows.h>//ʹ��windows.API
#include <iostream>//��׼�����
#include <Shlwapi.h>//ʹ����strFormatByteSize
#include <iomanip>//ʹ��io����

using namespace std;
#pragma comment(lib, "shlwapi.lib")//���ӿ�Shlwapi.lib

void main() {
	//���ȣ���ȡϵͳ��Ϣ
	SYSTEM_INFO si;//SYSTEM_INFO�ṹ���а��������ϵͳ�������Ϣ
	ZeroMemory(&si, sizeof(si));//�ṹ������
	GetSystemInfo(&si);//��ȡ��ǰϵͳ��Ϣ

	//ʹ����Ǹ��������һЩ�ߴ���и�ʽ��
	TCHAR szPageSize[MAX_PATH];//�ַ��������ڱ���ҳ���С���˴��ַ���������ʵ�����·������MAX_PATHû�й�ϵ
	StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);//��ҳ���ֽ�si.dwPageSizeת�����ַ�������szPageSize
	DWORD dwMenSize = (DWORD)si.lpMaximumApplicationAddress -
		(DWORD)si.lpMinimumApplicationAddress; //�����ڴ�ɷ��ʵ������ַ�ռ��С
	TCHAR szMenSize[MAX_PATH];//�ַ��������ڱ��������ַ�ռ��С���˴��ַ���������ʵ�����·������MAX_PATHû�й�ϵ
	StrFormatByteSize(dwMenSize, szMenSize, MAX_PATH);//�������ַ�ռ��Сsi.dwMenSizeת�����ַ�������szMenSize

	TCHAR szAllocationGranularity[MAX_PATH];//�ַ��������ڱ��������ַ�ռ��С���˴��ַ���������ʵ�����·������MAX_PATHû�й�ϵ
	StrFormatByteSize(si.dwAllocationGranularity, szAllocationGranularity, MAX_PATH);//�������ַ�ռ��Сsi.dwMenSizeת�����ַ�������szMenSize

	//���ڴ���Ϣ��ʾ��
	cout << "�����ַ�ռ��С:" << szPageSize << endl;
	cout.fill('0');//������ַ�����Ϊ��0����
	cout << "��СӦ�ó����ַ��0x" << hex << setw(8)
		<< (DWORD)si.lpMinimumApplicationAddress
		<< endl;
	cout << "���Ӧ�ó����ַ��0x" << hex << setw(8)
		<< (DWORD)si.lpMaximumApplicationAddress
		<< endl;

	cout << "�ܹ����õ������ڴ棺" << szMenSize << endl;

	cout << "�ڴ�������ȣ�" << szAllocationGranularity << endl;
	cout << "CPU��������" << si.dwNumberOfProcessors << endl;
}