#include "Solution.h"

Solution::Solution(SolutionParams parameters){
    this->total_assignments=parameters.numHours;
    this->works = vector<vector<bool>>(parameters.numHours,vector<bool>(parameters.numNurses,false));
    this->nurse_works = vector<bool>(parameters.numNurses, false);
    this->assignments = 0;
    this->score=parameters.numNurses;
}

bool Solution::isComplete(){
    return assignments == total_assignments;
}

void Solution::addAssignment(vector<bool> new_assignment){
    works[assignments] = new_assignment;
    assignments++;
    updateNurseWorks();
}

void Solution::popLastAssignment(){
    assignments--;
    updateNurseWorks(); //Otherwise getScore() is incorrect
}

vector<vector<bool>> Solution::getAssignments(){
    return works;
}

void Solution::resetAssignments(){
    assignments = 0;
}

int Solution::getScore(){
    /*int workingNurses = 0;
    for(int n = 0; n < nurse_works.size(); ++n){
        workingNurses+=nurse_works[n];
    }
    return workingNurses;*/ 
    return score;
}

int Solution::getGreedy(){
    return getScore();
}

void Solution::updateNurseWorks(){
    int worlkingNurses=0;
    for(int n = 0; n < nurse_works.size(); ++n){
        bool nworks = false;
        for(int h = 0; h < assignments && !nworks; ++h){
            nworks |= works[h][n];
        }
        nurse_works[n] = nworks;
	workingNurses+=nurse_works[n];
    }
}
