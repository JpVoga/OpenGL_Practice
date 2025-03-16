#include "hash.hpp"

namespace CG {
    size_t StringViewHash::operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    bool StringViewEqual::operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return (lhs == rhs);
    }
}