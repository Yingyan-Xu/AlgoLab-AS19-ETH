//
//  main.cpp
//  dominoes
//
//  Created by ayan on 2019/9/22.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int main(int argc, const char * argv[]) {
    fstream fin;
    fin.open("/Users/apple/Desktop/algo lab/week1/dominoes/testsets/test2.in");
    if(!fin) {cerr << "open file error" << endl; return 1;}
    
    int n; //num of test cases;
    fin >> n;
    
    for(int i=0; i<n; i++){
        int n_tile; //num of tiles
        fin >> n_tile;
        int max_num = 1;
        for(int j=0, h; j<n_tile; j++){
            if(j >= max_num){
                string tmp;
                getline(fin, tmp);
                break;
            }
            
            fin >> h; //height of cur tile
            max_num = min(max(max_num, j+h), n_tile);
        }
        cout << max_num << endl;
    }
    
    return 0;
}
