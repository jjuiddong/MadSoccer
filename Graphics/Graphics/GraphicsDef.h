
#pragma once


#include <list>
#include <algorithm>


namespace graphics
{
	class CDisplayObject;
	class CWindow;

	typedef std::list<CDisplayObject*> DispObjList;
	typedef DispObjList::iterator DispObjItor;

	typedef std::list<CWindow*> WindowList;
	typedef WindowList::iterator WindowItor;

}
