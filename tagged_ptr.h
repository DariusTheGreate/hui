#ifndef _TAGGED_PTR_
#define _TAGGED_PTR_

#include "huidef.h"

namespace hui{
    template<typename T>
    class tagged_ptr{
    public:
        T* m_ptr;
        hui::size_t m_tag;
    };
};

#endif


