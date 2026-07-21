#pragma once

#ifdef NDEBUG
    #define DH_ASSERT(condition) ((void)0)
#else
#define DH_ASSERT(condition) \
        do { \
            if (!(condition)) { \
                __builtin_trap(); \
            } \
        } while(0)
#endif