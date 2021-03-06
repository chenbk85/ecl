/**
 * @file
 *
 * @brief JSON string class template.
 *
 * @ingroup json ecl
 */
#ifndef ECL_JSON_STRING_HPP
#define ECL_JSON_STRING_HPP

#include <cstdio>
#include <cstring>

#include <algorithm>
#include <array>

#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

 /**
  * @brief Fixed-length string class for usage with ecl JSON serializer/deserializer.
  * @details Used for string values in key-value pairs:
  * @code
  * {
  *     ...
  *     "val_name" : "string value",
  *     ...
  * }
  * @endcode
  * Uses std::array as internal container.
  * This class is substitution for std::string, that cannot be used in
  * embedded systems because it would try to allocate memory. ecl::json::string
  * have constant size.
  *
  * @tparam SIZE max size of string.
  */
template<std::size_t SIZE>
class string
{
public:
    string()
    {
        m_val.fill('\0');
    }

    explicit string(const char* const ptr)
    {
        std::size_t sz = std::min(strlen(ptr), SIZE);

        for(std::size_t i = 0; i < sz; ++i)
        {
            m_val[i] = ptr[i];
        }
    }

    /**
     * @brief Serialized size in characters. Equals to string size.
     * @details Method used in compile-time JSON-object size calculation .
     * @return Max string size.
     */
    constexpr static std::size_t size()
    {
        return SIZE;
    }

    /**
     * @brief Serialization to ecl::stream object.
     * @details Serialization to ecl::stream object.
     *
     * @tparam STREAM Stream type. Tested with ecl::stream.
     * Probably std:: streams should work.
     *
     * @param st Reference to stream object,
     * which will be used for serialization.
     *
     * @param beautify true - human readable format.
     * false - no new lines, no spaces.
     *
     * @param indent Base indent, if method called from client code,
     * should be 0.
     *
     * @param indent_increment Count of spaces on each indent level.
     */
    template<typename STREAM>
    void serialize(STREAM& st,
                   bool /* beautify */,
                   std::size_t /* indent */,
                   std::size_t /* indent_increment */)                     const
    {
        st << "\"";

        for(std::size_t i = 0; i < strlen(m_val.data()) && i < SIZE; ++i)
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

    /**
     * @brief Deserialization from reference to char pointer.
     * @details Deserialization from reference to char pointer. Used inside JSON
     * objects, but can be used from client code. Pointer will be moved to last
     * successfully parsed position in string.
     *
     * @param s Reference to serialized JSON string.
     * @return true - deserialization successful, false - unsuccessful.
     */
    bool deserialize_ref(const char*& s, std::size_t& length, bool)
    {
        ECL_JSON_TEST_SYMBOL_RETURN(s, '"', length)

        for(std::size_t i = 0; i < SIZE; ++i)
        {
            if(*s == '"')
            {
                ECL_JSON_SHIFT_S_L(s, length)
                ECL_JSON_TEST_LENGTH_RETURN(length, false)

                break;
            }

            ECL_JSON_TEST_LENGTH_RETURN(length, false)
            ECL_JSON_TEST_LENGTH_RETURN(length - 1, false)

            if(*s == '\\' && *(s + 1) == '"')
            {
                ECL_JSON_SHIFT_S_L(s, length)
            }

            m_val[i] = *s;
            ECL_JSON_SHIFT_S_L(s, length)
            ECL_JSON_TEST_LENGTH_RETURN(length, false)
        }

        return true;
    }

    /**
     * @brief Copy ctor
     *
     * @param other Another string<SIZE> object
     */
    string(const string<SIZE>& other)
    {
        m_val = other.m_val;
    }

    /**
     * @brief Assignment operator for ecl::json::string type.
     * @details One string can be assigned to another. Data will be copied.
     *
     * @param other String, that should be copied.
     */
    string<SIZE>& operator= (string<SIZE>& other)
    {
        m_val = other.m_val;
        return *this;
    }

    /**
     * @brief Assignment operator for char* type.
     * @details Copies string from char pointer.
     *
     * @param ptr pointer to string.
     */
    string<SIZE>& operator= (const char* const ptr)
    {
        std::size_t sz = std::min(strlen(ptr), SIZE);
        for(std::size_t i = 0; i < sz; ++i)
        {
            m_val[i] = ptr[i];
        }

        return *this;
    }

    /**
     * @brief char* cast operator
     * @details String can be casted to char*
     * @return Pointer to internal data.
     */
    operator char*()
    {
        return m_val.data();
    }

    /**
     * @brief const char* cast operator
     * @details String can be casted to const char*
     * @return Pointer to constant internal data.
     */
    operator const char*()                                                 const
    {
        return m_val.data();
    }

    /**
     * @brief Access to internal data.
     * @details Access to internal data.
     *
     * @return Pointer to internal data.
     */
    char* data()
    {
        return m_val.data();
    }

    /**
     * @brief Access to internal data. const version.
     * @details Access to internal data. const version.
     *
     * @return Pointer to constant internal data.
     */
    const char* data()                                                     const
    {
        return m_val.data();
    }

    std::array<char, SIZE + 1> m_val { { } }; // 1 for '0'
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_STRING_HPP
