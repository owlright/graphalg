#pragma once
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <utility>

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

namespace graphalg {

using std::runtime_error;
using std::vector;

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

template <typename T> struct CompareFirst {
    bool operator()(const T& a, const T& b) const { return a.first > b.first; }
};

using diPair = std::pair<double, int>;
using iiPair = std::pair<int, int>;

template <typename T>
bool operator==(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    if (vec1.size() != vec2.size()) {
        return false; // Vectors have different sizes, they are not equal
    }

    for (size_t i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) {
            return false; // Found a mismatching element, vectors are not equal
        }
    }

    return true; // All elements are equal, vectors are equal
}
