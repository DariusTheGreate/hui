#ifndef _DELETERS_
#define _DELETERS_

#include "memory.h"

namespace hui{
    template<typename T>
    static constexpr  void standartDeleter(T* ptr){
        hui::destroy(ptr);        
        hui::deallocate(ptr);
    }

    template<class T>
    struct default_deleter{
        //enable_if<is_convertible<T, U>>
        //template<typename U>
        //default_deleter(const default_deleter<U>& defdel){}

        void operator ()(T* ptr){
            hui::deallocate(ptr);
        }
    };

    template<class T>
    struct default_deleter<T[]>{
        void operator()(T* ptr){
            hui::deallocate_n(ptr);
        }
    };
};

#endif

