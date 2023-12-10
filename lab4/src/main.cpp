#include <dlfcn.h>

#include <GCF.hpp>
#include <Square.hpp>
#include <iostream>

using namespace std;

int main() {
    void *lib1Handle = dlopen("libGCFLib.so", RTLD_LAZY);
    void *lib2Handle = dlopen("libSquareLib.so", RTLD_LAZY);

    if (!(lib1Handle && lib2Handle)) {
        cerr << "Cannot open library" << endl;
    }

    typedef int (*IIFunc)(int, int);
    typedef float (*FFFunc)(float, float);

    dlerror();

    int switcher = 1;

    while (1) {
        cout << "0 - Switch algorithm\n1 - Find the greatest common divisor\n2 - Find the area of a figure "
                "using two sides\n-1 - Exit\nEnter command: ";
        int command;

        if (!(cin >> command)) {
            cout << endl;
            cout << "Completion of work" << endl;
            dlclose(lib1Handle);
            dlclose(lib2Handle);
            break;
        }

        const char *dlsymError;

        switch (command) {
            case 0:
                switcher = switcher == 1 ? 2 : 1;
                cout << "Algorithm mode changed" << endl;
                break;

            case 1:
                IIFunc lib1Function;
                int aInt, bInt, resultInt;

                if (switcher == 1) {
                    lib1Function = (IIFunc)dlsym(lib1Handle, "EuclideanGCF");
                } else {
                    lib1Function = (IIFunc)dlsym(lib1Handle, "NaiveGCF");
                }

                dlsymError = dlerror();

                if (dlsymError) {
                    cerr << "Can not load symbol from library" << endl;
                    dlclose(lib1Handle);
                    dlclose(lib2Handle);

                    return 1;
                }

                cout << "Enter two numbers separated by space: ";
                cin >> aInt >> bInt;
                resultInt = (*lib1Function)(aInt, bInt);
                cout << "Lib1 in mode " << switcher << " result: " << resultInt << endl;
                break;

            case 2:
                FFFunc lib2Function;
                float aFloat, bFloat, resultFloat;

                if (switcher == 1) {
                    lib2Function = (FFFunc)dlsym(lib2Handle, "RectangleSquare");
                } else {
                    lib2Function = (FFFunc)dlsym(lib2Handle, "RightTriangleSquare");
                }

                dlsymError = dlerror();

                if (dlsymError) {
                    cerr << "Can not load symbol from library" << endl;
                    dlclose(lib1Handle);
                    dlclose(lib2Handle);

                    return 1;
                }

                cout << "Enter two numbers separated by space: ";
                cin >> aFloat >> bFloat;
                resultFloat = (*lib2Function)(aFloat, bFloat);
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
