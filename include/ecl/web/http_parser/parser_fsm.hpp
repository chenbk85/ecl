#ifndef ECL_WEB_PARSER_FSM_HPP
#define ECL_WEB_PARSER_FSM_HPP

#include <ecl/fsm.hpp>

#include <ecl/web/request.hpp>
#include <ecl/web/http_parser/parser_kv_pairs.hpp>

#include <ecl/web/parser_callback_itf.hpp>

#include <cstring>

namespace ecl
{

namespace web
{

enum class parser_state
{
    init,
    request_line_parsed,
    header_parse,
    headers_parsed,
    wait_for_body,
    body_parsed,
    complete
};

struct event_line
{
    explicit event_line(char* l) : line(l)
    {}

    char*  line { nullptr };
};
struct empty_line {};
struct end_of_req {};

class parser_fsm : public state_machine<parser_fsm,
                                        parser_state,
                                        parser_state::init>
{
public:
    parser_fsm(i_parser_callback* cb_itf)
        : m_callback_interface { cb_itf }
    {}

private:
    void uri_param_parsed(kv_pair_t param)
    {
        m_callback_interface->set_uri_param(param);
    }

    void on_init_enter()
    {
        m_callback_interface->start_of_request();
    }

    void on_complete_enter()
    {
        m_callback_interface->end_of_request();
    }

    bool g_request_line_parse(const event_line& e)
    {
        char* method_str = e.line;
        if(nullptr == method_str)
        {
            return false;
        }

        char* uri_str = strchr(method_str, ' ');
        if(nullptr == uri_str)
        {
            return false;
        }

        char* version_str = strchr(uri_str + 1, ' ');
        if(nullptr == version_str)
        {
            return false;
        }

        *uri_str = 0x00;     ++uri_str;
        *version_str = 0x00; ++version_str;

        char* uri_param_str = strchr(uri_str, '?');
        if(nullptr != uri_param_str)
        {
            *uri_param_str = 0x00; ++uri_param_str;

            if(kv_parser_state::done !=
                   m_uri_param_parser.start_parse(uri_param_str,
                       std::bind(&parser_fsm::uri_param_parsed, this, std::placeholders::_1)))
            {
                return false;
            }
        }

        size_t method_str_len = strlen(method_str);
        if(0 == strncmp(method_str,
                        to_string(method::GET),
                        method_str_len))
        {
            m_callback_interface->set_method(method::GET);
        }
        else if(0 == strncmp(method_str,
                             to_string(method::POST),
                             method_str_len))
        {
            m_callback_interface->set_method(method::POST);
        }
        else if(0 == strncmp(method_str,
                             to_string(method::PUT),
                             method_str_len))
        {
            m_callback_interface->set_method(method::PUT);
        }
        else
        {
            return false;
        }

        m_callback_interface->set_uri(uri_str);

        size_t version_str_len = strlen(version_str);
        if(0 == strncmp(version_str,
                        to_string(version::HTTP10),
                        version_str_len))
        {
            m_callback_interface->set_version(version::HTTP10);
        }
        else if(0 == strncmp(version_str,
                             to_string(version::HTTP11),
                             version_str_len))
        {
            m_callback_interface->set_version(version::HTTP11);
        }
        else if(0 == strncmp(version_str,
                             to_string(version::HTTP20),
                             version_str_len))
        {
            m_callback_interface->set_version(version::HTTP20);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool g_header_parse(const event_line& e)
    {
        char* name = e.line;
        if(nullptr == name)
        {
            return false;
        }

        char* value = strchr(name, ':');
        if(nullptr == value)
        {
            return false;
        }

        *value = 0x00; ++value;

        while(*name == ' ')
        {
            *name = 0;
            ++name;
        }
        while(*value == ' ')
        {
            *value = 0;
            ++value;
        }

        m_callback_interface->set_header(kv_pair_t(name, value));

        return true;
    }

    bool g_body_parsed(const event_line& e)
    {
        if(nullptr == e.line)
        {
            return false;
        }

        const uint8_t* const body_ptr = (const uint8_t* const)(e.line);

        // TODO: size!!!
        m_callback_interface->push_body_part(body_ptr, 0);

        return true;
    }

    using s = parser_state;
    using p = parser_fsm;

    using transition_table_t = transition_table
    <
        row< s::init                , event_line , s::request_line_parsed , nullptr , &p::g_request_line_parse >,
        row< s::request_line_parsed , event_line , s::header_parse        , nullptr , &p::g_header_parse       >,
        row< s::header_parse        , event_line , s::header_parse        , nullptr , &p::g_header_parse       >,
        row< s::header_parse        , empty_line , s::headers_parsed      , nullptr , nullptr                  >,
        row< s::headers_parsed      , end_of_req , s::complete            , nullptr , nullptr                  >,
        row< s::headers_parsed      , event_line , s::body_parsed         , nullptr , &p::g_body_parsed        >,
        row< s::body_parsed         , end_of_req , s::complete            , nullptr , nullptr                  >,

        row< s::request_line_parsed , rst        , s::init                , nullptr , nullptr                  >,
        row< s::header_parse        , rst        , s::init                , nullptr , nullptr                  >,
        row< s::headers_parsed      , rst        , s::init                , nullptr , nullptr                  >,
        row< s::body_parsed         , rst        , s::init                , nullptr , nullptr                  >,
        row< s::complete            , rst        , s::init                , nullptr , nullptr                  >
    >;

    using callback_table_t = callback_table
    <
        scb< s::init     , &p::on_init_enter     >,
        scb< s::complete , &p::on_complete_enter >
    >;

public:
    template<typename event_t>
    parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

private:
    kv_parser m_uri_param_parser
    {
        str_const("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"),
        str_const("%+.-"),
        str_const("="),
        str_const("&;")
    };

    i_parser_callback* m_callback_interface;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_FSM_HPP
