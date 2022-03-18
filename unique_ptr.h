#ifndef _UNIQUE_PTR_
#define _UNIQUE_PTR_

#include "allocator.h"
#include "deleters.h"
#include "utility.h"
#include <cstddef>
#include <functional>
#include <memory>

/*USE CAREFULLY, IN PROGRESS*/

std::unique_ptr<int> gp; 

namespace hui{
    template<typename T, typename Deleter = hui::default_deleter<T>>
    class unique_ptr {
    public:
        using pointer = T*;
        using value_type = T;
        using deleter_type = Deleter;
        //...
    public:
        unique_ptr(std::nullptr_t){
            ptr = nullptr;
        }
        //make_unique
        unique_ptr(pointer p) : ptr(p) {
        }

        //not sure about deler
        unique_ptr(unique_ptr&& ruptr) : ptr(ruptr.ptr), deler(hui::forward<deleter_type>(ruptr.get_deleter())) {
            ruptr.ptr = nullptr;
        }

        unique_ptr& operator =(unique_ptr&& uptr_in){
            ptr = uptr_in.ptr;
            deler = hui::move(hui::forward<deleter_type>(uptr_in.get_deleter()));//do we need this, is this okay?
        }

        ~unique_ptr(){
            reset();
        }
        
        //and what if ptr == nullptr
        value_type& operator *(){
            return *ptr;
        }

        value_type* operator -> (){
            return ptr;
        }

        value_type* get_ptr(){
            return ptr;
        }

        void reset(pointer p_in = pointer()){
            if(ptr != p_in){
                hui::swap(ptr, p_in);
                deler(p_in);
            }
        }

        deleter_type& get_deleter(){
            return deler;
        }

    private:
        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator =(const unique_ptr&) = delete;
    private:    
        //use pair here?..
        pointer ptr;
        Deleter deler;
    };
};

#endif

