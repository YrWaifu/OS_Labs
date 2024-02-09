#include <iostream>

#include "../GCFLib/GCF.hpp"
#include "../SquareLib/Square.hpp"

using namespace std;

int main() {
    int switcher = 1;

    while (1) {
        cout << "0 - Switch algorithm\n1 - Find the greatest common divisor\n2 - Find the area of a figure "
                "using two sides\n-1 - Exit\nEnter command: ";
        int command;

        if (!(cin >> command)) {
            cout << endl;
            cout << "Completion of work" << endl;
            break;
        }

        switch (command) {
            case 0:
                switcher = switcher == 1 ? 2 : 1;
                cout << "Algorithm mode changed" << endl;
                break;

            case 1:
                int aInt, bInt, resultInt;

                cout << "Enter two numbers separated by space: ";
                cin >> aInt >> bInt;
                resultInt = switcher == 1 ? EuclideanGCF(aInt, bInt) : NaiveGCF(aInt, bInt);
                cout << "Lib1 in mode " << switcher << " result: " << resultInt << endl;
                break;

            case 2:
                float aFloat, bFloat, resultFloat;

                cout << "Enter two numbers separated by space: ";
                cin >> aFloat >> bFloat;
                resultFloat =
                    switcher == 1 ? RectangleSquare(aFloat, bFloat) : RightTriangleSquare(aFloat, bFloat);
                cout << "Lib2 in mode " << switcher << " result: " << resultFloat << endl;
                break;

            case -1:
                cout << "Completion of work" << endl;
                return 0;
                break;

            default:
                break;
        }

        cout << "===========================================" << endl;
    }
}
