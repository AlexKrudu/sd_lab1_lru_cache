#pragma once

#include <unordered_map>
#include <deque>
#include <optional>
#include <list>

namespace lru {
    template<typename Key, typename Val>
    class LRUCache {
    public:
        explicit LRUCache(std::size_t capacity) : capacity_{capacity} {}

        std::optional<Val> get(const Key &key) {
            const auto size_before = size();
            const auto val_pos_iterator = key_val_map_.find(key);
            if (val_pos_iterator == key_val_map_.end()) return std::nullopt;

            auto &val_iterator = val_pos_iterator->second;
            const auto val = *val_iterator;

            elems_queue_.erase(val_iterator);
            elems_queue_.push_front(val);
            val_iterator = elems_queue_.begin();

            const auto size_after = size();
            assert(size_before == size_after);

            return val.value;
        }

        void put(const Key &key, const Val &val) {
            const auto size_before = size();
            assert(size_before <= capacity_);
            const auto val_pos_iterator = key_val_map_.find(key);
            if (val_pos_iterator != key_val_map_.end()) {
                elems_queue_.erase(val_pos_iterator->second);
                elems_queue_.push_front({key, val});
                val_pos_iterator->second = elems_queue_.begin();
                assert(size() == size_before);
                return;
            }

            if (elems_queue_.size() == capacity_) {
                const auto least_used_item_it = key_val_map_.find(elems_queue_.back().key);
                assert(least_used_item_it != key_val_map_.end());
                key_val_map_.erase(least_used_item_it);
                elems_queue_.pop_back();
                assert(size() == size_before - 1);
            }

            elems_queue_.push_front({key, val});
            key_val_map_[key] = elems_queue_.begin();
            assert(size() - size_before <= 1);
        }

        [[nodiscard]] std::size_t size() const {
            assert(elems_queue_.size() == key_val_map_.size());
            return elems_queue_.size();
        }

    private:
        struct KeyValuePair {
            Key key;
            Val value;
        };

        std::list<KeyValuePair> elems_queue_;
        std::unordered_map<Key, typename std::list<KeyValuePair>::iterator> key_val_map_;
        std::size_t capacity_;
    };
}
