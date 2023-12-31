#include "graph.h"
#include "../algorithms/algorithms.h"
#include <algorithm>
#include <graphviz/gvc.h>
#include <stack>

namespace graphalg {
using namespace graphalg::algorithms;
using std::queue;
using std::stack;
Node Graph::INVALID_NODE = -1;
bool is_in_vector(const Node& x, const vector<Node> vec)
{
    for (auto& i : vec) {
        if (i == x) {
            return true;
        }
    }
    return false;
}

void Graph::read_dot(const char* filename)
{
    // 打开 DOT 文件
    FILE* dotFile = fopen(filename, "r");
    if (!dotFile) {
        throw cRuntimeError("Failed to open %s.", filename);
    }

    // 读取 DOT 文件
    Agraph_t* graph = agread(dotFile, NULL);
    fclose(dotFile);

    if (!graph) {
        throw cRuntimeError("Failed to read DOT file.");
    }

    // 输出节点和边的信息
    Agnode_t* node;
    Agedge_t* edge;

    // 遍历节点
    for (node = agfstnode(graph); node; node = agnxtnode(graph, node)) {
        auto u = atoi(agnameof(node));

        // 遍历节点的出边
        for (edge = agfstout(graph, node); edge; edge = agnxtout(graph, edge)) {
            auto v = atoi(agnameof(aghead(edge)));
            if (u != v)
                add_edge(u, v);
        }

        // 遍历节点的入边
        for (edge = agfstin(graph, node); edge; edge = agnxtin(graph, edge)) {
            auto v = atoi(agnameof(aghead(edge)));
            if (u != v)
                add_edge(v, u);
        }
    }
    // 关闭图
    agclose(graph);
}

vector<Node> Graph::dfs(Node root, bool directionOut) const
{
    ASSERT(has_node(root));
    vector<Node> result { root };
    stack<Node> st;
    st.push(root);
    unordered_set<int> visited { root };
    auto& adj = directionOut ? adjout : adjin;
    while (!st.empty()) {
        auto u = st.top();
        st.pop();
        if (visited.find(u) == visited.end()) {
            visited.insert(u);
            result.push_back(u);
        }
        for (auto& [v, w] : adj.at(u)) {
            if (visited.find(v) == visited.end())
                st.push(v);
        }
    }
    return result;
}

vector<Node> Graph::bfs(Node root, bool directionOut) const
{
    ASSERT(has_node(root));
    vector<Node> result { root };
    queue<Node> que;
    que.push(root);
    unordered_set<Node> visited { root };
    auto& adj = directionOut ? adjout : adjin;
    while (!que.empty()) {
        auto u = que.front();
        que.pop();
        if (visited.find(u) == visited.end()) {
            visited.insert(u);
            result.push_back(u);
        }
        for (auto& [v, w] : adj.at(u)) {
            if (visited.find(v) == visited.end())
                que.push(v);
        }
    }
    return result;
}

bool Graph::is_tree() const
{
    for (auto& n : nodes) {
        if (outdegree(n) != 1 && outdegree(n) != 0) {
            return false;
        }
    }
    return true;
}

bool Graph::is_connected() const
{
    auto node = nodes[0];
    auto visited = dfs(node);
    if (visited.size() != nodes.size())
        return false;
    return true;
}

// Add edges
void Graph::add_edge(Node src, Node dest, double weight, bool bidirectional)
{
    add_node(src);
    add_node(dest);
    if (!has_edge(src, dest)) {
        adjout[src].push_back(EdgeWeight(dest, weight));
        adjin[dest].push_back(EdgeWeight(src, weight));
        if (bidirectional) {
            adjout[dest].push_back(EdgeWeight(src, weight));
            adjin[src].push_back(EdgeWeight(dest, weight));
        }
    } else {
        set_weight(src, dest, weight);
    }
}

void Graph::set_weight(Node src, Node dest, double weight)
{
    ASSERT(has_edge(src, dest));
    ASSERT(adjout.find(src) != adjout.end());
    ASSERT(adjin.find(dest) != adjin.end());
    for (auto& x : adjout.at(src)) {
        if (x.first == dest) {
            x.second = weight;
            break;
        }
    }
    for (auto& x : adjin.at(dest)) {
        if (x.first == src) {
            x.second = weight;
            break;
        }
    }
}

void Graph::remove_edge(Node src, Node dest)
{
    ASSERT(adjin.find(dest) != adjin.end());
    ASSERT(adjout.find(src) != adjout.end());
    if (has_edge(src, dest)) {
        auto remove_adj = [](vector<EdgeWeight>& vw, const Node& node) {
            int index = 0;
            for (auto& [v, w] : vw) {
                if (v == node) {
                    break;
                }
                index++;
            }
            vw.erase(vw.begin() + index);
        };
        remove_adj(adjout[src], dest);
        remove_adj(adjin[dest], src);
    }
}

void Graph::add_node(Node n)
{
    if (!has_node(n)) {
        if (n > max_vertice)
            max_vertice = n;
        nodes.push_back(n);
        adjout[n] = vector<EdgeWeight>();
        adjin[n] = vector<EdgeWeight>();
    }
}

void Graph::remove_node(Node n)
{
    // ! delete a node also delete all its edges
    if (has_node(n)) {
        adjout.erase(n);
        adjin.erase(n);
        auto it = std::find(nodes.begin(), nodes.end(), n);
        if (it != nodes.end())
            nodes.erase(it);
        if (n == max_vertice) {
            it = std::max_element(nodes.begin(), nodes.end());
            max_vertice = *it;
        }
        // 删除其他节点中对该节点的边
        for (auto& entry : adjout) {
            vector<EdgeWeight>& neighbors = entry.second;

            neighbors.erase(
                remove_if(neighbors.begin(), neighbors.end(), [n](const Edge& edge) { return edge.first == n; }),
                neighbors.end());
        }
    }
}
bool Graph::has_node(Node n) const { return is_in_vector(n, nodes); }

bool Graph::has_edge(const Node& src, const Node& dest) const
{
    if (adjout.find(src) == adjout.end()) {
        return false;
    } else {
        const auto& outneighbours = out_neighbors(src);
        for (auto& x : outneighbours) {
            if (x.first == dest) {
                return true;
            }
        }
    }
    return false;
}

bool Graph::has_edge(const Edge& e) const
{
    auto& src = e.first;
    auto& dest = e.second;
    return has_edge(src, dest);
}

void Graph::init_weight_matrix()
{
    if (dist)
        throw cRuntimeError("can't init dist twice!");
    int n = get_max_vertice() + 1;
    dist = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        dist[i] = (double*)malloc(n * sizeof(double));
    }
}

