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

int Solution:getDemand(){
    return demand[assignments];
}

int Solution:getNumNurses(){
    return numNurses;
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

bool Solution::validSolution(){
	//Can merge lots of loops 
	
	//(1) At least each hour h there must be at least demand_h nurses working
	for(int h=0; h<assignments; h++)
		if (demand[h]>nurses_working[h]) return false;
	
	//(2) Each nurse must work at least minHours -> trivially satisfied (otherwise postprocess)
	//(3) Each nurse must work at most maxHours
	for(int n=0; n<numNurses; n++){
		int worked_hours=0;
		int h=0;
		while(h<assignments && worked_hours <= maxHours){
			worked_hours+=works[n][h];
			h++;
		}	
		if(worked_hours>maxHours) return false;
	}
	//(4) Each nurse must work at most maxConsec hours 
	for(int n=0; n<numNurses; n++){
		bool ok=true;
		int consec=0;
		int h=0;
		while(h<assignments && ok){
			if(works[n][h]){
				consec++;
				if(consec>maxConsec) ok=false;
			}
			else consec=0;
			h++;
		}
		if(!ok) return false;
	}
	//(5) Each nurse must stay at most maxPresence
	for(int n=0; n<numNurses; n++){		//It can be done easier for sure
		if(nurse_works[n]){
			bool first=false;
			int h=0;
			while(!first && h<assignments-maxPresence){
				first=works[n][h];	
				h++;
			}
			if(first){
				for(h+=maxPresence-1; h<assignments;h++){
					if(works[n][h]) return false;
				}
			}
		}	
	}
	//(6) A nurse must rest at most one consecutive hour
	for(int n=0; n<numNurses; n++){
		if(nurse_works[n]){
			int h=0;
			int rest=0;
			bool first=false;
			while(!first && h<assignments-2){
                                first=works[n][h];
                                h++;
                        }
			if(first){
				for(;h<assignments;h++){
					if(!works[n][h]) rest++;
					else if(rest>1) return false;
					else rest=0; 
				}	
			}
		}
	}

}
