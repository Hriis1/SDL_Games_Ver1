#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <functional>

struct A_Point {
    int x, y;
    A_Point(int x, int y) : x(x), y(y) {}
};

struct A_Node {
    A_Point point;
    int g, h;
    A_Node* parent;
    A_Node(A_Point pt, int g, int h, A_Node* parent = nullptr) : point(pt), g(g), h(h), parent(parent) {}
    int f() const { return g + h; }
};

struct CompareNodes {
    bool operator()(const A_Node* lhs, const A_Node* rhs) const {
        return lhs->f() > rhs->f();
    }
};

inline int heuristic(const A_Point& a, const A_Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

inline std::vector<A_Point> getNeighbors(const A_Point& p, const std::function<bool(int, int)>& isWalkable) {
    std::vector<A_Point> neighbors;
    std::vector<A_Point> directions = { {0,1}, {1,0}, {0,-1}, {-1,0} };
    for (const auto& dir : directions) {
        A_Point neighbor(p.x + dir.x, p.y + dir.y);
        if (isWalkable(neighbor.x, neighbor.y)) {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

inline std::vector<A_Point> reconstructPath(A_Node* node) {
    std::vector<A_Point> path;
    while (node != nullptr) {
        path.push_back(node->point);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

inline std::vector<A_Point> aStar(A_Point start, A_Point goal, int mapWidth, const std::function<bool(int, int)>& isWalkable) {

    //Validation
    if (!isWalkable(goal.x, goal.y))
    {
        std::cout << "Destination is not walkable" << std::endl;
        return {}; // No path found
    }
    if (!isWalkable(start.x, start.y))
    {
        std::cout << "Start is not walkable" << std::endl;
        return {}; // No path found
    }

    std::priority_queue<A_Node*, std::vector<A_Node*>, CompareNodes> openSet;
    std::unordered_map<int, A_Node*> allNodes;

    auto hashPoint = [mapWidth](const A_Point& p) { return p.y * mapWidth + p.x; };

    A_Node* startNode = new A_Node(start, 0, heuristic(start, goal));
    openSet.push(startNode);
    allNodes[hashPoint(start)] = startNode;

    while (!openSet.empty()) {
        A_Node* current = openSet.top();
        openSet.pop();

        if (current->point.x == goal.x && current->point.y == goal.y) {
            std::vector<A_Point> path = reconstructPath(current);
            for (auto& pair : allNodes) {
                delete pair.second;
            }
            return path;
        }

        for (const auto& neighbor : getNeighbors(current->point, isWalkable)) {
            int tentative_g = current->g + 1;
            int h = heuristic(neighbor, goal);

            auto it = allNodes.find(hashPoint(neighbor));
            if (it == allNodes.end() || tentative_g < it->second->g) {
                A_Node* neighborNode = new A_Node(neighbor, tentative_g, h, current);
                openSet.push(neighborNode);
                allNodes[hashPoint(neighbor)] = neighborNode;
            }
        }
    }

    for (auto& pair : allNodes) {
        delete pair.second;
    }

    return {}; // No path found
}

#endif // ASTAR_H
