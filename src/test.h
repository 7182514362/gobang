#include <iostream>

void testMacro() {
  int len = sizeof(int) * 8;
  std::cout << "sizeof(int)=%d\n" << len;

  len = sizeof(int *) * 8;
  std::cout << "sizeof(int*)=%d\n" << len;

#ifdef _MSC_VER
  std::cout << "_MSC_VER is defined\n";
#endif

#ifdef __GNUC__
  std::cout << "__GNUC__ is defined\n";
#endif

#ifdef __INTEL__
  std::cout << "__INTEL__  is defined\n";
#endif

#ifdef __i386__
  std::cout << "__i386__  is defined\n";
#endif

#ifdef __x86_64__
  std::cout << "__x86_64__  is defined\n";
#endif

#ifdef _WIN32
  std::cout << "_WIN32 is defined\n";
#endif

#ifdef _WIN64
  std::cout << "_WIN64 is defined\n";
#endif

#ifdef __linux__
  std::cout << "__linux__ is defined\n";
#endif

#ifdef __LP64__
  std::cout << "__LP64__ is defined\n";
#endif

#ifdef __amd64
  std::cout << "__amd64 is defined\n";
#endif
}