//
//  main.cpp
//  even_matrices_v2
//
//  Created by ayan on 2019/9/22.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
    ifstream fin;
    fin.open("/Users/apple/Desktop/algo lab/week1/even_matrices/testsets/test3.in");
    
    int n; //num of test cases
    fin >> n;
    
    for(int i=0; i<n; i++){
        int size; //num of int per row/col
        fin >> size;
        vector<vector<bool>> sum;
        sum.resize(size+1);
        sum[0].resize(size+1);
        int even_matrix = 0;
        
        for(int p=1; p<=size; p++){
            sum[p].resize(size+1);
            for(int q=1; q<=size; q++){
                int x;
                fin >> x;
                sum[p][q] = sum[p-1][q-1] xor sum[p][q-1] xor sum[p-1][q] xor (x==1);
            }
            for(int i2=0; i2<p; i2++){
                int even_pair = 0;
                for(int j=0; j<=size; j++){
                    even_pair += (sum[p][j] xor sum[i2][j]) ? 0 : 1;
                    //cout << "j " << j << " E " << even_pair << endl;
                }
                int odd_pair = size + 1 - even_pair;
                even_matrix += even_pair * (even_pair - 1) / 2 + odd_pair * (odd_pair - 1) / 2;
                //cout << "p " << p << " i2 " << i2 << " E " << even_pair << " O " << odd_pair << endl;
            }
        }
        //print sum
        /*cout << "sum" << endl;
        for(int k1=0; k1<=size; k1++){
            for(int k2=0; k2<=size; k2++){
                cout << sum[k1][k2] << " ";
            }
            cout << endl;
        }*/
        
        cout << even_matrix << endl;
    }
    
    return 0;
}
