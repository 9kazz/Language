#ifndef VERIFY_H
#define VERIFY_H

enum error_codes {
    END_WITH_ERR          = 0,
    EC_MIN_CRITICAL_ERROR = 1 << 10, // All errors more than this constant are critical
    EC_NULL_POINTER       = 1 << 10,
    EC_PARENT_ERR         = 2 << 10
};

#ifdef DEBUG
    #define ONDEBUG(code) code
#else
    #define ONDEBUG(code)
#endif

#endif