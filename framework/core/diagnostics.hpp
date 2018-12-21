//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class diagnostics : public singleton<diagnostics>
{
    public:
        using diagnostics_type = std::vector<string_type>;
        using collected_status_type = std::vector<status>;

    private:
        diagnostics_type                my_warnings;
        diagnostics_type                my_errors;

        collected_status_type           my_collected_status;

    public:
        diagnostics() = default;

        // add diagnostic
        // dump diagnostics
        // add status

        const diagnostics_type&         warnings() const;
        const diagnostics_type&         errors() const;

        const collected_status_type&    collected_status() const;
};

inline const typename diagnostics::diagnostics_type& diagnostics::warnings() const
{
    return my_warnings;
}

inline const typename diagnostics::diagnostics_type& diagnostics::errors() const
{
    return my_errors;
}

inline const typename diagnostics::collected_status_type& diagnostics::collected_status() const
{
    return my_collected_status;
}

END_NAMESPACE

#endif // __DIAGNOSTICS_H__
