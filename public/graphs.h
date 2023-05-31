#pragma once

#include <bits/stdc++.h>

using namespace std;

enum class COLOR {
	WHITE,
	BLACK,
	GREY
};

namespace Graph {

	#define N 105

	extern int adjMtrx[N][N];
	extern int used[N];
	template <class T>
	using Graph = multimap<T, T>;

	template <class T>
	using WGraph = vector<pair<pair<T, T>, T>>;

	int adjMtrx[N][N];
	int used[N];

	template <class T>
	void BFS(Graph<T>& edges, T v) {
		//queue<T>(queue<T>::container_type(v))
		queue<T> Open;
		Open.push(v);
		BFS<T>(edges, Open, set<T>{});
	}

	template <class T>
	void BFS(Graph<T>& edges, queue<T>& Open, set<T>& Close) {
		T v = Open.front();
		Open.pop();

		if (Close.find(v) != Close.end()) return;
		cout << v << endl;
		Close.insert(v);

		auto Potomki = edges.equal_range(v);
		for (auto it = Potomki.first; it != Potomki.second; ++it) {
			Open.push(it->second);
		}

		while (!Open.empty()) {

			BFS(edges, Open, Close);
		}
	}

	template <class T>
	void DFS(Graph<T>& edges, T v) {
		stack<T> Open;
		Open.push(v);
		DFS<T>(edges, Open, set<T>{});
	}

	template <class T>
	void DFS(Graph<T>& edges, stack<T>& Open, set<T>& Close) {
		T v = Open.top();
		Open.pop();

		if (Close.find(v) != Close.end()) return;
		cout << v << endl;
		Close.insert(v);

		auto Potomki = edges.equal_range(v);
		for (auto it = Potomki.first; it != Potomki.second; ++it) {
			Open.push(it->second);
		}

		while (!Open.empty()) {

			DFS(edges, Open, Close);
		}
	}

	template <class T>
	Graph<T> getGraph() {
		Graph<T> graph;

		graph.insert(make_pair(1, 2));
		graph.insert(make_pair(2, 1));
		graph.insert(make_pair(1, 4));
		graph.insert(make_pair(4, 2));
		graph.insert(make_pair(5, 4));
		graph.insert(make_pair(2, 3));
		graph.insert(make_pair(3, 5));
		graph.insert(make_pair(1, 6));
		graph.insert(make_pair(6, 7));
		graph.insert(make_pair(7, 8));

		return graph;
	}

	void testDFS() {
		Graph<int> graph;
		graph = getGraph<int>();
		DFS(graph, 1);
	}

	void testBFS() {
		Graph<int> graph;
		graph = getGraph<int>();
		BFS(graph, 1);
	}


	void initAdjMtrx() {
		adjMtrx[0][1] = 1;
		adjMtrx[1][0] = 1;
		adjMtrx[0][3] = 1;
		adjMtrx[3][1] = 1;
		adjMtrx[4][3] = 1;
		adjMtrx[1][2] = 1;
		adjMtrx[2][4] = 1;
		adjMtrx[0][5] = 1;
		adjMtrx[5][6] = 1;
		adjMtrx[6][7] = 1;
	}

	void dynamicBFS(int v_init) {
		queue<int> open;

		used[v_init] = 1;
		open.push(v_init);

		while (!open.empty()) {
			int v_cur = open.front();
			open.pop();

			cout << v_cur << endl;

			for (int i = 0; i < N; ++i) {
				if (adjMtrx[v_cur][i] && !used[i]) {
					used[i] = 1;
					open.push(i);
				}
			}
		}
	}

	void dynamicDFS(int v_init) {
		stack<int> open;

		used[v_init] = 1;
		open.push(v_init);

		while (!open.empty()) {
			int v_cur = open.top();
			open.pop();

			cout << v_cur << endl;

			for (int i = 0; i < N; ++i) {
				if (adjMtrx[v_cur][i] && !used[i]) {
					used[i] = 1;
					open.push(i);
				}
			}
		}

	}

	int components() {
		for (int i = 0; i < N; ++i) {
			used[i] = 0;
		}
		int cnt = 0;
		for (int i = 0; i < N; ++i) {
			if (!used[i]) {
				dynamicDFS(i);
				++cnt;
			}
		}
		return cnt;
	}

	list<pair<int, int>> shortestPath(const vector<vector<int>>& graph, int from, int to) {
		int n = graph[0].size();
		vector<int> used(n, 0);
		list<pair<int,int>> path;

		priority_queue<int> open;
		open.push(from);
		used[from] = 1;

		while (!open.empty()) {
			int cur_v = open.top();
			open.pop();		

			for (int i = 0; i < n; ++i) {
				if (graph[cur_v][i] && !used[i]) {
					open.push(i);

					used[i] = 1;
				}
			}
		}
		return path;
	}

}