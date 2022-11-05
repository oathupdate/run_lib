#pragma once

#include <chrono>
#include <ctime>

namespace widget {

class Time {
 public:
    static int64_t Now() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    static int64_t NowMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }
    static int64_t ToDay() {
        time_t t = time(NULL);
        struct tm* st = localtime(&t);
        return t - st->tm_hour * 3600 - st->tm_min * 60 - st->tm_sec;
    }
    static const int64_t KOneDay = 24 * 3600;
};

} // namespace widget
