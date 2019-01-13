//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FLAGS_H__
#define __FLAGS_H__

#pragma once

BEGIN_NAMESPACE(core)

enum class flags : uint64_t
{
    clear     = 0x00,
    nothing   = 0x00,
    marked    = 0x01,
    genuine   = 0x02,
    synthetic = 0x04, // additional (artificial) tokens which are inserted into the token stream ...
    final     = 0x08
};

using T = std::underlying_type_t <flags>;

inline flags operator | (const flags& lhs, const flags& rhs)
{
    return static_cast<flags>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline flags operator |= (flags& lhs, const flags& rhs)
{
    lhs = static_cast<flags>(static_cast<T>(lhs) | static_cast<T>(rhs));
    return lhs;
}

inline flags operator & (const flags& lhs, const flags& rhs) 
{
    return static_cast<flags>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline flags operator &= (flags& lhs, const flags& rhs) 
{
    lhs = static_cast<flags>(static_cast<T>(lhs) & static_cast<T>(rhs));
    return lhs;
}

inline flags operator ~ (const flags& lhs) 
{
    return static_cast<flags>(~static_cast<T>(lhs));
}

#define MARKED(__x) (((*__x).flags() & flags::marked) == flags::marked)
#define MARK(__x)   ((*__x).flags() = modify_flags<flags>((*__x).flags(), flags::marked, flags::clear))

END_NAMESPACE

#endif // __FLAGS_H__
