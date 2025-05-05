#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph {
    int V;  // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Add an edge to the graph
    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // Parallel Depth-First Search
    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        parallelDFSUtil(startVertex, visited);
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        // cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }

    // Parallel Breadth-First Search
    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            // cout << v << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[v].size(); ++i) {
                int n = adj[v][i];
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }

    // Sequential DFS (Iterative using stack)
    void sequentialDFS(int startVertex) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(startVertex);

        while (!s.empty()) {
            int v = s.top();
            s.pop();

            if (!visited[v]) {
                visited[v] = true;
                // cout << v << " ";

                for (auto it = adj[v].rbegin(); it != adj[v].rend(); ++it) {
                    if (!visited[*it])
                        s.push(*it);
                }
            }
        }
    }

    // Sequential BFS
    void sequentialBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            // cout << v << " ";

            for (int n : adj[v]) {
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};

int main() {
    srand(time(0));
    int v=1e2;
    int e=v*3;

    Graph g(v);
    // g.addEdge(0, 1);
    // g.addEdge(0, 2);
    // g.addEdge(1, 3);
    // g.addEdge(1, 4);
    // g.addEdge(2, 5);
    // g.addEdge(2, 6);
    for (int i = 0; i < e; ++i) {
        int u = rand() % v;
        int w = rand() % v;
        if (u != w) {
            g.addEdge(u, w);
            g.addEdge(w, u); // Ensure undirected
        }
    }
    double start_time, end_time;

    // Sequential DFS
    cout << "Sequential DFS: ";
    start_time = omp_get_wtime();
    g.sequentialDFS(0);
    end_time = omp_get_wtime();
    cout << "\nTime (Sequential DFS): " << end_time - start_time << " seconds\n\n";

    // Parallel DFS
    cout << "Parallel DFS: ";
    start_time = omp_get_wtime();
    g.parallelDFS(0);
    end_time = omp_get_wtime();
    cout << "\nTime (Parallel DFS): " << end_time - start_time << " seconds\n\n";

    // Sequential BFS
    cout << "Sequential BFS: ";
    start_time = omp_get_wtime();
    g.sequentialBFS(0);
    end_time = omp_get_wtime();
    cout << "\nTime (Sequential BFS): " << end_time - start_time << " seconds\n\n";

    // Parallel BFS
    cout << "Parallel BFS: ";
    start_time = omp_get_wtime();
    g.parallelBFS(0);
    end_time = omp_get_wtime();
    cout << "\nTime (Parallel BFS): " << end_time - start_time << " seconds\n";

    return 0;
}
