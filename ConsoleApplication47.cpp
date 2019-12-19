
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <Shlobj.h>//программа уже не сможет быть сервисом 
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

const int defBuf = 1024;


void nullStr(char *str, int count = 0) {
	char *wrk = str;
	int l = count;
	if (0 == l)
	{
		l = defBuf;
	}
	for (int i = 0; i < count; i++) {
		*wrk = 0;
		wrk++;
	}
}

void printFileInfo(char *filename)//битовая логика вспомнить
{
	printf("--%s\n", filename);
	DWORD attr = GetFileAttributesA(filename);
	if (INVALID_FILE_ATTRIBUTES != attr)
	{
		printf("file attributes:\n");
		if ((attr&FILE_ATTRIBUTE_DIRECTORY) == attr)printf("    directory\n");
		if ((attr&FILE_ATTRIBUTE_COMPRESSED) == attr)printf("    compressed\n");
		if ((attr&FILE_ATTRIBUTE_HIDDEN) == attr)printf("    hidden\n");
		if ((attr&FILE_ATTRIBUTE_ARCHIVE) == attr)printf("    archive\n");
		if ((attr&FILE_ATTRIBUTE_NORMAL) == attr)printf("    normal\n");
		if ((attr&FILE_ATTRIBUTE_OFFLINE) == attr)printf("    offline\n");
		if ((attr&FILE_ATTRIBUTE_READONLY) == attr)printf("    readonly\n");
		if ((attr&FILE_ATTRIBUTE_SYSTEM) == attr)printf("    system\n");
		if ((attr&FILE_ATTRIBUTE_TEMPORARY) == attr)printf("    system\n");
		if ((attr&FILE_ATTRIBUTE_VIRTUAL) == attr)printf("    virtual\n");
	}
	else {
		printf("I cannot to recive file attributes\n ");
	}
}

void printFileSize(char *filename)
{

	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//создать дискриптор файла
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("I cannot to open file handle\n");
	}
	else
	{
		LARGE_INTEGER fileSize;

		if (GetFileSizeEx(hFile, &fileSize))
		{

			printf("fileSize %d\n", fileSize);//размер в байтах 
		}
		else printf("I cannot to recive file size\n");

	}
}
void printTime(FILETIME time)
{
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&time, &sysTime);
	printf("%d.%d.%d %d:%d\n", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
}

void printFileTime(char *filename)
{

	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//создать дискриптор файла
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("I cannot to open file handle\n");
	}
	else
	{
		FILETIME dateCreate;
		FILETIME dateLastAccess;
		FILETIME dateLastChange;

		if (GetFileTime(hFile, &dateCreate, &dateLastAccess, &dateLastChange))
		{
			printf("\ndate create:");
			printTime(dateCreate);
			printf("\ndate last access:");
			printTime(dateLastAccess);
			printf("\ndate last change:");
			printTime(dateLastChange);
		}
		else printf("I cannot to recive file size\n");

	}
}
void printFoundedData(LPWIN32_FIND_DATAA founded) {
	printf("filename %s\n  ", founded->cFileName);
	printf("\ndate create:");
	printTime(founded->ftCreationTime);
	printf("\ndate last access:");
	printTime(founded->ftLastAccessTime);
	printf("\ndate last change:");
	printTime(founded->ftLastWriteTime);
}

void findFilesSimple(char *mask)
{
	LPWIN32_FIND_DATAA founded = (LPWIN32_FIND_DATAA)malloc(sizeof(WIN32_FIND_DATAA));
	HANDLE searchHandle = FindFirstFileA(mask, founded);
	if (INVALID_HANDLE_VALUE != searchHandle)
	{
		do {
			printFoundedData(founded);


		} while (FindNextFileA(searchHandle, founded));


		FindClose(searchHandle);
	}

	free(founded);

}

void findFiles(char *initdir, char *ext)
{
	char *dir = (char*)malloc(defBuf * sizeof(char));
	nullStr(dir);
	strcpy_s(dir, defBuf, initdir);
	strcat_s(dir, defBuf, "\\\0");
	printf("%s\n", dir);

	char *mask = (char*)malloc(defBuf * sizeof(char));
	nullStr(mask);
	strcpy_s(mask, defBuf, dir);
	strcat_s(mask, defBuf, "*\0");


	LPWIN32_FIND_DATAA founded = (LPWIN32_FIND_DATAA)malloc(sizeof(WIN32_FIND_DATAA));
	HANDLE searchHandle = FindFirstFileA(mask, founded);
	if (INVALID_HANDLE_VALUE != searchHandle)
	{
		do {

			if (((founded->dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) == founded->dwFileAttributes) && (strcmp(founded->cFileName, ".") != 0) && (strcmp(founded->cFileName, "..") != 0))

			{
				char *newdir = (char*)malloc(defBuf * sizeof(char));
				nullStr(newdir);
				strcpy_s(newdir, defBuf, dir);
				strcat_s(newdir, defBuf, founded->cFileName);

				findFiles(newdir, ext);
				free(newdir);
			}
			else if (NULL != strstr(founded->cFileName, ext))
			{
				printFoundedData(founded);
			}


		} while (FindNextFileA(searchHandle, founded));


		FindClose(searchHandle);
	}

	free(dir);
}






