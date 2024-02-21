//
// Created by user on 14.02.2024.
//

#ifndef LESTA_TEST_RINGBUFFER2_HPP
#define LESTA_TEST_RINGBUFFER2_HPP

#include <vector>
#include <list>

template<typename T, size_t N>
class RingBuffer;

template<typename T, size_t N>
class RingBufferIterator {
public:
    using ringbuffer_t = RingBuffer<T, N>;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const T *;
    using iterator = RingBufferIterator<T, N>;
    using iterator_category = std::bidirectional_iterator_tag;
    using iterator_type = typename RingBuffer<T, N>::iterator_type;
private:
    using list_iter = typename std::list<T>::iterator;
    list_iter iter;
    std::list<T> &list;
    iterator_type iter_type;
public:
    explicit RingBufferIterator(std::list<T> &list, list_iter iter, iterator_type iter_type = iterator_type::Default) :
            list(list), iter(iter), iter_type(iter_type) {}

    RingBufferIterator(const iterator &other) = default;

    iterator &operator=(const iterator &other) {
        iter = other.iter;
        return *this;
    }

    reference operator*() {
        return *iter;
    }

    reference operator*() const {
        return *iter;
    }

    pointer operator->() {
        return iter;
    }

    const_pointer operator->() const {
        return iter;
    }

    iterator &operator++() {
        if (iter == list.end()) iter = list.begin();
        iter++;
        iter_type = iterator_type::Inc;
        return *this;
    }

    iterator operator++(int) {
        auto tmp = *this;
        if (iter == list.end()) iter = list.begin();
        iter++;
        iter_type = iterator_type::Inc;
        return tmp;
    }

    iterator &operator--() {
        if (iter == list.begin()) iter = list.end();
        iter--;
        iter_type = iterator_type::Inc;
        return *this;
    }

    iterator operator--(int) {
        auto tmp = *this;
        if (iter == list.begin()) iter = list.end();
        iter--;
        iter_type = iterator_type::Inc;
        return tmp;
    }

    difference_type operator-(const iterator &other) const {
        return iter - other.iter;
    }

    bool operator==(const iterator &other) const {
        return iter == other.iter && (other.iter_type != iterator_type::End || iter_type != iterator_type::Default);
    }

    bool operator!=(const iterator &other) const {
        return !(*this == other);
    }
};

template<typename T, size_t N>
class RingBufferConstIterator {
public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using size_type = size_t;
    using reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using iterator = RingBufferConstIterator<T, N>;
    using iterator_category = std::bidirectional_iterator_tag;
    using iterator_type = typename RingBuffer<T, N>::iterator_type;
private:
    using list_iter = typename std::list<T>::const_iterator;
    list_iter iter;
    const std::list<T> &list;
    iterator_type iter_type;
public:
    explicit RingBufferConstIterator(const std::list<T> &list, list_iter iter,
                                     iterator_type iter_type = iterator_type::Default) :
            list(list), iter(iter), iter_type(iter_type) {}

    iterator &operator=(const iterator &other) {
        iter = other.iter;
        return *this;
    }

    reference operator*() const {
        return iter == list.end() && iter_type == iterator_type::Inc ? *list.begin() : *iter;
    }

    pointer operator->() {
        return iter;
    }

    const_pointer operator->() const {
        return iter;
    }

    iterator &operator++() {
        if (iter == list.end()) iter = list.begin();
        iter++;
        iter_type = iterator_type::Inc;
        return *this;
    }

    iterator operator++(int) {
        auto tmp = *this;
        if (iter == list.end()) iter = list.begin();
        iter++;
        iter_type = iterator_type::Inc;
        return tmp;
    }

    iterator &operator--() {
        if (iter == list.begin()) iter = list.end();
        iter--;
        iter_type = iterator_type::Inc;
        return *this;
    }

    iterator operator--(int) {
        auto tmp = *this;
        if (iter == list.begin()) iter = list.end();
        iter--;
        iter_type = iterator_type::Inc;
        return tmp;
    }

    difference_type operator-(const iterator &other) const {
        return iter - other.iter;
    }

    bool operator==(const iterator &other) const {
        return iter == other.iter && (other.iter_type != iterator_type::End || iter_type != iterator_type::Default);
    }

    bool operator!=(const iterator &other) const {
        return !(*this == other);
    }
};

template<typename T, size_t N>
class RingBuffer {
    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;
    using iterator = RingBufferIterator<T, N>;
    using const_iterator = RingBufferConstIterator<T, N>;
    std::list<T> buffer;
    size_type head = 0;
    size_type tail = 0;
    size_type count = 0;
    enum class iterator_type {
        Inc, End, Default
    };
public:
    friend class RingBufferIterator<T, N>;

    friend class RingBufferConstIterator<T, N>;

    RingBuffer() {}

    void push_back(const value_type &value) {
        if (count == N) {
            buffer.push_back(value);
            buffer.pop_front();
        } else {
            buffer.push_back(value);
            ++count;
        }
    }

    void push_back(value_type &&value) {
        if (count == N) {
            buffer.push_back(std::move(value));
            buffer.pop_front();
        } else {
            buffer.push_back(std::move(value));
            ++count;
        }
    }

    void pop_front() {
        if (count > 0) {
            buffer.pop_front();
            --count;
        }
    }

    reference front() {
        return buffer.front();
    }

    const_reference front() const {
        return buffer.front();
    }

    reference back() {
        return buffer.back();
    }

    const_reference back() const {
        return buffer.back();
    }

    size_type size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    bool full() const {
        return count == N;
    }

    void clear() {
        buffer.clear();
        head = 0;
        tail = 0;
        count = 0;
    }

    iterator begin() {
        return count > 0 ? iterator(buffer, buffer.begin()) : end();
    }

    const_iterator begin() const {
        return count > 0 ? const_iterator(buffer, buffer.cbegin()) : cend();
    }

    const_iterator cbegin() const {
        return count > 0 ? const_iterator(buffer, buffer.cbegin()) : cend();
    }

    iterator end() {
        return iterator(buffer, buffer.end(), iterator_type::End);
    }

    const_iterator end() const {
        return const_iterator(buffer, buffer.cend(), iterator_type::End);
    }

    const_iterator cend() const {
        return const_iterator(buffer, buffer.cend(), iterator_type::End);
    }
};

#endif //LESTA_TEST_RINGBUFFER2_HPP
