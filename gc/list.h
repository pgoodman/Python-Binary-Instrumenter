/*
 * list.h
 *
 *  Created on: 2013-01-25
 *      Author: pag
 */

#ifndef LIST_H_
#define LIST_H_

#include <atomic>

namespace gc {

    namespace detail {
        template <typename T>
        struct list {
            std::atomic<list<T> *> next;
            T value;
        };
    }


    /// Represents an atomic, add-only list.
    template <typename T>
    struct atomic_list {
    private:

        typedef detail::list<T> list_item;

        std::atomic<list_item *> head;

    public:

        /// Initialise the empty list.
        atomic_list(void) throw()
            : head(ATOMIC_VAR_INIT(nullptr))
        { }

        /// Free the list.
        ~atomic_list(void) throw() {
            list_item *item(head.exchange(nullptr));
            for(list_item *next(nullptr); item; item = next) {
                next = item->next.load();
                delete item;
            }
        }

        /// Atomically add an item to the list.
        T *append(void) throw() {
            list_item *list_head(nullptr);
            list_item *item(new list_item);

            do {
                list_head = head.load();
                item->next = list_head;
            } while(!head.compare_exchange_weak(list_head, item));

            return &(item->value);
        }

        /// Call a function for each item in the list.
        template <typename... Args>
        void for_each(void (*visitor)(T &, Args&...), Args&... args) throw() {
            list_item *item(head.load());
            for(; item; item = item->next.load()) {
                visitor(item->value, args...);
            }
        }
    };

}


#endif /* LIST_H_ */
