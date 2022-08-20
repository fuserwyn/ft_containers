#pragma once

namespace ft
{
    template <class T>
    struct node
    {
        T			value;
        node*		left;
        node*		right;
        node*		parent;
        bool		isBlack;

        node() : value(T()), left(0), right(0), parent(0), isBlack(false) {}

        node(const T &val) : value(val), left(0), right(0), parent
                (0), isBlack(false) {}

        node& operator=(const node& src) {
            value = src.value;
            left = src.left;
            right = src.right;
            parent = src.parent;
            isBlack = src.isBlack;
            return *this;
        }
    };
}