#ifndef ECL_WEB_PARSER_CALLBACK_ITF
#define ECL_WEB_PARSER_CALLBACK_ITF

#include <ecl/web/constants.hpp>
#include <ecl/web/kv_pair.hpp>

namespace ecl
{

namespace web
{

struct i_parser_callback
{
    virtual ~i_parser_callback() {}

    virtual void start_of_request ()                               noexcept = 0;
    virtual void set_method       ( method      )                  noexcept = 0;
    virtual void set_uri          ( const char* )                  noexcept = 0;
    virtual void set_uri_param    ( kv_pair_t   )                  noexcept = 0;
    virtual void set_version      ( version     )                  noexcept = 0;
    virtual void set_header       ( kv_pair_t   )                  noexcept = 0;
    virtual void push_body_part   ( const uint8_t*, std::size_t )  noexcept = 0;
    virtual void end_of_request   ()                               noexcept = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_CALLBACK_ITF
