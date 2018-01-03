#include "Solution.h"

Solution::Solution(SolutionParams parameters){
    this->numHours=parameters.numHours;
    this->works = vector<vector<bool> >(parameters.numNurses,vector<bool>(parameters.numHours,false));
    this->nurse_works = vector<bool>(parameters.numNurses, false);
    this->demand = vector<int>(parameters.numHours);
    demand = parameters.demand; //Can do it in a single expression?
    this->nurses_working = vector<int>(parameters.numHours, 0);
    this->score = 0;
    this->numNurses = parameters.numNurses;
    this->maxHours = parameters.maxHours;
    this->maxConsec = parameters.maxConsec;
    this->maxPresence = parameters.maxPresence;
}

bool Solution::isComplete(){
	for(int h=0; h<numHours; h++){
		if(nurses_working[h]<demand[h]) return false; //Sure?
	}
	return true;
}

void Solution::addAssignment(Candidate c){
    works[c.nurse][c.hour] = true;
    nurses_working[c.hour]++;
    if(!nurse_works[c.nurse]) score++;
    nurse_works[c.nurse]=true;
}

/*void Solution::popLastAssignment(){
    assignments--;
    nurses_working[assignments]=0;
    updateNurseWorks(); //Otherwise getScore() is incorrect
}*/

/*void Solution::addAssignment(vector<bool> new_assignment, int h){ //Overwrites assignment h, use only in local search
    works[h] = new_assignment;
    updateNursesWorking(assignments);
    updateNurseWorks();
}*/

vector<vector<bool> > Solution::getAssignments(){
    return works;
}

void Solution::reset(){
	for(int n=0; n<numNurses; n++){
		for(int h=0;h<numHours;h++){
			works[n][h]=false;
			score=0;
		}
	}
}

int Solution::getScore(){
    	/*score=0;
	for(int n=0;n<numNurses;n++){
		score += nurse_works[n];			
	}*/
	return score;
}

int Solution::getGreedy(int n, int h){ //obviously will have to tune and stuff
	return score=10*(nurses_working[h]+1>demand[h])+(score+!nurse_works[n]);
}

int Solution::getDemand(int h){
    return demand[h];
}

int Solution::getNumNurses(){
    return numNurses;
} 

int Solution::getNumHours(){
    return numHours;
}

vector<bool> Solution::getNurseWorks(){
    return nurse_works;
}

/*int Solution::getAssignedHours(){
	return assignments;
}*/

/*void Solution::updateNurseWorks(){ //do we really need to check all hours? better just the assigned / unassigned one (only check previous if unassigning and under some conditions) 
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
	}
}*/

bool Solution::validSolution(){

	//should also work for partial solutions
	bool ok=true;
	int n=0;
	//(1) At least each hour h there must be at least demand_h nurses working
	if(isComplete()){
		for(int h=0; h<numHours; h++)
			if (demand[h]>nurses_working[h]) return false;
	}
	
	//(2) Each nurse must work at least minHours -> trivially satisfied (otherwise postprocess)
	//CARE! NOT NECESSARILY!


	//(3) Each nurse must work at most maxHours
	//(4) Each nurse must work at most maxConsec hours 
	//(5) Each nurse must stay at most maxPresence
	//(6) A nurse must rest at most one consecutive hour
	while(n<numNurses && ok){
		ok&=validSolution(n);	
		n++;
	}
	return ok;
}
bool Solution::validSolution(int n){
	if(nurse_works[n]){
		int worked_hours=0;
		int h=0;
		int consec=0;
		int first=-numHours;	
		bool presence=false;
		int rest=0;
		while(h<numHours && worked_hours<=maxHours && consec<=maxConsec && !presence){
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
int Solution::validCandidate(int n, int h){
	if(works[n][h]) return -1;
	if(demand[h]<=nurses_working[h]) return -1;
	works[n][h] = true;
	bool valid = validSolution(n);
	works[n][h] = false;
	if(valid) return getGreedy(n,h);
	return -1;
}
