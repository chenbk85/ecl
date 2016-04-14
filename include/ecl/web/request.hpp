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
      std::size_t MAX_URI_LENGTH     = 256
    , std::size_t MAX_URI_PARAMETERS = 8
    , std::size_t MAX_HEADER_LENGTH  = 128
    , std::size_t MAX_HEADERS_COUNT  = 16
>
struct request
{
    void clear()
    {
        met = method::GET;
        ver = version::HTTP10;

        uri = nullptr;

        for(auto& h : headers)
        {
            h.clear();
        }
        headers_count = 0;

        for(auto& p : uri_parameters)
        {
            p.clear();
        }

        uri_parameters_count = 0;
    }

    request()
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

    constexpr static std::size_t max_header_length()                    noexcept
    {
        return MAX_HEADER_LENGTH;
    }

    constexpr static std::size_t max_headers_count()                    noexcept
    {
        return MAX_HEADERS_COUNT;
    }

    method          met                                 { method::GET     };
    version         ver                                 { version::HTTP10 };

    char            uri[MAX_URI_LENGTH]                 { nullptr         };

    kv_pair<16,16>  uri_parameters[MAX_URI_PARAMETERS];
    std::size_t     uri_parameters_count                { 0               };

    kv_pair<32,128> headers[MAX_HEADERS_COUNT];
    std::size_t     headers_count                       { 0               };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_HPP
