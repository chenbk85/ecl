#ifndef ECL_COMMAND_HPP
#define ECL_COMMAND_HPP

#include <ecl/singleton.hpp>
#include <ecl/list.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>
#include <type_traits>
#include <algorithm>

#ifndef DEFAULT_INDENT_INCREMENT
#define DEFAULT_INDENT_INCREMENT 4
// #pragma message "[ECL command.hpp] Defaulting to indent increment = 4"
#endif

namespace ecl
{

template<typename ST>
static void print_indent(ST& st, std::size_t indent)                    noexcept
{
    for(std::size_t i = 0; i < indent; ++i)
    {
        st << " ";
    }
}

namespace detail
{

template<typename cmd>
class i_receiver
{
protected:
    virtual ~i_receiver()
    {}

public:
    virtual void receive(cmd&)                                              = 0;
};

} // namespace detail

template<typename NAME, typename cmd>
class command
{
    using list_t         = ecl::list<detail::i_receiver<cmd>*>;
    using list_node_t    = typename list_t::node_t;
    using link_singleton = ecl::singleton<list_t>;

public:
    template<typename ST>
    bool dispatch(ST& /* st */)
    {
        for(auto& i : link_singleton::instance())
        {
            i->receive(*static_cast<cmd*>(this));
        }

        return true;
    }

    bool init(const std::size_t argc,
              const char**      argv)
    {
        m_argc = argc;
        m_argv = argv;

        return true;
    }

    constexpr static const char* name()         noexcept(noexcept(NAME::name()))
    {
        return NAME::name();
    }

    template<typename ST>
    static void show_help(ST& st,
                          std::size_t indent,
                          std::size_t indent_inc = DEFAULT_INDENT_INCREMENT)
                                              noexcept(
                                                  noexcept(
                                                      print_indent(st,
                                                                   indent)) &&
                                                  noexcept(st.operator<<("")) &&
                                                  noexcept(name())
                                              )
    {
        (void)indent_inc;
        print_indent(st, indent);
        st << name() << "\n\r";
    }

protected:
    std::size_t  m_argc { 0 };
    const char** m_argv { nullptr };
};

template<typename cmd>
class receiver : public detail::i_receiver<cmd>
{
    using list_t         = ecl::list<detail::i_receiver<cmd>*>;
    using list_node_t    = typename list_t::node_t;
    using link_singleton = ecl::singleton<list_t>;

protected:
    receiver()
    {
        link_singleton::instance().push_back(&m_node);
    }

    virtual ~receiver()                                                 override
    {
        list_t& list_ref = link_singleton::instance();
        auto it = std::find(list_ref.begin(), list_ref.end(), this);
        if(it != list_ref.end())
        {
            list_ref.erase(it);
        }
    }

private:
    list_node_t m_node { this, nullptr, nullptr };
};

} // namespace ecl

#endif // ECL_COMMAND_HPP
