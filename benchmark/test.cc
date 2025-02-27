#include <iostream>
#include <vector> 
#include "../hive.hpp"

using namespace std;

int main() {
    vector<int> a;
    a.push_back(12);

    for(int i =0 ;i < 100;i++) {
        print(a.capacity());
        a.push_back(i);
    }

    return 0;
}