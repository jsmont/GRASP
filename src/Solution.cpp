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

vector<vector<bool> > Solution::getAssignments(){
    return works;
}

void Solution::reset(){
	for(int n=0; n<numNurses; n++){
		for(int h=0;h<numHours;h++){
			works[n][h]=false;
			score=0;
			nurses_working[h]=0;
		}
		nurse_works[n]=false;
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
	return score + !nurse_works[n];
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


void Solution::removeCandidate(Candidate c){
	works[c.nurse][c.hour]=false;
	updateNurseWorks(c.nurse);
	nurses_working[c.hour]--;
}

void Solution::updateNurseWorks(int n){
	nurse_works[n]=false;	
	for(int h=0; h<numHours; h++) nurse_works[n] = works[n][h] || nurse_works[n];
}

void Solution::copy(Solution &sol){
	reset();
	works=sol.getAssignments();
	score=sol.getScore();
	nurse_works = sol.getNurseWorks();
	nurses_working = sol.getNursesWorking();	
}

vector<int> Solution::getNursesWorking(){
	return nurses_working;
}
