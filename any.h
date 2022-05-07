#ifndef _HUI_ANY_
#define _HUI_ANY_

#include<experimental/any> 
#include <type_traits>

#include "my_traits.h"
#include "utility.h"
#include "memory.h"

// https://www.fluentcpp.com/2021/02/05/how-stdany-works/
namespace hui{
    
    class any{
    public:
      
       template<typename U> 
       any(U&& u_in){
           handle = dispathAndConstructHandler<U>();
           store = dispathAndConstructStorage(u_in);
       }

    private:
        union Storage{
            void* data;
            using stack_storage_type = std::aligned_storage<sizeof(void*), alignof(void*)>; 
            stack_storage_type storage; 
                     
            // add checks for copying?
            Storage() = default;
            Storage(const Storage&) = delete;
            Storage& operator=(const Storage&) = delete;
        };
        
        struct Handler
        {
            //oid deleter(void* data);
            //template<typename T>
            //void create(void* data, T&& val);
        }; 

        //NOTE: Decaying 
        //0 -> stack
        //1 -> heap 
        //2 -> arrays?
        //3 -> shared_ptr?
        template<typename T, bool type>
        class MemoryHandler;

        template<typename T>
        struct MemoryHandler<T, false> : public Handler{
            static void deleter(void* data){
                //static_cast<T*>(data) -> ~T(); 
                hui::destroy(static_cast<T*>(data));
            } 
            
            template<typename Pr>
            static void create(void* data, Pr&& val){
                data = new (data) T(hui::forward<Pr>(val)); 
                //hui::construct(data, hui::forward<Pr>(val));

                std::cout << "in create: " << val << " " << *((int*)data) << "\n";
            } 
        };

        template<typename T>
        struct MemoryHandler<T,true>  : public Handler{
            static void deleter(void* data){
                delete static_cast<T*>(data);
            }  

            template<typename Pr>
            static void create(void* data, Pr&& val)
            {
                //char* buffer = hui::allocate_n<char>(1);  
                data = new (data) T(hui::forward<Pr>(val));
                std::cout << "in create: " << val << " " << *((int*)data) << "\n";
            }
        };
        
        template<typename T, bool fits = !(std::is_nothrow_move_constructible<T>::value 
                && sizeof(T) <= sizeof(Storage::stack_storage_type) 
                && alignof(T) <= alignof(Storage::stack_storage_type))> 
        using checkIfFits = std::integral_constant<bool, fits>;
        
        template<typename T>
        using Manager = std::conditional_t<checkIfFits<T>::value, MemoryHandler<T, 0>, MemoryHandler<T,1>>;
        
        template<typename T> 
        auto dispathAndConstructHandler()
        {
            using ManagerType = Manager<T>;
            //Handler h = {
            //    ManagerType::deleter,
            //    ManagerType::create
            //};
            ManagerType* h = new ManagerType();
            return h;
        } 
        

        template<typename T>
        Storage* dispathAndConstructStorage(T& val)//need to make ot sfinae in order to pass by T&&
        {
            Storage* store = new Storage();
            if constexpr(hui::is_same_v<Manager<T>, MemoryHandler<T, 0>>){
                std::cout << "heap stprage\n";
                //store->data = new T(hui::move(val));  
                static_cast<Manager<T>*>(handle) -> create(&store -> data, hui::move(val));
            }
            else {
                std::cout << "stack storage\n";
                static_cast<Manager<T>*>(handle) -> create(&store -> storage, hui::move(val));
                //new  (&store->storage) T(hui::forward<T>(val));
            }
            return store;
        }
 
        /*template<typename T>
        Storage* dispathAndConstructStorage(T&& val)
        {
           Storage* store = new Storage(); 

           new (store->storage) T(hui::forward<T>(val));
           return store;
        }*/

    public:
        Storage* store;
        Handler* handle;
    };

};

#endif


