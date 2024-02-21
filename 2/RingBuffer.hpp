//
// Created by user on 13.02.2024.
//

#ifndef LESTA_TEST_RINGBUFFER_HPP
#define LESTA_TEST_RINGBUFFER_HPP

#include <vector>

template<typename T, size_t N, class Container = std::vector<T>>
class RingBuffer;

template<typename T, size_t N, class Container>
class RingBufferIterator {
public:
    using ringbuffer_t = RingBuffer<T, N, Container>;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = RingBufferIterator<T, N, Container>;
    using iterator_category = std::bidirectional_iterator_tag;
    enum class iterator_type { Inc, End, Default };
private:
    ringbuffer_t& buffer;
    size_type index;
    iterator_type iter_type = iterator_type::Default;
public:
    RingBufferIterator(ringbuffer_t& buffer, size_type index, iterator_type iterType = iterator_type::Default) : buffer(buffer), index(index), iter_type(iterType) {}
    RingBufferIterator(const iterator& other) = default;
    iterator& operator=(const iterator& other) {
        buffer = other.buffer;
        index = other.index;
        iter_type = other.iter_type;
        return *this;
    }
    reference operator*() {
        return index == buffer.tail && iter_type == iterator_type::Inc ? buffer.buffer[buffer.head] : buffer.buffer[index];
    }
    reference operator*() const {
        return index == buffer.tail && iter_type == iterator_type::Inc ? buffer.buffer[buffer.head] : buffer.buffer[index];
    }
    pointer operator->() {
        return &buffer.buffer[index];
    }
    const_pointer operator->() const {
        return &buffer.buffer[index];
    }
    iterator& operator++() {
        if (index == buffer.tail)
            index = (buffer.head + 1) % N;
        else
            index = (index + 1) % N;
        iter_type = iterator_type::Inc;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        if (index == buffer.tail)
            index = (buffer.head + 1) % N;
        else
            index = (index + 1) % N;
        iter_type = iterator_type::Inc;
        return tmp;
    }
    iterator& operator--() {
        if (index == buffer.head)
            index = (buffer.tail + N - 1) % N;
        else
            index = (index + N - 1) % N;
        iter_type = iterator_type::Inc;
        return *this;
    }
    iterator operator--(int) {
        iterator tmp = *this;
        if (index == buffer.head)
            index = (buffer.tail + N - 1) % N;
        else
            index = (index + N - 1) % N;
        iter_type = iterator_type::Inc;
        return tmp;
    }
    difference_type operator-(const iterator& other) const {
        return (index - other.index + N) % N;
    }
    bool operator==(const iterator& other) const {
        return other.index == index && (other.iter_type != iterator_type::End || iter_type != iterator_type::Default);
    }
    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }
};

template<typename T, size_t N, class Container>
class RingBufferConstIterator {
public:
    using ringbuffer_t = RingBuffer<T, N, Container>;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using size_type = size_t;
    using reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = RingBufferConstIterator<T, N, Container>;
    using iterator_category = std::bidirectional_iterator_tag;
    enum class iterator_type { Inc, End, Default };
private:
    const ringbuffer_t& buffer;
    size_type index;
    iterator_type iter_type = iterator_type::Default;
public:
    RingBufferConstIterator(const ringbuffer_t& buffer, size_type index, iterator_type iterType = iterator_type::Default) : buffer(buffer), index(index), iter_type(iterType) {}
    RingBufferConstIterator(const iterator& other) = default;
    iterator& operator=(const iterator& other) {
        buffer = other.buffer;
        index = other.index;
        iter_type = other.iter_type;
        return *this;
    }
    reference operator*() {
        return index == buffer.tail && iter_type == iterator_type::Inc ? buffer.buffer[buffer.head] : buffer.buffer[index];
    }
    reference operator*() const {
        return index == buffer.tail && iter_type == iterator_type::Inc ? buffer.buffer[buffer.head] : buffer.buffer[index];
    }
    pointer operator->() {
        return &buffer.buffer[index];
    }
    const_pointer operator->() const {
        return &buffer.buffer[index];
    }
    iterator& operator++() {
        if (index == buffer.tail)
            index = (buffer.head + 1) % N;
        else
            index = (index + 1) % N;
        iter_type = iterator_type::Inc;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        if (index == buffer.tail)
            index = (buffer.head + 1) % N;
        else
            index = (index + 1) % N;
        iter_type = iterator_type::Inc;
        return tmp;
    }
    iterator& operator--() {
        if (index == buffer.head)
            index = (buffer.tail + N - 1) % N;
        else
            index = (index + N - 1) % N;
        iter_type = iterator_type::Inc;
        return *this;
    }
    iterator operator--(int) {
        iterator tmp = *this;
        if (index == buffer.head)
            index = (buffer.tail + N - 1) % N;
        else
            index = (index + N - 1) % N;
        iter_type = iterator_type::Inc;
        return tmp;
    }
    difference_type operator-(const iterator& other) const {
        return (index - other.index + N) % N;
    }
    bool operator==(const iterator& other) const {
        return other.index == index && (other.iter_type != iterator_type::End || iter_type != iterator_type::Default);
    }
    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }
};

template<typename T, size_t N, class Container>
class RingBuffer {
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = RingBufferIterator<T, N, Container>;
    using const_iterator = RingBufferConstIterator<T, N, Container>;
    Container buffer;
    size_type head = 0;
    size_type tail = 0;
    size_type count = 0;
public:
    friend class RingBufferIterator<T, N, Container>;
    friend class RingBufferConstIterator<T, N, Container>;
    RingBuffer() : buffer(N) {}
    void push_back(const value_type& value) {
        if (count == N) {
            buffer[head] = value;
            head = (head + 1) % N;
            tail = (tail + 1) % N;
        } else {
            buffer[tail] = value;
            tail = (tail + 1) % N;
            ++count;
        }
    }
    void push_back(value_type&& value) {
        if (count == N) {
            buffer[head] = std::move(value);
            head = (head + 1) % N;
            tail = (tail + 1) % N;
        } else {
            buffer[tail] = std::move(value);
            tail = (tail + 1) % N;
            ++count;
        }
    }
    void pop_front() {
        if (count > 0) {
            head = (head + 1) % N;
            --count;
        }
    }
    reference front() {
        return buffer[head];
    }
    const_reference front() const {
        return buffer[head];
    }
    reference back() {
        return buffer[(head + count - 1) % N];
    }
    const_reference back() const {
        return buffer[(head + count - 1) % N];
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
        // we don't need to clear the buffer itself
        head = 0;
        tail = 0;
        count = 0;
    }
    iterator begin() {
        return count > 0 ? iterator(*this, head, iterator::iterator_type::Default) : end();
    }
    const_iterator begin() const {
        return count > 0 ? const_iterator(*this, head, const_iterator::iterator_type::Default) : cend();
    }
    const_iterator cbegin() const {
        return count > 0 ? const_iterator(*this, head, const_iterator::iterator_type::Default) : cend();
    }
    iterator end() {
        return iterator(*this, tail, iterator::iterator_type::End);
    }
    const_iterator end() const {
        return const_iterator(*this, tail, const_iterator::iterator_type::End);
    }
    const_iterator cend() const {
        return const_iterator(*this, tail, const_iterator::iterator_type::End);
    }
};

#endif //LESTA_TEST_RINGBUFFER_HPP
