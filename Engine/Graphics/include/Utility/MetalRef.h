#pragma once

#include <utility>

template<typename T>
class MetalRef {
 public:
  MetalRef() = default;
  explicit MetalRef(T* ptr) : m_ptr(ptr) { m_ptr->retain(); }

  ~MetalRef() {
    if (!m_ptr)
      return;
    m_ptr->release();
  }

  MetalRef(const MetalRef<T>& rhs) = delete;

  MetalRef& operator=(const MetalRef<T>& rhs) = delete;

  MetalRef(MetalRef<T>&& rhs) noexcept {
    *this = std::move(rhs);
  }

  MetalRef& operator=(MetalRef<T>&& rhs) noexcept {
    if (this != &rhs)
      std::swap(m_ptr, rhs.m_ptr);
    return *this;
  }

  auto operator->() { return m_ptr; }
  auto operator->() const { return m_ptr; }
  explicit operator bool() const { return m_ptr; }
  [[nodiscard]] T* get() const { return m_ptr; }

 private:
  T* m_ptr = nullptr;
};
