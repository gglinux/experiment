#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <math.h> 

using namespace std;

void StartClone(int nCloneID); 
int main(int argc, char *argv[])
{
	int nClone=0;//nClone,the number of clone
	//int nClone;
	if(argc>1){
   //nClone=0;
		::sscanf(argv[1],"%d",&nClone);//��������ʾ���л�ȡ��Ҫ��¡��ID��
   	}
	//�������޸ģ�����һֱ�������̣�����������������,Ϊʲô�������������
	cout<<"Process ID:"<<::GetCurrentProcessId()<<",Clone ID:"<<nClone<<endl;//��ӡ�����̵�ID�Ϳ�¡��ID��
	const int c_nCloneMax=5;
	if(nClone<c_nCloneMax){
		StartClone(++nClone);//give the child id,start the child process;
	}
	getchar();
	return 0;
}
void StartClone(int nCloneID){
	TCHAR szFilename[MAX_PATH];//TCHAR ������CHAR���ͣ�������ŷ��ص��ļ�����MAX_PATH 260
	GetModuleFileName(NULL,szFilename,MAX_PATH);//��ȡ��ǰ�����Ѽ���ģ����ļ�������·������ģ������ɵ�ǰ���̼���
	//������null����ʾ��ǰ���̵�ȫ·����szfilename��ָ�룬�����ļ����������������Max_path,װ�ص����ֵ

	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\"%d",szFilename,nCloneID);//�Ѹ�ʽ��������д��ĳ���ַ�����������
	//�����ǽ�szfilename���ַ�����nCloneID��ʮ����������ת�浽������szcmdLine��

	STARTUPINFO si;//�ýṹ����ָ���½��̵�����������
	ZeroMemory(&si,sizeof(si));//ZeroMemory��ָ�����ڴ������
	si.cb=sizeof(si);//����STARTUPINFO�ṹ�е��ֽ���

	PROCESS_INFORMATION pi;//�����й��½��̼������̵߳���Ϣ

	BOOL bCreateOK=::CreateProcess(szFilename,szCmdLine,NULL,NULL,false,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);//����һ���µĽ���
	if(bCreateOK){
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
