#pragma once

// #include <unordered_map>
#include <set>

// template<typename T>
// class HashFunctionObject {
//   public:
//     size_t operator () (const T& t) const { return t.hash(); }
// };

// template<typename Key, typename Value>
// using Map = std::unordered_map<Key, Value, HashFunctionObject<Key> >;

extern std::set<unsigned long long> hashSet;