int main()
{
	SetConsoleCP(1251);//со шрифтом Lucida console,иначе козявки 
	SetConsoleOutputCP(1251);

	char *buffer = (char*)malloc(defBuf * sizeof(char));
	char *dir = (char*)malloc(defBuf * sizeof(char));
	//	/*char *ext = (char*)malloc(defBuf * sizeof(char));
	//	char *temp = buffer;
	//	nullStr(buffer);
	//	nullStr(dir);
	//	nullStr(ext);
	//	strcpy_s(dir, defBuf, "d:");
	//	strcpy_s(ext, defBuf, "txt");*/
	//	//strcpy_s(buffer, defBuf, "d:\\tree.txt\0");//всегда занулять строку при вызове функций winapi
	//	//strcpy_s(buffer, defBuf, "d:*");//всегда занулять строку при вызове функций winapi
	//	//printFileInfo(buffer);
	//	//printf("*****************\n");
	//	//printFileSize(buffer);
	//	//printf("*****************\n");
	//	//printFileTime(buffer);
	//	//printf("*****************\n");
	//	//findFiles(dir, ext);
	////	findFilesSimple(buffer);
	//	//получение размера файла
	//


		////	GetCurrentDirectoryA(defBuf, buffer);//получает наш буфер и размер и возвращает сюда наш текущий каталог
		////	//GetCurrentDirectoryW Для шрифтов не ansi
		////	printf("1.currentDir=%s\n", buffer);
		////
			////2.меняем текущий каталог
			//nullStr(buffer, defBuf);
			//strcpy_s(buffer, defBuf, "d:\\AppData");
			//SetCurrentDirectoryA(buffer);
		////
		//////3.получение текущего каталога
		////	nullStr(buffer, defBuf);
		////	if (GetCurrentDirectoryA(defBuf, buffer) > 0)
		////	{//получает наш буфер и размер и возвращает сюда наш текущий каталог 
		////		printf("3.currentDir=%s\n", buffer);
		////	}
		////	else
		////	{
		////		printf("3.I`m cannot relative to  current directory\n");
		////	}
		//////файл будет создаваться в этом каталоге
		////	FILE *TESTFILE;
		////	int err = fopen_s(&TESTFILE, "file.txt", "w");
		////	if (err == 0)
		////	{
		////		for (int i = 0; i < 10; i++)
		////		{
		////			fprintf(TESTFILE, "%d,", i);
		////
		////		}
		////		fclose(TESTFILE);
		////	}
		////	else
		////	{
		////		printf("I cannot to access for file\n");
		////	}
		//4.создать каталог
		//	PSECURITY_ATTRIBUTES ATTR=(PSECURITY_ATTRIBUTES)malloc(sizeof(PSECURITY_ATTRIBUTES));
		// или SECURITY_ATTRIBUTES *ATTR=(SECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));

		//9.Получить каталог AppData( для сохранения файлов настроек)
	nullStr(buffer);
	if (SHGetFolderPathA(0, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, buffer) == S_OK)
	{
		printf("User AppData path= %s\n", buffer);
	}

	//2.меняем текущий каталог
	//nullStr(dir);
//	strcpy_s(dir, defBuf, buffer);
	//SetCurrentDirectoryA(buffer);


	nullStr(dir);
	//nullStr(buffer);
//strcpy_s(dir, defBuf, "myCompany");
	//PathAppendA(buffer, dir);
	PathAppendA(buffer, "MyCo");
	//	LPCWSTR path = (L"d:\\learn\\myfolder2");
	if (FALSE == PathFileExistsA(buffer))//проверяем если нет такого каталога,то создаем
	{//проверяет правильности пути
		if (CreateDirectoryA(buffer, NULL) == 0)
		{
			printf("1.I cannot to create directory %s \n", buffer);

		}
		else
		{
			printf("1.successfully created directory %s \n", buffer);

		}

	}
	else
	{
		printf("1.Folder %s alredy exists\n", buffer);
	}

	//	PathAppendA(buffer, dir);

		//strcpy_s(dir, defBuf, buffer);
		//SetCurrentDirectoryA(buffer);

		//nullStr(buffer);
		//nullStr(dir);
		//strcpy_s(dir, defBuf, "myProgramm");
	PathAppendA(buffer, "MyProg");
	if (FALSE == PathFileExistsA(buffer))//проверяем если нет такого каталога,то создаем
	{//проверяет правильности пути
		if (CreateDirectoryA(buffer, NULL) == 0)
		{
			printf("2.I cannot to create directory %s \n", buffer);
		}
		else
		{
			printf("2.successfully created directory %s \n", buffer);
		}

	}
	else
	{
		printf("2.Folder %s alredy exists\n", buffer);
	}

	//PathAppendA(buffer, dir);
	//SetCurrentDirectoryA(buffer);


	//nullStr(dir);
	//strcpy_s(dir, defBuf, buffer);
//	SetCurrentDirectoryA(dir);

	//nullStr(buffer);
	PathAppendA(buffer, "config.txt");
	//	strcpy_s(dir, defBuf, "config.txt");
	if (FALSE == PathFileExistsA(buffer))//проверяем если нет такого каталога,то создаем
	{//проверяет правильности пути
		SetCurrentDirectoryA(buffer);
		FILE *TESTFILE;
		int err = fopen_s(&TESTFILE, "config.txt", "w");
		if (err == 0)
		{
			for (int i = 0; i < 10; i++)
			{
				fprintf(TESTFILE, "%d\n,", i);

			}
			fclose(TESTFILE);
		}
		else
		{
			printf("I cannot to access for file\n");
		}
	}
	else
	{
		printf("3.Folder %s alredy exists\n", buffer);
		FILE *TESTFILE;
		int err = fopen_s(&TESTFILE, "config.txt", "r");
		if (err == 0)
		{
			char *buf = (char *)malloc(1024 * sizeof(char));
			char *work = buf;
			while (EOF != (*work = fgetc(TESTFILE))) {
				if ('\n' == *work) {

					printf("%d\n", buf);




				}
			}

			fclose(TESTFILE);
			free(buf);
		}
		else
		{
			printf("I cannot to access for file\n");
		}

	}

	free(buffer);
	return 0;
}
//версия 6   это винда выше виста


