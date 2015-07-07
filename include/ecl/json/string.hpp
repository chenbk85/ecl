#ifndef ECL_JSON_STRING_HPP
#define ECL_JSON_STRING_HPP

#include <cstdio>
#include <cstring>

#include <array>

#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

template<size_t SIZE>
class string
{
public:
    string()
    {
        m_val.fill('\0');
    }

    string(const char* const ptr)
    {
        for(size_t i = 0; i < SIZE; ++i)
        {
            m_val[i] = ptr[i];
        }
    }

    constexpr static size_t size()
    {
        return SIZE;
    }

    template<typename STREAM>
    void serialize(STREAM& st, bool beautify, size_t indent)               const
    {
        (void)beautify;
        (void)indent;

        st << "\"";

        for(size_t i = 0; i < strlen(m_val.data()) && i < SIZE; ++i)
        {
            if(m_val[i] == '"'  ||
               m_val[i] == '\\' ||
               m_val[i] == '/')
            {
                st << "\\";
                st << m_val[i];
            }
            else if(m_val[i] == '\b')
            {
                st << "\\b";
            }
            else if(m_val[i] == '\f')
            {
                st << "\\f";
            }
            else if(m_val[i] == '\n')
            {
                st << "\\n";
            }
            else if(m_val[i] == '\r')
            {
                st << "\\r";
            }
            else if(m_val[i] == '\t')
            {
                st << "\\t";
            }
            else
            {
                st << m_val[i];
            }
        }

        st << "\"";
    }

    bool deserialize(const char* s)
    {
        const char* ptr = s;
        return deserialize_ref(ptr);
    }

    bool deserialize_ref(const char*& s)
    {
        spaces_rollup(s);
        if(*s != '"')
        {
            return false;
        }
        s++;

        for(size_t i = 0; i < SIZE; ++i)
        {
            spaces_rollup(s);
            if(*s == '"')
            {
                s++;
                break;
            }

            spaces_rollup(s);
            if(*s == '\\' && *(s + 1) == '"')
            {
                s++;
            }

            m_val[i] = *s;
            s++;
        }

        return true;
    }

    string<SIZE>& operator= (string<SIZE>& other)
    {
        m_val = other.m_val;
        return *this;
    }

    string<SIZE>& operator= (const char* const ptr)
    {
        for(size_t i = 0; i < SIZE; ++i)
        {
            m_val[i] = ptr[i];
        }

        return *this;
    }

    operator char*()
    {
        return m_val.data();
    }

    operator const char*()                                                 const
    {
        return m_val.data();
    }

    char* data()
    {
        return m_val.data();
    }

    const char* data()                                                     const
    {
        return m_val.data();
    }

    std::array<char, SIZE + 1> m_val { { } }; // 1 for '0'
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_STRING_HPP
