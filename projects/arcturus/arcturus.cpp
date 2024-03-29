//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/status.hpp>

#include <core/text.hpp>
#include <core/unicode.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>
#include <core/flags.hpp>
#include <core/counter.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/domain_helper.hpp>

#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/data_provider.hpp>
#include <core/file_data_provider.hpp>
#include <core/string_data_provider.hpp>

#include <core/content.hpp>

#include <core/command_line.hpp>
#include <core/configurator.hpp>

#include <core/dag.hpp>
#include <core/tree.hpp>

#include <core/visitor.hpp>
#include <core/visitable.hpp>

#include <core/logger.hpp>

#include <core/context.hpp>

#include <frontend/grammar/token.hpp>
#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>

#include <symtable/scope/scope.hpp>
#include <symtable/scope/scope_namespace.hpp>
#include <symtable/scope/scope_structure.hpp>
#include <symtable/scope/scope_function.hpp>
#include <symtable/scope/scope_parameter.hpp>
#include <symtable/scope/scope_block.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>
#include <frontend/fsa/fsa_re.hpp>
#include <frontend/fsa/fsa_codegen.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/parser/parse_dag.hpp>
#include <frontend/parser/parse_tree.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/parser_algorithm.hpp>

#include <frontend/semantics/semantics.hpp>
#include <frontend/semantics/semantics.inl>

#include <backend/ir/quadruple.hpp>

#include <backend/ir/ir_visitor.hpp>
#include <backend/ir/dag_tac_visitor.hpp>
#include <backend/ir/tree_tac_visitor.hpp>

#include <backend/ir/ssa.hpp>

#include <backend/ir/ir.hpp>
#include <backend/ir/ir.inl>

#include <controller/controller.hpp>

#include <trees/arcturus_tree.hpp>
#include <arcturus.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(controller)

struct arktur_token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        ws,     //  6
        eol,    //  7
        eos,    //  8
        indent, //  9, literal = '    '
        dedent, // 10, literal = '    '

        a,   // 8
        b,   // 9
        c,   // 10
        d,   // 11
        e,   // 12
        f,   // 13
        g,   // 14
        n,   // 15
        p, // + 16
        m, // * 17
        l, // ( 18
        r, // ) 19

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

struct arctur_tree_traits
{
    DECLARE_ENUM
    (
        kind,
        uint32_t,
        unknown = 0,

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

END_NAMESPACE

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(arcturus)

int main()
{
    using token_type = token<arktur_token_traits>;

    //auto root = arcturus_tree<token_type, arctur_tree_traits, void, int>();

    return 0;
}
