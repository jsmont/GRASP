#include "Solution.h"

Solution::Solution(SolutionParams parameters){
    this->total_assignments=parameters.numHours;
    this->works = vector<vector<bool>>(parameters.numHours,vector<bool>(parameters.numNurses,false));
    this->nurse_works = vector<bool>(parameters.numNurses, false);
    this->demand = vector<int>(parameters.numHours);
    demand = parameters.demand; //Can do it in a single expression?
    this->nurses_working = vector<int>(parameters.numHours, 0);
    this->assignments = 0;
    this->score=parameters.numNurses;
}

bool Solution::isComplete(){
    return assignments == total_assignments;
}

void Solution::addAssignment(vector<bool> new_assignment){
    works[assignments] = new_assignment;
    updateNursesWorking(assignment);
    assignments++;
    updateNurseWorks();
}

void Solution::popLastAssignment(){
    assignments--;
    nurses_working[assignment]=0;
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

int Solution::getGreedy(){ //obviously will have to tune and stuff
    int diff;

    for (h=0; h<assignments; h++){
	if(nurses_working[h]<demand[h]) diff += 50*(demand[h]-nurses_working[h]);
	else diff += 5*(nurses_working[h] - demand[h]);
    }
    return diff-score;
}

void Solution::updateNurseWorks(){ //do we really need to check all hours? better just the assigned / unassigned one (only check previous if unassigning and under some conditions) 
    int workingNurses=0;
    for(int n = 0; n < nurse_works.size(); ++n){
        bool nworks = false;
        for(int h = 0; h < assignments && !nworks; ++h){
            nworks |= works[h][n];
        }
        nurse_works[n] = nworks;
	workingNurses+=nurse_works[n];
    }
}

//Maybe can merge with previous function
void Solution::updateNursesWorking(int h){
	for (int n = 0; n < nurse_works.size(); ++n){
		nurses_working[h] += works[h][n];
	}
}

void Solution::
