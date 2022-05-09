#ifndef _HUI_ANY_
#define _HUI_ANY_

#include<experimental/any> 
#include <type_traits>

#include "my_traits.h"
#include "utility.h"
#include "memory.h"

//#define ANY_IMPLEMENTATION_USING_VIRTUAL_FUNCTIONS_
#define ANY_HARD_STD_KIND_IMPLEMENTATION_ 

// https://www.fluentcpp.com/2021/02/05/how-stdany-works/
namespace hui{

#ifdef ANY_HARD_STD_KIND_IMPLEMENTATION_    
    class any{
    public:
       template<typename U>
       any(U&& u_in){
           handle = dispathAndConstructHandler<U>();
           store = dispathAndConstructStorage(u_in);
       }

       //TODO(fix): being hidded by templated constructor. Use some sfinae shit to fix it;
       any(const any& a_in) : store(nullptr), handle(a_in.handle) { 
           handle->copier(a_in.store, store);
       }

       auto get_data(){
           return handle -> acess(strore);
       }

       ~any(){
           handle -> deleter(store);
           delete store;
           delete handle;
       }

    private:
        union Storage{
            void* data;
            using stack_storage_type = std::aligned_storage<sizeof(data), alignof(void*)>::type; 
            stack_storage_type storage; 
                     
            // add checks for copying?
            Storage() = default;
            Storage(const Storage&) = delete;
            Storage& operator=(const Storage&) = delete;
        };
        
        struct Handler
        {
           //virtual void deleter(Storage* data) {};
            void (*deleter)(Storage*);           
            void (*copier)(Storage*, Storage*);
            auto (*acess)(Storage*);
            
            Handler() = default;

            Handler(const Handler& handle_in) : deleter(handle_in.deleter), copier(handle_in.copier), acess(handle_in.acess){
            }
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
        
        //heap version
        template<typename T>
        struct MemoryHandler<T, true>{
            static void deleter(Storage* store) {
                delete static_cast<T*>(store -> data);
                //std::cout << "deleted in heap version\n";
            } 
            
            template<typename Pr>
            static void create(Storage* store, Pr&& val){
                store -> data = new T(hui::forward<Pr>(val)); 
                //std::cout << "created in heap version\n";
            } 

            static void copy(Storage* from, Storage* to){
               *static_cast<T*>(to -> data) = *static_cast<T*>(to -> data);
            }

            static auto acess(Storage* store){
                return static_cast<T*>(store -> data);
            }
        };

        //stack version
        template<typename T>
        struct MemoryHandler<T,false> {
            static void deleter(Storage* store) {
                hui::destroy(reinterpret_cast<T*>(&(store -> storage)));
                //std::cout << "deleted in stack version\n";
            }  

            template<typename Pr>
            static void create(Storage* store, Pr&& val)
            {
                //char* buffer = hui::allocate_n<char>(1);  
                new (&(store -> storage)) T(hui::forward<Pr>(val));
                //std::cout << "created in stack version\n";
            }
            
            static void copy(Storage* from, Storage* to){
            }
        };
        
        template<typename _Tp, typename _Safe = std::is_nothrow_move_constructible<_Tp>,
           bool _Fits = (sizeof(_Tp) <= sizeof(Storage))
                && (alignof(_Tp) <= alignof(Storage))>
        using _Internal = std::integral_constant<bool, _Safe::value && _Fits>;

        template<typename T>
        using _Manager = std::conditional_t<_Internal<T>::value, MemoryHandler<T, 0>, MemoryHandler<T,1>>;


        template<typename T, bool fits = !(std::is_nothrow_move_constructible<T>::value 
                && sizeof(T) <= sizeof(Storage::stack_storage_type) 
                && alignof(T) <= alignof(Storage::stack_storage_type))> 
        using checkIfFits = std::integral_constant<bool, fits>;
        
        template<typename T>
        using checkIfNothrowMoveConstructible = std::integral_constant<bool, (std::is_nothrow_move_constructible<T>::value)>;
        
        //TODO: understand why aligment/sizeof dispathing doesnt work
        template<typename T>
        //using Manager = std::conditional_t<!checkIfNothrowMoveConstructible<T>::value, MemoryHandler<T, 1>, MemoryHandler<T,0>>;
        //using Manager = std::conditional_t<checkIfFits<T>::value, MemoryHandler<T, 0>, MemoryHandler<T,1>>;
        using Manager = _Manager<T>;
 
                template<typename T>
        constexpr auto castHandleToManager(){
            return static_cast<Manager<T>*>(handle);
        }
        
        template<typename T> 
        auto dispathAndConstructHandler()
        {
            //using ManagerType = Manager<T>;
            //if constexpr(hui::is_same<ManagerType, MemoryHandler<T, 0>>::val)
            //    std::cout << "heap version in handler dispathing\n";
            //ManagerType* h = new ManagerType();
            return new Handler();
            //return h;
        } 

        template<typename T>
        Storage* dispathAndConstructStorage(T& val)//need to make ot sfinae in order to pass by T&&
        {
            Storage* store = new Storage();
            if constexpr(hui::is_same_v<Manager<T>, MemoryHandler<T, 1>>){
                //std::cout << "heap stprage\n";
                //static_cast<Manager<T>*>(handle) -> create(store, hui::move(val));
                //castHandleToManager<T>() -> create(store, hui::move(val));
                MemoryHandler<T, 1>::create(store, std::move(val)); 
                handle -> deleter = MemoryHandler<T, 1>::deleter;
            }
            else {
                //std::cout << "stack storage\n";
                //static_cast<Manager<T>*>(handle) -> create(store, hui::move(val));
                //castHandleToManager<T>() -> create(store, hui::move(val));
                MemoryHandler<T, 0>::create(store, std::move(val)); 
                handle -> deleter = MemoryHandler<T, 0>::deleter;
            }
            return store;
        }

    public:
        Storage* store;
        Handler* handle;
    };

#endif


#ifdef ANY_IMPLEMENTATION_USING_VIRTUAL_FUNCTIONS_

    class any{

        struct StorageBase{
            virtual ~StorageBase() = default; 
            virtual StorageBase* get_copy(){ return new StorageBase();} 
        };
        
        template<typename T>
        struct Storage : StorageBase {
            T val;

            Storage(T&& val) : val(hui::forward<T>(val)){
            }

            StorageBase* get_copy() override {
                return new Storage<T>(val);
            }

            ~Storage() = default;
        };

        StorageBase* maintainer; 
    public:
        template<typename T>
        any(T&& val) : maintainer(new Storage<T>(hui::forward<T>(val))) {}
        ~any() { delete maintainer; }

        auto get_value(){
            //TODO: how to get value from StorageBase<X> if i dont know X at this point?
        }
    };

#endif

};
#endif


