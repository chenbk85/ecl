#ifndef ECL_WEB_REQUEST_PARSER_HPP
#define ECL_WEB_REQUEST_PARSER_HPP

#include <ecl/web/http_parser/parser_fsm.hpp>

#include <cstring>

#include <algorithm>

namespace ecl
{

namespace web
{

template
<
      std::size_t MAX_REQUEST_CHUNK_SIZE = 1024
    , std::size_t MAX_URI_LENGTH         = 256
    , std::size_t MAX_URI_PARAMETERS     = 8
    , std::size_t MAX_HEADER_LENGTH      = 128
    , std::size_t MAX_HEADERS_COUNT      = 16
>
class request_parser
{
public:
    using request_t = request
    <
          MAX_URI_LENGTH
        , MAX_URI_PARAMETERS
        , MAX_HEADER_LENGTH
        , MAX_HEADERS_COUNT
    >;

    const request_t parse(request_raw_t raw, size_t size)
    {
        memset(m_request_chunk_raw, 0, MAX_REQUEST_CHUNK_SIZE);

        if(size > MAX_REQUEST_CHUNK_SIZE)
        {
            return nullptr;
        }

        memcpy(m_request_chunk_raw, raw, size);

        parser_state st = m_parser.process_event(rst());

        char* current = m_request_chunk_raw;
        char* next    = nullptr;

        do
        {
            next = strchr(current, m_lf);

            if(nullptr != next)
            {
                if(*(next - 1) == m_cr)
                {
                    *(next - 1) = 0;
                }

                *next = 0;
                ++next;
            }

            bool is_empty_line = (0 == strlen(current));

//            std::cout << "Current   " << current << std::endl;
//            std::cout << "Next      " << next    << std::endl;
//            std::cout << "Empy line " << is_empty_line << std::endl;
//            std::cout << "-----" << std::endl;

            if(is_empty_line)
            {
                st = m_parser.process_event(empty_line());
            }
            else
            {
                st = m_parser.process_event(event_line { current });
            }

            if(nullptr == next)
            {
                st = m_parser.process_event(end_of_req());
                break;
            }

            current = next;
        } while(st != parser_state::complete);

        if(st == parser_state::complete)
        {
            return m_parser.get_request();
        }

        return nullptr;
    }

private:
    char       m_request_chunk_raw [MAX_REQUEST_CHUNK_SIZE];

    request_t  m_request {};
    parser_fsm m_parser {};

    const char m_cr   { '\r' };
    const char m_lf   { '\n' };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_PARSER_HPP
