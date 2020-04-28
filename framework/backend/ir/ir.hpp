//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_H__
#define __IR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename TreeTraits>
class ir : private noncopyable
{
    public:
        using token_type = Token;
        using tree_traits_type = TreeTraits;

        using tree_type = typename parser<token_type, tree_traits_type>::tree_type;
        using trees_type = typename parser<token_type, tree_traits_type>::trees_type;

        using dag_type = typename parser<token_type, tree_traits_type>::dag_type;
        using dags_type = typename parser<token_type, tree_traits_type>::dags_type;

        using record_type = typename symbol_ir<token_type>::record_type;

        using dag_key_pair = std::tuple<typename token_type::token_type,
                                        typename token_type::codepoints_type,
                                        typename symbol_ir<token_type>::value_type>;
        using dag_key_type = std::vector<dag_key_pair>;

        struct dag_hash
        {
            std::size_t operator () (const dag_key_type& key) const
            {
                std::size_t result = key.size();

                for(auto e : key)
                {
                    result ^= std::hash<std::size_t>{}(static_cast<std::size_t>(std::get<0>(e))) + 0x9E3779B9 + (result << 6) + (result >> 2); // aka boost hash_combine
                }

                return result;
            }
        };

        struct dag_key_comparator
        {
            bool operator () (const dag_key_type& lhs, const dag_key_type& rhs) const
            {
                return lhs == rhs;
            }
        };

        using dag_cache_type = std::unordered_map<dag_key_type, dag_type, dag_hash, dag_key_comparator>;

        using kids_type = std::vector<dag_type>;

        using quadruple_type = quadruple<token_type>;
        using code_type = std::list<quadruple_type>; //??

    private:
        static dag_key_type build_dag_key(const tree_type& tree);
        static dag_type     find_dag(const dag_key_type& key, const dag_cache_type& cache);

    public:
        static void         cst_to_ast(tree_type& cst);
        static void         ast_to_asd(const tree_type& ast, dag_type& result_asd);
};

END_NAMESPACE

#endif // __IR_H__
