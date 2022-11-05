#include <fstream>
#include <sstream>

#include "file.h"

namespace widget {

bool File::Load(const std::string name, std::string *content) {
    std::ifstream fin(name);
    if (!fin) {
        return false;
    }

    std::stringstream str_stream;
    str_stream << fin.rdbuf();
    *content = str_stream.str();
    return true;
}

} // namespace widget
