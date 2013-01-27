/*
 * pthread.h
 *
 *  Created on: 2013-01-25
 *      Author: pag
 */

#ifndef PTHREAD_H_
#define PTHREAD_H_

#include <pthread.h>

namespace gc {

    /// Wrapped version of `pthread_create`.
    int pthread_create (
        pthread_t *__restrict __newthread,
        __const pthread_attr_t *__restrict __attr,
        void *(*__start_routine) (void *),
        void *__restrict __arg
    );

    /// Wrapped version of `pthread_join`.
    int pthread_join (
    	pthread_t __th,
    	void **__thread_return
    );

}


#endif /* PTHREAD_H_ */
