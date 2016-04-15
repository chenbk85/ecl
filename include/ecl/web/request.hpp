#ifndef ECL_WEB_REQUEST_HPP
#define ECL_WEB_REQUEST_HPP

#include <ecl/web/constants.hpp>
#include <ecl/web/kv_pair.hpp>

#include <cstddef>

namespace ecl
{

namespace web
{

using request_raw_t = const char*;

template
<
      std::size_t MAX_REQUEST_SIZE   = 1536
    , std::size_t MAX_URI_LENGTH     = 128
    , std::size_t MAX_URI_PARAMETERS = 32
    , std::size_t MAX_HEADERS_COUNT  = 32
>
struct request
{
    void clear()                                                        noexcept
    {
        met = method::GET;
        ver = version::HTTP10;

        for(auto& c : uri)
        {
            c = 0x00;
        }

        for(auto& h : headers)
        {
            h = std::make_pair(nullptr, nullptr);
        }
        headers_count = 0;

        for(auto& p : uri_parameters)
        {
            p = std::make_pair(nullptr, nullptr);
        }
        uri_parameters_count = 0;
    }

    request()
    {
        clear();
    }

    ~request()
    {
        clear();
    }

    constexpr static std::size_t max_uri_length()                       noexcept
    {
        return MAX_URI_LENGTH;
    }

    constexpr static std::size_t max_uri_parameters_count()             noexcept
    {
        return MAX_URI_PARAMETERS;
    }

    constexpr static std::size_t max_headers_count()                    noexcept
    {
        return MAX_HEADERS_COUNT;
    }

    method      met                                 { method::GET     };
    version     ver                                 { version::HTTP10 };

    char        uri[MAX_URI_LENGTH];

    kv_pair_t   uri_parameters[MAX_URI_PARAMETERS];
    std::size_t uri_parameters_count                { 0 };

    kv_pair_t   headers[MAX_HEADERS_COUNT];
    std::size_t headers_count                       { 0 };

    char        body[MAX_REQUEST_SIZE]              {};

private:
    char        m_pool[MAX_REQUEST_SIZE]            {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_HPP
