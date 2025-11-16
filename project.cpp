#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Vertex {
  int name;
  vector<int> adj;
  Vertex(int _name) : name{_name} {}
};

class Graph {
private:
  typedef map<int, Vertex *> VertexMap;
  VertexMap vertices;

public:
  void addVertex(int name) {
    if (vertices.find(name) == vertices.end()) {
      vertices[name] = new Vertex(name);
    } else {
      cout << "Vertex " << name << " already exists!\n";
    }
  }
  void addEdge(int from, int to) {
    if (vertices.find(from) == vertices.end()) {
      cout << "Vertex " << from << " does not exist\n";
    } else if (vertices.find(to) == vertices.end()) {
      cout << "Vertex " << to << " does not exist!\n";
    } else {
      Vertex *f = vertices[from];
      Vertex *t = vertices[to];
      f->adj.push_back(to);
      t->adj.push_back(from);
    }
  }
  void sortAdjacencyList() {
    for (auto &[name, vertex] : vertices) {
      sort(vertex->adj.begin(), vertex->adj.end(),
           [](int a, int b) { return a < b; });
    }
  }
  void showAdjacencyList() {
    cout << "Adjacency List:\n";
    for (auto &[name, vertex] : vertices) {
      cout << "(" << name << "): {";
      for (size_t i = 0; i < vertex->adj.size(); i++) {
        cout << vertex->adj[i];
        if (i + 1 < vertex->adj.size()) {
          cout << ", ";
        }
      }
      cout << "}\n";
    }
  }
};

class Agent {
protected:
  int startPosition;
  int endPosition;
  int currentPosition;
  int turns = 0;
  int waits = 0;
  int pardons = 0;
};
class BFS : public Agent {
private:
public:
};

class DFS : public Agent {
private:
public:
};

class Simulation {
private:
  Graph g;

public:
  Simulation(Graph _g) : g{_g} {}
};

int main() {
  // GRAPH 1
  Graph g1;
  for (int i = 1; i <= 6; i++) {
    g1.addVertex(i);
  }
  g1.addEdge(1, 2);
  g1.addEdge(1, 3);
  g1.addEdge(1, 4);
  g1.addEdge(3, 4);
  g1.addEdge(2, 5);
  g1.addEdge(2, 6);
  g1.addEdge(3, 6);
  g1.sortAdjacencyList();
  g1.showAdjacencyList();
  return 0;
}