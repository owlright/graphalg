#pragma once
#include <algorithm>
#include <cstdarg>
#include <graphviz/gvc.h>
#include <list>
#include <map>
#include <stdexcept>
#include <unordered_set>
#include <utility>
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

using std::list;
using std::make_pair;
using std::map;
using std::pair;
using std::unordered_set;
using std::vector;

template <typename T> using Mat = std::vector<std::vector<T>>;

class Graph {
public:
    using Edge = pair<int, int>;
    using EdgeWeight = pair<int, double>;

public:
    void add_edge(int src, int dest, double weight = 1.0, bool bidirectional = false);
    void set_weight(int src, int dest, double weight = 1.0);
    void remove_edge(int src, int dest);
    void add_node(int n);
    void remove_node(int n);
    bool has_node(int n);
    bool has_edge(const int& src, const int& dest);
    bool has_edge(const Edge&);

    void update_dist();
    double distance(int src, int dest) const;
    double weight(int src, int dst) const;
    int get_vertices_number() const { return adjout.size(); }
    int get_max_vertice() const { return max_vertice; }

public:
    const vector<int>& get_nodes() const { return nodes; }
    double** get_dist() const;
    const vector<EdgeWeight>& out_neighbors(int src) const
    {
        ASSERT(adjout.find(src) != adjout.end());
        return adjout.at(src);
    }
    const vector<EdgeWeight>& in_neighbors(int src) const
    {
        ASSERT(adjin.find(src) != adjin.end());
        return adjin.at(src);
    }
    int indegree(int v) const { return adjin.at(v).size(); }
    int outdegree(int v) const { return adjout.at(v).size(); }

public:
    void draw(const char* filename, const char* engine = "neato");

private:
    double** dist { nullptr };
    std::vector<int> nodes;
    int max_vertice { -1 };
    map<int, vector<EdgeWeight>> adjout;
    map<int, vector<EdgeWeight>> adjin;

public:
    explicit Graph();
    ~Graph();
    Graph(const Graph& other);
    // assign constructor;
    // Graph a, b
    // a = b will call this;
    Graph& operator=(const Graph& other);
    // Move constructor
    Graph(Graph&& other);
    // Move assignment operator
    // auto a = getNetworkCopy() will call this
    Graph& operator=(Graph&& other);
};

class cRuntimeError : public std::runtime_error {
public:
    explicit cRuntimeError(const char* format, ...) noexcept
        : std::runtime_error("")
        , message(nullptr)
    {
        va_list args;
        va_start(args, format);
        formatMessage(format, args);
        va_end(args);
    }

    ~cRuntimeError() noexcept override { delete[] message; }

    const char* what() const noexcept override { return message ? message : std::runtime_error::what(); }

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