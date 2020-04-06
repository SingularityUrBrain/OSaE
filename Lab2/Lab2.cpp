#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

vector<wstring> FindFile(const std::wstring& directory, vector<wstring> all_files)
{
	std::wstring tmp = directory + L"\\*";
	WIN32_FIND_DATAW file;
	HANDLE search_handle = FindFirstFileW(tmp.c_str(), &file);
	if (search_handle != INVALID_HANDLE_VALUE)
	{
		std::vector<std::wstring> directories;

		do
		{
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")) || (!lstrcmpW(file.cFileName, L"$")))
					continue;
			}

			tmp = directory + L"\\" + std::wstring(file.cFileName);
			//std::wcout << tmp << std::endl;
			all_files.push_back(tmp);

			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				directories.push_back(tmp);
				all_files.push_back(tmp);
			}
		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);

		for (std::vector<std::wstring>::iterator iter = directories.begin(), end = directories.end(); iter != end; ++iter)
			FindFile(*iter, all_files);
		return all_files;
	}
}

SIZE_T* GetFileClusters(
	PCTSTR lpFileName,
	ULONG ClusterSize,
	ULONG* ClCount,
	ULONG* FileSize)
{
	ClusterSize = 4096;
	HANDLE hFile;
	ULONG OutSize;
	ULONG Bytes, Cls, CnCount, r;
	SIZE_T* Clusters = NULL;
	BOOLEAN Result = FALSE;
	LARGE_INTEGER PrevVCN, Lcn;
	STARTING_VCN_INPUT_BUFFER InBuf;
	PRETRIEVAL_POINTERS_BUFFER OutBuf;
	hFile = CreateFile(lpFileName, GENERIC_READ, // FILE_READ_ATTRIBUTES
		FILE_SHARE_READ, // | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		NULL, OPEN_EXISTING, 0, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		*FileSize = GetFileSize(hFile, NULL);
		OutSize = sizeof(RETRIEVAL_POINTERS_BUFFER) + (*FileSize / ClusterSize) * sizeof(OutBuf->Extents);
		OutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(OutSize);
		InBuf.StartingVcn.QuadPart = 0;
		if (DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &InBuf,
			sizeof(InBuf), OutBuf, OutSize, &Bytes, NULL))
		{
			*ClCount = (*FileSize + ClusterSize - 1) / ClusterSize;
			Clusters = (SIZE_T*)malloc(*ClCount * sizeof(SIZE_T));
			PrevVCN = OutBuf->StartingVcn;
			for (r = 0, Cls = 0; r < OutBuf->ExtentCount; r++)
			{
				Lcn = OutBuf->Extents[r].Lcn;
				for (CnCount = OutBuf->Extents[r].NextVcn.QuadPart - PrevVCN.QuadPart;
					CnCount;
					CnCount--, Cls++, Lcn.QuadPart++)
					Clusters[Cls] = Lcn.QuadPart;
				PrevVCN = OutBuf->Extents[r].NextVcn;
			}
		}
		free(OutBuf);
		CloseHandle(hFile);
	}
	return Clusters;
}


int GetFileAttr(PCTSTR FilePath)
{
	ULONG64 d1 = 0, d2 = 0;
	char DriveName[3] = { 0 };
	ULONG SecPerCl, BtPerSec, ClusterSize, ClCount, FileSize, i;
	memcpy(DriveName, FilePath, 2);
	GetDiskFreeSpaceA(DriveName, &SecPerCl, &BtPerSec, NULL, NULL);
	ClusterSize = SecPerCl * BtPerSec;
	PSIZE_T x = GetFileClusters(FilePath, ClusterSize, &ClCount, &FileSize);
	//std::cout << ClCount;
	free(x);
	return ClCount;
}

int main()
{
	vector<wstring> all_files;
	all_files = FindFile(L"C:\\Users\\Nikita\\Downloads", all_files);
	int win = 0;
	int now = 0;
	int index = 0;
	for (int i = 0; i < all_files.size(); i++)
	{
		now = GetFileAttr(all_files[i].c_str());
		if (now >= win)
		{
			index = i;
			win = now;
		}
	}
	cout << "=======================================" << endl;
	wcout << all_files[index] << " - " << win << endl;
	getchar();
}
