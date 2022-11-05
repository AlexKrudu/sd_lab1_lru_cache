#include "gtest/gtest.h"
#include "../include/lru_cache.h"

#include <string>

static constexpr int N_CAPACITY = 1000;
static constexpr int N_RECORDS = 2000;

TEST(CacheTest, EmptyCache) {
    lru::LRUCache<int, int> cache(1);
    ASSERT_EQ(cache.size(), 0);
}

TEST(CacheTest, SimpleGetAfterPut){
    lru::LRUCache<int, int> cache(1);
    cache.put(1, 2);
    auto val = cache.get(1);
    ASSERT_TRUE(val.has_value());
    ASSERT_EQ(*val, 2);
}

TEST(CacheTest, KeyDoesNotExist){
    lru::LRUCache<int, int> cache(1);
    cache.put(1, 2);
    auto val = cache.get(2);
    ASSERT_FALSE(val.has_value());
}

TEST(CacheTest, KeyGotEvicted){
    lru::LRUCache<int, int> cache(1);
    cache.put(1, 2);
    cache.put(3, 4);
    ASSERT_FALSE(cache.get(1).has_value());
    auto val = cache.get(3);
    ASSERT_TRUE(val.has_value());
    ASSERT_EQ(*val, 4);
}

TEST(CacheTest, PutWithSameKey){
    lru::LRUCache<std::string, std::string> cache(2);
    cache.put("a", "xy");
    cache.put("a", "yz");
    ASSERT_EQ(cache.size(), 1);
    ASSERT_EQ(*cache.get("a"), "yz");
}

TEST(CacheTest, CapacityTest){
    ASSERT_GT(N_RECORDS, N_CAPACITY); // otherwise wrong test config

    lru::LRUCache<int, int> cache(N_CAPACITY);

    for (int i = 0; i < N_RECORDS; i++){
        cache.put(i, i);
    }

    for (int i = 0; i < N_CAPACITY; i++){
        ASSERT_FALSE(cache.get(i).has_value());
    }

    for (int i = N_CAPACITY; i < N_RECORDS; i++){
        ASSERT_EQ(*cache.get(i), i);
    }

    ASSERT_EQ(N_CAPACITY, cache.size());
}

TEST(CacheTest, GetSavesFromEviction){
    lru::LRUCache<int, int> cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.get(1);
    cache.put(3, 3);
    ASSERT_FALSE(cache.get(2).has_value());
    ASSERT_EQ(*cache.get(1), 1);
    ASSERT_EQ(*cache.get(3), 3);
}


int main(int argc, char** argv){
    testing::InitGoogleTest();
    int ret = RUN_ALL_TESTS();
    return ret;
}