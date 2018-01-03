#include "Grasp.h"
#include <stdlib.h>

Grasp::Grasp(SolutionParams params){
	this->params=params;
}

Solution Grasp::executeGrasp(int maxiter, float alpha){ 
	Solution bestSol(params);
	cout << "STARTING GRASP" << endl;
	cout << "ITERATIONS: " << maxiter << endl;
	cout << "ALPHA: " << alpha << endl;
	for(int k = 0; k < maxiter; k++){
		Solution sol(params);	
		cout << "Construction Phase" << endl;
		construct(sol,alpha);
		cout << "Local Search" << endl;
		local(sol);
		cout << "Score: " << sol.getScore() << endl;
		if (sol.getScore() < bestSol.getScore()) bestSol=sol;		
		cout << "Current best score: " << bestSol.getScore() << endl;
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
	int demand = sol.getDemand(0); //probably not necessary to get them from solution
	int numNurses = sol.getNumNurses();
	Candidate c(numNurses, false);
	for(int i=0; i<demand;i++) c[i]=true;
	C.push_back(c);
}

void Grasp::updateCandidates(std::list<Candidate> &C, Solution sol){	
	C.clear();
	int demand = sol.getDemand(sol.getAssignedHours()); 
	cout << "ASSIGNED HOURS: " << sol.getAssignedHours() << endl;
	cout << "DEMAND: " << demand << endl;
	int numNurses = sol.getNumNurses();
        Candidate c(numNurses, false);
	generateCandidates(C, c, demand, 0, sol.getAssignedHours(), sol);
}

void Grasp::generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos, int h, Solution sol){	
	cout << "POSITION " << pos << endl;
	if(remaining==0) C.push_back(c);
	else {
		if(c.size()-pos == remaining){	//Can only set trues
			if(sol.validCandidate(pos, h)) {
				c[pos]=true;
				generateCandidates(C, c, remaining-1, pos+1, h, sol);
			}
		}
		else {
			if(sol.validCandidate(pos, h)) { 
				c[pos]=true;
				generateCandidates(C, c, remaining-1, pos+1,h, sol);
			}
			c[pos]=false;
			generateCandidates(C, c, remaining, pos+1,h, sol);
		}
	}		
}

std::vector<bool> Grasp::RCL(float alpha, Solution sol, std::list<Candidate> &C){
	int max_greed = 0;
	int min_greed = 1000; //Use better init

	std::vector<int> greed (C.size(), 0);
	int i=0;	
	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ++it, ++i){
		sol.addAssignment(*it);
		greed[i]=sol.getGreedy();
		if(greed[i] > max_greed) max_greed=greed[i];
		else if(greed[i] < min_greed) min_greed=greed[i];	
		sol.popLastAssignment();
	}
	int threshold = min_greed + alpha*(max_greed-min_greed);
	i=0;
	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ++it, ++i)
		if(greed[i]>threshold) C.erase(it); //Check Correctness
	
	Candidate c;
        int id = rand()%C.size();
	std::list<Candidate>::iterator it = C.begin();
        std::advance(it, id);
        c = (*it);
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
		int i=0;
		for (std::list<Solution>::iterator it=neighbours.begin(); it != neighbours.end(); ++it, ++i){
                	if((*it).getScore() < score){
				score = (*it).getScore();
				id = i;
				better=true;			
			}			
		}
		if (better){
			std::list<Solution>::iterator it = neighbours.begin();
			std::advance(it, id);
		 	sol = (*it);
		}
	}	
}

void Grasp::findNeighbours(Solution sol, std::list<Solution> neighbours){
	generateNeighbours(sol, neighbours, -1, -1, 0);
}

void Grasp::generateNeighbours(Solution sol, std::list<Solution> neighbours, int h1, int h2, int h){
        if(h2<0 && h < sol.getNumHours()) {
		if(h1<0) generateNeighbours(sol, neighbours, h, h2, h+1); //fill h1
		else generateNeighbours(sol, neighbours, h1, h, h+1);	//fill h2
		generateNeighbours(sol, neighbours, h1, h2, h+1);	//fill none
        }
	else generateNeighbours(sol, neighbours, h1, h2);
}

void Grasp::generateNeighbours(Solution sol, std::list<Solution> neighbours, int h1, int h2){
	std:list<Candidate> C1, C2;
	Candidate c;
        int demand1 = sol.getDemand(h1);
	int demand2 = sol.getDemand(h2);
        int numNurses = sol.getNumNurses();
//	std::vector<bool> nurse_works(numNurses);
//	nurse_works=sol.getNurseWorks();
        Candidate c1(numNurses, false);
	Candidate c2(numNurses, false);
        generateCandidates(C1, c1, demand1, 0, h1, sol);
	generateCandidates(C2, c2, demand2, 0 ,h2, sol);
 
	for (std::list<Candidate>::iterator it1=C1.begin(); it1 != C1.end(); ++it1){
 		for (std::list<Candidate>::iterator it2=C1.begin(); it2 != C2.end(); ++it2){ 
			sol.addAssignment(*it1,h1);
			sol.addAssignment(*it2,h2);
			if(sol.validSolution())neighbours.push_back(sol);
		}
        }	
	
}	

/*void Grasp::generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int posstd::vector<bool> nurse_works){
        if(remaining==0) C.push_back(c);
        else {
                if(C.size()-pos == remaining && nurse_works[pos]){ 
                        c[pos]=true;
                        generateCandidate(C, c, remaining-1, pos+1);
                }
                else {
                        if(nurse_works[pos]{
				c[pos]=true;
                        	generateCandidate(C, c, remaining-1, pos+1);
                        }
			c[pos]=false;
                        generateCandidate(C, c, remaining, pos+1);
                }
        }
}*/
