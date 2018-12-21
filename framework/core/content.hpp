//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTENT_H__
#define __CONTENT_H__

#pragma once

BEGIN_NAMESPACE(core)

class content : private noncopyable
{
    public:
        using datum_type = data_provider::datum_type;
        using data_type = std::shared_ptr<datum_type[]>;

    private:
        data_type           my_data;
        size_type           my_count;

    public:
                            content();
                           ~content();

        const datum_type*   data() const;
        size_type           count() const;

        bool                load(data_provider& provider, operation_status& status);
};

END_NAMESPACE

#endif // __CONTENT_H__
