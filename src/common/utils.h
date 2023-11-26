#pragma once
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef DEBUG
#define ASSERT(expr)                                                                                                   \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            throw std::runtime_error(std::string("ASSERT: Condition '") + #expr + "' does not hold in function '"      \
                + __FUNCTION__ + "' at " + __FILE__ + ":" + std::to_string(__LINE__));                                 \
        }                                                                                                              \
    } while (false)
#else
#define ASSERT(expr)                                                                                                   \
    do {                                                                                                               \
    } while (false)
#endif

using std::cout;
using std::endl;
using std::list;
using std::make_pair;
using std::map;
using std::pair;
using std::runtime_error;
using std::unordered_set;
using std::vector;

namespace graphalg {

class cRuntimeError : public runtime_error {
public:
    explicit cRuntimeError(const char* format, ...) noexcept
        : runtime_error("")
        , message(nullptr)
    {
        va_list args;
        va_start(args, format);
        formatMessage(format, args);
        va_end(args);
    }

    ~cRuntimeError() noexcept override { delete[] message; }

    const char* what() const noexcept override { return message ? message : runtime_error::what(); }

private:
    char* message;

    void formatMessage(const char* format, va_list args)
    {
        const int bufferSize = 256;
        char buffer[bufferSize];

        // Try to format the message into a buffer
        int n = vsnprintf(buffer, bufferSize, format, args);

        if (n < 0) {
            // vsnprintf failure, set an error message
            message = new char[bufferSize];
            snprintf(message, bufferSize, "Error formatting exception message");
        } else if (n < bufferSize) {
            // Message fits in the buffer
            message = new char[n + 1];
            strncpy(message, buffer, n);
            message[n] = '\0';
        } else {
            // Message does not fit in the buffer, dynamically allocate memory
            message = new char[n + 1];
            vsnprintf(message, n + 1, format, args);
        }
    }
};
}

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