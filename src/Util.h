#include <iostream>

//#define LANDA_LOGGING

#ifdef LANDA_LOGGING

#define LANDA_LOG(msg) std::cout << msg << std::endl
#define LANDA_METHOD() std::cout << __FUNCTION__ << "()" << std::endl

#else

#define LANDA_LOG(msg)
#define LANDA_METHOD()

#endif