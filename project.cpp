#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
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
  vector<int> BFS(int start, int target) {
    queue<int> q;
    map<int, int> parent;
    map<int, bool> visited;

    q.push(start);
    visited[start] = true;
    parent[start] = -1;
    while (!q.empty()) {
      int current = q.front();
      q.pop();
      if (current == target) {
        vector<int> path;
        for (int v = target; v != -1; v = parent[v]) {
          path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
      }
      for (int neighbor : vertices[current]->adj) {
        if (!visited[neighbor]) {
          visited[neighbor] = true;
          parent[neighbor] = current;
          q.push(neighbor);
        }
      }
    }
    return {};
  }
};

enum agentType { ORDER, RANDOM };

class Agent {
protected:
  int position;
  unordered_set<int> goalPoints;
  int nextPosition = -1;
  int moves = 0;
  agentType type;

public:
  Agent(int startPosition, unordered_set<int> goals, agentType _agentType)
      : position{startPosition}, goalPoints{goals}, type{_agentType} {}
  bool hasFinished() { return goalPoints.empty(); }
  void visitPoint(int point) { goalPoints.erase(point); }
  void recordMove() { moves++; }
  void show() {
    string typeName = (type == ORDER) ? "ORDERED" : "RANDOMIZED";
    cout << typeName << " Agent: " << endl;
    cout << "Position: " << position << endl;
    cout << "Moves: " << moves << endl;
    cout << "Goal points not visited: ";
    if (goalPoints.empty()) {
      cout << "NONE\n";
    } else {
      cout << "{";
      for (auto it = goalPoints.begin(); it != goalPoints.end(); ++it) {
        if (it != goalPoints.begin())
          cout << ", ";
        cout << *it;
      }
      cout << "}\n";
    }
    cout << "Finished?: " << (hasFinished() ? "YES" : "NO") << endl << endl;
  }
  int getPosition() { return position; }
  void setPosition(int p) { position = p; }
  int getNextPosition() { return nextPosition; }
  bool hasGoalPoints() { return !goalPoints.empty(); }
  vector<int> pathToFirstGoalPoint(Graph &graph) {
    if (goalPoints.empty())
      return {};
    int target = *goalPoints.begin();
    return graph.BFS(position, target);
  }
  vector<int> pathToRandomGoalPoint(Graph &graph) {
    if (goalPoints.empty())
      return {};
    int randomIndex = rand() % goalPoints.size();
    auto it = goalPoints.begin();
    advance(it, randomIndex);
    int target = *it;
    return graph.BFS(position, target);
  }
  bool shouldCalculateNewPath() {
    return nextPosition == -1 || getPosition() == nextPosition;
  }
  void setNextStep(int pos) { nextPosition = pos; }
  agentType getType() { return type; }
  bool isGoalPoint(int point) const {
    return goalPoints.find(point) != goalPoints.end();
  }
};

