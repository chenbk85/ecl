#ifndef ECL_LIST_HPP
#define ECL_LIST_HPP

#include <cstddef>

#include <iterator>
#include <type_traits>
#include <utility>

namespace ecl
{

template<typename T>
struct list_node
{
    using value_type      = T;
    using node_t          = list_node<value_type>;

    using pointer         = typename std::add_pointer<node_t>::type;
    using const_pointer   = typename std::add_pointer
                                     <
                                         typename std::add_const<node_t>::type
                                     >::type;

    using reference       = typename std::add_lvalue_reference
                                     <
                                         value_type
                                     >::type;
    using const_reference = typename std::add_lvalue_reference
                                     <
                                         typename std::add_const<value_type>::type
                                     >::type;

    list_node(value_type& v, pointer p, pointer n)
        : val  ( v )
        , prev ( p )
        , next ( n )
    {}

    list_node(value_type&& v, pointer&& p, pointer&& n)
        : val  ( std::forward<value_type>(v) )
        , prev ( std::forward<pointer>(p)    )
        , next ( std::forward<pointer>(n)    )
    {}

    list_node(value_type v) : list_node(v, nullptr, nullptr)
    {}

    void swap(node_t& other)                                            noexcept
    {
        std::swap(val,  other.val );
        std::swap(prev, other.prev);
        std::swap(next, other.next);
    }

    value_type val  {};

    pointer    prev { nullptr };
    pointer    next { nullptr };
};

template<typename T>
class list
{
public:
    using node_t          = list_node<T>;
    using value_type      = typename node_t::value_type;
    using size_type       = std::size_t;

    using difference_type = std::ptrdiff_t;

    using pointer         = typename node_t::pointer;
    using const_pointer   = typename node_t::const_pointer;

    using reference       = typename node_t::reference;
    using const_reference = typename node_t::const_reference;

// Iterators start
protected:
    template<typename Pointer>
    struct base_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;

        base_iterator(Pointer n, Pointer e)                             noexcept
            : m_n(n)
            , m_e(e)
        {}

        void increment()                                                noexcept
        {
            if(m_n != m_e)
            {
                m_n = m_n->next;
            }
        }

        void decrement()                                                noexcept
        {
            if(m_n == m_e)
            {
                m_n = m_e->next;
                return;
            }

            if(m_n != m_e)
            {
                m_n = m_n->prev;
            }
        }

        Pointer m_n { nullptr };
        Pointer m_e { nullptr };
    };

public:
    struct const_iterator : public base_iterator<list::pointer>
    {
    private:
        using base = base_iterator<list::pointer>;
        using base::increment;
        using base::decrement;
        using base::m_n;
        using base::m_e;

    public:
        using self_type  = const_iterator;
        using value_type = const T;
        using reference  = const T&;
        using pointer    = const T*;

        const_iterator(list::pointer n, list::pointer e)
            : base_iterator<list::pointer>(n, e)
        {}

        self_type& operator++()                                         noexcept
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                      noexcept
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                         noexcept
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                      noexcept
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                    const noexcept
        {
            return m_n->val;
        }

        bool operator==(const self_type& rhs)                     const noexcept
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                     const noexcept
        {
            return !operator==(rhs);
        }

        bool operator==(const list::pointer& rhs)                 const noexcept
        {
            return m_n == rhs;
        }

        bool operator!=(const list::pointer& rhs)                 const noexcept
        {
            return !operator==(rhs);
        }

        friend class list;
    };

    struct iterator : public base_iterator<list::pointer>
    {
    private:
        using base = base_iterator<list::pointer>;
        using base::increment;
        using base::decrement;
        using base::m_n;
        using base::m_e;

    public:
        using self_type  = iterator;
        using value_type = T;
        using reference  = T&;
        using pointer    = T*;

        iterator(list::pointer n, list::pointer e)
            : base_iterator<list::pointer>(n, e)
        {}

        self_type& operator++()                                         noexcept
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                      noexcept
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                         noexcept
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                      noexcept
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                          noexcept
        {
            return m_n->val;
        }

        bool operator==(const self_type& rhs)                     const noexcept
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                     const noexcept
        {
            return !operator==(rhs);
        }

        bool operator==(const list::pointer& rhs)                 const noexcept
        {
            return m_n == rhs;
        }

        bool operator!=(const list::pointer& rhs)                 const noexcept
        {
            return !operator==(rhs);
        }

        operator const_iterator()
        {
            return const_iterator(m_n, m_e);
        }

        friend class list;
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin()                                                    noexcept
    {
        return iterator(m_header.prev, pointer(&m_header));
    }

