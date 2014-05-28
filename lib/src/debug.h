#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED
#include <iostream>
#include <ctime>



//#ifdef __WIN
//    #include <windows.h>
//    #ifdef ERROR
//        #undef ERROR
//    #endif
//#else
/*#define RED "\33[22;31m"
#define LRED "\33[01;21m"
#define GREEN "\33[22;32m"
#define LGREEN "\33[01;32m"
#define BLUE "\33[22;34m"
#define LBLUE "\33[01;34m"
#define BROWN "\33[22;33m"
#define YELLOW "\33[01;33m"
#define CYAN "\33[22;36m"
#define LCYAN "\33[22;36m"
#define MAGENTA "\33[22;35m"
#define LMAGENTA "\33[01;35m"
#define NOCOLOR "\33[0m"*/
//#endif


#ifndef __WIN
#define PHASE(X) \
    { \
        std::cout << "\33[01;33m" << "Running phase: " << X << "  ( " << ((double) clock() / CLOCKS_PER_SEC ) << "s )" << "\33[0m" << std::endl; \
    }

#define ERROR(X) \
    { \
        std::cerr << "\33[22;31m" << "Error: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << "\33[0m" << std::endl; \
    }

#define WARNING(X) \
    { \
        std::cerr << "\33[22;36m" << "Warning: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << "\33[0m" << std::endl; \
    }

#define DEBUG(X) \
    { \
        std::cerr << "Debug: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << std::endl; \
    }

#define INFO(X) \
    { \
        std::cerr << "\33[22;32m" << "Info: " << X << "\33[0m" << std::endl; \
    }
#else
#define PHASE(X) \
	{ \
		int iOriginColor; \
		CONSOLE_SCREEN_BUFFER_INFO screenInfo; \
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo); \
		iOriginColor = screenInfo.wAttributes; \
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); \
		std::cout << "Running phase: " << X << "  ( " << ((double) clock() / CLOCKS_PER_SEC ) << "s )" << std::endl; \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), iOriginColor);\
	}

#define ERROR(X) \
	{ \
		int iOriginColor; \
		CONSOLE_SCREEN_BUFFER_INFO screenInfo; \
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo); \
		iOriginColor = screenInfo.wAttributes; \
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY); \
		std::cerr << "Error: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << std::endl; \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), iOriginColor);\
	}

#define WARNING(X) \
	{ \
		int iOriginColor; \
		CONSOLE_SCREEN_BUFFER_INFO screenInfo; \
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo); \
		iOriginColor = screenInfo.wAttributes; \
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY); \
		std::cerr << "Warning: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << std::endl; \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), iOriginColor); \
	}
#define DEBUG(X) \
	if (true) { \
		std::cerr << "Debug: " << X  << " -> (" << __FILE__ << ":" << std::dec << __LINE__ << ")" << std::endl; \
	}

#define INFO(X) \
	if (true) { \
		int iOriginColor; \
		CONSOLE_SCREEN_BUFFER_INFO screenInfo; \
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo); \
		iOriginColor = screenInfo.wAttributes; \
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY); \
		std::cerr << "Info: " << X << std::endl; \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), iOriginColor); \
	}
#endif

#endif // DEBUG_H_INCLUDED
