#undef   UNICODE
# include <windows.h>
# include <tlhelp32.h>
# include <iostream>

using namespace std;
	
// 当在用户模式机内核模式下都提供所耗时间时，在内核模式下进行所耗时间的64位计算的帮助方法
DWORD GetKernelModePercentage(const FILETIME& ftKernel,
							  const FILETIME& ftUser)
{
    // 将FILETIME结构转化为64位整数
    ULONGLONG qwKernel=(((ULONGLONG)ftKernel.dwHighDateTime)
		<<32)+ftKernel.dwLowDateTime;
    ULONGLONG qwUser=(((ULONGLONG)ftUser.dwHighDateTime)
		<<32)+ftUser.dwLowDateTime;

    // 将消耗时间相加，然后计算消耗在内核模式下的时间百分比
    ULONGLONG qwTotal=qwKernel+qwUser;
    DWORD dwPct=(DWORD)(((ULONGLONG)100*qwKernel)/qwTotal);
    return(dwPct);
}

// 以下是将当前运行过程名和消耗在内核模式下的时间百分数都显示出来的应用程序
void main(int argc,char *argv[])
{
    
	if(argc<2) 
	{
		cout<<"请给出你要查询的程序名"<<endl;
		exit(0);
	}
	
	// 对当前系统中运行的过程拍取“快照”	
	HANDLE hSnapshot=::CreateToolhelp32Snapshot(
	   TH32CS_SNAPPROCESS,				// 提取当前过程
	   0);								// 如果是当前过程，就将其忽略

    // 初始化过程入口   
	PROCESSENTRY32 pe;
	::ZeroMemory(&pe,sizeof(pe));
	pe.dwSize=sizeof(pe);
	BOOL bMore=::Process32First(hSnapshot,&pe);
	BOOL found = FALSE;
	while(bMore)
	{
        // 打开用于读取的过程		
	
		if(!strcmp(pe.szExeFile,argv[1])) 
		{
			found = TRUE;
			HANDLE hProcess=::OpenProcess(
				PROCESS_QUERY_INFORMATION,		// 指明要得到信息
				FALSE,							// 不必继承这一句柄
				pe.th32ProcessID);				// 要打开的进程
			if (hProcess!=NULL)
			{
			    // 找出进程的时间
				FILETIME ftCreation,ftKernelMode,ftUserMode,ftExit;
				::GetProcessTimes(
					hProcess,					// 所感兴趣的进程
					&ftCreation,				// 进程的启动时间 
					&ftExit,					// 结束时间 (如果有的话)
					&ftKernelMode,				// 在内核模式下消耗的时间
					&ftUserMode);				// 在用户模式下消耗的时间

				// 计算内核模式消耗的时间百分比
				DWORD dwPctKernel=::GetKernelModePercentage(
					ftKernelMode,				// 在内核模式上消耗的时间
					ftUserMode);				// 在用户模式下消耗的时间

            // 向用户显示进程的某些信息
			cout<< "process ID: " << pe.th32ProcessID
				<< ",EXE file:" << pe.szExeFile
				<< ",%d in Kernel mode: " << dwPctKernel << endl;

            // 消除句柄
			::CloseHandle(hProcess);
			}
		}
        // 转向下一个进程
		bMore=::Process32Next(hSnapshot,&pe);
	}
	if(found==FALSE)
	{
		cout<<"当前系统没有这个可执行程序正在运行"<<endl;
		exit(0);
	} 
}