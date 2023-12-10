#include "GCF.hpp"

int NaiveGCF(int a, int b) {
    int minNum = (a < b) ? a : b;
    int gcd = 1;

    for (int i = 2; i <= minNum; ++i) {
        if (a % i == 0 && b % i == 0) {
            gcd = i;
        }
    }

    return gcd;
}

int EuclideanGCF(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}