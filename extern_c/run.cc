#include <vector>

#include "modules/dictionary/dictionary.h"
#include "run.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DIC dictionary::Dictionary::Instance()

int dictionary_update(const char *data, uint32_t size) {
    std::vector<dictionary::Word> ws;
    if (!DIC.ParseData(data, ws)) {
        return DIC_ERROR;
    }
    if (!DIC.Update(ws)) {
        return DIC_ERROR;
    }
    return DIC_OK;
}

#ifdef __cplusplus
};

#endif
