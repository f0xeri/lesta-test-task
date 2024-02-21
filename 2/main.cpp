#include <iostream>
#include <cassert>
#include <deque>

#define VECTOR 1
#define LIST 0

#ifdef VECTOR
#include "RingBuffer.hpp"
#elif LIST
#include "RingBuffer2.hpp"
#endif

void print(const RingBuffer<int, 5>& rb) {
    for (auto &i : rb)
        std::cout << i << " ";
    std::cout << std::endl;
}

int main() {
    RingBuffer<int, 5> rb;
    rb.push_back(1);
    rb.push_back(2);
    rb.push_back(3);
    assert(rb.front() == 1);
    assert(rb.back() == 3);
    assert(rb.size() == 3);
    print(rb);
    rb.push_back(4);
    rb.push_back(5);
    assert(rb.front() == 1);
    assert(rb.back() == 5);
    assert(rb.full() == true);
    print(rb);
    rb.push_back(6);
    rb.push_back(7);
    assert(rb.front() == 3);
    assert(rb.back() == 7);
    print(rb);
    rb.pop_front();
    rb.pop_front();
    assert(rb.front() == 5);
    assert(rb.back() == 7);
    print(rb);
    auto it = std::find(rb.begin(), rb.end(), 6);
    assert(std::distance(rb.begin(), it) == 1);
    auto it2 = std::prev(rb.cend());
    for (int i = 0; i < 15; ++i, --it2) {
        std::cout << *it2 << " ";
    }
    return 0;
}
