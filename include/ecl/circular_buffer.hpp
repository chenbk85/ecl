#ifndef ECL_CB_HPP
#define ECL_CB_HPP

#include <cstddef>
#include <type_traits>
#include <iterator>

namespace ecl
{

template<typename T, size_t CAPACITY>
class circular_buffer
{
public:
    circular_buffer() :
    m_offset(0),
    m_size(0)
    {
        static_assert(std::is_nothrow_copy_constructible<T>::value,
                      "T must be nothrow copy-constructible!");
        static_assert(std::is_pod<T>::value, "T must be POD!");
        static_assert(CAPACITY > 0, "Size must be greater than zero!");
    }

    void clear()
    {
        m_offset = 0;
        m_size = 0;
    }

    size_t capacity()                                                      const
    {
        return CAPACITY;
    }

    size_t size()                                                          const
    {
        return m_size;
    }

    size_t push(const T& v)
    {
        m_array[m_offset] = v;
        m_offset = wrap(m_offset + 1);

        if(m_size < CAPACITY) 
        {
            ++m_size;
        }

        return m_offset - m_size;
    }

    T pop()
    {
        if(m_size != 0)
        {
            --m_size;
            return m_array[wrap(m_offset - m_size + 1)];;
        }

        return T();
    }

    T& operator[](size_t index)
    {
        return m_array[wrap(index + m_offset - m_size)];
    }

    const T& operator[](size_t index)                                      const
    {
        return m_array[wrap(index + m_offset - m_size)];
    }

    class iterator 
    {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;
            iterator(size_t ind, pointer data) :
                index_(ind),
                data_(data)
            {}

            self_type operator++()
            {
                self_type i = *this;

                if(CAPACITY - 1 == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return i;
            }

            self_type operator++(int junk)
            {
                (void)(junk);

                if(CAPACITY - 1 == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type operator--()
            {
                self_type i = *this;

                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }

                return i;
            }

            self_type operator--(int junk)
            {
                (void)(junk);
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }
                return *this;
            }

            reference operator*()
            {
                return data_[wrap(index_)];
            }

            pointer operator->() 
            {   
                return data_; 
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return !operator==(rhs);
            }

        private:
            size_t  index_;
            pointer data_;
    };

    class const_iterator 
    {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;
            const_iterator(size_t ind, pointer data) :
                index_(ind), 
                data_(data)
            {}

            self_type operator++() 
            {
                self_type i = *this;

                if(CAPACITY - 1 == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return i;
            }

            self_type operator++(int junk)
            {
                (void)(junk);

                if(CAPACITY - 1 == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type operator--()
            {
                self_type i = *this;

                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }

                return i;
            }

            self_type operator--(int junk)
            {
                (void)(junk);
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }
                return *this;
            }

            reference operator*()                                          const
            {
                return data_[wrap(index_)];
            }

            pointer operator->() 
            {   
                return data_; 
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return !operator==(rhs);
            }

        private:
            size_t  index_;
            pointer data_;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin()
    {
        return iterator(0, m_array);
    }

    iterator end()
    {
        return iterator(m_size, m_array);
    }

    const_iterator begin()                                                 const
    {
        return const_iterator(0, m_array);
    }

    const_iterator end()                                                   const
    {
        return const_iterator(m_size, m_array);
    }

    reverse_iterator rbegin()
    {
        return ++reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return ++reverse_iterator(begin());
    }

    const_reverse_iterator rbegin()                                        const
    {
        return ++const_reverse_iterator(end());
    }

    const_reverse_iterator rend()                                          const
    {
        return ++const_reverse_iterator(begin());
    }

private:
    static size_t wrap(size_t i)
    {
        return i % CAPACITY;
    }

    size_t  m_offset;
    size_t  m_size;
    T       m_array[CAPACITY];
};

} // namespace ecl

#endif // ECL_CB_HPP
