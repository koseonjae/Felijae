#include <Base/Utility/ThreadChecker.h>

#include <cassert>

using namespace larco;

ThreadChecker::ThreadChecker() : m_threadId(std::this_thread::get_id()) {}

void ThreadChecker::checkThread() const {
  assert(m_threadId == std::this_thread::get_id() && "Invalid thread");
}
