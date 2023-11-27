
#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <ctime>
#include <iomanip>
#include <cstring>

template <class T> std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& matrix)
{
    for (auto i = 0; i < matrix.size(); i++) {
        const auto& row = matrix[i];
        for (const auto& element : row) {
            os << element << " ";
        }
        if (i != matrix.size() - 1)
            os << std::endl;
    }
    return os;
}

template <class T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& array)
{
    os << "[";
    for (auto i = 0; i < array.size(); i++) {
        if (i != 0)
            os << ",";
        os << array[i];
    }
    os << "]";
    return os;
}

template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}


template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& map)
{
    for (const auto& [key, value] : map) {
        os << key << ": " << value << ", ";
    }
    return os;
}

template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::map<K, V>& map)
{
    for (const auto& [key, value] : map) {
        os << key << ": " << value << ", ";
    }
    return os;
}

template <class T> std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& array)
{
    os << "[";
    int count = 0;
    for (const auto& i : array) {
        if (count != 0)
            os << ",";
        os << i;
        count++;
    }
    os << "]";
    return os;
}

template <class T> std::ostream& operator<<(std::ostream& os, const std::set<T>& array)
{
    os << "[";
    int count = 0;
    for (const auto& i : array) {
        if (count != 0)
            os << ",";
        os << i;
        count++;
    }
    os << "]";
    return os;
}

inline void printNiceTime(time_t& now)
{
    std::tm* localTime = std::localtime(&now);
    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%d:%d:%d", hours, minutes, seconds);
    std::cout << std::left << std::setw(11) << buffer << std::endl;
}