#include <stdio.h>

#define LOG(...) printf("FATAL: "); printf(__VA_ARGS__)

#ifdef _DEBUG
#define LOG_DEBUG(...) printf("%s %d:", __FILE__, __LINE__); printf(__VA_ARGS__)
#else
#define LOG(...)
#endif
