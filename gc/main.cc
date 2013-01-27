
#include <atomic>

#include <sys/types.h>
#include <cstdio>
#include <unistd.h>

#include "gc/list.h"
#include "gc/pthread.h"
#include "gc/thread_info.h"

extern int program_main(int, char **);

#define MAIN_THREAD_FUNC(func) main_thread_func##func;


namespace {

    static std::atomic<bool> MAIN_THREAD_DONE(ATOMIC_VAR_INIT(false));

    struct main_thread_args {
        int argc;
        char **argv;
        int return_value;
    };

    /// The main thread of the program.
    static void *main_thread_func(main_thread_args *args) {
        args->return_value = program_main(
            args->argc,
            args->argv);
        while(1) {
        	//printf("running child thread\n");
        }
        MAIN_THREAD_DONE.store(true);

        return nullptr;
    }

}

namespace gc {

    /// list of threads.
    atomic_list<thread_info> THREADS;

}

#define NUM_THREADS 8

int main(int argc, char **argv) throw() {

	int ret_val[NUM_THREADS];
	pthread_t main_thread[NUM_THREADS];
	pthread_attr_t main_thread_attr[NUM_THREADS];
	unsigned int i = 0;

    typedef void *(thread_routine)(void *);

    main_thread_args args = {
        argc, argv, 0
    };

    for(i = 0; i < NUM_THREADS; i++)
    {
    	main_thread[i] = pthread_t();
    	pthread_attr_init(&main_thread_attr[i]);

    	ret_val[i] = gc::pthread_create(
    					&main_thread[i],
    					&main_thread_attr[i],
    					(thread_routine *) main_thread_func,//MAIN_THREAD_FUNC(1),
    					&args);
    }

   // printf("do concurrent_check place 1\n");

    for(; ; ) {
        usleep(1000);
        
      //  printf("do concurrent_check\n");
        // TODO: do a concurrent leak check!

        if(MAIN_THREAD_DONE.load()) {
           // break;
        }

    }

    for(i = 0; i < NUM_THREADS; i++) {
    	ret_val[i] = gc::pthread_join(
    					main_thread[i],
    					NULL);
    }

    // TODO: tear down GC state
    return args.return_value;
}

extern "C" {
	int __cxa_call_unexpected;
	int __gxx_personality_v0;
}
