/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include <assert.h>
#include <condition_variable>
#include <deque>
#include <inttypes.h>
#include <mutex>

#ifndef SRSRAN_TTI_SEMPAHORE_H_
#define SRSRAN_TTI_SEMPAHORE_H_

namespace srsran {

/**
 * Implements priority semaphore based on a FIFO queue wait . This class enqueues T type element identifiers (method
 * push) and waits until the enqueued object is the first (method wait). The first element is released by method
 * release. The method release_all waits for all the elements to be released.
 *
 * @tparam T Object identifier type
 */
template <class T>
class tti_semaphore
{
private:
  std::mutex              mutex; ///< Used for scope mutexes
  std::condition_variable cvar;  ///< Used for notifying element identifier releases
  std::deque<T>           fifo;  ///< Queue to keep order

public:
  /**
   * Waits for the first element of the queue match the element identifier provided.
   *
   * @param id the element identifier
   */
  void wait(T id)
  {
    std::unique_lock<std::mutex> lock(mutex);

    // While the FIFO is not empty and the front ID does not match the provided element identifier, keep waiting
    while (not fifo.empty() and fifo.front() != id) {
      // Wait for a release
      cvar.wait(lock);
    }
  }

  /**
   * Enqueue (push) an element identifier to the queue
   *
   * @param id the element identifier
   */
  void push(T id)
  {
    std::unique_lock<std::mutex> lock(mutex);

    // Append the element identifier
    fifo.push_back(id);
  }

  /**
   * Releases (pops) the first element
   */
  void release()
  {
    std::unique_lock<std::mutex> lock(mutex);

    // Pop first element
    fifo.pop_front();

    // Notify release
    cvar.notify_all();
  }

  /**
   * Waits for all the elements to be released
   */
  void wait_all()
  {
    std::unique_lock<std::mutex> lock(mutex);

    // Wait until the FIFO is empty
    while (not fifo.empty()) {
      cvar.wait(lock);
    }
  }
};
} // namespace srsran

#endif // SRSRAN_TTI_SEMPAHORE_H_