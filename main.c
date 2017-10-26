//
// Created by Dx777 on 2017/10/23.
//
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

const unsigned char MINE = 0x8e;
BOOL searchMem(DWORD pid);
int main(){
    DWORD pvBufferLocal;
    DWORD A = 0x1005361,B = 0x1005361 + 30;
    DWORD dwNumberOfBytesRead;
    LPCVOID lpBaseAddress = 0x1005194;  // 雷区剩余数量 静态基址
    LPCVOID lpBaseAddress1 = 0x1005330;  // 共有多少雷区 静态基址
    LPCVOID lpBaseAddress2 = 0x1005334;  // 列宽行数  x
    LPCVOID lpBaseAddress3 = 0x1005338;  // 行宽行数  y
    LPCVOID lpBaseAddress4 = 0x1005361;  // 游戏棋盘内存开始地址
    LPDWORD lpdwProcessId;// 接收进程的pid
    DWORD sum,y,x;  //雷区总数 y行宽 列宽
    BYTE bWrite = 0x8e;
    BYTE bWrite1 = 0x00;
    int aa = 0;
    HWND hwnd = FindWindow(NULL,"扫雷");
    if  (hwnd == NULL){
        printf("游戏未启动");
        return 0;
    }

    GetWindowThreadProcessId(hwnd,&lpdwProcessId);
    HANDLE  hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,lpdwProcessId);

    ReadProcessMemory(hprocess,lpBaseAddress1,&sum,sizeof(int),NULL);  //获取到雷的数量列表
    printf("sum %d\n",sum);
    ReadProcessMemory(hprocess,lpBaseAddress2,&y, sizeof(int),NULL);  //获取到雷的数量列表
    printf("y %d\n",y);
    ReadProcessMemory(hprocess,lpBaseAddress3,&x,sizeof(int),NULL);  //获取到雷的数量列表
    printf("x %d\n",x);
    if (y > 20){
        y +=2;
    }
    for (int j = 0; j < x; ++j) {
        for (int i = 0; i < y; ++i) {
            ReadProcessMemory(hprocess,lpBaseAddress4,&pvBufferLocal,1,NULL);  //获取到雷的数量列表
                //printf("%x",pvBufferLocal);
            if (pvBufferLocal == 0x8f)
            {
                WriteProcessMemory(hprocess,(LPVOID)lpBaseAddress4,&bWrite, 1,NULL);
                printf("■");
            }else if(pvBufferLocal == 0xf){
                WriteProcessMemory(hprocess,(LPVOID)lpBaseAddress4,&bWrite1, 1,NULL);
                printf("□");
            }
            else if(pvBufferLocal == 0x10){
                printf("|");
            }
            else if(pvBufferLocal == 0x41){
                printf("1");
            }
            else if(pvBufferLocal == 0x42){
                printf("2");
            }
            else if(pvBufferLocal == 0x43){
                printf("3");
            }
//            if (pvBufferLocal == 0x8f)
//            {
//                printf("0",pvBufferLocal);
//            }else if (pvBufferLocal == 0x10){
//                printf("|",pvBufferLocal);
//            }
//            else if (pvBufferLocal == 0x0f) {
//                printf(" ",pvBufferLocal);
//            }else{
//                printf("%x  ",pvBufferLocal);
//            }
        lpBaseAddress4+=1;
        }
        WriteProcessMemory(hprocess,lpBaseAddress,&aa,1,NULL);
        ShowWindow(hwnd, SW_MINIMIZE);
        ShowWindow(hwnd, SW_SHOWNORMAL);
        printf("\n");
    }

    printf("hprocess %d\n",hprocess);
}
BOOL searchMem(DWORD pid){

    HANDLE  hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid); // 最大权限打开进程
    if  (hprocess == NULL){
        return  FALSE;
    }
    MEMORY_BASIC_INFORMATION32 mbi;
    PBYTE  pAddress = NULL;
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    while (pAddress < (DWORD)siSysInfo.lpMaximumApplicationAddress){
        if (VirtualQueryEx(hprocess,pAddress,&mbi, sizeof(mbi))!= sizeof(mbi)){
            break;
        }
        if ((mbi.Protect & PAGE_READWRITE) && (MEM_COMMIT == mbi.State)){

        }

    }
}
int GetProcess()
{
    int countProcess=0;                                 //当前进程数量计数变量
    PROCESSENTRY32 currentProcess;                      //存放快照进程信息的一个结构体
    currentProcess.dwSize = sizeof(currentProcess);     //在使用这个结构之前，先设置它的大小
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//给系统内的所有进程拍一个快照

    if (hProcess == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot()调用失败!\n");
        return -1;
    }
    BOOL bMore=Process32First(hProcess,&currentProcess);  //获取第一个进程信息
    while(bMore)
    {
        printf("PID=%5u    PName= %s\n",currentProcess.th32ProcessID,currentProcess.szExeFile); //遍历进程快照，轮流显示每个进程信息
        bMore=Process32Next(hProcess,&currentProcess);    //遍历下一个
        countProcess++;
    }
    CloseHandle(hProcess);  //清除hProcess句柄
    printf("共有以上%d个进程在运行\n",countProcess);
    system("pause");
    return 0;
}
