#ifndef _STRING_
#define _STRING_
#include "allocator.h"
#include "memory.h"
#include <iostream>

#include <string>

/*
 *ther`s a lot of string representation, like gcc string, that just holds pointer to heap and store all the metadata of its own on the left from that pointer:
    - gcc_string: [data_ptr]--
                      |
                      |
                      --> [[size][cap][ref_count][............data...........]]
    but there is 2 nasty optimisations:

    1) string single instance of null-string. cause every time you construct string, every time you move string, etc you do have an empty string. IN case you mallocing null string every time you use          it - you have like 20 nanosec delay. we dont want that(~). So gcc has global variable thats an empty string:
        [empty str 1] -----------
        [empty str 2] -----------
        [empty str 3] -----------
                                |
                                |
                                --> [[0][0][0]['\0']]
        you could just store NULL and check in if() wheter or not is is a NULL, but its ti much work for branch predictor.

    2) depricated cause of multithreading in c++11 COW optimisation:
        if you call copy constructor - just make a pointer to original string, and increase ref counter.
    

    - alexandrescu string => SSO:
      1) we store union of a normal string, and stack optimised string
        union {
            - [[data_ptr][size][cap]]
                 |
                 --->["hello world\0"]
            - [[23 bytes of string][amount_of_free_place_in_that_string]], for example => [["hello, world\0"][10]], or which is SUPER COOL: [[hello, world!!!!!!!!!!!][0]]. And this 0 IS '\0'
        }

      2) we know that malloc returns to us memory bucket that aligned to 8, so sometimes we can increase size of our string
      
      3) we use COW, fot strings that are greater then 255
      
        it is stil not good, cause every size check and every call to string have to check wheter is is small string or normal string. BUT facebook string works better on benchmarks
      in the end faceboko got..Wait for it.. 1% perfomance!  
    - gcc (v > 5):
        union{
              - [[data_ptr][size][cap]['\0']]
                    |
        ["data"] <---

              - [[data_ptr][size]["string data"]]
                     |           |
                     ------------
       }

       so there is no conditional branching. moreover sizeof(str) is 32


       std string is also responsable for manipulation of a different charachter types. wchar, char_16, etc.
       
 * */

namespace hui{
    template<typename T, typename Allocator = hui::Allocator<T>>
    class basic_string{
        using alloc_traits = typename hui::allocator_traits<Allocator>;    
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using const_reference = const T&;
        using r_reference = T&&;
        using size_type = typename alloc_traits::size_type;
        using difference_type = typename alloc_traits::difference_type;
        using reference = typename alloc_traits::reference;
        using pointer = typename alloc_traits::pointer;
        using const_pointer = typename alloc_traits::const_pointer;
        
        //empty case constructor 
        basic_string() /*noexcept(hui::is_nothrow_copy_constructible_v)*/ 
            : m_p(get_ptr_to_sso_buff()), m_str_len(0) {}   
    
        basic_string(char* data) {
            while(data[m_str_len++] != '\0'){}
            std::cout << m_str_len << "\n";
 
            if(m_str_len < get_buffer_sso_size() + 1){
                m_p = get_ptr_to_sso_buff();
            }

            else{
                m_p = alloc_traits::allocate(alloc, m_str_len); 
            }
            
            hui::uninitialized_copy_n(data, m_str_len, m_p);
        }

        basic_string(const basic_string& str2){
            //COW?
            
            m_str_len = str2.m_str_len;
            
            if(m_str_len < get_buffer_sso_size() + 1){
                m_p = get_ptr_to_sso_buff();
            }

            else{
                m_p = alloc_traits::allocate(alloc, m_str_len);
            }
            
            hui::uninitialized_copy_n(str2.m_p, str2.m_str_len, m_p);
            
        }

        ~basic_string(){
            if(m_str_len > get_buffer_sso_size() + 1)
                alloc_traits::deallocate(alloc, m_p, m_str_len);
        }

        pointer c_str() const noexcept{
            return m_p;
        }

    private:
        pointer m_p = nullptr;
        Allocator alloc;
        size_type m_str_len = 0;

        //std::string str;        
    private:
        constexpr static int get_buffer_sso_size(){
            return 15/sizeof(T);
        }
        
        union {
            value_type m_local_buf[get_buffer_sso_size() + 1];
            size_type cap;
        };

        constexpr char* get_ptr_to_sso_buff(){
            return reinterpret_cast<char*>(&m_local_buf);
        }
    };

    using string = hui::basic_string<char>;


};

#endif

