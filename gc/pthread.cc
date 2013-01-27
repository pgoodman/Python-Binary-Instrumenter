/*
 * pthread.cc
 *
 *  Created on: 2013-01-25
 *      Author: pag
 */

#include <cstdio>
#include <cstdlib>

#include "gc/pthread.h"
#include "gc/thread_info.h"
#include "gc/list.h"


namespace gc {


    /// Thread-private `thread_info` structure. This points to one of the
    /// `thread_info` structs in the list.
    __thread thread_info *INFO;


    /// Type of a pthread thread routine.
    typedef void *(thread_routine)(void *);


    /// Data passed into a thread function that contains the original stuff
    /// needed to create the intended thread, as well as some extra things.
    struct thread_data {
        thread_routine *func;
        thread_info *info;
        void *data;
    };


    /// Small wrapper around a thread routine.
    void *thread_routine_func(thread_data *data) throw() {
        INFO = data->info;
        printf("thread function callback wrapper\n");
        void *ret(data->func(data->data));
        data->info->is_dead.store(true);
        delete data;
        return ret;
    }


    /// Wrapped version of `pthread_create`. The responsibility of this function
    /// is to properly set up the thread local information needed by the GC.
    int pthread_create (
        pthread_t *__restrict __newthread,
        __const pthread_attr_t *__restrict __attr,
        void *(*__start_routine) (void *),
        void *__restrict __arg
    ) {
        thread_data *data(new thread_data);
        data->data = __arg;
        data->func = __start_routine;
        data->info = THREADS.append();

        // run the original.
        int ret(::pthread_create(
            __newthread,
            __attr,
            (thread_routine *) thread_routine_func,
            data));

        data = nullptr;
        return ret;
    }

    	/// wrapped version of `pthread_join`. The responsibility of this function
        /// is to properly free the thread local slots.
        int pthread_join (
        		pthread_t __th,
        		void **__thread_return) {

            // run the original.
            int ret(::pthread_join(
                __th,
                __thread_return));

            return ret;
        }
}


