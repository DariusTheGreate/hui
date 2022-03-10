#ifndef _EXCEPTION_
#define _EXCEPTION_

#include <exception>

namespace hui{
    class exception
    {
    public:
        exception() { }
        virtual ~exception() = default ;
    //#if cpp_version > 03
        exception(const exception&) = default;
        exception& operator=(const exception&) = default;
        exception(exception&&) = default;
        exception& operator=(exception&&) = default;
    //#endif

        virtual const char* what() const {
            return "standart hui::exception problem\n";
        }; 
    };

    class bad_alloc : public exception {
    public:
        bad_alloc() throw() {}
        virtual ~bad_alloc() throw() {};
        virtual const char* what() const throw() {
            return "bad cast";
        }
    };
};
#endif

