#ifndef XOR_XOR_LIST_HPP
#define XOR_XOR_LIST_HPP
#include "xor_list.h"

namespace my_std
{

    template <typename T>
    T *Allocator<T>::allocate()
    {
        return static_cast<T *>(std::malloc(sizeof(T)));
    }

    template <typename T>
    template <typename... Args>
    void Allocator<T>::construct(T *ptr, Args &&...args)
    {
        ::new (ptr) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void Allocator<T>::destroy(T *ptr)
    {
        ptr->~T();
    }

    template <typename T>
    void Allocator<T>::deallocate(T *ptr)
    {
        std::free(ptr);
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::Node::Node(T val) : m_data(val), m_next_prev(nullptr) {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list() : m_head(nullptr), m_tail(nullptr) {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(const allocator &alloc) : m_allocator(alloc), m_head(nullptr), m_tail(nullptr) {}

    template <typename T, typename allocator>
    template <typename inputIt>
    xor_list<T, allocator>::xor_list(inputIt first, inputIt last, const allocator &alloc) : m_allocator(alloc), m_head(nullptr), m_tail(nullptr)
    {
        for (auto it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(size_type count) : m_head(nullptr), m_tail(nullptr)
    {
        for (size_type i = 0; i < count; ++i)
        {
            push_back(0);
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(std::initializer_list<value_type> init, const allocator &aloc) : m_head(nullptr), m_tail(nullptr)
    {
        for (const auto &elem : init)
        {
            push_back(elem);
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(size_type count, const_reference init) : m_head(nullptr), m_tail(nullptr)
    {
        for (size_type i = 0; i < count; ++i)
        {
            push_back(init);
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(size_type count, const allocator &aloc) : m_head(nullptr), m_tail(nullptr), m_allocator(aloc)
    {
        for (size_type i = 0; i < count; ++i)
        {
            push_back(0);
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::~xor_list()
    {
        clear();
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(const xor_list &rhv) : m_head(nullptr), m_tail(nullptr)
    {
        Node *current = rhv.m_head;
        Node *prev = nullptr;
        Node *next;

        while (current)
        {
            Node *newNode = m_allocator.allocate();
            new (newNode) Node(current->m_data);

            if (!m_head)
            {
                m_head = newNode;
                m_tail = newNode;
                newNode->m_next_prev = nullptr;
            }
            else
            {
                newNode->m_next_prev = XOR(m_tail, nullptr);
                m_tail->m_next_prev = XOR(m_tail->m_next_prev, newNode);
                m_tail = newNode;
            }

            next = XOR(current->m_next_prev, prev);
            prev = current;
            current = next;
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(const xor_list &rhv, const allocator &aloc) : m_head(nullptr), m_tail(nullptr), m_allocator(aloc)
    {
        Node *current = rhv.m_head;
        Node *prev = nullptr;
        Node *next;

        while (current)
        {
            Node *newNode = m_allocator.allocate();
            new (newNode) Node(current->m_data);

            if (!m_head)
            {
                m_head = newNode;
                m_tail = newNode;
                newNode->m_next_prev = nullptr;
            }
            else
            {
                newNode->m_next_prev = XOR(m_tail, nullptr);
                m_tail->m_next_prev = XOR(m_tail->m_next_prev, newNode);
                m_tail = newNode;
            }

            next = XOR(current->m_next_prev, prev);
            prev = current;
            current = next;
        }
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(xor_list &&rhv) noexcept : m_head(rhv.m_head), m_tail(rhv.m_tail)
    {
        rhv.m_head = nullptr;
        rhv.m_tail = nullptr;
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::xor_list(xor_list &&rhv, const allocator &aloc) noexcept : m_head(rhv.m_head), m_tail(rhv.m_tail)
    {
        rhv.m_head = nullptr;
        rhv.m_tail = nullptr;
    }

    template <typename T, typename allocator>
    const xor_list<T, allocator> &xor_list<T, allocator>::operator=(const xor_list &rhv)
    {
        if (this == &rhv)
        {
            return *this;
        }
        clear();
        Node *current = rhv.m_head;
        Node *prev = nullptr;
        Node *next;

        while (current)
        {
            Node *newNode = m_allocator.allocate();
            new (newNode) Node(current->m_data);

            if (!m_head)
            {
                m_head = newNode;
                m_tail = newNode;
                newNode->m_next_prev = nullptr;
            }
            else
            {
                newNode->m_next_prev = XOR(m_tail, nullptr);
                m_tail->m_next_prev = XOR(m_tail->m_next_prev, newNode);
                m_tail = newNode;
            }

            next = XOR(current->m_next_prev, prev);
            prev = current;
            current = next;
        }
        return *this;
    }

    template <typename T, typename allocator>
    const xor_list<T, allocator> &xor_list<T, allocator>::operator=(xor_list &&rhv) noexcept
    {
        if (this == &rhv)
        {
            return *this;
        }

        m_head = rhv.m_head;
        m_tail = rhv.m_tail;
        m_allocator = rhv.m_allocator;

        rhv.m_head = nullptr;
        rhv.m_tail = nullptr;

        return *this;
    }

    template <typename T, typename allocator>
    const xor_list<T, allocator> &xor_list<T, allocator>::operator=(std::initializer_list<value_type> init)
    {
        clear();
        for (const auto &elem : init)
        {
            push_back(elem);
        }

        return *this;
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::push_back(const_reference val)
    {
        Node *new_node = m_allocator.allocate();
        m_allocator.construct(new_node, val);
        if (!m_tail)
        {
            m_head = m_tail = new_node;
        }
        else
        {
            new_node->m_next_prev = XOR(nullptr, m_tail);
            m_tail->m_next_prev = XOR(new_node, XOR(m_tail->m_next_prev, nullptr));
            m_tail = new_node;
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::pop_back()
    {
        if (!m_tail)
        {
            throw std::logic_error("List is empty");
        }
        else if (!m_head->m_next_prev)
        {
            m_head->~Node();
            m_allocator.deallocate(m_head);
            m_head = m_tail = nullptr;
        }
        else
        {
            Node *prev = XOR(nullptr, m_tail->m_next_prev);
            prev->m_next_prev = XOR(nullptr, XOR(m_tail, prev->m_next_prev));

            m_allocator.destroy(m_tail);
            m_allocator.deallocate(m_tail);

            m_tail = prev;
        }
    }

    template <typename T, typename allocator>
    bool xor_list<T, allocator>::empty() const
    {
        return m_head == nullptr;
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::swap(xor_list &rhv)
    {
        std::swap(*this, rhv);
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::resize(size_type s, const_reference init)
    {
        if (size() > s)
        {
            while (s != size())
            {
                pop_back();
            }
        }
        else
        {
            while (s > size())
            {
                push_back(0);
            }
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::pop_front()
    {
        if (!m_head)
        {
            throw std::logic_error("List is empty");
        }
        else if (!m_head->m_next_prev)
        {
            m_head->~Node();
            m_allocator.deallocate(m_head);
            m_head = m_tail = nullptr;
        }
        else
        {
            Node *next = XOR(nullptr, m_head->m_next_prev);
            next->m_next_prev = XOR(nullptr, XOR(m_head, next->m_next_prev));

            m_allocator.destroy(m_head);
            m_allocator.deallocate(m_head);

            m_head = next;
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::push_front(const_reference val)
    {
        Node *new_node = m_allocator.allocate();
        m_allocator.construct(new_node, val);
        if (!m_head)
        {
            m_head = m_tail = new_node;
        }
        else
        {
            new_node->m_next_prev = XOR(nullptr, m_head);
            m_head->m_next_prev = XOR(new_node, XOR(m_head->m_next_prev, nullptr));
            m_head = new_node;
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::print() const
    {
        Node *curr = m_head;
        Node *prev = nullptr;
        Node *next;
        while (curr != nullptr)
        {
            std::cout << curr->m_data << " ";
            next = XOR(prev, curr->m_next_prev);
            prev = curr;
            curr = next;
            if (curr == nullptr)
            {
                std::cout << "asdsadas";
            }
        }
        std::cout << std::endl;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::Node *xor_list<T, allocator>::XOR(Node *first, Node *second) const
    {
        return reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(first) ^ reinterpret_cast<uintptr_t>(second));
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::clear() noexcept
    {
        Node *current = m_head;
        Node *prev = nullptr;

        while (current)
        {
            Node *next = XOR(current->m_next_prev, prev);
            m_allocator.deallocate(current);
            prev = current;
            current = next;
        }

        m_head = nullptr;
        m_tail = nullptr;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::size_type xor_list<T, allocator>::size() const
    {
        Node *current = m_head;
        Node *prev = nullptr;

        size_type count = 0;
        while (current)
        {
            ++count;
            Node *next = XOR(current->m_next_prev, prev);
            prev = current;
            current = next;
        }
        return count;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_reference xor_list<T, allocator>::front() const
    {
        if (!m_head)
        {
            throw std::logic_error("List is empty");
        }
        return m_head->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::reference xor_list<T, allocator>::front()
    {
        if (!m_head)
        {
            throw std::logic_error("List is empty");
        }
        return m_head->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_reference xor_list<T, allocator>::back() const
    {
        if (!m_tail)
        {
            throw std::logic_error("List is empty");
        }
        return m_tail->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::reference xor_list<T, allocator>::back()
    {
        if (!m_tail)
        {
            throw std::logic_error("List is empty");
        }
        return m_tail->m_data;
    }

    // =====================================const iterator ============================================

    template <typename T, typename allocator>
    bool xor_list<T, allocator>::const_iterator::operator==(const const_iterator &rhv) const
    {
        return rhv.ptr == this->ptr;
    }

    template <typename T, typename allocator>
    bool xor_list<T, allocator>::const_iterator::operator!=(const const_iterator &rhv) const
    {
        return rhv.ptr != this->ptr;
    }

    template <typename T, typename allocator>
    xor_list<T, allocator>::const_iterator::const_iterator(const const_iterator &rhv) : ptr{rhv.ptr}, prev{rhv.prev}, next{rhv.next} {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::const_iterator::const_iterator(const_iterator &&rhv) : ptr{rhv.ptr}, prev{rhv.prev}, next{rhv.next} {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::const_iterator::const_iterator(Node *ptr1, Node *ptr2)
    {
        if (!ptr2)
        {
            ptr = ptr1;
            prev = nullptr;
            next = ptr ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(nullptr) ^ reinterpret_cast<uintptr_t>(ptr->m_next_prev)) : nullptr;
        }
        else
        {
            ptr = reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(ptr2) ^ reinterpret_cast<uintptr_t>(ptr1->m_next_prev));
            prev = ptr1;
            next = ptr ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(prev) ^ reinterpret_cast<uintptr_t>(ptr->m_next_prev)) : nullptr;
        }
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator &xor_list<T, allocator>::const_iterator::operator=(const const_iterator &rhv)
    {
        ptr = rhv.ptr;
        prev = rhv.prev;
        next = rhv.next;
        return *this;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator &xor_list<T, allocator>::const_iterator::operator=(const_iterator &&rhv)
    {
        ptr = rhv.ptr;
        prev = rhv.prev;
        next = rhv.next;
        return *this;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_reference xor_list<T, allocator>::const_iterator::operator*() const
    {
        if (!this->ptr)
        {
            throw std::logic_error("Trying to dereference an invalid iterator");
        }
        return this->ptr->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_pointer xor_list<T, allocator>::const_iterator::operator->() const
    {
        if (!this->ptr)
        {
            throw std::logic_error("Trying to dereference an invalid iterator");
        }
        return &this->ptr->m_data;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator &xor_list<T, allocator>::const_iterator::operator++()
    {
        if (!ptr)
        {
            throw std::logic_error("Incrementing an invalid iterator");
        }
        Node *temp = ptr;
        ptr = reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(prev) ^ reinterpret_cast<uintptr_t>(ptr->m_next_prev));
        prev = temp;
        next = (ptr) ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(temp) ^ reinterpret_cast<uintptr_t>(ptr->m_next_prev)) : nullptr;
        return *this;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator xor_list<T, allocator>::const_iterator::operator++(value_type)
    {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator &xor_list<T, allocator>::const_iterator::operator--()
    {
        if (ptr == nullptr && prev == nullptr)
        {
            throw std::logic_error("Decrementing an invalid iterator");
        }

        if (ptr == nullptr)
        {
            Node *temp = ptr;
            ptr = prev;
            next = temp;
            prev = (ptr) ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(ptr->m_next_prev) ^ reinterpret_cast<uintptr_t>(next)) : nullptr;
        }
        else
        {
            Node *temp = ptr;
            ptr = reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(ptr->m_next_prev) ^ reinterpret_cast<uintptr_t>(next));
            next = temp;
            prev = (ptr) ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(ptr->m_next_prev) ^ reinterpret_cast<uintptr_t>(next)) : nullptr;
        }
        return *this;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::const_iterator xor_list<T, allocator>::const_iterator::operator--(value_type)
    {
        const_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_iterator xor_list<T, allocator>::cbegin() const
    {
        return const_iterator(m_head, nullptr);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::const_iterator xor_list<T, allocator>::cend() const
    {
        return const_iterator(m_tail, m_tail ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(m_tail->m_next_prev) ^ reinterpret_cast<uintptr_t>(nullptr)) : nullptr);
    }

    // iterator

    template <typename T, typename allocator>
    xor_list<T, allocator>::iterator::iterator(const iterator &rhv) : const_iterator{rhv.ptr} {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::iterator::iterator(iterator &&rhv) : const_iterator{rhv.ptr} {}

    template <typename T, typename allocator>
    xor_list<T, allocator>::iterator::iterator(Node *ptr1, Node *ptr2) : const_iterator{ptr1, ptr2} {}

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::iterator &xor_list<T, allocator>::iterator::operator=(const iterator &rhv)
    {
        this->ptr = rhv.ptr;
        this->prev = rhv.prev;
        this->next = rhv.next;
        return *this;
    }

    template <typename T, typename allocator>
    const typename xor_list<T, allocator>::iterator &xor_list<T, allocator>::iterator::operator=(iterator &&rhv)
    {
        this->ptr = rhv.ptr;
        this->prev = rhv.prev;
        this->next = rhv.next;
        return *this;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::reference xor_list<T, allocator>::iterator::operator*()
    {
        if (!this->ptr)
        {
            throw std::logic_error("Trying to dereference an invalid iterator");
        }
        return this->ptr->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::pointer_type xor_list<T, allocator>::iterator::operator->()
    {
        if (!this->ptr)
        {
            throw std::logic_error("Trying to dereference an invalid iterator");
        }
        return &this->ptr->m_data;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::begin()
    {
        return iterator(m_head, nullptr);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::end()
    {
        return iterator(m_tail, m_tail ? reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(m_tail->m_next_prev) ^ reinterpret_cast<uintptr_t>(nullptr)) : nullptr);
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::assign(size_type count, value_type val)
    {
        clear();
        for (int i = {}; i < count; ++i)
        {
            push_back(val);
        }
    }

    template <typename T, typename allocator>
    template <typename inputIt>
    void xor_list<T, allocator>::assign(inputIt first, inputIt last)
    {
        clear();

        for (auto it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::assign(std::initializer_list<value_type> init)
    {
        clear();

        for (const auto &it : init)
        {
            push_back(it);
        }
    }

    template <typename T, typename allocator>
    bool xor_list<T, allocator>::operator==(const xor_list &rhv) const
    {
        auto it_lhs = this->begin();
        auto it_rhs = rhv.begin();

        while (it_lhs != this->end() && it_rhs != rhv.end())
        {
            if (!(*it_lhs == *it_rhs))
            {
                return false;
            }
            ++it_lhs;
            ++it_rhs;
        }

        return it_lhs == this->end() && it_rhs == rhv.end();
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::merge(xor_list &other)
    {
        if (this == std::addressof(other))
        {
            return;
        }

        Node *new_head = nullptr;
        Node *new_tail = nullptr;

        Node *l1 = m_head;
        Node *l2 = other.m_head;

        Node *prev_l1 = nullptr;
        Node *prev_l2 = nullptr;
        Node *prev_new_tail = nullptr;

        while (l1 && l2)
        {
            if (l1->val <= l2->val)
            {
                if (!new_head)
                {

                    new_head = l1;
                    new_tail = l1;
                    prev_new_tail = nullptr;
                }
                else
                {

                    Node *next_l1 = XOR(prev_l1, l1->m_next_prev);
                    new_tail->m_next_prev = XOR(prev_new_tail, l1);
                    prev_new_tail = new_tail;
                    new_tail = l1;
                    l1 = next_l1;
                }

                Node *next_l1 = XOR(prev_l1, l1->m_next_prev);
                prev_l1 = l1;
                l1 = next_l1;
            }
            else
            {
                if (!new_head)
                {
                    new_head = l2;
                    new_tail = l2;
                    prev_new_tail = nullptr;
                }
                else
                {

                    Node *next_l2 = XOR(prev_l2, l2->m_next_prev);
                    new_tail->m_next_prev = XOR(prev_new_tail, l2);
                    prev_new_tail = new_tail;
                    new_tail = l2;
                    l2 = next_l2;
                }

                Node *next_l2 = XOR(prev_l2, l2->m_next_prev);
                prev_l2 = l2;
                l2 = next_l2;
            }
        }

        while (l1)
        {
            if (!new_head)
            {
                new_head = l1;
                new_tail = l1;
                prev_new_tail = nullptr;
            }
            else
            {
                Node *next_l1 = XOR(prev_l1, l1->m_next_prev);
                new_tail->m_next_prev = XOR(prev_new_tail, l1);
                prev_new_tail = new_tail;
                new_tail = l1;
                l1 = next_l1;
            }
        }

        while (l2)
        {
            if (!new_head)
            {
                new_head = l2;
                new_tail = l2;
                prev_new_tail = nullptr;
            }
            else
            {
                Node *next_l2 = XOR(prev_l2, l2->m_next_prev);
                new_tail->m_next_prev = XOR(prev_new_tail, l2);
                prev_new_tail = new_tail;
                new_tail = l2;
                l2 = next_l2;
            }
        }

        m_head = new_head;
        m_tail = new_tail;

        other.m_head = nullptr;
        other.m_tail = nullptr;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::erase(iterator pos)
    {
        if (pos.ptr == nullptr)
        {
            throw std::logic_error("Attempt to erase an invalid iterator");
        }

        Node *node_to_delete = pos.ptr;
        Node *prev = XOR(nullptr, node_to_delete->m_next_prev);
        Node *next = XOR(prev, node_to_delete->m_next_prev);

        if (prev)
        {
            prev->m_next_prev = XOR(XOR(prev->m_next_prev, node_to_delete), next);
        }
        else
        {

            m_head = next;
        }

        if (next)
        {
            next->m_next_prev = XOR(prev, XOR(next->m_next_prev, node_to_delete));
        }
        else
        {

            m_tail = prev;
        }

        m_allocator.destroy(node_to_delete);
        m_allocator.deallocate(node_to_delete);

        return iterator(next);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::erase(iterator f, iterator l)
    {
        while (f != l)
        {
            f = erase(f);
        }
        return l;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert_def(iterator pos, const_reference val)
    {
        return insert(pos, val);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert_rev(iterator pos, const_reference val)
    {
        ++pos;
        return insert(pos, val);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert(iterator pos, iterator f, iterator l)
    {
        while (f != l)
        {
            pos = insert(pos, *f++);
        }
        return pos;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert(iterator pos, std::initializer_list<value_type> init)
    {
        for (const auto &val : init)
        {
            pos = insert(pos, val);
        }
        return pos;
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert(iterator pos, value_type val)
    {
        Node *new_node = new Node(val);
        if (pos == begin())
        {
            new_node->m_next_prev = m_head;
            if (m_head)
            {
                m_head->m_prev_next = new_node;
            }
            m_head = new_node;
        }
        else
        {
            Node *current = pos.m_head;
            Node *previous = current->m_prev_next;
            new_node->m_next_prev = current;
            new_node->m_prev_next = previous;
            if (previous)
            {
                previous->m_next_prev = new_node;
            }
            current->m_prev_next = new_node;
        }
        return iterator(new_node);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::rfind(const_reference elem)
    {
        Node *current = m_head;
        Node *last_found = nullptr;

        while (current)
        {
            if (current->m_data == elem)
            {
                last_found = current;
            }
            current = current->m_next_prev;
        }

        return last_found ? iterator(last_found) : end();
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::find(const_reference elem)
    {
        Node *current = m_head;

        while (current)
        {
            if (current->m_data == elem)
            {
                return iterator(current);
            }
            current = current->m_next_prev;

            return end();
        }
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::size_type xor_list<T, allocator>::remove(const_reference val)
    {
        if (!m_head)
        {
            return 0;
        }

        std::vector<T> elements;
        Node *current = m_head;

        while (current)
        {
            elements.push_back(current->m_data);
            current = current->m_next_prev;
        }

        auto new_end = std::remove(elements.begin(), elements.end(), val);

        size_type count_removed = std::distance(new_end, elements.end());

        clear();

        for (auto it = elements.begin(); it != new_end; ++it)
        {
            push_back(*it);
        }

        return count_removed;
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::unique()
    {
        if (!m_head)
        {

            return;
        }

        std::vector<T> elements;
        Node *current = m_head;

        while (current)
        {
            elements.push_back(current->m_data);
            current = current->m_next_prev;
        }

        auto last = std::unique(elements.begin(), elements.end());

        clear();

        for (auto it = elements.begin(); it != last; ++it)
        {
            push_back(*it);
        }
    }

    template <typename T, typename allocator>
    void xor_list<T, allocator>::sort()
    {
        if (!m_head || !m_head->m_next_prev)
        {
            return;
        }

        std::vector<T> elements;

        for (Node *current = m_head; current; current = current->m_next_prev)
        {
            elements.push_back(current->m_data);
        }

        std::sort(elements.begin(), elements.end());

        clear();

        for (const auto &value : elements)
        {
            push_back(value);
        }
    }
    template <typename T, typename allocator>
    void xor_list<T, allocator>::reverse()
    {
        if (!m_head || !m_head->m_next_prev)
        {

            return;
        }

        Node *prev = nullptr;
        Node *current = m_head;
        Node *next;

        while (current)
        {

            next = XOR(current->m_next_prev, prev);

            current->m_next_prev = XOR(prev, next);

            prev = current;
            current = next;
        }

        std::swap(m_head, m_tail);
    }

    template <typename T, typename allocator>
    typename xor_list<T, allocator>::iterator xor_list<T, allocator>::insert(iterator pos, size_type size, const_reference val)
    {
       
        if (pos == begin())
        {
            for (size_type i = 0; i < size; ++i)
            {
                push_front(val); 
            }
            return begin();
        }

        Node *current = pos.m_node;
        Node *prev = nullptr; 

        
        if (current)
        {
            prev = XOR(nullptr, current->m_next_prev);
        }

        Node *new_first = nullptr;
        Node *new_last = nullptr;  

        for (size_type i = 0; i < size; ++i)
        {
            Node *new_node = m_allocator.allocate();
            m_allocator.construct(new_node, val);

            if (!new_first)
            {
                new_first = new_node; 
            }

            if (new_last)
            {
              
                new_node->m_next_prev = XOR(new_last, nullptr);
                new_last->m_next_prev = XOR(new_last->m_next_prev, new_node);
            }

            new_last = new_node; 
        }

    
        if (prev)
        {
            prev->m_next_prev = XOR(new_last, XOR(prev->m_next_prev, current));
        }

        if (current)
        {
            current->m_next_prev = XOR(new_last, XOR(current->m_next_prev, prev));
        }

        if (!prev)
        {
            m_head = new_first;
        }

        if (!current)
        {
            m_tail = new_last;
        }

        return iterator(new_first);
    }

}
#endif