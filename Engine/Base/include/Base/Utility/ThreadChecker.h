#pragma once

#include <thread>

namespace goala {
class ThreadChecker final {
public:
  ThreadChecker();

  void checkThread() const;

private:
  std::thread::id m_threadId; // 생성자 쓰레드 ID
};
} // namespace goala
