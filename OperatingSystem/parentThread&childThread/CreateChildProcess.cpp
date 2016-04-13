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
		::sscanf(argv[1],"%d",&nClone);//从命令提示符中获取需要克隆的ID号
   	}
	//第三次修改，它会一直创建进程，出现类似死机现象,为什么会出现这种现象
	cout<<"Process ID:"<<::GetCurrentProcessId()<<",Clone ID:"<<nClone<<endl;//打印出进程的ID和克隆的ID号
	const int c_nCloneMax=5;
	if(nClone<c_nCloneMax){
		StartClone(++nClone);//give the child id,start the child process;
	}
	getchar();
	return 0;
}
void StartClone(int nCloneID){
	TCHAR szFilename[MAX_PATH];//TCHAR 类似于CHAR类型，用来存放返回的文件名，MAX_PATH 260
	GetModuleFileName(NULL,szFilename,MAX_PATH);//获取当前进程已加载模块的文件的完整路径，该模块必须由当前进程加载
	//参数：null，表示当前进程的全路径，szfilename是指针，返回文件名，是输出参数。Max_path,装载的最大值

	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\"%d",szFilename,nCloneID);//把格式化的数据写入某个字符串缓冲区。
	//含义是将szfilename以字符串，nCloneID以十进制整数，转存到缓冲区szcmdLine中

	STARTUPINFO si;//该结构用于指定新进程的主窗口特性
	ZeroMemory(&si,sizeof(si));//ZeroMemory将指定的内存块清零
	si.cb=sizeof(si);//包含STARTUPINFO结构中的字节数

	PROCESS_INFORMATION pi;//返回有关新进程及其主线程的信息

	BOOL bCreateOK=::CreateProcess(szFilename,szCmdLine,NULL,NULL,false,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);//创建一个新的进程
	if(bCreateOK){
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
