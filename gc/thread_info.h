/*
 * thread_info.h
 *
 *  Created on: 2013-01-25
 *      Author: pag
 */

#ifndef THREAD_INFO_H_
#define THREAD_INFO_H_

#include "gc/list.h"

namespace gc {

    /// Represents the info stored per thread.
    struct thread_info {
    public:

        /// True iff this thread is no longer running.
        std::atomic<bool> is_dead;

        thread_info(void) throw()
            : is_dead(ATOMIC_VAR_INIT(false))
        { }
    };


    /// Atomic list of all threads.
    extern atomic_list<thread_info> THREADS;


    /// Each thread has a thread-private pointer to its `thread_info` struct.
    extern __thread thread_info *INFO;
}

#endif /* THREAD_INFO_H_ */
