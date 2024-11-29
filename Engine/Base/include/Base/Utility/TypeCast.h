#pragma once

#ifndef NDEBUG // Debug mode

#include <cassert>
#include <type_traits>

namespace detail {
template <typename DerivedPtr, typename BasePtr>
DerivedPtr downCastWithRuntimeCheck(BasePtr arg) {
  static_assert(std::is_pointer_v<BasePtr>, "Arg must be a pointer");
  using Base = std::remove_pointer_t<BasePtr>;
  static_assert(std::is_pointer_v<DerivedPtr>, "Casted typed must be a pointer");
  using Derived = std::remove_pointer_t<DerivedPtr>;
  static_assert(std::is_base_of_v<Base, Derived>, "Arg is not a base class of Derived type");
  static_assert(!std::is_abstract_v<Derived>, "Cannot cast to an abstract class");
  assert(dynamic_cast<DerivedPtr>(arg) && "Failed to downcast");
  return static_cast<DerivedPtr>(arg);
}
} // namespace detail

#define SAFE_DOWN_CAST(DerivedPointer, arg) detail::downCastWithRuntimeCheck<DerivedPointer>(arg)

#else // Release mode
#define SAFE_DOWN_CAST(DerivedPointer, arg) static_cast<DerivedPointer>(arg)
#endif
