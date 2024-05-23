#ifndef LAB2_INCLUDE_HASHTABLE_H
#define LAB2_INCLUDE_HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

template <typename Key, typename Value>
class HashTable {
    struct Pair {
        Key key;
        Value value;
    };

    std::vector<std::list<Pair>> _data;
    size_t _size;

    size_t multiplicationHash(const Key& key) const {
        static constexpr double A = 0.6180339887; // ������� �������
        if constexpr (std::is_arithmetic_v<Key>) {
            double hashedValue = key * A;
            hashedValue -= std::floor(hashedValue);
            return static_cast<size_t>(hashedValue * capacity());
        }
        else {
            double hashedValue = 0;
            for (char c : static_cast<std::string>(key))
                hashedValue += static_cast<double>(c) * A;
            hashedValue -= std::floor(hashedValue);
            return static_cast<size_t>(hashedValue * capacity());
        }
    }

public:
    HashTable(size_t capacity) : _size(0), _data(capacity) {}

    HashTable(const HashTable& other) : _data(other._data), _size(other._size) {}

    HashTable& operator=(const HashTable& other) {
        if (this == &other)
            return *this;
        _data = other._data;
        _size = other._size;
        return *this;
    }

    ~HashTable() {
        _data.clear();
        _size = 0;
    }

    size_t capacity() const {
        return _data.size();
    }

    void print() const {
        for (size_t i = 0; i < capacity(); ++i) {
            std::cout << "Level " << i << ": ";
            for (const auto& pair : _data[i]) {
                std::cout << "{" << pair.key << " : " << pair.value << "} ";
            }
            std::cout << std::endl;
        }
    }

    void insert(const Key& key, const Value& value) {
        size_t index = multiplicationHash(key);
        for (const auto& pair : _data[index]) {
            if (pair.key == key) {
                return;
            }
        }
        _data[index].push_back({ key, value });
        _size++;
    }

    void insert_or_assign(const Key& key, const Value& value) {
        size_t index = multiplicationHash(key);
        for (auto& pair : _data[index]) {
            if (pair.key == key) {
                pair.value = value;
                return;
            }
        }
        _data[index].push_back({ key, value });
        _size++;
    }

    bool contains(const Value& value) const {
        for (const auto& list : _data) {
            for (const auto& pair : list) {
                if (pair.value == value) {
                    return true;
                }
            }
        }
        return false;
    }

    Value* search(const Key& key) {
        size_t index = multiplicationHash(key);
        for (auto& pair : _data[index]) {
            if (pair.key == key) {
                return &pair.value;
            }
        }
        return nullptr;
    }

    bool erase(const Key& key) {
        size_t index = multiplicationHash(key);
        auto& bucket = _data[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                _size--;
                return true;
            }
        }
        return false;
    }

    size_t count(const Key& key) const {
        size_t index = multiplicationHash(key);
        size_t count = 0;
        for (const auto& pair : _data[index]) {
            if (pair.key == key) {
                count++;
            }
        }
        return count;
    }

    size_t romanToDecimal(const std::string& roman) {
        HashTable<char, int> _nums(1);
        _nums.insert('I', 1);
        _nums.insert('V', 5);
        _nums.insert('X', 10);
        _nums.insert('L', 50);
        _nums.insert('C', 100);
        _nums.insert('D', 500);
        _nums.insert('M', 1000);

        int result = 0;
        int prevValue = 0;

        for (int i = roman.size() - 1; i >= 0; --i) {
            int curValue = *_nums.search(roman[i]);
            if (curValue < prevValue) {
                result -= curValue;
            }
            else {
                result += curValue;
            }
            prevValue = curValue;
        }

        return result;
    }
};

#endif
