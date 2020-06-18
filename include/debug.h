#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef fail_on_false
#define fail_on_false(ret, fmt, ...) \
do { \
    errsv = errno; \
    if (ret == -1) \
    { \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        goto fail; \
    } \
} while (0)
#endif

#ifdef DEBUG
#define debug_printf(format, ...) fprintf(stderr, "%s:%s:%d: " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define debug_printf_n(format, ...) debug_printf(format "\n", ##__VA_ARGS__)
#else
#define debug_printf_n(format, ...) do{} while(0)
#define debug_printf(format, ...) do{} while(0)
#endif

#define TRUES "true"
#define FALSES "false"

#define true_false(val) (((val) == 1) ? TRUES : FALSES)
#define false_true(val) (((val) == 1) ? FALSES : TRUES)

inline int printhex(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (i > 0) printf(", ");
        printf("0x%02X", buf[i]);
    }
    printf("\n");
};

#ifdef __cplusplus
}
#endif
