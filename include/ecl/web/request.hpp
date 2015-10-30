#ifndef ECL_WEB_REQUEST_HPP
#define ECL_WEB_REQUEST_HPP

#include <ecl/web/constants.hpp>
#include <ecl/web/headers.hpp>
#include <ecl/web/uri_param.hpp>

#ifndef MAX_HEADERS_COUNT
#define MAX_HEADERS_COUNT 64
// #pragma message "[ECL web/headers.hpp] Defaulting to Max headers coubtn = 64"
#endif

#ifndef MAX_URI_PARAMETERS
#define MAX_URI_PARAMETERS 64
// #pragma message "[ECL web/headers.hpp] Defaulting to Max headers coubtn = 64"
#endif

namespace ecl
{

namespace web
{

using request_raw_t = const char*;

struct request
{
    void clear()
    {
        met = method::GET;
        ver = version::HTTP10;

        uri = nullptr;
        uri_param_string = nullptr;
        body = nullptr;

        for(auto& h : headers)
        {
            h.clear();
        }

        for(auto& p : uri_parameters)
        {
            p.clear();
        }

        headers_count = 0;
    }

    request()
    {
        clear();
    }

    constexpr static std::size_t max_headers_count()                    noexcept
    {
        return MAX_HEADERS_COUNT;
    }

    method         met                         { method::GET  };
    version        ver                         { version::HTTP10 };
    const char*    uri                         { nullptr };
    const char*    uri_param_string            { nullptr };
    uri_param      uri_parameters[MAX_URI_PARAMETERS];
    const char*    body                        { nullptr };
    header         headers[MAX_HEADERS_COUNT];
    std::size_t    headers_count               { 0 };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_HPP
