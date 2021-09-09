#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include "Offsets.hpp"

extern DWORD baseAddress;
extern DWORD engineAddress;
extern DWORD clientState;

extern HANDLE hProcSnap;
extern PROCESSENTRY32 procEntry32;

extern HANDLE hModuleSnap;
extern MODULEENTRY32 modEntry32;

extern DWORD pID;

extern HANDLE hProc;

template <class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite)
{
	WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addressToRead)
{
	dataType rpmBuffer;
	ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);
	return rpmBuffer;
}

extern bool attatchProc(char* procName);

extern DWORD getModule(char* moduleName);

extern uintptr_t patternScan(char* base, size_t size, char* pattern);

extern void init();

extern std::uint32_t find(const char* proc);