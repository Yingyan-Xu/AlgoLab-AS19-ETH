//
//  main.cpp
//  even_pairs
//
//  Created by ayan on 2019/9/22.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int nChoosek(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;
    
    int result = n;
    for(int i = 2; i <= k; ++i) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

int main(int argc, const char * argv[]) {
    fstream fin;
    fin.open("/Users/apple/Desktop/algo lab/week1/even_pairs/testsets/test3.in");
    if(!fin) {cerr << "open file error" << endl; return 1;}
    
    int n; //num of test cases;
    fin >> n;
    
    for(int i=0; i<n; i++){
        int num = 0; //num of int per seq
        fin >> num;
        vector<int> p_sum;
        int sum=0, num_even=0, num_odd=0;
        for(int j=0; j<num; j++){
            int x;
            fin >> x;
            sum += x;
            p_sum.push_back(sum);
            if(sum % 2 == 0) num_even++;
            else num_odd++;
        }
        cout << num_even + nChoosek(num_even, 2) + nChoosek(num_odd, 2) << endl;
    }
    return 0;
}
