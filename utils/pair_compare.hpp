#pragma once

#include "less.hpp"
#include "pair.hpp"

namespace ft
{
    template<class Key, class T, class Compare = ft::less<Key> >
    class pair_compare
    {
    public:
        typedef ft::pair<const Key, T>	value_type;
        typedef Compare 				key_compare;

        pair_compare(const key_compare &compare = key_compare())
                : _compare(compare)
        {}

        bool operator()(const value_type &x, const value_type &y) const
        { return (_compare(x.first, y.first)); }

    private:
        key_compare _compare;
    };
}