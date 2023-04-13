#pragma once

#define Multithreded false

#ifdef WL_DIST // In Distribution Multithreaded mode is always enabled!
#define Multithreded true
#endif 
