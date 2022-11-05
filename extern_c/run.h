#ifndef _RUN_H_
#define _RUN_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    DIC_ERROR = -1,
    DIC_OK = 0
};

int dictionary_update(const char *data, uint32_t size);

#ifdef __cplusplus
};

#endif
#endif
