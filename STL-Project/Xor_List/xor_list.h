#ifndef XOR_XOR_LIST_H
#define XOR_XOR_LIST_H

#include <iostream>
#include <compare>
#include <cstdint>
#include <memory>
#include <utility>
#include <initializer_list>
#include <new>
#include <cstdlib>

namespace my_std
{
    template <typename T>
    class Allocator
    {
    public:
        T *allocate();
        template <typename... Args>
        void construct(T *ptr, Args &&...args);
        void destroy(T *ptr);
        void deallocate(T *ptr);
    };

    template <typename T, typename allocator = Allocator<T>>
    class xor_list
    {
    public:
        class iterator;
        class const_iterator;

    public:
        using value_type = T;
        using size_type = std::size_t;
        using allocator_type = allocator;
        using pointer_type = T *;
        using const_pointer = const T *;
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using const_reference = const T &;
        struct Node
        {
            T m_data;
            Node *m_next_prev;
            Node(T val);
        };

    public:
        xor_list();
        explicit xor_list(const allocator &alloc);
        ~xor_list();
        xor_list(const xor_list &rhv);
        xor_list(const xor_list &rhv, const allocator &aloc);
        xor_list(xor_list &&rhv) noexcept;
        xor_list(xor_list &&rhv, const allocator &aloc) noexcept;
        explicit xor_list(size_type count);
        explicit xor_list(size_type count, const allocator &aloc = allocator());
        xor_list(size_type count, const_reference init);
        xor_list(std::initializer_list<value_type> init, const allocator &aloc = allocator());
        template <typename inputIt>
        xor_list(inputIt f, inputIt l, const allocator &aloc = allocator());

    public:
        void assign(size_type count, value_type val);
        template <typename inputIt>
        void assign(inputIt first, inputIt last);
        void assign(std::initializer_list<value_type> init);

    public:
        void swap(xor_list &rhv);
        bool empty() const;
        void resize(size_type s, const_reference init = value_type());
        void clear() noexcept;
        void print() const;
        void push_back(const_reference val);
        void push_front(const_reference val);
        void pop_back();
        void pop_front();
        size_type size() const;
        const_reference front() const;
        reference front();
        const_reference back() const;
        reference back();

    public:
        Node *XOR(Node *first, Node *second) const;
        const xor_list &operator=(const xor_list &rhv);
        const xor_list &operator=(xor_list &&rhv) noexcept;
        const xor_list &operator=(std::initializer_list<value_type> init);

    public:
        bool operator==(const xor_list &rhv) const;
        iterator begin();
        const_iterator cbegin() const;

        iterator end();
        const_iterator cend() const;
        iterator insert(iterator pos, value_type val);
        iterator insert(iterator pos, size_type size, const_reference val);
        iterator insert(iterator pos, std::initializer_list<value_type> init);
        iterator insert(iterator pos, iterator f, iterator l);
        iterator insert_def(iterator pos, const_reference val);
        iterator insert_rev(iterator pos , const_reference val);
        iterator erase(iterator pos);
        iterator erase(iterator f, iterator l);
        size_type remove(const_reference val);
        void reverse();
        void sort();

        void merge(xor_list &other);
        void unique();
        iterator find(const_reference elem);
        iterator rfind(const_reference elem);

    private:
        Node *m_head;
        Node *m_tail;
        Allocator<Node> m_allocator;
    };

    template <typename T, typename allocator>
    class xor_list<T, allocator>::const_iterator
    {
        friend class xor_list<T, allocator>;

    public:
        const_iterator(const const_iterator &rhv);
        const_iterator(const_iterator &&rhv);

        const const_iterator &operator=(const const_iterator &rhv);
        const const_iterator &operator=(const_iterator &&rhv);
        const_reference operator*() const;
        const_pointer operator->() const;

        const const_iterator &operator++();
        const const_iterator operator++(value_type);
        const const_iterator &operator--();
        const const_iterator operator--(value_type);

        bool operator==(const const_iterator &rhv) const;
        bool operator!=(const const_iterator &rhv) const;

    protected:
        explicit const_iterator(Node *ptr, Node *ptr2);
        Node *ptr;
        Node *next;
        Node *prev;
    };

    template <typename T, typename allocator>
    class xor_list<T, allocator>::iterator : public xor_list<T, allocator>::const_iterator
    {
        friend class xor_list<T, allocator>;

    public:
        iterator(const iterator &rhv);
        iterator(iterator &&rhv);

        reference operator*();
        pointer_type operator->();

        const iterator &operator=(const iterator &rhv);
        const iterator &operator=(iterator &&rhv);

    protected:
        explicit iterator(Node *ptr1, Node *ptr2);
    };
}
#include "xor_list.hpp"
#endif