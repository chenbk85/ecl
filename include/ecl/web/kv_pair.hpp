#ifndef ECL_WEB_KV_PAIR
#define ECL_WEB_KV_PAIR

namespace ecl
{

namespace web
{

template
<
      std::size_t K_LENGTH
    , std::size_t V_LENGTH
>
struct kv_pair
{
    char name  [K_LENGTH];
    char value [V_LENGTH];

    void clear()
    {
        for(auto& c : name)
        {
            c = 0x00;
        }

        for(auto& c : value)
        {
            c = 0x00;
        }
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_KV_PAIR
