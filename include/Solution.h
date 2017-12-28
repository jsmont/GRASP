#ifndef _H_SOLUTION_
#define _H_SOLUTION_

#include<iostream>
#include<vector>
using namespace std;

typedef struct {
    int numHours;
    int numNurses;
    vector<int> demand;
} SolutionParams;


class Solution {
  
    vector<vector<bool>> works;
    vector<bool> nurse_works;
    vector<int> demand;
    vector<int> nurses_working;
    int assignments;
    int total_assignments;
    int score;

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
    void updateNursesWorking(int h)
};

#endif
