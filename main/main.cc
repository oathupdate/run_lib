#include <fstream>
#include <sstream>
#include <glog/logging.h>

#include "modules/dictionary/dictionary.h"
#include "widget/time/time.h"
#include "widget/file/file.h"

using widget::Time;
using widget::File;

#define DIC dictionary::Dictionary::Instance()

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    std::string op;
    if (argc > 1) {
        op = argv[1];
    }
    std::vector<dictionary::Word> ws;
    if (op == "s") {
        int64_t start = Time::ToDay();
        int64_t end = start + Time::KOneDay;
        ws = DIC.FindByRange(start, end);
    } else if (op == "i") {
        DIC.ImportFromEudic("/home/root/word.csv");
    } else {
        ws = DIC.FindByRange(0, Time::Now());
    }
    for (auto &w : ws) {
        w.Output();
    }
    return 0;
}
