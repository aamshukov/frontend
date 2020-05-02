//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class basic_block : private noncopyable
{
    public:
        using id_type = std::size_t;

        using basic_block_type = std::shared_ptr<basic_block>;
        using basic_blocks_type = std::vector<basic_block_type>;

    private:
        id_type                     my_id; // 0 - entry-block, 1 - exit-block
        string_type                 my_label;

        basic_blocks_type           my_successors;
        basic_blocks_type           my_predecessors;

    public:
                                    basic_block(const id_type& id, const string_type& label);
                                   ~basic_block();

        const id_type&              id() const;
        id_type&                    id();

        const string_type&          label() const;
        string_type&                label();

        const basic_blocks_type&    successors() const;
        basic_blocks_type&          successors();

        const basic_blocks_type&    predecessors() const;
        basic_blocks_type&          predecessors();
};

inline const typename basic_block::id_type& basic_block::id() const
{
    return my_id;
}

inline typename basic_block::id_type& basic_block::id()
{
    return my_id;
}

inline const string_type& basic_block::label() const
{
    return my_label;
}

inline string_type& basic_block::label()
{
    return my_label;
}

inline const typename basic_block::basic_blocks_type& basic_block::successors() const
{
    return my_successors;
}

inline typename basic_block::basic_blocks_type& basic_block::successors()
{
    return my_successors;
}

inline const typename basic_block::basic_blocks_type& basic_block::predecessors() const
{
    return my_predecessors;
}

inline typename basic_block::basic_blocks_type& basic_block::predecessors()
{
    return my_predecessors;
}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
