#ifndef ECL_WEB_REQUEST_PARSER_HPP
#define ECL_WEB_REQUEST_PARSER_HPP

#include <ecl/web/http_parser/parser_fsm.hpp>
#include <ecl/web/parser_callback_itf.hpp>

#include <cstring>

#include <algorithm>

namespace ecl
{

namespace web
{

template
<
      std::size_t MAX_REQUEST_CHUNK_SIZE = 1536
    , std::size_t MAX_URI_LENGTH         = 128
    , std::size_t MAX_URI_PARAMETERS     = 32
    , std::size_t MAX_HEADERS_COUNT      = 32
>
class request_parser : public i_parser_callback
{
public:
    request_parser() {}

    virtual ~request_parser() {}

    virtual void start_of_request()                                     noexcept
    {
        m_request.clear();
    }

    virtual void set_method(method m)                                   noexcept
    {
        m_request.met = m;
    }

    virtual void set_uri(const char* uri)                               noexcept
    {
        std::size_t uri_len = strlen(uri);
        if(uri_len > MAX_URI_LENGTH)
        {
            // TODO: status code 414 URI_TOO_LONG
            return;
        }

        strncpy(m_request.uri, uri, MAX_URI_LENGTH);
    }

    virtual void set_uri_param(kv_pair_t p)                             noexcept
    {
        if(m_request.uri_parameters_count == MAX_URI_PARAMETERS)
        {
            // TODO: no more room for uri param
            return;
        }

        m_request.uri_parameters[m_request.uri_parameters_count] = p;
        ++m_request.uri_parameters_count;
    }

    virtual void set_version(version v)                                 noexcept
    {
        m_request.ver = v;
    }

    virtual void set_header(kv_pair_t h)                                noexcept
    {
        if(m_request.headers_count == MAX_HEADERS_COUNT)
        {
            // TODO: no more room for uri param
            return;
        }

        m_request.headers[m_request.headers_count] = h;
        ++m_request.headers_count;
    }

    virtual void push_body_part(const uint8_t* data, std::size_t size)  noexcept
    {
        // TODO: forward body
        std::size_t safe_size = std::min(size, MAX_REQUEST_CHUNK_SIZE);

        memcpy(m_request.body, data, safe_size);
    }

    virtual void end_of_request()                                       noexcept
    {
        m_request.clear();
    }

    using request_t = request
    <
          MAX_REQUEST_CHUNK_SIZE
        , MAX_URI_LENGTH
        , MAX_URI_PARAMETERS
        , MAX_HEADERS_COUNT
    >;

    const request_t parse(request_raw_t raw, size_t size)
    {
        memset(m_request_chunk_raw, 0, MAX_REQUEST_CHUNK_SIZE);

        if(size > MAX_REQUEST_CHUNK_SIZE)
        {
            return m_request;
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
            return m_request;
        }

        // TODO: flags for incomplete request!
        return m_request;
    }

private:
    char       m_request_chunk_raw [MAX_REQUEST_CHUNK_SIZE] {};

    request_t  m_request {};
    parser_fsm m_parser  { this };

    const char m_cr   { '\r' };
    const char m_lf   { '\n' };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_PARSER_HPP
