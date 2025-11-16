#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>
using namespace std;

struct Vertex {
  int name;
  vector<int> adj;
  Vertex(int _name) : name{_name} {}
};

class Graph {
private:
  map<int, unique_ptr<Vertex>> vertices;

public:
  void addVertex(int name) {
    if (vertices.find(name) == vertices.end()) {
      vertices[name] = make_unique<Vertex>(name);
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
      vertices[from]->adj.push_back(to);
      vertices[to]->adj.push_back(from);
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
  int position;
  unordered_set<int> goalPoints;
  int endPosition;
  int currentPosition;
  int moves = 0;
  int waits = 0;
  int pardons = 0;
  string agentType;

public:
  Agent(int startPosition, unordered_set<int> &goals, int _endPosition,
        string _agentType)
      : position{startPosition}, goalPoints{goals}, endPosition{_endPosition},
        agentType{_agentType} {}
  bool hasFinished() { return goalPoints.empty() && position == endPosition; }
  void visitPoint(int point) { goalPoints.erase(point); }
  void recordMove() { moves++; }
  void recordWait() { waits++; }
  void recordPardon() { pardons++; }
  void show() {
    cout << agentType << " Agent:\n";
    cout << "Position: " << position << endl;
    cout << "Moves: " << moves << endl;
    cout << "Waits: " << waits << endl;
    cout << "Pardons: " << pardons << endl;
    cout << "Goal points not visited: ";
    if (goalPoints.empty()) {
      cout << "NONE\n";
    } else {
      cout << "{";
      for (int goal : goalPoints) {
        if (goalPoints.find(goal) != goalPoints.end()) {
          cout << ", " << goal;
        }
      }
      cout << "}";
    }
    cout << "End position: " << endPosition << endl;
    cout << "Finished?: " << (hasFinished() ? "YES" : "NO") << endl << endl;
  }
  int getPosition() const { return position; }
  void setPosition(int p) { position = p; }
  unordered_set<int> &getGoalPoints() { return goalPoints; }
};

class BFS : public Agent {
private:
public:
  BFS(int _startPosition, unordered_set<int> &goals, int _endPosition)
      : Agent{_startPosition, goals, _endPosition, "BFS"} {}
};

class DFS : public Agent {
private:
public:
  DFS(int _startPosition, unordered_set<int> &goals, int _endPosition)
      : Agent{_startPosition, goals, _endPosition, "DFS"} {}
};

class Simulation {
private:
  Graph *g;
  vector<unique_ptr<Agent>> agents;
  int currentTurn = 0;

public:
  Simulation(Graph *_g) : g{_g} {}
  void addAgent(unique_ptr<Agent> a) { agents.push_back(move(a)); }
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

  // TRZECI GRAF
  for (int i = 1; i <= 17; i++) {
    g3.addVertex(i);
  }
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

  //sortowanie list polaczen grafow
  g1.sortAdjacencyList(); // SORTOWANIE PRZED WYŚWIETLENIEM
  g2.sortAdjacencyList(); // SORTOWANIE PRZED WYŚWIETLENIEM
  g3.sortAdjacencyList(); // SORTOWANIE PRZED WYŚWIETLENIEM
  //wyswietlenie list polaczen
  cout<<"Graf 1: \n";
  g1.showAdjacencyList();
  cout<<"Graf 2: \n";
  g2.showAdjacencyList();
  cout<<"Graf 3: \n";
  g3.showAdjacencyList();
  return 0;
}