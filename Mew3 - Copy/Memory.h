#pragma once
#include "stdafx.h"
#include <thread>
#include <Psapi.h>

class Memory
{
protected:
	DWORD process_id;

public:	
	Memory();
	~Memory();
	HANDLE hProcess;

	bool openProcess(char* ProcessName);

	//Writing Operation to a specific address with a given Value (Buffer) and a give Size 
	bool write(uintptr_t dwAddress, LPCVOID lpBuffer, size_t dwSize);
	template<typename T> inline bool write(uintptr_t dwAddress, const T& tValue)
	{
		return write(dwAddress, &tValue, sizeof(T));
	}

	template <class T> inline bool write2(uintptr_t dwAddress, const T& Value) //working
	{
		return WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(dwAddress), reinterpret_cast<LPCVOID>(&Value), sizeof(T), NULL);
	}

	//Reading Operation from a specific address with a given Variable (Buffer) to save into to
	bool read(uintptr_t dwAddress, LPVOID lpBuffer, size_t dwSize);
	template <typename T, size_t SIZE = sizeof(T)> T read(DWORD_PTR dwAddress, const T& tDefault = T())
	{
		T tREt;
		if (!read(dwAddress, &tREt, SIZE))
		{
			return tDefault;
		}
		return tREt;
		;
		
	}

	template <typename T = uintptr_t>
	T GetVFunc(DWORD address, size_t index)
	{
		uintptr_t vtable = Core::Mem->read<uintptr_t>(address);
		uintptr_t vfunc = Core::Mem->read<uintptr_t>(vtable + (index * sizeof(uintptr_t)));
	}

	template <typename T = uintptr_t>
	T GetVFuncPtr(DWORD inst, int Index)
	{
		DWORD table = Core::Mem->read<DWORD>(inst);
		DWORD func = table + sizeof(DWORD) * Index;
		return func;
	}

	DWORD Module(char* ModuleName);
	DWORD ModuleSize(char* ModuleName);
};
