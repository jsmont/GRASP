#include "Solution.h"

Solution::Solution(SolutionParams parameters){
    this->total_assignments=parameters.numHours;
    this->works = vector<vector<bool>>(parameters.numHours,vector<bool>(parameters.numNurses,false));
    this->nurse_works = vector<bool>(parameters.numNurses, false);
    this->demand = vector<int>(parameters.numHours);
    demand = parameters.demand; //Can do it in a single expression?
    this->nurses_working = vector<int>(parameters.numHours, 0);
    this->assignments = 0;
    this->score = 0;
    this->numNurses = parameters.numNurses;
    this->maxHours = parameters.maxHours;
    this->maxConsec = parameters.maxConsec;
    this->maxPresence = parameters.maxPresence;
}

bool Solution::isComplete(){
    return assignments == total_assignments;
}

void Solution::addAssignment(vector<bool> new_assignment){
    works[assignments] = new_assignment;
    updateNursesWorking(assignment);
    assignments++;
    updateNurseWorks();
    updateScore();
}

void Solution::popLastAssignment(){
    assignments--;
    nurses_working[assignment]=0;
    updateNurseWorks(); //Otherwise getScore() is incorrect
    updateScore(); 
}

void Solution:addAssignment(vector<bool new_assignment, int h){ //Overwrites assignment h, use only in local search
    works[h] = new_assignment;
    updateNursesWorking(assignment);
    updateNurseWorks();
    updateScore();
}

vector<vector<bool>> Solution::getAssignments(){
    return works;
}

void Solution::resetAssignments(){
    assignments = 0;
}

int Solution::getScore(){
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

int Solution:getDemand(){
    return demand[assignments];
}

int Solution:getNumNurses(){
    return numNurses;
} 

int Solution:getNumHours(){
    return total_assignments;
}

vector<bool> Solution:getNurseWorks(){
    return nurse_works;
}

int Solution::getAssignments(){
	return assignments;
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
    score=workingNurses;
}

//Maybe can merge with previous function
void Solution::updateNursesWorking(int h){
	for (int n = 0; n < nurse_works.size(); ++n){
		nurses_working[h] += works[h][n];
		if(nurses_working[h]>score) score=nurses_working[h];
	}
}

bool Solution::validSolution(){

	//should also work for partial solutions
	
	//(1) At least each hour h there must be at least demand_h nurses working
	if(isComplete(){
		for(int h=0; h<assignments; h++)
			if (demand[h]>nurses_working[h]) return false;
	}
	
	//(2) Each nurse must work at least minHours -> trivially satisfied (otherwise postprocess)
	//CARE! NOT NECESSARILY!


	//(3) Each nurse must work at most maxHours
	//(4) Each nurse must work at most maxConsec hours 
	//(5) Each nurse must stay at most maxPresence
	//(6) A nurse must rest at most one consecutive hour

	for(int n=0; n<numNurses; n++){
		validSolution(n);	
	}
	return true;
}
bool Solution::validSolution(int n){
	if(nurse_works[n]){
		int worked_hours=0;
		int h=0;
		int consec=0;
		int first=-assignments;	
		bool presence=false;
		int rest=0;
		while(h<assignments && worked_hours<=maxHours && consec<=maxConsec && !presence){
			if(works[n][h]){
				if(first<0) first=h;
				else presence=(h-first>=maxPresence);
				worked_hours++;
				consec++;
				if(rest>1) return false;
				rest=0;
			}
			else{
				consec=0;
				if(first>0) rest++;
			}
			h++;
		}	
		if(worked_hours>maxHours||consec>maxConsec||presence) return false;
	}	
	return true;
}
bool Solution::validCandidate(int n, int h){
	bool tmp = works[n][h];
	works[n][h] = true;
	bool valid = validSolution(n);
	works[n][h] = tmp;
	return valid;
}
