#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "vectors.h"
#define MEMSCAN_API __declspec(dllexport)



MEMSCAN_API int get_pid_by_name(const char* process_name) {
	PROCESSENTRY32 pe;
	HANDLE snapshot;
	DWORD pid = 0;

	wchar_t w_process_name[MAX_PATH];
	size_t convertedChars = 0;
	errno_t err = mbstowcs_s(&convertedChars, w_process_name, MAX_PATH, process_name, _TRUNCATE);

	if (err != 0) {
		return -1; // Indicate error converting string
	}

	// Take a snapshot of all processes
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		printf("Failed to obtain PID by name !\n");
		return 0; // Failed to take snapshot
	}

	pe.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process
	if (Process32First(snapshot, &pe)) {
		do {
			// Check if the process name matches
			if (wcscmp(pe.szExeFile, w_process_name) == 0) {
				pid = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &pe));
	}
	else {
		pid = 0; // Failed to retrieve process information
		printf("Failed to retrieve process information, check the process name entered\n");
	}

	CloseHandle(snapshot);
	return pid;
}


// INTEGER
MEMSCAN_API int RPM_INT(HANDLE hProcess, unsigned char* address, int* data)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, address, data, sizeof(int), &bytesRead))
		return 1;
	else
		return 0;
}

MEMSCAN_API int WPM_INT(HANDLE hProcess, unsigned char* address, int data)
{
	SIZE_T bytesWritten;
	if (WriteProcessMemory(hProcess, address, &data, sizeof(int), &bytesWritten))
		return 1;
	else
		return 0;
}
// FLOAT
MEMSCAN_API int RPM_FLOAT(HANDLE hProcess, unsigned char* address, float* data)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, address, data, sizeof(float), &bytesRead))
		return 1;
	else
		return 0;

}

MEMSCAN_API int WPM_FLOAT(HANDLE hProcess, unsigned char* address, float data)
{
	SIZE_T bytesWritten;
	if (WriteProcessMemory(hProcess, address, &data, sizeof(float), &bytesWritten))
		return 1;
	else
		return 0;
}

// Double
MEMSCAN_API int RPM_DOUBLE(HANDLE hProcess, unsigned char* address, double* data)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, address, data, sizeof(double), &bytesRead))
		return 1;
	else
		return 0;
}

MEMSCAN_API int WPM_DOUBLE(HANDLE hProcess, unsigned char* address, double data)
{
	SIZE_T bytesWritten;
	if (WriteProcessMemory(hProcess, address, &data, sizeof(double), &bytesWritten))
		return 1;
	else
		return 0;
}

// INT
MEMSCAN_API int scanINT(HANDLE hProcess, int target, vector_uchar_ptr* addresses)
{
	unsigned char* START_ADDRESS = 0x0;
	
	/*
	Returns: -1 Memory Allocation Failure
	Returns: -2 Memory Reading Failure
	*/
	MEMORY_BASIC_INFORMATION mbi;
	while (VirtualQueryEx(hProcess, START_ADDRESS, &mbi, sizeof(mbi)))
	{
		if ((mbi.State == MEM_COMMIT) && (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY)))
		{

			unsigned char* buffer = (unsigned char*)malloc(mbi.RegionSize);
			if (buffer == NULL) {
				return -1;
			}
			SIZE_T bytesRead;

			if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead)) {

				for (SIZE_T i = 0; i < bytesRead / sizeof(int); i++) {
					if (sizeof(int) + i * sizeof(int) > bytesRead)
						break;
					int data = *(int*)(buffer + i * sizeof(int));
					if (data == target) {
						appendVectorUCharPtr(addresses, (unsigned char*)((unsigned char*)mbi.BaseAddress + i * sizeof(int)));
					}
				}
				
			}
			free(buffer);

		}
		// Move to the next region
		START_ADDRESS += mbi.RegionSize;
	}
	shrinkToFitVectorUCharPtr(addresses);
	return 1;
}

MEMSCAN_API int scanNextINT(HANDLE hProcess, vector_uchar_ptr* addressArray, int target)
{
	for (int i = 0; i < addressArray->size; i++)
	{
		SIZE_T bytesRead;
		int buffer;
		if (ReadProcessMemory(hProcess, addressArray->array[i], &buffer, sizeof(int), &bytesRead))
		{
			if (buffer != target)
			{
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
			}
		}
		else
		{
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
		}
	}
	return 1;
}

// FLOAT

