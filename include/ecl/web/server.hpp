#ifndef ECL_WEB_SERVER_HPP
#define ECL_WEB_SERVER_HPP

#include <iostream>
#include <string>

#include <ecl/stream.hpp>

//#include <ecl/web/http_parser/request_parser.hpp>

#include <ecl/web/http_parser/parser_wrapper.hpp>

namespace ecl
{

namespace web
{

template
<
      typename    RESOURCES
    , std::size_t MAX_REQUEST_CHUNK_SIZE = 1536
    , std::size_t MAX_URI_LENGTH         = 128
    , std::size_t MAX_URI_PARAMETERS     = 32
    , std::size_t MAX_HEADERS_COUNT      = 32
>
class server
{
private:
    using resources_t = RESOURCES;
    using parser_t    = parser_wrapper;

public:
    template<typename STREAM>
    void process_request(STREAM& st, const char* req_raw, std::size_t size)
    {
        m_parser.parse(req_raw, size);
            // status_code c = m_resources.template call<STREAM, request_t>(st, m_request);
    }

    resources_t m_resources {};
    parser_t    m_parser    {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
