#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>

#include "widget/sql/sql.h"

namespace dictionary {

class Word {
 public:
    Word(const std::string &e, const std::string &p, const std::string &i) :
        english_(e), pronunciation_(p), interpretation_(i) {}
    Word() {}
    void Encode();
    void Output();
    std::string english_;
    std::string pronunciation_;
    std::string interpretation_;
    std::string example_;
    uint32_t time_ = 0;
    uint32_t last_study_time_ = 0;
    uint32_t error_count_ = 0;  // 错误次数
};

} // namespace dictionary
