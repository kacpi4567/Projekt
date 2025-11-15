#include <algorithm> // Dodano dla sort()
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct Vertex {
  typedef Vertex *Edge;
  vector<Edge> adj;
  int name;
  Vertex(int s) : name(s) {}
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
      cout << "Vertex " << name << " already exists!" << endl;
    }
  }
  void addEdge(int from, int to) {
    if (vertices.find(from) == vertices.end() ||
        vertices.find(to) == vertices.end()) {
      cout << "Vertex does not exist!" << endl;
      return;
    }
    Vertex *f = vertices[from];
    Vertex *t = vertices[to];
    f->adj.push_back(t);
    t->adj.push_back(f);
  }
  void sortAdjacencyLists() {
    for (auto &[name, vertex] : vertices) {
      sort(vertex->adj.begin(), vertex->adj.end(),
           [](Vertex *a, Vertex *b) { return a->name < b->name; });
    }
  }
};

class BFS {
private:
  typedef set<int> PSet;
  int position;
  PSet *points;
  Graph *g;
  int moves = 0;
  int waits = 0;

public:
  BFS(int start_position, PSet *s, Graph *_g)
      : position{start_position}, points{s}, g{_g} {}
  void turn() {}
  void show() {
    cout << "POSITION:\n" << position;
    cout << "\nPOINTS LEFT TO VISIT:\n";
    int i = 1;
    for (int element : *points) {
      if (i != points->size()) {
        cout << element << ", ";
        i++;
      } else {
        cout << element;
      }
    }
  }
};

class DFS {
private:
  typedef set<int> PSet;
  int position;
  PSet *points;
  Graph *g;
  int moves = 0;
  int waits = 0;

public:
  DFS(int start_position, set<int> *s, Graph *_g)
      : position{start_position}, points{s}, g{_g} {}
  void turn() {}
  void show() {
    cout << "POSITION:\n" << position;
    cout << "\nPOINTS LEFT TO VISIT:\n";
    int i = 1;
    for (int element : *points) {
      if (i != points->size()) {
        cout << element << ", ";
        i++;
      } else {
        cout << element;
      }
    }
  }
};

int main() {
  Graph g1, g2, g3;

  // PIERWSZY GRAF
  for (int i = 1; i <= 23; i++) {
    g1.addVertex(i);
  }
  for (int i = 1; i < 23; i++) {
    if (i == 6 || i == 12 || i == 18) {
      continue;
    } else {
      g1.addEdge(i, i + 1);
    }
  }

  for (int i = 1; i < 18; i++) {
    g1.addEdge(i, i + 6);
  }

  // DRUGI GRAF
  for (int i = 1; i <= 23; i++) {
    g2.addVertex(i);
  }

  for (int i = 0; i < 1; i++) {
    g2.addEdge(1, 5);
    g2.addEdge(5, 11);
    g2.addEdge(11, 12);
    g2.addEdge(12, 13);
    g2.addEdge(2, 5);
    g2.addEdge(2, 6);
    g2.addEdge(6, 11);
    g2.addEdge(6, 14);
    g2.addEdge(3, 6);
    g2.addEdge(3, 7);
    g2.addEdge(7, 14);
    g2.addEdge(7, 15);
    g2.addEdge(15, 16);
    g2.addEdge(16, 17);
    g2.addEdge(4, 7);
    g2.addEdge(4, 8);
    g2.addEdge(9, 8);
    g2.addEdge(8, 15);
    g2.addEdge(8, 19);
    g2.addEdge(19, 20);
    g2.addEdge(20, 21);
    g2.addEdge(5, 6);
    g2.addEdge(6, 7);
    g2.addEdge(7, 8);
    g2.addEdge(9, 10);
    g2.addEdge(11, 14);
    g2.addEdge(14, 15);
    g2.addEdge(15, 19);
    g2.addEdge(12, 22);
    g2.addEdge(16, 20);
    g2.addEdge(20, 23);
    g2.addEdge(13, 18);
    g2.addEdge(18, 17);
    g2.addEdge(17, 21);
  }

  // TRZECI GRAF
  for (int i = 1; i <= 17; i++) {
    g3.addVertex(i);
  }

  for (int i = 0; i < 1; i++) {
    g3.addEdge(13, 14);
    g3.addEdge(3, 4);
    g3.addEdge(4, 5);
    g3.addEdge(5, 10);
    g3.addEdge(1, 2);
    g3.addEdge(1, 9);
    g3.addEdge(6, 10);
    g3.addEdge(10, 11);
    g3.addEdge(10, 12);
    g3.addEdge(9, 8);
    g3.addEdge(8, 7);
    g3.addEdge(7, 10);
    g3.addEdge(16, 15);
    g3.addEdge(3, 2);
    g3.addEdge(2, 9);
    g3.addEdge(9, 17);
    g3.addEdge(13, 4);
    g3.addEdge(8, 16);
    g3.addEdge(14, 5);
    g3.addEdge(5, 6);
    g3.addEdge(6, 7);
    g3.addEdge(7, 15);
  }

  // SORTOWANIE I WYŚWIETLANIE GRAFU 2
  g1.sortAdjacencyLists(); // SORTOWANIE PRZED WYŚWIETLENIEM
  g2.sortAdjacencyLists(); // SORTOWANIE PRZED WYŚWIETLENIEM
  g3.sortAdjacencyLists(); // SORTOWANIE PRZED WYŚWIETLENIEM

  // cout << "GRAF 1 (posortowane listy sasiedztwa):" << endl;
  // for (auto &[name, vertex] : g1.vertices) {
  //   cout << "Vertex " << name << " -> {";
  //   for (int i = 0; i < vertex->adj.size(); i++) {
  //     cout << vertex->adj[i]->name;
  //     if (i < vertex->adj.size() - 1) {
  //       cout << ", ";
  //     }
  //   }
  //   cout << "}" << endl;
  // }
  // cout << "\nGRAF 3 (posortowane listy sasiedztwa):" << endl;
  // for (auto &[name, vertex] : g2.vertices) {
  //   cout << "Vertex " << name << " -> {";
  //   for (int i = 0; i < vertex->adj.size(); i++) {
  //     cout << vertex->adj[i]->name;
  //     if (i < vertex->adj.size() - 1) {
  //       cout << ", ";
  //     }
  //   }
  //   cout << "}" << endl;
  // }
  // cout << "\nGRAF 3 (posortowane listy sasiedztwa):" << endl;
  // for (auto &[name, vertex] : g3.vertices) {
  //   cout << "Vertex " << name << " -> {";
  //   for (int i = 0; i < vertex->adj.size(); i++) {
  //     cout << vertex->adj[i]->name;
  //     if (i < vertex->adj.size() - 1) {
  //       cout << ", ";
  //     }
  //   }
  //   cout << "}" << endl;
  // }

  set<int> t1 = {1, 7, 16};
  BFS b1(1, &t1, &g1);
  b1.show();
  DFS d1(1, &t1, &g1);

  return 0;
}