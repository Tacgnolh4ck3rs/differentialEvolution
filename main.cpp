#include <iostream>
#include <string>
#include <vector>
#include "diffevoproblem.hpp"

using namespace std;

int main(int argc, char **argv) {
    DiffEvoProblem problem(100, {{-40.0, 40.0}, {-40.0, 40.0}});
    vector<double> result = problem.solve();
    cout << "The result of the problem is equal to: ";
    printVector(result);
    cout << endl;
    return 0;
}
