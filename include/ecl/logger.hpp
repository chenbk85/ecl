#pragma once
#include "stream.hpp"

namespace ecl
{

// level
enum class lvl {
    critical,
    error,
    warning,
    info,
    debug
};

template<typename... OUT>
class logger
{
    logger(const lvl    def_level = lvl::info)
    
    logger& operator() (const lvl& l)
    {
        m_level = l;
        return *this;
    }

private:

    void out_all()                                                         const
    {
        out<0, OUT...>();
    }

    template<size_t COUNT, typename O, typename... TAIL>
    bool out()                                                             const
    {
        return O::print(m_buf) && out<COUNT + 1, TAIL...>();
    }

    template<size_t COUNT>
    bool out()                                                             const
    {
        static_assert((COUNT == sizeof...(OUT)), "Out count missmatch!");
        return true;
    }
};
    
} // namespace ecl