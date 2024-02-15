#include "main.hpp"

map<int, Node> graph;
map<string, FuncPtr> myFunc;

void processAllJobs(vector<int> &startNodes) {
    queue<int> jobQueue;

    for (int i = 0; i < startNodes.size(); i++) {
        jobQueue.push(startNodes[i]);
    }

    while (!jobQueue.empty()) {
        int currentJobId = jobQueue.front();
        jobQueue.pop();

        Node *currentJobNode = &(graph.find(currentJobId)->second);
        string currentJob = currentJobNode->job;

        cout << "Processing JOB: " << currentJobId << endl;

        if (currentJob != "NONE") {
            int result = myFunc[currentJob]();
            if (result == -1) {
                cout << "Error occurred" << endl;
                break;
            }
            cout << "Result: " << result << endl;
        } else {
            cout << "No job specified" << endl;
        }

        currentJobNode->busy = true;
        int numberOfHeirs = currentJobNode->heirs.size();

        if (numberOfHeirs != 0) {
            int nextJobId = currentJobNode->heirs.front();
            Node *nextJobNode = &(graph.find(nextJobId)->second);

            if (!nextJobNode->busy) {
                nextJobNode->busy = true;
                jobQueue.push(nextJobId);
            }
        }
    }
}

int main() {
    ifstream configFile("dag.ini");
    string line;
    vector<int> startNodes;
    vector<int> endNodes;
    int id;
    Node *currentNode;
    myFunc["sum"] = sum;
    myFunc["sub"] = sub;
    myFunc["div"] = div;
    myFunc["mul"] = mul;
    myFunc["myAbs"] = myAbs;
    myFunc["myMod"] = myMod;

    while (getline(configFile, line)) {
        if (line.find('=') != string::npos) {
            istringstream is(line);
            string key, value;
            if (getline(is, key, '=') && getline(is, value)) {
                if (key == "NODE") {
                    Node node;
                    id = stoi(value);
                    graph[stoi(value)] = node;
                } else if (key == "CONNECTION") {
                    int from, to;
                    istringstream(value) >> from >> to;
                    graph[from].heirs.push_back(to);
                } else if (key == "START_NODE") {
                    startNodes.push_back(stoi(value));
                } else if (key == "END_NODE") {
                    endNodes.push_back(stoi(value));
                } else if (key == "JOB") {
                    currentNode = &(graph.find(id)->second);
                    currentNode->job = value;
                }
            }
        }
    }

    configFile.close();

    if (!isGraphConnected(endNodes)) {
        cout << "Граф не связан или пересвязан" << endl;
        return 1;
    }

    if (hasCycles()) {
        cout << "Граф содержит циклы" << endl;
        return 1;
    }

    if (startNodes.empty() || endNodes.empty()) {
        cout << "Отсутствуют стартовые или завершающие узлы" << endl;
        return 1;
    }

    processAllJobs(startNodes);

    return 0;
}