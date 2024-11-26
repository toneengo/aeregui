#include <stdio.h>

#ifdef _DEBUG
    #define DBG
#endif

#ifdef _DEBUG
    #define dbg_log(...) printf("%s (%i): ", __FILE__, __LINE__), printf(__VA_ARGS__), printf("\n")
#else
    #define dbg_log(...) do {} while(0)
#endif

