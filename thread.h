#include <thread>
#include <mutex>
#include <pthread.h>

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
    private:
    };
};

