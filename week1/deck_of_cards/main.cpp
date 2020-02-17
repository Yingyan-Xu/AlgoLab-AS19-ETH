//
//  main.cpp
//  deck_of_cards
//
//  Created by ayan on 2019/9/23.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

int main()
{
    fstream fin;
    fin.open("/Users/apple/Desktop/algo lab/week1/pow/testsets/test3.in");
    if(!fin) {cerr << "open file error" << endl; return 1;}
    
    int n_test; //num of test cases
    fin >> n_test;

    for(int i=0; i<n_test; i++){
        int n_card = 0, target_num = 0;
        fin >> n_card >> target_num;

        //load the seq and compute sum
        vector<int> card;
        for(int j=0, x; j<n_card; j++){
            fin >> x;
            card.push_back(x);
        }
        
        //find the idx
        int cur_sum = card[0], left = 0, right = 0;
        int final_left = left, final_right = right;
        int min_diff = cur_sum > target_num ? cur_sum - target_num : target_num - cur_sum;
        
        while(cur_sum != target_num && left <= right && right < n_card-1){
            if(cur_sum < target_num){
                right++;
                cur_sum += card[right];
            }
            else if(cur_sum > target_num){
                cur_sum -= card[left];
                left++;
                if(left > right) {right++; cur_sum += card[right];}
            }
            int diff = cur_sum > target_num ? cur_sum - target_num : target_num - cur_sum;
            if(diff < min_diff){
                min_diff = diff;
                final_left = left;
                final_right = right;
            }
            //cout << "left " << left << " right " << right << " cur sum " << cur_sum << endl;
            //cout << " min diff " << min_diff << endl;
        }
        cout << final_left << " " << final_right << endl;
    }

    return 0;
}
