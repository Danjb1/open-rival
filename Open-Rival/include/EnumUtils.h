#ifndef ENUM_UTILS_H
#define ENUM_UTILS_H

#include <type_traits>

/*
 * Macro to allow enum values to be combined and evaluated as flags.
 *
 * Based on:
 *  - DEFINE_ENUM_FLAG_OPERATORS from <winnt.h>
 *  - https://stackoverflow.com/a/63031334/1624459
 */
#define MAKE_ENUM_FLAGS(TEnum)                                                                                         \
    inline TEnum operator~(TEnum a)                                                                                    \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(~static_cast<TUnder>(a));                                                            \
    }                                                                                                                  \
    inline TEnum operator|(TEnum a, TEnum b)                                                                           \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    inline TEnum operator&(TEnum a, TEnum b)                                                                           \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    inline TEnum operator^(TEnum a, TEnum b)                                                                           \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        return static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));                                    \
    }                                                                                                                  \
    inline TEnum& operator|=(TEnum& a, TEnum b)                                                                        \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }                                                                                                                  \
    inline TEnum& operator&=(TEnum& a, TEnum b)                                                                        \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }                                                                                                                  \
    inline TEnum& operator^=(TEnum& a, TEnum b)                                                                        \
    {                                                                                                                  \
        using TUnder = typename std::underlying_type_t<TEnum>;                                                         \
        a = static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));                                       \
        return a;                                                                                                      \
    }

#endif  // ENUM_UTILS_H
