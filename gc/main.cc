
#include <atomic>

#include <sys/types.h>
#include <unistd.h>

#include "gc/list.h"
#include "gc/pthread.h"
#include "gc/thread_info.h"

extern int program_main(int, char **);

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

        MAIN_THREAD_DONE.store(true);

        return nullptr;
    }
}

namespace gc {

    /// list of threads.
    atomic_list<thread_info> THREADS;

}

int main(int argc, char **argv) throw() {

    typedef void *(thread_routine)(void *);

    main_thread_args args = {
        argc, argv, 0
    };

    pthread_t main_thread = pthread_t();
    pthread_attr_t main_thread_attr;

    pthread_attr_init(&main_thread_attr);
    gc::pthread_create(
        &main_thread,
        &main_thread_attr,
        (thread_routine *) main_thread_func,
        &args);

    for(; ; ) {
        usleep(1000);
        
        // TODO: do a concurrent leak check!

        if(MAIN_THREAD_DONE.load()) {
            break;
        }
    }

    // TODO: tear down GC state
    return args.return_value;
}
