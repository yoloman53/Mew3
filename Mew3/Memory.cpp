#include "stdafx.h"
#include "memory.h"
#include <iostream>

Memory::Memory()
{
}

Memory::~Memory()
{
	CloseHandle(hProcess);
}

bool Memory::openProcess(char * ProcessName)
{
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);

	do
	{
		if (!strcmp(ProcEntry.szExeFile, ProcessName))
		{
			process_id = ProcEntry.th32ProcessID;
			CloseHandle(hPID);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
			std::cout << "Process found \t... \n";
			return true;
		}
	} while (Process32Next(hPID, &ProcEntry));

	return false;

}

bool Memory::write(uintptr_t dwAddress, LPCVOID lpBuffer, size_t dwSize)
{
	SIZE_T in = NULL;
	return WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(dwAddress), reinterpret_cast<LPCVOID>(&lpBuffer), dwSize, NULL);
}
	
bool Memory::read(uintptr_t dwAddress, LPVOID lpBuffer, size_t dwSize)
{
	SIZE_T out = NULL;
	return (ReadProcessMemory(hProcess, (LPCVOID)dwAddress, lpBuffer, dwSize, &out) == TRUE);
}

DWORD Memory::Module(char * ModuleName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));
	return 0;
}

DWORD Memory::ModuleSize(char * ModuleName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseSize;
		}
	while (Module32Next(hModule, &mEntry));
	return 0;
}

/*
//Convert WCHAR* to CHAR*
char *szModule = new char[4046];
char *ptr_szModule = mEntry.szModule;
//wcstombs(szModule, mEntry.szModule, 12);
*/