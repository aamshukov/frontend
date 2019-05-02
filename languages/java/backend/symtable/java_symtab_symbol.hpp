//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __JAVA_SYMTAB_SYMBOL_H__
#define __JAVA_SYMTAB_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(java)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

class java_symtab_symbol : public symtab_symbol<token<java_token_traits>>
{
};

END_NAMESPACE

#endif // __JAVA_SYMTAB_SYMBOL_H__
