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

typedef struct Candidate{
	int nurse;
        int hour;
	int greed;
    bool operator==(const Candidate& c) const {
        return (c.nurse == nurse) && (c.hour == hour);
    }
} Candidate;


class Solution {
  
    vector<vector<bool> > works;
    vector<bool> nurse_works;
    vector<int> demand;
    vector<int> nurses_working;
    //int assignments;
    int numHours;
    int score;
    int numNurses;
    int maxHours;
    int maxConsec;
    int maxPresence;

public:

    Solution(SolutionParams parameters);
    bool isComplete();
    //void addAssignment(vector<bool> new_assignment);
    //void popLastAssignment();
    void addAssignment(Candidate c);
    vector<vector<bool> > getAssignments();
    void reset();
    int getScore();
    int getGreedy(int n, int h);
    int getDemand(int h);
    int getNumNurses();
    int getNumHours();
    //int getAssignedHours();
    vector<bool> getNurseWorks();
    bool validSolution();
    bool validSolution(int n);
    int validCandidate(int n, int h);

private:
    
    //void updateNurseWorks();
    //void updateNursesWorking(int h);
};

#endif
