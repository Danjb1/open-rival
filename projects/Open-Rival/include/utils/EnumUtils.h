#pragma once

#include <type_traits>

/*
 * Macro to allow enum values to be combined and evaluated as flags.
 *
 * Based on:
 *  - DEFINE_ENUM_FLAG_OPERATORS from <winnt.h>
 *  - https://stackoverflow.com/a/63031334/1624459
 */
#define MAKE_ENUM_FLAGS(TEnum)                                                                                         \
    constexpr inline TEnum operator~(TEnum a)                                                                          \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(~static_cast<TUnder>(a));                                                            \
    }                                                                                                                  \
    constexpr inline TEnum operator|(TEnum a, TEnum b)                                                                 \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    constexpr inline TEnum operator&(TEnum a, TEnum b)                                                                 \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    constexpr inline TEnum operator^(TEnum a, TEnum b)                                                                 \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    constexpr inline TEnum& operator|=(TEnum& a, TEnum b)                                                              \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }                                                                                                                  \
    constexpr inline TEnum& operator&=(TEnum& a, TEnum b)                                                              \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }                                                                                                                  \
    constexpr inline TEnum& operator^=(TEnum& a, TEnum b)                                                              \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }

namespace EnumUtils {

/**
 * Converts an enum to its underlying type.
 *
 * Based on: https://stackoverflow.com/a/14589519/1624459
 */
template <typename T>
constexpr auto toIntegral(T e)
{
    return static_cast<std::underlying_type_t<T>>(e);
}

}  // namespace EnumUtils
