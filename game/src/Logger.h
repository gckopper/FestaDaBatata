#include <stdio.h>


#ifdef _DEBUG
#define LOG_DEBUG(...) printf("%s %d: ", __FILE__, __LINE__); printf(__VA_ARGS__)
#define LOG(...) LOG_DEBUG(__VA_ARGS__)
#else
#define LOG(...) printf("ERROR: "); printf(__VA_ARGS__)
#define LOG_DEBUG(...)
#endif
