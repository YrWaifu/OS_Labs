#include "main.hpp"

// Проверяет на наличие циклов в графе, начиная с определенного узла
bool detectCycles(int currentNode, bool visited[], bool inProcessNodes[]) {
    if (!visited[currentNode]) {
        visited[currentNode] = true;
        inProcessNodes[currentNode] = true;

        for (int i = 0; i < graph[currentNode].heirs.size(); i++) {
            int nextNode = graph[currentNode].heirs[i];
            if (!visited[nextNode] && detectCycles(nextNode, visited, inProcessNodes)) {
                return true;
            } else if (inProcessNodes[nextNode]) {
                return true;
            }
        }
    }

    inProcessNodes[currentNode] = false;
    return false;
}

// Проверка на наличие циклов в графе
bool hasCycles() {
    bool* visitedNodes = new bool[graph.size()];
    bool* inProcessNodes = new bool[graph.size()];

    for (int i = 0; i < graph.size(); i++) {
        visitedNodes[i] = false;
        inProcessNodes[i] = false;
    }

    for (int i = 0; i < graph.size(); i++) {
        if (detectCycles(i, visitedNodes, inProcessNodes)) {
            return true;
        }
    }

    delete[] visitedNodes;
    delete[] inProcessNodes;

    return false;
}

// Проверка на связность графа
bool isGraphConnected(vector<int>& endNodes) {
    int outDegree;
    int nodeId;
    bool foundInEndNodes;

    for (auto& pair : graph) {
        nodeId = pair.first;
        outDegree = pair.second.heirs.size();
        foundInEndNodes = find(endNodes.begin(), endNodes.end(), nodeId) != endNodes.end();

        if (outDegree > 1 && !foundInEndNodes) {
            return false;
        }
    }

    return true;
}