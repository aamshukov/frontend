//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>

#include <frontend\grammar\token.hpp>
#include <java\frontend\grammar\java_token.hpp>

BEGIN_NAMESPACE(java)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

typename java_token_traits::enum_map_type java_token_traits::mapping;

END_NAMESPACE
