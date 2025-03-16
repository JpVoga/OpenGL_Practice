#pragma once

#include "_cgControl.hpp"

namespace CG {
    struct StringViewHash {
        using is_transparent = void;
        size_t operator()(std::string_view sv) const noexcept;
    };

    struct StringViewEqual {
        using is_transparent = void;
        bool operator()(std::string_view lhs, std::string_view rhs) const noexcept;
    };
}