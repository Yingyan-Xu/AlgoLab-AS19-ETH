//
//  main.cpp
//  build_the_sum
//
//  Created by ayan on 2019/9/22.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, const char * argv[]) {
    fstream fin;
    fin.open("/Users/apple/Desktop/algo lab/week1/build_the_sum/testsets/test1.in");
    if(!fin) {cerr << "open file error" << endl; return 1;}
    
    int n; //num of test cases;
    fin >> n;
    
    for(int i=0; i<n; i++){
        int sum = 0;
        int num_of_int = 0;
        fin >> num_of_int;
        for(int j=0; j<num_of_int; j++){
            int x = 0;
            fin >> x;
            sum += x;
        }
        cout << sum << endl;
    }
    return 0;
}
