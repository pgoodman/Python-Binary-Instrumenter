
#include <atomic>

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

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

int main(int argc, char **argv) throw() {

    typedef void *(pthread_func_t)(void *);

    main_thread_args args = {
        argc, argv, 0
    };

    pthread_t main_thread = pthread_t();
    pthread_attr_t main_thread_attr;

    pthread_attr_init(&main_thread_attr);
    pthread_create(
        &main_thread,
        &main_thread_attr,
        (pthread_func_t *) main_thread_func,
        &args);

    for(; !MAIN_THREAD_DONE.load(); ) {
        usleep(1000);

        // TODO: do a concurrent leak check!
    }

    // TODO: tear down GC state
    return args.return_value;
}
