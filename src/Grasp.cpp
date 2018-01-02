#include "Grasp.h"

Grasp::Grasp(){
}

Solution Grasp::executeGrasp(int maxiter, float alpha){ 
	Solution bestSol;

	for(int k = 0; k < maxiter; k++){
		Solution sol;
		construct(sol,alpha);
		local();
		if (sol.getScore() < bestSol.getScore()) bestSol=sol;
		//Added a Score field for Solution class to avoid to compute it again
	}
	return bestSol;
}

void Grasp::construct(Solution sol, float alpha){
	std::list<Candidate> C;	
	initCandidates(C,sol);
	while(!sol.isComplete()){			
		sol.addAssignment(RCL(alpha,sol, C));
		updateCandidates(C,sol);
	}
}


void Grasp::initCandidates(std::list<Candidate> &C, Solution sol){ //Since we are doing incrementally by hours, first hour is trivial. Could add in other order
	C.clear(); //just in case
	int demand = sol.getDemand(); //probably not necessary to get them from solution
	int numNurses = sol.getNumNurses();
	Candidate c(numNurses, false);
	for(int i=0; i<demand;i++) c[i]=true;
	C.push_back(c);
}

void Grasp::updateCandidates(std::list<Candidate> &C, Solution sol){	
	C.clear();
	int demand = sol.getDemand();
        int numNurses = sol.getNumNurses();
        Candidate c(numNurses, false);
	generateCandidates(C, c, demand, 0);
}

void Grasp::generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos){	
	if(remaining==0) C.push_back(c);
	else {
		if(C.size()-pos == remaining){	//Can only set trues
			c[pos]=true;
			generateCandidate(C, c, remaining-1, pos+1);
		}	
		else {
			c[pos]=true;
			generateCandidate(C, c, remaining-1, pos+1);
			c[pos]=false;
			generateCandidate(C, c, remaining, pos+1);
		}
	}		
}

Candidate Grasp::RCL(float alpha, Solution sol, std::list<Candidate> &C){
	int max_greed = 0;
	int min_greed = 1000; //Use better init

	std::vector<int> greed (C.size(), 0);	
	for (std::list<int>::iterator it=C.begin(), int i = 0; it != C.end(); ++it, ++i){
		sol.addAssignment(*it);
		greed[i]=sol.getGreedy();
		if(greed[i] > max_greed) max_greed=greed[i];
		else if(greed[i] < min_greed) min_greed=greed[i];
		sol.popLastAssignement();
	}
	int threshold = min_greed + alpha*(max_greed-min_greed);
	for (std::list<int>::iterator it=C.begin(), int i=0; it != C.end(); ++it, ++i)
		if(greed[i]>threshold) C.erase(it); //Check Correctness
	
	Candidate *c;
        int id = std::rand()%C.size();
        c = *C[id];
        C.clear();
        return c;

}

void Grasp::local(Solution sol){
	std::list<Solution> neighbours;
	bool better = true;
	int score, id;
	while(better){
		score = sol.getScore();
		better=false;
		findNeighbours(sol, neighbours);
		for (std::list<int>::iterator it=neighbours.begin(), int i = 0; it != neighbours.end(); ++it, ++i){
                	if((*it).getScore() < score){
				score = (*it).getScore();
				id = i;
				better=true;			
			}			
		}
		if (better) sol = neighbours(id);
	}
}

void Grasp::findNeighbours(Solution sol, std::list<Solution> neighbours){
	generateNeighbours(sol, neighbours, 2, 0);
}

void Grasp::generateNeighbours(Solution sol, std::list<Solution> neighbours, int remaining, int h){
        if(remaining!=0) {
                if(sol.getNumHours()-pos == remaining){  //Can only set trues
               		generateNeighbours(sol, neighbours, h);
                        generateNeighbours(sol, neighbours,remaining-1, h+1);
                }
                else {
                        generateNeighbours(sol, neighbours, h);
                        generateNeighbours(sol, neighbours,remaining-1, h+1);
			generateNeighbours(sol, neighbours,remaining-1, h+1);

                }
        }
}

void Grasp::generateNeighbours(Solution sol, std::list<Solution> neighbours, int h){



}	
