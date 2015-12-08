#include <iostream>

#include <ecl/tree/simple_binary_tree.hpp>
#include <ecl/tree/red_black_tree.hpp>

#include <string>

#include <random>

#define NODES_COUNT    100

#define TREE_PREFIX    "[BT] | "
#define RB_TREE_PREFIX "[RB] | "

using key_type       = int;
using value_type     = std::string;

using tree_t         = ecl::tree::simple_binary_tree<key_type, value_type>;
using tree_node_t    = typename tree_t::node_t;

using rb_tree_t      = ecl::tree::red_black_tree<key_type, value_type>;
using rb_tree_node_t = typename rb_tree_t::node_t;

static tree_node_t simple_nodes [] =
{
    { 6,  "6"  },
    { 17, "17" },
    { 1,  "1"  },
    { 5,  "5"  },
    { 4,  "4"  },
    { 21, "21" },
    { 18, "18" },
    { 16, "16" },
    { 11, "11" },
    { 14, "14" },
    { 15, "15" },
    { 2,  "2"  },
    { 20, "20" },
    { 10, "10" },
    { 23, "23" },
    { 24, "24" },
    { 9,  "9"  },
    { 8,  "8"  },
    { 3,  "3"  },
    { 12, "12" },
    { 13, "13" },
    { 22, "22" },
    { 7,  "7"  },
    { 19, "19" }
};

static rb_tree_node_t rb_nodes [] =
{
    { 6,  "6"  },
    { 17, "17" },
    { 1,  "1"  },
    { 5,  "5"  },
    { 4,  "4"  },
    { 21, "21" },
    { 18, "18" },
    { 16, "16" },
    { 11, "11" },
    { 14, "14" },
    { 15, "15" },
    { 2,  "2"  },
    { 20, "20" },
    { 10, "10" },
    { 23, "23" },
    { 24, "24" },
    { 9,  "9"  },
    { 8,  "8"  },
    { 3,  "3"  },
    { 12, "12" },
    { 13, "13" },
    { 22, "22" },
    { 7,  "7"  },
    { 19, "19" }
};

template<typename T, std::size_t N>
constexpr static std::size_t array_size(T (&)[N])
{
    return N;
}

void print_indent(std::size_t indent)
{
    if(indent == 0)
    {
        return;
    }

    for(std::size_t i = 0; i < indent - 1; ++i)
    {
        std::cout << "         ";
    }
    std::cout << "         ";
    // std::cout << "+-------";
}

template<typename N>
void print(N n, std::size_t indent = 0)
{
    if(n->have_right())
    {
        print(n->right, indent + 1);
    }

    print_indent(indent);
    std::cout << "[N] " << n->key << std::endl;

    if(n->have_left())
    {
        // std::cout << "+-------";
        print(n->left, indent + 1);
    }
}

template<>
void print<rb_tree_node_t*>(rb_tree_node_t* n, std::size_t indent)
{
    if(n->have_right())
    {
        print(n->right, indent + 1);
    }

    print_indent(indent);
    if(n->color == ecl::tree::node_color::BLACK)
    {
        std::cout << "[B] " << n->key << std::endl;
    }
    else
    {
        std::cout << "[R] " << n->key << std::endl;
    }

    if(n->have_left())
    {
        print(n->left, indent + 1);
    }
}

template<typename T>
void fill_tree_dynamic(const std::string prefix, T& tree, key_type from, key_type to, std::size_t count)
{
    std::size_t ins_count = 0;

    std::cout << prefix << "inserting dynamicaly created nodes."  << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;
    std::cout << prefix << "nodes count:    " << count << std::endl;

    std::default_random_engine e;
    std::uniform_int_distribution<int> uniform_dist(from, to);

    for(std::size_t i = 0; i < count; ++i)
    {
        int v = uniform_dist(e);
        typename T::node_t::pointer p = new typename T::node_t(v, std::to_string(v));

        std::cout << prefix << "insert (" << v << "; " << std::to_string(v) << ") : ";

        if(tree.insert(p) != p)
        {
            std::cout << "already exists in tree, deleting." << std::endl;
            delete p;
        }
        else
        {
            std::cout << "done." << std::endl;
            ++ins_count;
        }
    }

    std::cout << prefix << "done. inserted: " << ins_count << std::endl << std::endl;
}

template<typename T, std::size_t N>
void fill_tree_static(const std::string prefix, T& tree, typename T::node_t (& nodes)[N])
{
    std::size_t count = 0;

    std::cout << prefix << "inserting staticaly allocated nodes from array." << std::endl;
    std::cout << prefix << "nodes count: " << N << std::endl;

    for(auto& n: nodes)
    {
        std::cout << prefix << "inserting node (" << n.key << "; " << n.val << ") : ";
        if(tree.insert(&n) != &n)
        {
            std::cout << "already exists in tree." << std::endl;
        }
        else
        {
            std::cout << "done." << std::endl;
            ++count;
        }
    }

    std::cout << prefix << "done. inserted: " << count << std::endl << std::endl;
}

template<typename T>
void dump_tree(const std::string prefix, const T& tree)
{
    std::cout << prefix << "printing tree." << std::endl;
    if(nullptr != tree.root())
    {
        print(tree.root());
    }
    std::cout << prefix << "done." << std::endl << std::endl;

    std::cout << prefix << "iterating over tree." << std::endl;

    for(auto& v : tree)
    {
        std::cout << prefix << "iterator : " << v << std::endl;
    }

    auto it_end = tree.rend();

    for(auto it = tree.rbegin(); it != it_end; ++it)
    {
        std::cout << prefix << "reverse iterator : " << *it << std::endl;
    }

    std::cout << prefix << "done." << std::endl << std::endl;
}

template<typename T>
void erase_tree_static(const std::string prefix, T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "erasing nodes."   << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "erasing node " << i << ": ";
        if(nullptr != tree.erase(i))
        {
            ++count;
            std::cout << "done." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. erased: " << count << std::endl;
}

template<typename T>
void erase_tree_dynamic(const std::string prefix, T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "erasing nodes."   << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "erasing node " << i << ": ";
        typename T::node_t::pointer p = tree.erase(i);
        if(nullptr != p)
        {
            ++count;
            delete p;
            std::cout << "done." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. erased: " << count << std::endl;
}

template<typename T>
void find_in_tree(const std::string prefix, const T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "find nodes."      << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "search for node " << i << ": ";
        if(tree.end() != tree.find(i))
        {
            ++count;
            std::cout << "found." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
    }

    std::cout << prefix << "done. found: " << count << std::endl;
}

template<typename T, std::size_t N>
void test_tree(const std::string prefix, typename T::node_t (& nodes)[N])
{
    T static_tree;
    fill_tree_static(prefix, static_tree, nodes);
    dump_tree(prefix, static_tree);
    find_in_tree(prefix, static_tree, 0, static_cast<key_type>(N) + 1);
    erase_tree_static(prefix, static_tree, 0, static_cast<key_type>(N) + 1);

    T dynamic_tree;
    fill_tree_dynamic(prefix, dynamic_tree, 0, NODES_COUNT, NODES_COUNT);
    dump_tree(prefix, dynamic_tree);
    find_in_tree(prefix, dynamic_tree, 0, NODES_COUNT + 1);
    erase_tree_dynamic(prefix, dynamic_tree, 0, NODES_COUNT + 1);
}

int main(int, char**, char**)
{
    test_tree<tree_t>(TREE_PREFIX, simple_nodes);
    // test_tree<rb_tree_t>(RB_TREE_PREFIX, rb_nodes);
}
