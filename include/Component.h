#pragma once

#include <typeinfo>
#include <cstdint>
#include <bitset>

namespace Entra {
    constexpr int MAX_COMPONENTS = 32;

    typedef std::size_t ComponentId;
    typedef uint32_t Signiture;

    constexpr ComponentId INVALID_COMPONENT_ID = 0;
    constexpr Signiture INVALID_SIGNITURE = 0;
}