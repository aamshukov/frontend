//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_PARSER_H__
#define __EARLEY_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class earley_parser : public parser<Token, TreeTraits>
{
    // optimization heuristics might be introduced:
    //  - optimization lists, which are predictor-list, completer-list and scanner-list
    //  - if grammar is static it is possible to precalculate 'prediction table',
    //    this implementation handles generic cases where grammars may change between invokations
    //  - predictor prediction table, see above item
    //  - 'execute predictor' flag indicates if predictor should proceed, set when an item is added
    //  - 'execute completer' flag indicates if completer should proceed, set when an item is added
    public:
        using token_type = typename parser<Token, TreeTraits>::token_type;
        using tree_traits_type = typename parser<Token, TreeTraits>::tree_traits_type;

        using lexical_analyzer_type = typename parser<token_type, tree_traits_type>::lexical_analyzer_type;

        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

        enum class flags : uint64_t
        {
            unknown       = 0x00000000,
            init          = 0x00000001, // .
            scanner       = 0x00000002, //  .
            predictor     = 0x00000004, //   . which action introduced item
            completer     = 0x00000008, //  .
            error_scanner = 0x00000010, // .
            error_mask    = 0x07000000,
            marked        = 0x00000080
        };

        using flags_type = tmpl_flags<flags>;

        struct chart;
        using chart_type = std::shared_ptr<chart>;

        struct item;
        using item_type = std::shared_ptr<item>;

        struct item_hash
        {
            std::size_t operator () (const item_type&) const
            {
                return 0; // returning 0 causes item_key_comparator to be called
            }
        };

        struct item_key_comparator
        {
            bool operator () (const item_type& lhs, const item_type& rhs) const
            {
                return *lhs == *rhs;
            }
        };

        using items_type = std::unordered_set<item_type, item_hash, item_key_comparator>;

        struct item
        {
            uint32_t    id;             // unique id

            rule_type   rule;           // production (rule)
            uint32_t    dot;            // dot - position in rhs, if dot_position = rhs.size() it means it points to the end of the rhs

            chart_type  origin_chart;   // pointer to the orinal set/chart where recognition started
            chart_type  master_chart;   // chart it belongs to

            item_type   lptr;           // pointer to left brother
            items_type  rptrs;          // rightmost child(s), if grammar is ambiguous

            flags_type  flags;          // action (predictor, completer, scanner, errorscanner), error cost

            bool operator == (const item& other)
            {
                return rule != nullptr && other.rule != nullptr && (*rule).id() == (*other.rule).id() &&
                       dot == other.dot &&
                       origin_chart != nullptr && other.origin_chart != nullptr && (*origin_chart).id == (*other.origin_chart).id &&
                       lptr == other.lptr;
            }
        };

        struct chart // state or chart
        {
            uint32_t    id;     // unique id
            items_type  items;  // list of items
            token_type  token;
        };

        using charts_type = std::vector<chart_type>;

        // what to build while calling parse, AST or tree(s)
        enum class tree_kind
        {
            build_ast,
            build_trees
        };

        // the stack to keep elemens of parse tree's level
        struct rhs_stack_element
        {
            enum class element_type
            {
                item = 1, // earley item
                rptrs,    // rightmost child(s), if grammar is ambiguous
                symbol    // terminal and its position in the rule.rhs
            };

            using data_type = std::variant<item_type, items_type, symbol_type>;

            data_type data;
            token_type token;
            element_type type;
        };

        using rhs_stack_type = std::stack<rhs_stack_element>;

        using earley_tree = parser_tree<token_type, tree_traits_type>;

        struct parse_tree_element // (Tr = parsing tree, P = papa)
        {
            tree_type tree; // Tr
            tree_type papa; // P
        };

        using parse_tree_elements_type = std::list<parse_tree_element>;

    private:
        charts_type             my_charts;      // list of sets/charts
        trees_type              my_trees;       // tree or list of trees

        grammar&                my_grammar;     // might be chnaged during parsing

        tree_kind               my_tree_kind;   // what to build, trees or AST

    protected:
        virtual tree_type       handle_start(const item_type& item) = 0;

        virtual tree_type       handle_terminal(const symbol_type& symbol,
                                                uint32_t position,     // for ast operators mapping
                                                const rule_type& rule, // for ast operators mapping
                                                const token_type& token,
                                                const tree_type& node) = 0;

        virtual tree_type       handle_before_nonterminal(const item_type& item,
                                                          uint32_t position,     // for ast operators mapping
                                                          const rule_type& rule, // for ast operators mapping
                                                          const tree_type& node,
                                                          bool ambiguous) = 0;
        virtual tree_type       handle_after_nonterminal(const item_type& item,
                                                         uint32_t position,     // for ast operators mapping
                                                         const rule_type& rule, // for ast operators mapping
                                                         const tree_type& node,
                                                         bool ambiguous) = 0;

    private:
        static item_type        create_item(const rule_type& rule,            // production (rule)
                                            const chart_type& origin_chart,   // original chart recognition started
                                            const chart_type& master_chart,   // chart to add to
                                            const item_type& lptr,            // l-ptr
                                            flags action);                    // action introduced this item
        static void             set_rptr(item_type& item, const item_type& new_item);

        static bool             is_item_marked(const item_type& item);

        static void             mark_item(item_type& item);
        static void             unmark_item(item_type& item);

        static chart_type       create_chart(uint32_t id);

        static void             closure(const grammar& gr, chart_type& chart);

        static void             predict(const grammar& gr, const item_type& item, chart_type& chart);
        static void             complete(item_type& item, chart_type& chart);

        static void             scan(chart_type& chart, charts_type& charts, const token_type& token, chart_type& result);

        static void             populate_rhs_stack(const item_type& item, rhs_stack_type& stack);
        static void             clone_tree(const parse_tree_element& source, parse_tree_element& result);

    private:

        bool                    validate_charts(const chart_type& chart);

        void                    build_charts();

        void                    build_parse_trees();
        void                    build_parse_trees(item_type& item, tree_type& papa, tree_type& tree, trees_type& trees);

        void                    build_ast();
        void                    build_ast_tree(item_type& item, tree_type& papa, tree_type& tree, trees_type& trees);

    public:
                                earley_parser(const lexical_analyzer_type& lexical_analyzer, grammar& gr, tree_kind kind);
                               ~earley_parser();

        const charts_type&      charts() const;
        const trees_type&       trees() const;

        void                    parse() override;
};

template <typename Token, typename TreeTraits>
inline const typename earley_parser<Token, TreeTraits>::charts_type& earley_parser<Token, TreeTraits>::charts() const
{
    return my_charts;
}

template <typename Token, typename TreeTraits>
inline const typename earley_parser<Token, TreeTraits>::trees_type& earley_parser<Token, TreeTraits>::trees() const
{
    return my_trees;
}

END_NAMESPACE

#endif // __EARLEY_PARSER_H__
