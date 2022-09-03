#include <unordered_map>
#include <vector>
#include <iostream>
#include <functional>
#include <deque>

using namespace std;

void dfs1(const int from, const unordered_multimap<int, int> &graph, vector<bool> &visited, vector<int> &order)
{
    visited[from] = true;

    auto range = graph.equal_range(from);
    for (auto it = range.first; it != range.second; ++it)
    {
        const int to = it->second;
        if (!visited[to])
            dfs1(to, graph, visited, order);
    }

    order.push_back(from);
}

void dfs2(const int from, const unordered_multimap<int, int> &graph, vector<bool> &visited, vector<int> &components, int cnt_components)
{
    visited[from] = true;
    components[from] = cnt_components;

    auto range = graph.equal_range(from);
    for (auto it = range.first; it != range.second; ++it)
    {
        const int to = it->second;
        if (!visited[to])
            dfs2(to, graph, visited, components, cnt_components);
    }
}

int GetStronglyConnectedComponentCount(const int n, const unordered_multimap<int, int> &graph, const unordered_multimap<int, int> &graphT)
{
    vector<int> order;
    vector<bool> visited(n);
    vector<int> components(n);

    fill(begin(visited), end(visited), false);
    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
            dfs1(i, graph, visited, order);
    }

    fill(begin(visited), end(visited), false);
    int cnt_components = 0;
    for (int i = n - 1; i >= 0; --i)
    {
        if (!visited[order[i]])
        {
            dfs2(order[i], graphT, visited, components, cnt_components++);
        }
    }

    return cnt_components;
}

int main()
{
    int n, m;
    unordered_multimap<int, int> graph;
    unordered_multimap<int, int> graphT;
    cin >> n >> m;

    for (int i = 0; i < m; ++i)
    {
        int from, to;
        cin >> from >> to;
        --from;
        --to;

        graph.emplace(from, to);
        graphT.emplace(to, from);
    }

    cout << GetStronglyConnectedComponentCount(n, graph, graphT);

    return 0;
}