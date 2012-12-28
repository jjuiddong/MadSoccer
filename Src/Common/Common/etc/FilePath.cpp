
#include "stdafx.h"
#include "FilePath.h"
#include <shlwapi.h> // �� ��� ���Ͽ� FilePath�� ���õ� �Լ����� ����. �� �̿��غ���.
#pragma comment(lib, "shlwapi")


using namespace common;


//------------------------------------------------------------------------
// fileName�� ��ο� Ȯ���ڸ� ������ �����̸��� �����Ѵ�.
//------------------------------------------------------------------------
std::string common::GetFileName(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	PathRemoveExtensionA(name);
	return name;
}
