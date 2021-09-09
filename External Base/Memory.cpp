#include "Includes.h"

DWORD baseAddress = NULL;
DWORD engineAddress = NULL;
DWORD clientState = NULL;

HANDLE hProcSnap = NULL;
PROCESSENTRY32 procEntry32;

HANDLE hModuleSnap = NULL;
MODULEENTRY32 modEntry32;

DWORD pID = NULL;

HANDLE hProc = NULL;

bool attatchProc(char* procName)
{
	procEntry32.dwSize = sizeof(PROCESSENTRY32);
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take snapshot of process list." << std::endl;
		return false;
	}

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!strcmp(procName, procEntry32.szExeFile))
		{
			std::cout << "Found process " << procEntry32.szExeFile << " with process ID " << procEntry32.th32ProcessID << std::endl;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			pID = procEntry32.th32ProcessID;

			if (hProc == NULL)
			{
				std::cout << "Failed getting handle to process." << std::endl;
			}
			CloseHandle(hProcSnap);
			return true;
		}
	}

	std::cout << "Couldn't find " << procName << " in the process snapshot." << std::endl;
	CloseHandle(hProcSnap);
	return false;
}

DWORD getModule(LPSTR moduleName)
{
	hModuleSnap = INVALID_HANDLE_VALUE;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);

	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take a snapshot of modules." << std::endl;
		CloseHandle(hModuleSnap);
		return 0;
	}

	modEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hModuleSnap, &modEntry32))
	{
		if (!strcmp(moduleName, modEntry32.szModule))
		{
			std::cout << "Found module " << modEntry32.szModule << " with base address " << std::hex << (DWORD)modEntry32.modBaseAddr << std::endl;
			CloseHandle(hModuleSnap);
			return (DWORD)modEntry32.modBaseAddr;
		}
	}

	while (Module32Next(hModuleSnap, &modEntry32))
	{
		if (!strcmp(moduleName, modEntry32.szModule))
		{
			std::cout << "Found module " << modEntry32.szModule << " with base address 0x" << std::hex << std::uppercase << (DWORD)modEntry32.modBaseAddr << std::endl;
			CloseHandle(hModuleSnap);
			return (DWORD)modEntry32.modBaseAddr;
		}
	}

	std::cout << "Couldn't find " << moduleName << " module in module snapshot of the process." << std::endl;
	CloseHandle(hModuleSnap);
	return false;
}

std::uint32_t find(const char* proc)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	if (Process32First(snapshot, &pe))
	{
		while (Process32Next(snapshot, &pe))
		{
			if (!strcmp(proc, pe.szExeFile))
			{
				CloseHandle(snapshot);
				return pe.th32ProcessID;
			}
		}
	}
	CloseHandle(snapshot);
	return 0;
}

void init()
{
	if (attatchProc((char*)"csgo.exe"))
	{
		baseAddress = getModule((LPSTR)"client.dll");
		engineAddress = getModule((LPSTR)"engine.dll");

		LocalPlayer::setLocalPlayer();

		clientState = rpm<DWORD>(engineAddress + offs::dwClientState);
	}
}