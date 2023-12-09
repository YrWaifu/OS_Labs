#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

struct Point {
    int x, y, z;
};

double triangleArea(Point a, Point b, Point c) {
    int abX = b.x - a.x;
    int abY = b.y - a.y;
    int abZ = b.z - a.z;

    int acX = c.x - a.x;
    int acY = c.y - a.y;
    int acZ = c.z - a.z;

    return 0.5 * fabs(abX * acY - abY * acX + abX * acZ - abZ * acX + abY * acZ - acY * abZ);
}

void largestTriangle(vector<Point>& points, size_t start, size_t end, double& largestArea, int threadID) {
    for (size_t i = start; i < end; ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            for (size_t k = j + 1; k < points.size(); ++k) {
                double area = triangleArea(points[i], points[j], points[k]);
                if (area > largestArea) {
                    largestArea = area;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    auto startClock = chrono::high_resolution_clock::now();

    if (argc < 2) {
        cout << "Usage: ./a.out {number of threads}";
        return 1;
    }

    int numThreads = atoi(argv[1]);

    vector<Point> points;
    Point tempPoint;
    size_t numPoints = 0;

    ifstream data("input2.txt");

    if (!data.is_open()) {
        throw std::runtime_error("File bad!");
    }

    while (!data.eof()) {
        data >> tempPoint.x >> tempPoint.y >> tempPoint.z;
        points.push_back(tempPoint);
        numPoints++;
    }

    vector<thread> threads(numThreads);
    vector<double> largestAreas(numThreads);

    size_t blockSize = numPoints / numThreads;
    size_t remainder = numPoints % numThreads;

    size_t start = 0;
    for (int i = 0; i < numThreads; ++i) {
        size_t end = start + blockSize;
        if (i < remainder) {
            ++end;
        }
        threads[i] = thread(largestTriangle, ref(points), start, end, ref(largestAreas[i]), i);

        start = end;
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    double largestArea = 0.0;

    for (int i = 0; i < numThreads; ++i) {
        if (largestAreas[i] > largestArea) {
            largestArea = largestAreas[i];
        }
    }

    cout << "Max area: " << largestArea << endl;

    auto endClock = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = endClock - startClock;

    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    return 0;
}
