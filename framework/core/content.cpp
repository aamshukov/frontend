//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>

BEGIN_NAMESPACE(core)

content::content()
       : my_count(0)
{
}

content::~content()
{
}

const content::datum_type* content::data() const
{
    const datum_type* result(nullptr);

    if(my_data)
    {
        result = my_data.get();
    }

    return result;
}

size_type content::count() const
{
    return my_count;
}

bool content::load(data_provider& provider, operation_status& status)
{
    log_info(L"Loading content ...");

    bool result = provider.load(my_data, my_count, status);

    log_info(L"Loaded content.");

    return result;
}

END_NAMESPACE
