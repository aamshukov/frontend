//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_VISUALIZATION_H__
#define __IR_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename T>
class ir_visualization : private noncopyable
{
    public:
        using symbol_type = symbol::symbol_type;
        using symbols_type = symbol::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using ir_type = T;
        using token_type = typename ir_type::token_type;

        using tree_type = typename ir_type::tree_type;
        using trees_type = typename ir_type::trees_type;

        using dag_type = typename ir_type::dag_type;
        using dags_type = typename ir_type::dags_type;

    private:
        static void collect_tree_dot_labels(const tree_type& root, std::wostream& stream);
        static void build_tree_dot_graph(const tree_type& tree, std::wostream& stream);

        static void collect_dag_dot_labels(const dag_type& root, std::wostream& stream);
        static void build_dag_dot_graph(const dag_type& dag, std::wostream& stream);

    public:
        static void print_tree(const tree_type& tree, std::size_t level, std::wostream& stream);
        static void print_trees(const trees_type& trees, std::wostream& stream);

        static void decorate_tree(const tree_type& tree, const string_type& dot_file_name, std::size_t k);
        static void decorate_trees(const trees_type& trees, const string_type& dot_file_name);

        static void decorate_dag(const dag_type& dag, const string_type& dot_file_name);
};

END_NAMESPACE

#endif // __IR_VISUALIZATION_H__