void Graph::update_dist()
{
    if (dist)
        reset_dist();
    else {
        init_weight_matrix();
        reset_dist();
    }
    int n = get_max_vertice() + 1;
    ASSERT(dist);
    floyd_warshall(dist, n);
}

void Graph::reset_dist()
{
    ASSERT(dist);
    int n = get_max_vertice() + 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = INFINITY;
        }
    }
    for (auto& uv : adjout) {
        auto& u = uv.first;
        auto& neighbours = uv.second;
        for (auto& [v, w] : neighbours) {
            dist[u][v] = w;
        }
    }
}

double Graph::distance(Node src, Node dest) const
{
    if (dist) {
        return dist[src][dest];
    } else {
        return dijistra(*this, src, dest);
    }
}

double Graph::weight(Node src, Node dst) const
{
    ASSERT(adjout.find(src) != adjout.end());
    ASSERT(has_edge(src, dst));
    if (dist) {
        return dist[src][dst];
    }
    auto& vws = adjout.at(src);
    for (auto& [v, w] : vws) {
        if (v == dst)
            return w;
    }
    throw cRuntimeError("edge %d->%d not exist", src, dst);
}

double Graph::get_cost() const
{
    double cost = 0;
    for (auto& n : nodes) {
        for (auto& [v, w] : adjout.at(n)) {
            cost += w;
        }
    }
    return cost;
}