MEMSCAN_API int scanFLOAT(HANDLE hProcess, float target, vector_uchar_ptr* addresses)
{
	/*
		Returns: -1 Memory Allocation Failure
		Returns: -2 Memory Reading Failure

	 */
	unsigned char* START_ADDRESS = 0x0;
	MEMORY_BASIC_INFORMATION mbi;

	while (VirtualQueryEx(hProcess, START_ADDRESS, &mbi, sizeof(mbi)))
	{
		if ((mbi.State == MEM_COMMIT) && (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY)))
		{

			unsigned char* buffer = (unsigned char*)malloc(mbi.RegionSize);
			if (buffer == NULL) {
				return -1;
			}
			SIZE_T bytesRead;

			if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead)) {

				for (SIZE_T i = 0; i < bytesRead / sizeof(float); i++) {
					if (sizeof(float) + i * sizeof(float) > bytesRead)
						break;
					float data = *(float*)(buffer + i * sizeof(float));
					if (data == target) {
						appendVectorUCharPtr(addresses, (unsigned char*)((unsigned char*)mbi.BaseAddress + i * sizeof(float)));
					}
				}
			}
			free(buffer);

		}
		// Move to the next region
		START_ADDRESS += mbi.RegionSize;
	}
	shrinkToFitVectorUCharPtr(addresses);
	return 1;
}

MEMSCAN_API int scanNextFLOAT(HANDLE hProcess, vector_uchar_ptr* addressArray, float target)
{
	for (int i = 0; i < addressArray->size; i++)
	{
		SIZE_T bytesRead;
		float buffer;
		if (ReadProcessMemory(hProcess, addressArray->array[i], &buffer, sizeof(float), &bytesRead))
		{
			if (buffer != target)
			{
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
			}
		}
		else
		{
			{
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
			}
		}
	}
	return 1;
}

// DOUBLE


MEMSCAN_API int scanDOUBLE(HANDLE hProcess, double target, vector_uchar_ptr* addresses)
{	
	/*
		Returns: -1 Memory Allocation Failure
		Returns: -2 Memory Reading Failure

	 */
	unsigned char* START_ADDRESS = 0x0;
	MEMORY_BASIC_INFORMATION mbi;

	while (VirtualQueryEx(hProcess, START_ADDRESS, &mbi, sizeof(mbi)))
	{
		if ((mbi.State == MEM_COMMIT) && (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY)))
		{
			unsigned char* buffer = (unsigned char*)malloc(mbi.RegionSize);
			if (buffer == NULL) {
				return -1;
			}
			SIZE_T bytesRead;
			if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead)) {
				for (SIZE_T i = 0; i < bytesRead / sizeof(double); i++) {
					if (sizeof(double) + i * sizeof(double) > bytesRead)
						break;
					double data = *(double*)(buffer + i * sizeof(double));
					if (data == target) {
						appendVectorUCharPtr(addresses, (unsigned char*)((unsigned char*)mbi.BaseAddress + i * sizeof(double)));
					}
				}
			}
			free(buffer);

		}
		// Move to the next region
		START_ADDRESS += mbi.RegionSize;
	}
	shrinkToFitVectorUCharPtr(addresses);
	return 1;
}

MEMSCAN_API int scanNextDOUBLE(HANDLE hProcess, vector_uchar_ptr* addressArray, double target)
{
	for (int i = 0; i < addressArray->size; i++)
	{
		SIZE_T bytesRead;
		double buffer;
		if (ReadProcessMemory(hProcess, addressArray->array[i], &buffer, sizeof(double), &bytesRead))
		{
			if (buffer != target)
			{
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
			}
		}
		else
		{
			
				deleteElementVectorUCharPtr(addressArray, i);
				i--;
		}
	}
	return 1;
}
MEMSCAN_API vector_uchar_ptr* createAddressArray()
{
	vector_uchar_ptr* vector = (vector_uchar_ptr*)malloc(sizeof(vector_uchar_ptr));
	createVectorUCharPtr(vector);
	return vector;
}
MEMSCAN_API void freeAddressArray(vector_uchar_ptr* address)
{
	deleteVectorUCharPtr(address);
	free(address);
}
// Freeing Data
MEMSCAN_API void freeINTAddress(int* data)
{
	free(data);
}
MEMSCAN_API void freeFLOATAddress(float* data)
{
	free(data);
}
MEMSCAN_API void freeDOUBLEAddress(double* data)
{
	free(data);
}
MEMSCAN_API HANDLE getHandleByPID(int pid)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
}
MEMSCAN_API int closeHandle(HANDLE hProcess)
{
	return CloseHandle(hProcess);
}