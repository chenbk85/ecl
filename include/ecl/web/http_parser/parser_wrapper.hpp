#ifndef ECL_WEB_PARSER_WRAPPER_HPP
#define ECL_WEB_PARSER_WRAPPER_HPP

#include <ecl/web/http_parser/http_parser.h>

#include <functional>

namespace ecl
{

namespace web
{

int message_begin_cb    ( http_parser* p )                                { std::cout << "| message_begin_cb    " <<       std::endl; return 0; }
int header_field_cb     ( http_parser* p, const char *at, size_t length ) { std::cout << "| header_field_cb     " << std::string(at).substr(0, length) << std::endl; return 0; }
int header_value_cb     ( http_parser* p, const char *at, size_t length ) { std::cout << "| header_value_cb     " << std::string(at).substr(0, length) << std::endl; return 0; }
int request_url_cb      ( http_parser* p, const char *at, size_t length ) { std::cout << "| request_url_cb      " << std::string(at).substr(0, length) << std::endl; return 0; }
int response_status_cb  ( http_parser* p, const char *at, size_t length ) { std::cout << "| response_status_cb  " << std::string(at).substr(0, length) << std::endl; return 0; }
int body_cb             ( http_parser* p, const char *at, size_t length ) { std::cout << "| body_cb             " << std::string(at).substr(0, length) << std::endl; return 0; }
int headers_complete_cb ( http_parser* p )                                { std::cout << "| headers_complete_cb " <<       std::endl; return 0; }
int message_complete_cb ( http_parser* p )                                { std::cout << "| message_complete_cb " <<       std::endl; return 0; }
int chunk_header_cb     ( http_parser* p )                                { std::cout << "| chunk_header_cb     " <<       std::endl; return 0; }
int chunk_complete_cb   ( http_parser* p )                                { std::cout << "| chunk_complete_cb   " <<       std::endl; return 0; }

class parser_wrapper
{
private:
    using callback_data_t = std::function<int(http_parser*, const char*, size_t)>;
    using callback_t      = std::function<int(http_parser*)>;

public:
    parser_wrapper()                                                    noexcept
    {
        http_parser_init(&m_parser, http_parser_type::HTTP_REQUEST);
        http_parser_settings_init(&m_settings);

        // callback_t      on_message_begin    = std::bind(&parser_wrapper::message_begin_cb    , this , std::placeholders::_1 );
        // callback_data_t on_header_field     = std::bind(&parser_wrapper::header_field_cb     , this , std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 );
        // callback_data_t on_header_value     = std::bind(&parser_wrapper::header_value_cb     , this , std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 );
        // callback_data_t on_url              = std::bind(&parser_wrapper::request_url_cb      , this , std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 );
        // callback_data_t on_status           = std::bind(&parser_wrapper::response_status_cb  , this , std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 );
        // callback_data_t on_body             = std::bind(&parser_wrapper::body_cb             , this , std::placeholders::_1 , std::placeholders::_2 , std::placeholders::_3 );
        // callback_t      on_headers_complete = std::bind(&parser_wrapper::headers_complete_cb , this , std::placeholders::_1 );
        // callback_t      on_message_complete = std::bind(&parser_wrapper::message_complete_cb , this , std::placeholders::_1 );
        // callback_t      on_chunk_header     = std::bind(&parser_wrapper::chunk_header_cb     , this , std::placeholders::_1 );
        // callback_t      on_chunk_complete   = std::bind(&parser_wrapper::chunk_complete_cb   , this , std::placeholders::_1 );

        m_settings.on_message_begin    = &message_begin_cb;
        m_settings.on_header_field     = &header_field_cb;
        m_settings.on_header_value     = &header_value_cb;
        m_settings.on_url              = &request_url_cb;
        m_settings.on_status           = &response_status_cb;
        m_settings.on_body             = &body_cb;
        m_settings.on_headers_complete = &headers_complete_cb;
        m_settings.on_message_complete = &message_complete_cb;
        m_settings.on_chunk_header     = &chunk_header_cb;
        m_settings.on_chunk_complete   = &chunk_complete_cb;
    }

    std::size_t parse(const char* data, std::size_t len)
    {
        std::size_t parsed = http_parser_execute(&m_parser, &m_settings, data, len);

        http_errno e = HTTP_PARSER_ERRNO(&m_parser);

        std::cout << "| errno: " << e << std::endl;
        std::cout << "| errno: " << http_errno_name(e) << std::endl;
        std::cout << "| errno: " << http_errno_description(e) << std::endl;

        return parsed;
    }

private:
    http_parser          m_parser   {};
    http_parser_settings m_settings {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_WRAPPER_HPP
