#include "main.hpp"

int sum() { return 10 + 5; }

int div() { return 10 / 5; }

int sub() { return 10 - 5; }

int mul() { return 10 * 5; }

int myAbs() { return 10 > 0 ? 10 : -10; }

int myMod() {
    int val;

    cout << "Введите число: ";
    cin >> val;

    return val;
}