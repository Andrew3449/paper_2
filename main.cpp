#include <bits/stdc++.h>
#include <chrono>
#include <random>

using u32    = uint_least32_t;
using engine = std::mt19937;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using namespace std;

bool LEFT_TO_RIGHT = true;
bool RIGHT_TO_LEFT = false;
vector<int> sums;
/*
 --FUNCTIONS FOR JOHNSON-TROTTER ALGORITHM--
*/
// function finds the position of largest mobile integer in a[]
int search(int a[], int n, int m)
{
    for (int i = 0; i < n; i++){
        if (a[i] == m)
            return i + 1;
    }
    return -1;
}

// function finds the largest integer
int getMobile(int a[], bool d[], int n)
{
    int prev = 0, mobile = 0;
    for (int i = 0; i < n; i++) {
        // 0 = right to left, 1 = left to right
        if (d[a[i] - 1] == RIGHT_TO_LEFT && i >= 0) {
            if (a[i] > a[i - 1] && a[i] > prev) {
                mobile = a[i];
                prev = mobile;
            }
        }
        if (d[a[i] - 1] == LEFT_TO_RIGHT && i != n - 1) {
            if (a[i] > a[i + 1] && a[i] > prev) {
                mobile = a[i];
                prev = mobile;
            }
        }
    }

    if (mobile == 0 && prev == 0)
        return 0;

    return mobile;
}

// Prints a single permutation
void printOnePerm(int a[], bool dir[], int n)
{
    int mobile = getMobile(a, dir, n);
    int pos = search(a, n, mobile);

    // swapping the elements according to the
    // direction i.e. dir[].
    if (dir[a[pos - 1] - 1] == RIGHT_TO_LEFT)
        swap(a[pos - 1], a[pos - 2]);

    else if (dir[a[pos - 1] - 1] == LEFT_TO_RIGHT)
        swap(a[pos], a[pos - 1]);

    // changing directions for elements
    for (int i = 0; i < n; i++) {
        if (a[i] > mobile) {
            if (dir[a[i] - 1] == LEFT_TO_RIGHT)
                dir[a[i] - 1] = RIGHT_TO_LEFT;
            else if (dir[a[i] - 1] == RIGHT_TO_LEFT)
                dir[a[i] - 1] = LEFT_TO_RIGHT;
        }
    }

    for (int i = 0; i < n; i++)
        cout << a[i];
    cout << " ";
}

// function generates a factorial since that's how many permutations there are
int factorial(int n)
{
    int res = 1;
    for (int i = 1; i <= n; i++)
        res = res * i;
    return res;
}

// function calls printOnePerm() factorial(n) times
void printPermutation(int n)
{
    int a[n]; // array of current permutations
    bool dir[n]; // array of current directions

    // store the elements from 1 to n and printing first permutation
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
        cout << a[i];
    }
    cout << endl;

    // initially set all directions to 0
    for (int i = 0; i < n; i++)
        dir[i] = RIGHT_TO_LEFT;

    // generate all permutations
    for (int i = 1; i < factorial(n); i++)
        printOnePerm(a, dir, n);
}

/*
 --FUNCTIONS FOR LEXICOGRAPHIC ALGORITHM--
*/
// function reverses part of the array
void reverse(int a[], int s, int e){
    while (s < e){
        swap(a[s], a[e]);
        s++;
        e--;
    }
}

int generateLexPerm(int p[], int n){
    int i = n - 2;
    // find highest value of i where p[i] < p[i+1]
    while (i >= 0 && p[i] >= p[i + 1]) {
        i--;
    }
    // if no element smaller than the next was found, return
    if (i < 0) {
        return -1;
    }

    // find highest value of j where p[i] > p[j]
    int j = n - 1;
    while (j > i && p[j] <= p[i]) {
        j--;
    }

    // swap the elements
    swap(p[i], p[j]);
    reverse(p, i+1, n-1);
    return 0;
}

void lexPerm(int n){
    int perms[n];
    // initialize the first permutation
    for (int i = 1; i <= n; i++) {
        perms[i - 1] = i;
        cout << perms[i - 1];
    }
    cout << endl;
    while (generateLexPerm(perms, n) != -1){
        for (int i = 0; i < n; i++){
            cout << perms[i];
        }
        cout << " ";
    }
}

/*
 --FUNCTIONS FOR BINARY REFLECTED GRAY CODE (COMBINATIONS)--
*/
void generate(int n, int x, int index, bool r){
    if ((x > n) || (x < 0))
        return;
    if (n == 0){
        //for(int i = 0; i < index; i++){
            // if (sums[i])
                // cout << i+1;
        //}
        // cout << endl;
        return;
    }
    sums[index] = r;
    generate(n-1, x-r, index+1, false);
    sums[index] = !r;
    generate(n-1, x -!r, index+1, true);
}
void combinations(int n, int k){
    sums.resize(n);
    generate(n, k, 0, false);
}


int main()
{
    int n = 3; // n starts at 1, ends at 8 (you can try 9, would not recommend further)
    int m = 20, i = m/2; // for combinations only, m starts at 20, i is m/2
    cout << "----PART A----\n\n";
    cout << "Lexicographic" << endl;
    auto t1 = high_resolution_clock::now();
    lexPerm(n);
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> time1 = t2 - t1;
    cout << "\nTime Taken: " << time1.count() << " ms\n" << endl;

    cout << "Johnson-Trotter" << endl;
    auto t3 = high_resolution_clock::now();
    printPermutation(n);
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> time2 = t4 - t3;
    cout << "\nTime Taken: " << time2.count() << " ms\n" << endl;

    cout << "----PART B----\n\n";
    cout << "Binary Reflected Gray Codes" << endl;
    auto t5 = high_resolution_clock::now();
    combinations(m, i);
    auto t6 = high_resolution_clock::now();

    for (int sum : sums)
        cout << sum << " ";

    duration<double, std::milli> time3 = t6 - t5;
    cout << "\nTime Taken: " << time3.count() << " ms\n" << endl;

    return 0;
}