class Simulation {
private:
  Graph *g;
  vector<unique_ptr<Agent>> agents;
  int currentTurn = 0;

public:
  Simulation(Graph *_g) : g{_g} {}
  void addAgent(unique_ptr<Agent> a) { agents.push_back(move(a)); }
  void start() {
    int turn = 0;
    while (true) {
      bool allFinished = true;
      for (auto &agent : agents) {
        if (!agent->hasGoalPoints()) {
          continue;
        }
        if (agent->shouldCalculateNewPath()) {
          vector<int> path;
          if (agent->getType() == ORDER) {
            path = agent->pathToFirstGoalPoint(*g);
          } else {
            path = agent->pathToRandomGoalPoint(*g);
          }
          if (!path.empty() && path.size() > 1) {
            agent->setNextStep(path[1]);
          }
        }
      }
      for (auto &agent : agents) {
        if (!agent->hasGoalPoints()) {
          continue;
        }
        if (agent->getNextPosition() != -1) {
          int oldPos = agent->getPosition();
          int newPos = agent->getNextPosition();
          agent->setPosition(newPos);
          bool wasGoalPoint = agent->isGoalPoint(agent->getPosition());
          if (wasGoalPoint) {
            agent->visitPoint(agent->getPosition());
          }
          if (oldPos != newPos && agent->hasGoalPoints()) {
            agent->recordMove();
          }
          agent->setNextStep(-1);
        }
        if (!agent->hasFinished()) {
          allFinished = false;
        }
      }
      if (allFinished) {
        cout << "Simulation finished after " << turn << " turns!\n" << endl;
        for (auto &a : agents) {
          a->show();
        }
        break;
      }
      turn++;
      if (turn > 1000) {
        cout << "Simulation stopped after 1000 turns!\n" << endl;
        for (auto &a : agents) {
          a->show();
        }
        break;
      }
    }
  }
  void start(int turnLimit) {
    int turn = 0;
    while (true) {
      bool allFinished = true;
      for (auto &agent : agents) {
        if (!agent->hasGoalPoints()) {
          continue;
        }
        if (agent->shouldCalculateNewPath()) {
          vector<int> path;
          if (agent->getType() == ORDER) {
            path = agent->pathToFirstGoalPoint(*g);
          } else {
            path = agent->pathToRandomGoalPoint(*g);
          }
          if (!path.empty() && path.size() > 1) {
            agent->setNextStep(path[1]);
          }
        }
      }
      for (auto &agent : agents) {
        if (!agent->hasGoalPoints()) {
          continue;
        }
        if (agent->getNextPosition() != -1) {
          int oldPos = agent->getPosition();
          int newPos = agent->getNextPosition();
          agent->setPosition(newPos);
          bool wasGoalPoint = agent->isGoalPoint(agent->getPosition());
          if (wasGoalPoint) {
            agent->visitPoint(agent->getPosition());
          }
          if (oldPos != newPos && agent->hasGoalPoints()) {
            agent->recordMove();
          }
          agent->setNextStep(-1);
        }
        if (!agent->hasFinished()) {
          allFinished = false;
        }
      }
      if (allFinished) {
        cout << "Simulation finished after " << turn << " turns!\n" << endl;
        for (auto &a : agents) {
          a->show();
        }
        break;
      }
      turn++;
      if (turn >= turnLimit) {
        cout << "Simulation stopped after " << turnLimit << " turns!\n" << endl;
        for (auto &a : agents) {
          a->show();
        }
        break;
      }
    }
  }
};

int main() {
  srand(time(NULL));
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

  g1.sortAdjacencyList();
  g2.sortAdjacencyList();
  g3.sortAdjacencyList();

  Simulation s1(&g1);
  Simulation s2(&g2);
  Simulation s3(&g3);
  s1.addAgent(
      make_unique<Agent>(1, unordered_set<int>{4, 7, 12, 22, 11, 15}, ORDER));
  s1.addAgent(
      make_unique<Agent>(1, unordered_set<int>{4, 7, 12, 22, 11, 15}, RANDOM));
  s1.addAgent(
      make_unique<Agent>(1, unordered_set<int>{7, 2, 10, 17, 21, 16}, ORDER));
  s1.addAgent(
      make_unique<Agent>(1, unordered_set<int>{7, 2, 10, 17, 21, 16}, RANDOM));

  s2.addAgent(make_unique<Agent>(1, unordered_set<int>{8, 15, 17, 9, 22, 11}, ORDER));
  s2.addAgent(make_unique<Agent>(1, unordered_set<int>{8, 15, 17, 9, 22, 11}, RANDOM));
  s2.addAgent(make_unique<Agent>(2, unordered_set<int>{4, 20, 18, 6, 19, 23}, ORDER));
  s2.addAgent(make_unique<Agent>(2, unordered_set<int>{4, 20, 18, 6, 19, 23}, RANDOM));
  
  s3.addAgent(make_unique<Agent>(1, unordered_set<int>{10, 7, 12, 17, 5, 16}, ORDER));
  s3.addAgent(make_unique<Agent>(1, unordered_set<int>{10, 7, 12, 17, 5, 16}, RANDOM));
  s3.addAgent(make_unique<Agent>(3, unordered_set<int>{8, 11, 13, 15, 2, 6}, ORDER));
  s3.addAgent(make_unique<Agent>(3, unordered_set<int>{8, 11, 13, 15, 2, 6}, RANDOM));
  cout << "GRAPH 1:\n\n";
  s1.start();
  cout << "GRAPH 2:\n\n";
  s2.start();
  cout << "GRAPH 3:\n\n";
  s3.start();
  return 0;
}