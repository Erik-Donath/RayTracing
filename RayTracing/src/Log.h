#pragma once
#include <iostream>

#ifndef WL_DIST // Console is In Distribution disabled
#define Log(x) std::cout << x << std::endl;
#else
#define Log(x) ;
#endif
