#pragma once

#include <type_traits>

namespace larco {
template <typename Enum>
struct EnableBitMaskOperators {
  static constexpr bool enable = false;
};

template <typename Enum>
constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator|(Enum lhs, Enum rhs) {
  using Underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
    static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs)
  );
}

template <typename Enum>
constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator&(Enum lhs, Enum rhs) {
  using Underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
    static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs)
  );
}

template <typename Enum>
constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator^(Enum lhs, Enum rhs) {
  using Underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
    static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs)
  );
}

template <typename Enum>
constexpr typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type operator~(Enum lhs) {
  using Underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
    ~static_cast<Underlying>(lhs)
  );
}
}

#define ENABLE_BITMASK(Enum)          \
template <>                           \
struct EnableBitMaskOperators<Enum> { \
static constexpr bool enable = true;  \
}
