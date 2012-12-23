//------------------------------------------------------------------------
// Name:    String.h
// Author:  jjuiddong
// Date:    2012-12-05
// 
// �����ڵ�, ��Ƽ����Ʈ���� �� ������ �� �ִ� ��Ʈ�� Ŭ������ �����Ѵ�.
//
// �ѱ��� �� ��ȯ�Ǳ� ���ؼ� �ݵ�� ȣ�����־�� �Ѵ�. (�Ʒ��� �ִ� �Լ�)
// std::locale::global(std::locale(""));
//------------------------------------------------------------------------

#pragma once


namespace common
{
	std::string wstring2string(const std::wstring &wstr);
	std::wstring string2wstring(const std::string &str);

	std::string format(const char* fmt, ...);

}

