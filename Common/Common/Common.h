
#ifndef __COMMON_H__
#define __COMMON_H__


#include <Windows.h>
#include <list>
#include <map>
#include <algorithm>
#include <string>

#include "math/Math.h"




#define SAFE_DELETE(p) {if (p) { delete p; p=NULL;}}
#define SAFE_DELETEA(p) {if (p) { delete[] p; p=NULL;}}







#endif // __COMMON_H__
