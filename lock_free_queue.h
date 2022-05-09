#ifndef _HUI_LOCK_FREE_QUEUE_
#define _HUI_LOCK_FREE_QUEUE_

#include "huidef.h"
#include "atomic.h"

#include <string>
#include <vector>

template<typename T>
class lock_free_queue{

private:

    struct atomic_node{
        T data;
        std::string s;
        std::vector<int> v;
        std::atomic<hui::size_t> next;
    };


};

#endif