void Graph::draw(const char* filename, const char* engine)
{
    // set up a graphviz context
    GVC_t* gvc = gvContext();

    // Create a simple digraph
    char gname[] = "network";
    char shape[] = "plaintext";
    char empty[] = "";
    char width[] = "0.1";
    char height[] = "0.1";
    char margin[] = "0.01";
    char overlap[] = "overlap";
    char False[] = "false";
    Agraph_t* g = agopen(gname, Agdirected, 0);

    for (auto& uv : adjout) {
        auto& u = uv.first;
        auto& neighbours = uv.second;
        for (auto& [v, w] : neighbours) {
            auto n = agnode(g, const_cast<char*>(std::to_string(u).c_str()), 1);
            auto m = agnode(g, const_cast<char*>(std::to_string(v).c_str()), 1);
            agsafeset(n, "shape", shape, empty);
            agsafeset(n, "width", width, empty);
            agsafeset(n, "height", height, empty);
            agsafeset(n, "margin", margin, empty);
            agsafeset(m, "shape", shape, empty);
            agsafeset(m, "width", width, empty);
            agsafeset(m, "height", height, empty);
            agsafeset(m, "margin", margin, empty);
            auto e = agedge(g, n, m, 0, 1);
            agsafeset(e, "arrowsize", ".5", empty);
        }
    }
    agsafeset(g, overlap, False, empty);

    // Compute a layout using layout engine from command line args
    gvLayout(gvc, g, engine);
    const char* dotPos = strrchr(filename, '.');
    if (dotPos) {
        const char* ext = dotPos + 1;
        gvRenderFilename(gvc, g, ext, filename);
    } else {
        gvRenderFilename(gvc, g, "png", (std::string(filename) + ".png").c_str());
    }
    // Write the graph according to -T and -o options
    gvRenderJobs(gvc, g);

    // Free layout data
    gvFreeLayout(gvc, g);

    // Free graph structures
    agclose(g);

    // close output file, free context, and return number of errors
    gvFreeContext(gvc);
}

Graph::Graph() { }
Graph::~Graph()
{
    if (dist) {
        for (int i = 0; i < max_vertice + 1; i++) {
            free(dist[i]);
        }
        free(dist);
    }
}

// copy constructor
Graph::Graph(const Graph& other)
{
    // cout << "copy constructor" << endl;
    nodes = other.nodes;
    max_vertice = other.max_vertice;
    adjin = other.adjin;
    adjout = other.adjout;

    if (other.dist) {
        int n = other.get_max_vertice() + 1;
        dist = (double**)malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            dist[i] = (double*)malloc(n * sizeof(double));
            memcpy(dist[i], other.dist[i], n * sizeof(double));
        }
    }
}
// move constructor
Graph::Graph(Graph&& other)
{
    // cout << "move constructor"<<endl;
    // Transfer ownership of the memory
    dist = other.dist;
    other.dist = nullptr;
    nodes = std::move(other.nodes);
    max_vertice = other.max_vertice;
    adjin = std::move(other.adjin);
    adjout = std::move(other.adjout);
}

Graph& Graph::operator=(const Graph& other)
{
    // cout << "copy assignment constructor" <<endl;
    if (this != &other) {
        nodes = other.nodes;
        max_vertice = other.max_vertice;
        adjin = other.adjin;
        adjout = other.adjout;
        dist = other.dist;
    }
    return *this;
}

Graph& Graph::operator=(Graph&& other)
{
    // cout << "move assignment constructor"<<endl;
    if (this != &other) {
        // Transfer ownership of the memory
        dist = other.dist;
        other.dist = nullptr;
        nodes = std::move(other.nodes);
        max_vertice = other.max_vertice;
        adjin = std::move(other.adjin);
        adjout = std::move(other.adjout);
    }
    return *this;
}

}