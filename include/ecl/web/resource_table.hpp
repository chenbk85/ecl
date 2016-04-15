#ifndef ECL_WEB_RESOURCE_TABLE
#define ECL_WEB_RESOURCE_TABLE

#include <ecl/web/request.hpp>

#include <cstring>

namespace ecl
{

namespace web
{

template
<
      typename    PAGE_400
    , typename    PAGE_404
    , typename    PAGE_500
    , typename... RESOURCES
>
class resource_table : public PAGE_400::second_type
                     , public PAGE_404::second_type
                     , public PAGE_500::second_type
                     , public RESOURCES::second_type...
{
public:
    template<typename STREAM, typename RQ>
    status_code call(STREAM& st, const RQ& req)
    {
        if(0 == strlen(req.uri))
        {
            return this->PAGE_400::second_type::template exec<STREAM>(st, req);
        }

        return call_internal<STREAM, RQ, RESOURCES...>(st, req);
    }

private:
    template<typename STREAM, typename RQ, typename R_PAIR, typename... TAIL>
    status_code call_internal(STREAM& st, const RQ& req)
    {
        if(is_name_match<typename R_PAIR::first_type>(req.uri))
        {
            return this->R_PAIR::second_type::template exec<STREAM>(st, req);
        }

        return call_internal<STREAM, RQ, TAIL...>(st, req);
    }

    template<typename STREAM, typename RQ>
    status_code call_internal(STREAM& st, const RQ& req)
    {
        return this->PAGE_404::second_type::template exec<STREAM>(st, req);
    }

    template<typename NAME>
    bool is_name_match(const char* uri)
    {
        if(std::strlen(uri) != NAME::size())
        {
            return false;
        }

        if(0 == std::strncmp(uri, NAME::name(), NAME::size()))
        {
            return true;
        }

        return false;
    }

    // std::tuple<PAGE_400::second_ty>
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_TABLE