    iterator end()                                                      noexcept
    {
        return iterator(pointer(&m_header), pointer(&m_header));
    }

    const_iterator begin()                                        const noexcept
    {
        return const_iterator(m_header.prev, pointer(&m_header));
    }

    const_iterator end()                                          const noexcept
    {
        return const_iterator(pointer(&m_header), pointer(&m_header));
    }

    const_iterator cbegin()                                       const noexcept
    {
        return const_iterator(m_header.prev, pointer(&m_header));
    }

    const_iterator cend()                                         const noexcept
    {
        return const_iterator(pointer(&m_header), pointer(&m_header));
    }

    reverse_iterator rbegin()                                           noexcept
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()                                             noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin()                               const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend()                                 const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin()                              const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend()                                const noexcept
    {
        return const_reverse_iterator(begin());
    }

// Iterators end

    reference front()                                                   noexcept
    {
        return m_header.prev->val;
    }

    const_reference front()                                       const noexcept
    {
        return m_header.prev->val;
    }

    reference back()                                                    noexcept
    {
        return m_header.next->val;
    }

    const_reference back()                                        const noexcept
    {
        return m_header.next->val;
    }

    bool empty()                                                  const noexcept
    {
        return (0 == m_size);
    }

    size_type size()                                              const noexcept
    {
        return m_size;
    }

    void clear()                                                        noexcept
    {
        m_header.prev = &m_header;
        m_header.next = &m_header;
        m_size = 0;
    }

    iterator insert(const_iterator pos, pointer v)                      noexcept
    {
        pointer n = pos.m_n;

        if(cbegin() == pos)
        {
            v->next = m_header.prev;
            m_header.prev->prev = v;
            m_header.prev = v;
        }
        else if(cend() == pos)
        {
            v->prev = m_header.next;
            v->next = &m_header;
            m_header.next->next = v;
            m_header.next = v;
        }
        else
        {
            link(n->prev, v);
            link(v,       n);
        }

        ++m_size;

        return iterator(v, pointer(&m_header));
    }

    iterator erase(const_iterator pos)                                  noexcept
    {
        const_pointer n = pos.m_n;
        if(cbegin() == pos)
        {
            m_header.prev = n->next;
            n->next->prev = &m_header;
        }
        else if(m_header.next == n->prev)
        {
            m_header.next = n->prev->prev;
            n->prev->next = &m_header;
        }
        else
        {
            link(n->prev, n->next);
        }

        --m_size;

        return iterator(n->next, pointer(pos.m_e));
    }

    iterator erase(const_iterator first, const_iterator last)           noexcept
    {
        while(first != last)
        {
            first = erase(first);
        }
        return iterator(last.m_n, last.m_e);
    }

    void push_back(pointer n)                                           noexcept
    {
        n->next = &m_header;
        link(m_header.next, n);
        m_header.next = n;

        if(m_header.prev == &m_header)
        {
            m_header.prev = n;
        }

        ++m_size;
    }

    void push_front(pointer n)                                          noexcept
    {
        n->prev = &m_header;
        link(n, m_header.prev);
        m_header.prev = n;

        if(m_header.next == &m_header)
        {
            m_header.next = n;
        }

        ++m_size;
    }

    void pop_back()                                                     noexcept
    {
        if(empty())
        {
            return;
        }

        m_header.next = m_header.next->prev;
        m_header.next->next = &m_header;

        --m_size;

        if(empty())
        {
            m_header.prev = &m_header;
        }
    }

    void pop_front()                                                    noexcept
    {
        if(empty())
        {
            return;
        }

        m_header.prev = m_header.prev->next;
        m_header.prev->prev = &m_header;

        --m_size;

        if(empty())
        {
            m_header.next = &m_header;
        }
    }

    void reverse()                                                      noexcept
    {
        if(empty())
        {
            return;
        }

        pointer p = m_header.prev;
        p->prev = pointer(&m_header);
        while(p != pointer(&m_header))
        {
            pointer tmp = p->next;
            std::swap(p->prev, p->next);
            p = tmp;
        }

        std::swap(m_header.prev, m_header.next);
    }

    void swap(list& other)                                              noexcept
    {
        std::swap(m_size,   other.m_size  );
        std::swap(m_header, other.m_header);
    }

private:
    void link(pointer p, pointer n)                                     noexcept
    {
        p->next = n;
        n->prev = p;
    }

    node_t    m_header { {}, &m_header, &m_header };
    size_type m_size   { 0 };
};

} // namespace ecl

#endif // ECL_LIST_HPP
