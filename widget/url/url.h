#include <string>

namespace widget {

class Url {
 public:
    static std::string Encode(const std::string& str);
    static std::string Decode(const std::string& str);
};

} // namespace widget

