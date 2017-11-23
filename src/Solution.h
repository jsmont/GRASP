#ifndef _H_SOLUTION_
#define _H_SOLUTION_

#include<iostream>
#include<vector>
using namespace std;

typedef struct {
    int numHours;
    int numNurses;
} SolutionParams;


class Solution {
  
    vector<vector<bool>> works;
    vector<bool> nurse_works;
    int assignments;
    int total_assignments;

public:

    Solution(SolutionParams parameters);
    bool isComplete();
    void addAssignment(vector<bool> new_assignment);
    void popLastAssignment();
    vector<vector<bool>> getAssignments();
    void resetAssignments();
    int getScore();
    int getGreedy();

private:
    
    void updateNurseWorks();
};

#endif
