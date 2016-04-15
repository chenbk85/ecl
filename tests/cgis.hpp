#include <ecl/name_type.hpp>
#include <ecl/json.hpp>

#include <cstddef>
#include <cstdint>

#include <iostream>

namespace name
{
    ECL_DECL_NAME_TYPE(json_1)
    ECL_DECL_NAME_TYPE(json_2)
    ECL_DECL_NAME_TYPE(json_3)

    ECL_DECL_NAME_TYPE(val_1)
    ECL_DECL_NAME_TYPE(val_2)
    ECL_DECL_NAME_TYPE(val_3)
    ECL_DECL_NAME_TYPE(val_4)
    ECL_DECL_NAME_TYPE(val_5)
    ECL_DECL_NAME_TYPE(val_6)
    ECL_DECL_NAME_TYPE(val_7)
    ECL_DECL_NAME_TYPE(val_8)
    ECL_DECL_NAME_TYPE(val_9)
} // namespace name

class upload
{
public:
    template<typename T, typename RQ>
    ecl::web::status_code exec(T& st, const RQ& req)
    {
        ecl::web::write_status_line(st, req.ver, ecl::web::status_code::OK);

        return ecl::web::status_code::OK;
    }
};

class settings
{
private:
    using document_t = ecl::json::object
    <
        ecl::json::node<name::val_1, int16_t              >,
        ecl::json::node<name::val_2, uint16_t             >,
        ecl::json::node<name::val_3, ecl::json::string<8> >,
        ecl::json::node<name::val_4, int64_t              >,
        ecl::json::node<name::val_5, uint64_t             >
    >;

    document_t m_doc;

public:
    template<typename T, typename RQ>
    ecl::web::status_code exec(T& st, const RQ& req)
    {
        (void)st;

        for(auto& h : req.headers)
        {
            if((0 == strcmp(ecl::web::to_string(ecl::web::header_name::CONTENT_TYPE), h.first)) &&
               (nullptr != strstr(h.second, ecl::web::to_string(ecl::web::content_type::APPLICATION_JSON))))
            {
                if(m_doc.deserialize(req.body))
                {
                    m_doc.serialize(std::cout, true);
                    std::cout << std::endl;
                    return ecl::web::status_code::OK;
                }

                break;
            }
        }

        ecl::web::redirect(st, "/400.html", req.ver);

        return ecl::web::status_code::BAD_REQUEST;
    }
};

class info
{
public:
    info() {}

private:
    using document_t = ecl::json::object
    <
        ecl::json::node<name::json_1, bool                  >,
        ecl::json::node<name::json_2, uint32_t              >,
        ecl::json::node<name::json_3, ecl::json::string<64> >
    >;

    document_t m_doc {};

public:
    template<typename T, typename RQ>
    ecl::web::status_code exec(T& st, const RQ& req)
    {
        ecl::web::write_status_line(st, req.ver, ecl::web::status_code::OK);

        ecl::web::set_content_type_header(st, ecl::web::content_type::APPLICATION_JSON);

        st << "\r\n";

        m_doc.f<name::json_1>() = !m_doc.f<name::json_1>();
        m_doc.f<name::json_2>() = m_counter++;
        m_doc.f<name::json_3>() = "Test json string with \"escaped\" \\characters/.\n\r\tCR LF TAB.";

        m_doc.serialize(st);
        st << "\r\n";

        return ecl::web::status_code::OK;
    }

private:
    uint32_t m_counter { 0 };
};

class auth
{
public:
    auth() {}

private:
    constexpr static std::size_t MAX_PARAMS_COUNT { 16 };

    void param_parsed(ecl::web::kv_pair_t p)
    {
        if(m_params_count == MAX_PARAMS_COUNT)
        {
            return;
        }
        m_params[m_params_count] = p;
        ++m_params_count;
    }

public:
    template<typename T, typename RQ>
    ecl::web::status_code exec(T& st, const RQ& req)
    {
        char buf[128];

        if(strlen(req.body) < 128)
        {
            strcpy(buf, req.body);
        }
        else
        {
            std::cout << "Too big body :(" << std::endl;
            return ecl::web::status_code::OK;
        }

        for(auto& p : m_params)
        {
            p = std::make_pair(nullptr, nullptr);
        }

        if(ecl::web::kv_parser_state::done !=
               m_parser.start_parse(buf, std::bind(&auth::param_parsed, this, std::placeholders::_1)))
        {
            std::cout << "Parse failed!" << std::endl;
            authorized = false;
        }
        else
        {
            std::cout << "Parse ok!" << std::endl;
            authorized = false;
            for(std::size_t i = 0; i < MAX_PARAMS_COUNT; ++i)
            {
                if((0 == strcmp(m_params[i].first,  "pass")) &&
                   (0 == strcmp(m_params[i].second, "239")))
                {
                    authorized = true;
                    ecl::web::redirect(st, "/authorized_index.html", req.ver);
                    return ecl::web::status_code::OK;
                }
            }
        }

        ecl::web::redirect(st, "/index.html", req.ver);
        return ecl::web::status_code::OK;
    }

private:
    ecl::web::kv_pair_t m_params[MAX_PARAMS_COUNT] {};
    std::size_t         m_params_count { 0 };

    bool authorized { false };
    ecl::web::kv_parser m_parser
    {
        ecl::str_const("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"),
        ecl::str_const("%+.-"),
        ecl::str_const("="),
        ecl::str_const("&;")
    };
};
