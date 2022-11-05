#include <string>

namespace widget {

class File {
 public:
    static bool Load(const std::string name, std::string *content);
};

} // namespace widget
