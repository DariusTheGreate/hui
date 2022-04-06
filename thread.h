#include <thread>
#include <mutex>
#include <pthread.h>

#include "utility.h"

namespace hui{
    std::thread tr;
    using huitphread = pthread_t;

    class thread{
    protected:
        class id{
            huitphread thread_id;
    
        public:
            id() noexcept : thread_id() { }

            explicit id(huitphread id_in) : thread_id(id_in) { }
    
        private:
            friend class thread;
    
            friend bool
            operator==(thread::id __x, thread::id __y) noexcept;
    
            friend bool
            operator<(thread::id __x, thread::id __y) noexcept;
        };
    

    public:
        thread() = default;

        template<typename Functor, typename... Args>
            class caller_wrapper{
                Functor f;
                caller_wrapper(Functor&& f_, Args&&... args) : f(f_) {
                    call((void*)args...);
                }
                
                void* call(void* args){
                    return nullptr;
                }
        };

        template<class Functor, class... Args>
        explicit thread(Functor&& f, Args&&... args){
           // int status = pthread_create(&m_id.thread_id, NULL, hui::forward<Functor&&>(f), hui::forward<Args&&>(args)...);   
        }
        
        void join(void** attr = nullptr){
            int status = pthread_join(m_id.thread_id, attr);
        }

    private:
        id m_id;

    };
};

