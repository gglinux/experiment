#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static LPCTSTR g_szMutexName="w2kdg.ProcTerm.mutex.Suicide";// ?

void StartClone(){
	TCHAR szFilename[MAX_PATH];//TCHAR ?
	GetModuleFileName(NULL,szFilename,MAX_PATH);//function ?

	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\"%d",szFilename);

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);

	PROCESS_INFORMATION pi;

	BOOL bCreateOK=::CreateProcess(szFilename,szCmdLine,NULL,NULL,false,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
	if(bCreateOK){
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
void Parent(){
	HANDLE hMutexSuicide=CreateMutex(NULL,true,g_szMutexName);
	if(hMutexSuicide!=NULL){
		cout<<"Creating the child process."<<endl;
		StartClone();
		cout<<"Telling the child process to quit"<<endl;
		getchar();
		//父进程释放互斥体，使计数为0
		ReleaseMutex(hMutexSuicide);
		CloseHandle(hMutexSuicide);
	}
}
void Child(){
	//互斥体
	HANDLE hMutexSuicide=OpenMutex(SYNCHRONIZE,false,g_szMutexName);
	if(hMutexSuicide!=NULL){
		cout<<"Child waiting for suicide instructions."<<endl;
		//会判断互斥量的线程ID是否为0，如果为非0，表示互斥量处于未触发状态
		//当父进程调用ReleaseMutex，子进程处于可调度
		WaitForSingleObject(hMutexSuicide,INFINITE);
		cout<<"Child quiting"<<endl;
		getchar();
		CloseHandle(hMutexSuicide);
	}
}
int main(int argc,char *argv[]){
	if(argc>1&&::strcmp(argv[1],"child")==0){
		Child();
	}
	else{
		Parent();
	}
	return 0;
}