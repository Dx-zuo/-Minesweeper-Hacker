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
    LPCVOID lpBaseAddress = 0x1005194;  // ����ʣ������ ��̬��ַ
    LPCVOID lpBaseAddress1 = 0x1005330;  // ���ж������� ��̬��ַ
    LPCVOID lpBaseAddress2 = 0x1005334;  // �п�����  x
    LPCVOID lpBaseAddress3 = 0x1005338;  // �п�����  y
    LPCVOID lpBaseAddress4 = 0x1005361;  // ��Ϸ�����ڴ濪ʼ��ַ
    LPDWORD lpdwProcessId;// ���ս��̵�pid
    DWORD sum,y,x;  //�������� y�п� �п�
    BYTE bWrite = 0x8e;
    BYTE bWrite1 = 0x00;
    int aa = 0;
    HWND hwnd = FindWindow(NULL,"ɨ��");
    if  (hwnd == NULL){
        printf("��Ϸδ����");
        return 0;
    }

    GetWindowThreadProcessId(hwnd,&lpdwProcessId);
    HANDLE  hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,lpdwProcessId);

    ReadProcessMemory(hprocess,lpBaseAddress1,&sum,sizeof(int),NULL);  //��ȡ���׵������б�
    printf("sum %d\n",sum);
    ReadProcessMemory(hprocess,lpBaseAddress2,&y, sizeof(int),NULL);  //��ȡ���׵������б�
    printf("y %d\n",y);
    ReadProcessMemory(hprocess,lpBaseAddress3,&x,sizeof(int),NULL);  //��ȡ���׵������б�
    printf("x %d\n",x);
    if (y > 20){
        y +=2;
    }
    for (int j = 0; j < x; ++j) {
        for (int i = 0; i < y; ++i) {
            ReadProcessMemory(hprocess,lpBaseAddress4,&pvBufferLocal,1,NULL);  //��ȡ���׵������б�
                //printf("%x",pvBufferLocal);
            if (pvBufferLocal == 0x8f)
            {
                WriteProcessMemory(hprocess,(LPVOID)lpBaseAddress4,&bWrite, 1,NULL);
                printf("��");
            }else if(pvBufferLocal == 0xf){
                WriteProcessMemory(hprocess,(LPVOID)lpBaseAddress4,&bWrite1, 1,NULL);
                printf("��");
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

    HANDLE  hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid); // ���Ȩ�޴򿪽���
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
    int countProcess=0;                                 //��ǰ����������������
    PROCESSENTRY32 currentProcess;                      //��ſ��ս�����Ϣ��һ���ṹ��
    currentProcess.dwSize = sizeof(currentProcess);     //��ʹ������ṹ֮ǰ�����������Ĵ�С
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//��ϵͳ�ڵ����н�����һ������

    if (hProcess == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot()����ʧ��!\n");
        return -1;
    }
    BOOL bMore=Process32First(hProcess,&currentProcess);  //��ȡ��һ��������Ϣ
    while(bMore)
    {
        printf("PID=%5u    PName= %s\n",currentProcess.th32ProcessID,currentProcess.szExeFile); //�������̿��գ�������ʾÿ��������Ϣ
        bMore=Process32Next(hProcess,&currentProcess);    //������һ��
        countProcess++;
    }
    CloseHandle(hProcess);  //���hProcess���
    printf("��������%d������������\n",countProcess);
    system("pause");
    return 0;
}
