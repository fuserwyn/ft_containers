#pragma once

#include <memory>
#include "../utils//less.hpp"
#include "../utils/pair.hpp"
#include "../utils/pair_compare.hpp"
#include "../utils/utils.hpp"
#include "../utils/map_iterator.hpp"
#include "../utils/iterator_traits.hpp"
#include "../map/red_black_tree.hpp"
#include "../map/node.hpp"

namespace ft
{
    template <class Key, class Compare = ft::less<Key>, class Alloc = std::allocator<Key> >
    class set{
    public:
        typedef Key														key_type;
        typedef key_type												value_type;
        typedef Compare													key_compare;
        typedef key_compare												value_compare;
        typedef Alloc													allocator_type;
        typedef typename allocator_type::reference						reference;
        typedef typename allocator_type::const_reference				const_reference;
        typedef typename allocator_type::pointer						pointer;
        typedef typename allocator_type::const_pointer					const_pointer;
        typedef ft::rbt_iterator<value_type>						    iterator;
        typedef ft::rbt_iterator<const value_type>					    const_iterator;
        typedef ft::reverse_iterator<iterator>							reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;
        typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
        typedef difference_type											size_type;

        typedef typename Alloc::template rebind<node<value_type> >::other		node_allocator_type;
        typedef red_black_tree<value_type, value_compare, node_allocator_type> 	tree_type;
        typedef typename ft::node<value_type> 									*p_node;

    public:
        explicit set(const key_compare &comp = key_compare(),
                     const allocator_type &alloc = allocator_type()){
            _alloc = alloc;
            node = rbtree.create_node(value_type());
            cmpr = comp;
            sz = 0;
        }

        template <class InputIterator>
        set(InputIterator first, InputIterator last,
            const key_compare &comp = key_compare(),
            const allocator_type &alloc = allocator_type()){
            _alloc = alloc;
            node = rbtree.create_node(value_type());
            cmpr = comp;
            sz = 0;
            insert(first, last);
        }

        ~set(){
            rbtree.clear(&node->parent);
            rbtree.clear(&node);
        }

        set(const set &src){
            _alloc = src._alloc;
            node = rbtree.create_node(value_type());
            cmpr = src.cmpr;
            sz = src.sz;
            *this = src;
        }

        set &operator=(const set &src){
            clear();
            _alloc = src._alloc;
            cmpr = src.cmpr;
            insert(src.begin(), src.end());
            return *this;
        }

        iterator begin(){
            return iterator(node, rbtree.min_node(node->parent));
        }

        const_iterator begin() const{
            return const_iterator(node, rbtree.min_node(node->parent));
        }

        iterator end(){
            return iterator(node, nullptr);
        }

        const_iterator end() const{
            return const_iterator(node, nullptr);
        }

        reverse_iterator rbegin(){
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const{
            return const_reverse_iterator(end());
        }

        reverse_iterator rend(){return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const{
            return const_reverse_iterator(begin());
        }

        bool empty() const{
            return sz == 0;
        }

        size_type size() const{
            return sz;
        }

        size_type max_size() const{
            return rbtree.max_size();
        }

        ft::pair<iterator, bool> insert(const value_type &val){
            bool res = rbtree.insert(&node->parent, rbtree.create_node(val));
            sz += res;
            p_node ptr = rbtree.find_node(node->parent, val);
            return ft::pair<iterator, bool>(iterator(node, ptr), res);
        }

        iterator insert(iterator, const value_type &val){
            sz += rbtree.insert(&node->parent, rbtree.create_node(val));
            p_node ptr = rbtree.find_node(node->parent, val);
            return iterator(node, ptr);
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last){
            for (; first != last; ++first)
                insert(*first);
        }

        void erase(iterator position){
            bool res = rbtree.erase(&node->parent, *position);
            if (res)
                --sz;
        }

        size_type erase(const key_type &k){
            bool res = (bool)rbtree.erase(&node->parent, k);
            if (res)
                --sz;
            return res;
        }

        void erase(iterator first, iterator last){
            while(first != last)
                erase(first++);
        }

        void swap(set &x){
            ft::swap(x._alloc, _alloc);
            ft::swap(x.node, node);
            ft::swap(x.cmpr, cmpr);
            ft::swap(x.sz, sz);
        }

        void clear(){
            rbtree.clear(&node->parent);
            node->parent = 0;
            sz = 0;
        }

        key_compare key_comp() const{
            return cmpr;
        }

        value_compare value_comp() const{
            return cmpr;
        }

        iterator find(const key_type &k){
            p_node ptr = rbtree.find_node(node->parent, k);
            return iterator(node, ptr);
        }

        const_iterator find(const key_type &k) const{
            p_node ptr = rbtree.find_node(node->parent, k);
            return const_iterator(node, ptr);
        }

        size_type count(const key_type &k) const{
            if (rbtree.find_node(node->parent, k))
                return 1;
            return 0;
        }

        iterator lower_bound(const key_type &k){
            return iterator(node, rbtree.lowest_elem(node->parent, k)); }

        const_iterator lower_bound(const key_type &k) const{
            return const_iterator(node, rbtree.lowest_elem(node->parent, k)); }

        iterator upper_bound(const key_type &k){
            p_node tmp = rbtree.lowest_elem(node->parent, k);
            iterator res(node, tmp);
            if (tmp && tmp->value == k)
                ++res;
            return iterator(res);
        }

        const_iterator upper_bound(const key_type &k) const{
            p_node tmp = rbtree.lowest_elem(node->parent, k);
            iterator res(node, tmp);
            if (tmp && tmp->value == k)
                ++res;
            return const_iterator(res);
        }

        ft::pair<iterator, iterator> equal_range(const key_type &k){
            return ft::make_pair(lower_bound(k), upper_bound(k));
        }

        ft::pair<const_iterator, const_iterator> equal_range(const key_type &k) const{
            return ft::make_pair(lower_bound(k), upper_bound(k));
        }

        allocator_type get_allocator() const{
            return _alloc;
        }

    private:
        tree_type		rbtree;
        allocator_type	_alloc;
        p_node			node;
        key_compare		cmpr;
        size_type		sz;
    };

    template <class Key, class Compare, class Allocator>
    bool operator==(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x.size() == y.size()
               && ft::equal(x.begin(), x.end(), y.begin())
               && ft::equal(y.begin(), y.end(), x.begin());
    }

    template <class Key, class Compare, class Allocator>
    bool operator!=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return !(x == y);
    }

    template <class Key, class Compare, class Allocator>
    bool operator<(const set<Key, Compare, Allocator> &x,const set<Key, Compare, Allocator> &y){
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end())
               && x != y;
    }

    template <class Key, class Compare, class Allocator>
    bool operator<=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x < y || x == y; }

    template <class Key, class Compare, class Allocator>
    bool operator>(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return y < x;
    }

    template <class Key, class Compare, class Allocator>
    bool operator>=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x > y || x == y;
    }

    template <class Key, class Compare, class Allocator>
    void swap(set<Key, Compare, Allocator> &x, set<Key, Compare, Allocator> &y){
        x.swap(y);
    }
}