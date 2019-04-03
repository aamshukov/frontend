//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DAG_H__
#define __DAG_H__

#pragma once

BEGIN_NAMESPACE(core)

struct dag
{
    enum class flags : uint64_t
    {
        clear   = 0x0000,
        deleted = 0x0001
    };

    using flags_type = tmpl_flags<flags>;

    using dag_type = std::shared_ptr<dag>;
    using dags_type = std::vector<dag_type>;

    dags_type papas; // parents
    dags_type kids;

    flags_type flags;

    virtual ~dag()
    {
    }
};

END_NAMESPACE

#endif // __DAG_H__
