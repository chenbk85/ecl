#ifndef ECL_WEB_CGI_HPP
#define ECL_WEB_CGI_HPP

#include <ecl/web/request.hpp>

namespace ecl
{

namespace web
{

template<typename... NAME>
class cgi
{
public:
    cgi()                                                               noexcept
    {
        static_assert(sizeof...(NAME) > 0,
                      "At least one NAME type should be specified!");
    }

    template<typename N>
    static bool check_resource_internal(const request* req)             noexcept
    {
        if((0 == strncmp(req->uri, N::name(), N::size())) &&
           (strlen(req->uri) == N::size()))
        {
            return true;
        }

        return false;
    }

    template<typename N1, typename N2, typename... TAIL>
    static bool check_resource_internal(const request* req)
                   noexcept(noexcept(check_resource_internal<N2, TAIL...>(req)))
    {
        if((0 == strncmp(req->uri, N1::name(), N1::size())) &&
           (strlen(req->uri) == N1::size()))
        {
            return true;
        }

        return check_resource_internal<N2, TAIL...>(req);
    }

    static bool check_resource(const request* req)
                       noexcept(noexcept(check_resource_internal<NAME...>(req)))
    {
        return check_resource_internal<NAME...>(req);
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_CGI_HPP
