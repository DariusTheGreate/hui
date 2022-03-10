#ifndef _TYPE_INFO_
#define _TYPE_INFO_
namespace hui{
    class type_info{
    public:
        virtual ~type_info();
        const char* name() noexcept {
            return   m_name ; 
        }

    private:
        const char* m_name;
    };
};

#endif

