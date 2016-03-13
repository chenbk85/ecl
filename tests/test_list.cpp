#include <iostream>
#include <ecl/list.hpp>

#include <algorithm>

using list_t      = ecl::list<int>;
using list_node_t = typename list_t::node_t;

list_node_t n1  { 1  };
list_node_t n2  { 2  };
list_node_t n3  { 3  };
list_node_t n4  { 4  };

list_node_t n5  { 5  };
list_node_t n6  { 6  };
list_node_t n7  { 7  };
list_node_t n8  { 8  };

list_node_t n9  { 9  };
list_node_t n10 { 10 };
list_node_t n11 { 11 };
list_node_t n12 { 12 };

list_node_t n13 { 13 };
list_node_t n14 { 14 };
list_node_t n15 { 15 };
list_node_t n16 { 16 };

template<typename L>
void front_back(const L& l)
{
    std::cout << "front: " << l.front() << " | " << "back:  " << l.back()  << std::endl;
}

template<typename L>
void erase_list(L& l)
{
    list_t::const_iterator it     = l.cbegin();
    list_t::const_iterator it_end = l.cend();
    for(; it != it_end; ++it)
    {
        std::cout << "erasing: " << *it << " ";
        auto er_it = l.erase(it);
        if(!l.empty())
        {
            std::cout << "next: " << *er_it << std::endl;
        }
        else
        {
            std::cout << "empty" << std::endl;
        }
    }
    std::cout << std::endl;
}

template<typename L>
void erase_list_range(L& l)
{
    list_t::const_iterator it     = l.cbegin();
    list_t::const_iterator it_end = l.cend();

    std::cout << "erasing from " << *it << " to " << *it_end << std::endl;
    auto er_it = l.erase(it, it_end);

    if(!l.empty())
    {
        front_back(l);
        std::cout << "next after erased: " << *er_it << std::endl;
    }
    else
    {
        std::cout << "empty" << std::endl;
    }
    std::cout << std::endl;
}

template<typename L>
void fill_list(L& l)
{
    l.push_back(&n11);
    front_back(l);
    l.push_back(&n12);
    front_back(l);
    l.push_back(&n13);
    front_back(l);
    l.push_back(&n14);
    front_back(l);

    l.push_front(&n6);
    front_back(l);
    l.push_front(&n5);
    front_back(l);
    l.push_front(&n4);
    front_back(l);
    l.push_front(&n3);
    front_back(l);

    auto it = std::find(l.cbegin(), l.cend(), 11);

    it = l.insert(it, &n10);
    std::cout << "inserted " << *it << std::endl;
    it = l.insert(it, &n9);
    std::cout << "inserted " << *it << std::endl;
    it = l.insert(it, &n8);
    std::cout << "inserted " << *it << std::endl;
    it = l.insert(it, &n7);
    std::cout << "inserted " << *it << std::endl;

    it = l.insert(l.cbegin(), &n2);
    std::cout << "inserted " << *it << std::endl;
    it = l.insert(l.cbegin(), &n1);
    std::cout << "inserted " << *it << std::endl;

    it = l.insert(l.cend(), &n15);
    std::cout << "inserted " << *it << std::endl;
    it = l.insert(l.cend(), &n16);
    std::cout << "inserted " << *it << std::endl;

    std::cout << std::endl;
}

template<typename L>
void dump_list(const L& l)
{
    std::cout << "c++11 range-based for:  ";
    for(auto& v : l)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    std::cout << "c++03 iterator:         ";
    auto it     = l.begin();
    auto it_end = l.end();
    for(; it != it_end; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "c++03 reverse iterator: ";
    auto rit     = l.rbegin();
    auto rit_end = l.rend();
    for(; rit != rit_end; ++rit)
    {
        std::cout << *rit << " ";
    }
    std::cout << std::endl;
    front_back(l);
}

int main(int argc, char* argv[], char** env)
{
    (void)(argc);
    (void)(argv);
    (void)(env);

    list_t l;

    std::cout << "fill list" << std::endl;
    fill_list(l);
    dump_list(l);

    std::cout << "clear list" << std::endl;
    l.clear();
    dump_list(l);

    std::cout << "fill list" << std::endl;
    fill_list(l);
    dump_list(l);

    std::cout << "erase list" << std::endl;
    erase_list(l);
    dump_list(l);

    std::cout << "fill list" << std::endl;
    fill_list(l);
    dump_list(l);

    std::cout << "erase list range" << std::endl;
    erase_list_range(l);
    dump_list(l);

    std::cout << "fill list" << std::endl;
    fill_list(l);
    dump_list(l);

    std::cout << "reverse list" << std::endl;
    l.reverse();
    dump_list(l);

    std::cout << "pop front 4 elements" << std::endl;
    for(std::size_t i = 0; i < 4; ++i)
    {
        l.pop_front();
        dump_list(l);
    }
    std::cout << "ok" << std::endl << std::endl;

    std::cout << "pop back 4 elements" << std::endl;
    for(std::size_t i = 0; i < 4; ++i)
    {
        l.pop_back();
        dump_list(l);
    }
    std::cout << "ok" << std::endl << std::endl;
}
