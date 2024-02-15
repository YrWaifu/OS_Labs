#ifndef DAG_H
#define DAG_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef struct Node {
    vector<int> heirs;
    bool busy = false;
    string job = "NONE";
} Node;

typedef int (*FuncPtr)();
extern map<int, Node> graph;
extern map<string, FuncPtr> myFunc;

int sum();
int div();
int sub();
int mul();
int myAbs();
int myMod();

bool detectCycles(int currentNode, bool visited[], bool inProcessNodes[]);
bool hasCycles();
bool isGraphConnected(vector<int>& endNodes);

#endif  // DAG_H