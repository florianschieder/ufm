#pragma once

#include <Windows.h>
#include <time.h>
#include <string>

#pragma region Common Definitions

#ifdef _WINDLL
#define METHOD __declspec(dllexport)
#else
#define METHOD
#endif

#define STATUS_SUCCESS 0x00000000L

#define OS_XP 20000
#define OS_VISTA 20001
#define OS_7 20002
#define OS_8 20003
#define OS_8_1 20004
#define OS_10 20005

// API types

typedef NTSTATUS(WINAPI* RtlGetVersion) (PRTL_OSVERSIONINFOW);

// Procedures

#ifdef UNICODE
#define Join JoinW
#define MakeUniqueClassName MakeUniqueClassNameW
typedef std::wstring String;
#else
#define Join JoinA
#define MakeUniqueClassName MakeUniqueClassNameA
typedef std::string String;
#endif

#define MakeUniqueClassNameA(className, id) \
std::string className = "WFM_"; \
className += id; \
className += "_"; \
const char alphanum[] = { \
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', \
    'A', 'B', 'C', 'D', 'E', 'F' }; \
int stringLength = sizeof(alphanum) - 1; \
srand((unsigned int) time(0)); \
for (int i = 1; i <= 8; i++) { \
    className += alphanum[rand() % stringLength]; \
} \
className += '\0';

#define MakeUniqueClassNameW(className, id) \
std::wstring className = L"WFM_"; \
className += id; \
className += L"_"; \
const char alphanum[] = { \
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', \
    'A', 'B', 'C', 'D', 'E', 'F' }; \
int stringLength = sizeof(alphanum) - 1; \
srand((unsigned int) time(0)); \
for (int i = 1; i <= 8; i++) { \
    className += alphanum[rand() % stringLength]; \
} \
className += L'\0';

#define JoinA(str1, str2) (std::string s1(str1).append(str2).c_str())
#define JoinW(str1, str2) (std::wstring s1(str1).append(str2).c_str())

#pragma endregion

#pragma region GUI Toolkit Definitions

#define PT(x) -MulDiv(x, 96, 72)
#define EventHandler static void
#define Event public: METHOD virtual void

#pragma endregion
