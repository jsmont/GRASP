#ifndef _H_SOLUTION_
#define _H_SOLUTION_

#include<iostream>
#include<vector>
using namespace std;

typedef struct {
    int numHours;
    int numNurses;
    vector<int> demand;
    int maxHours;
    int maxConsec;
    int maxPresence;
} SolutionParams;


class Solution {
  
    vector<vector<bool>> works;
    vector<bool> nurse_works;
    vector<int> demand;
    vector<int> nurses_working;
    int assignments;
    int total_assignments;
    int score;
    int numNurses;
    int maxHours;
    int maxConsec;
    int maxPresence;

public:

    Solution(SolutionParams parameters);
    bool isComplete();
    void addAssignment(vector<bool> new_assignment);
    void popLastAssignment();
    void addAssignment(vector<bool new_assignment, int h)
    vector<vector<bool>> getAssignments();
    void resetAssignments();
    int getScore();
    int getGreedy();
    int getDemand();
    int getNumNurses();
    int getNumHours();
    vector<bool> getNurseWorks()
    bool validSolution();
    bool validSolution(int n);
    bool validCandidate(int n, int h)

private:
    
    void updateNurseWorks();
    void updateNursesWorking(int h)
};

#endif
