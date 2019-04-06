//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_H__
#define __IR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename T>
class ir : private noncopyable
{
    public:
        using token_type = T;

        using tree_type = typename parser<token_type>::tree_type;
        using trees_type = typename parser<token_type>::trees_type;

        struct dag_node_hash
        {
            std::size_t operator () (const tree_type&) const
            {
                return 0;
            }
        };

        struct dag_node_key_comparator
        {
            bool operator() (const tree_type& lhs, const tree_type& rhs) const
            {
                return *lhs == *rhs;
            }
        };

        using dag_cache_type = std::unordered_map<tree_type, tree_type, dag_node_hash, dag_node_key_comparator>;

    private:
        static uint32_t     hash(uint32_t c); // c is key

    public:
        static void         cst_to_ast(tree_type& cst);
        static void         ast_to_asd(tree_type& ast);
};

template <typename T>
inline uint32_t ir<T>::hash(uint32_t c)
{
    // https://gist.github.com/badboy/6267743
    // Robert Jenkins' 96 bit Mix Function
    const uint32_t a = 57979867;
    const uint32_t b = 93252091;

    a=a-b; a=a-c; a=a^(c >> 13);
    b=b-c; b=b-a; b=b^(a << 8);
    c=c-a; c=c-b; c=c^(b >> 13);
    a=a-b; a=a-c; a=a^(c >> 12);
    b=b-c; b=b-a; b=b^(a << 16);
    c=c-a; c=c-b; c=c^(b >> 5);
    a=a-b; a=a-c; a=a^(c >> 3);
    b=b-c; b=b-a; b=b^(a << 10);
    c=c-a; c=c-b; c=c^(b >> 15);

    return c;
}

END_NAMESPACE

#endif // __IR_H__
