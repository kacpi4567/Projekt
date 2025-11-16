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