//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DAG_H__
#define __DAG_H__

#pragma once

BEGIN_NAMESPACE(core)

struct dag
{
    using dag_type = std::shared_ptr<dag>;
    using dags_type = std::vector<dag_type>;

    dags_type papas; // parents
    dags_type kids;

    virtual ~dag()
    {
    }
};

END_NAMESPACE

#endif // __DAG_H__
