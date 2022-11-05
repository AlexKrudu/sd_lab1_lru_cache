#include <iostream>

#include "include/lru_cache.h"

int main() {
    lru::LRUCache lRUCache = lru::LRUCache<int, int>(2);
    lRUCache.put(2, 1); // cache is {1=1}
    lRUCache.put(2, 2); // cache is {1=1, 2=2}
    std::cout << *lRUCache.get(2);    // return 3
    lRUCache.put(1, 1); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    lRUCache.put(4, 1); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    return 0;
}
