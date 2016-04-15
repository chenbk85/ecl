#ifndef ECL_WEB_RESOURCE_HPP
#define ECL_WEB_RESOURCE_HPP

#include <ecl/web/request.hpp>
#include <ecl/web/constants.hpp>

#include <cstdint>
#include <cstring>

#include <algorithm>

namespace ecl
{

namespace web
{

template
<
      typename     RES_DATA
    , content_type TYPE
    , bool         GZIP_COMPRESSED
    , status_code  CODE
>
class resource
{
public:
    template<typename T, typename RQ>
    status_code exec(T& st, const RQ& req)                                 const
    {
        write_status_line(st, req.ver, CODE);

        set_content_type_header(st, TYPE);
        if(GZIP_COMPRESSED)
        {
            set_content_encoding_header(st, content_encoding::GZIP);
        }

        st << "\r\n";
        st << RES_DATA::data << "\r\n";

        return CODE;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
