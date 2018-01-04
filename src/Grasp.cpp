#include "Grasp.h"
#include <stdlib.h>
#include <iterator>
#include <random>

Grasp::Grasp(SolutionParams params){
	this->params=params;
}

Solution Grasp::executeGrasp(int maxiter, float alpha){ 
	Solution bestSol(params);
	int bestScore = bestSol.getNumNurses();
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
		if (sol.getScore() < bestScore){
			bestSol=sol;		
			bestScore=sol.getScore();
		}
		cout << "Current best score: " << bestScore << endl;
	}
	return bestSol;
}

void Grasp::construct(Solution &sol, float alpha){

	std::list<Candidate> C;	
	initCandidates(C,sol);

	while(!C.empty()){	
        	Candidate c;
		c=RCL(alpha,sol, C);
		sol.addAssignment(c);
		updateCandidates(C,sol);
	}
}


void Grasp::initCandidates(std::list<Candidate> &C, Solution &sol){ 
	int numNurses = sol.getNumNurses();
	int numHours = sol.getNumHours();
	Candidate c;
	for(int n=0; n<numNurses; ++n){
		c.nurse=n;
		for(int h=0; h<numHours; ++h){
			int g = sol.validCandidate(n, h);
			if(g>=0){
				c.hour=h;	
				c.greed=g;
				C.push_back(c);
			}
		}
	}
}

void Grasp::updateCandidates(std::list<Candidate> &C, Solution &sol){	
	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ){
		int g = sol.validCandidate((*it).nurse,(*it).hour);
		(*it).greed=g;
		if(g<0) it = C.erase(it); 
		else it++;
	}
}

Candidate Grasp::RCL(float alpha, Solution &sol, std::list<Candidate> &C){
	int max_greed = 0;
	int min_greed = 1000; //Use better init

	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ++it){
		if((*it).greed > max_greed) max_greed=(*it).greed;
		else if((*it).greed < min_greed) min_greed=(*it).greed;	
	}
	int threshold = (int) min_greed + alpha*(max_greed-min_greed);
	std::list<Candidate> RCL;
	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ++it){
		if((*it).greed<=threshold) RCL.push_back(*it); 
	}
	Candidate c;       
	std::random_device rd;
	int id = rd()%RCL.size();

	std::list<Candidate>::iterator it = RCL.begin();
	std::advance(it, id);

    	c = (*it);

	C.remove(c);

	RCL.clear();
        return c;
}

void Grasp::local(Solution &sol){
	bool better = true;
	while(better){
		better = findNeighbours(sol);	
		if(better) cout << "FOUND A BETTER NEIGHBOUR" << endl;
		else cout << "NO BETTER NEIGHBOUR FOUND" << endl;
	}
}

bool Grasp::findNeighbours(Solution &sol){
	Solution bestSol(params);
	bestSol.copy(sol);

	std::list<Candidate> assigned, unassigned;
	Candidate c;
	for(int n=0; n<sol.getNumNurses(); n++){ 
		c.nurse=n;
		for(int h=0; h<sol.getNumHours(); h++){
			c.hour=h;
			if(sol.getWorks(n,h))assigned.push_back(c);
			else unassigned.push_back(c);
		}
	}
	
	cout << "STARTING PERMUTATIONS";

	int perm =2;
	perm_assigned(assigned, unassigned, sol, bestSol, perm, perm);

	cout << endl;
	
//	assigned.clear();
//	unassigned.clear();	

	if(bestSol.getScore()<sol.getScore()){
		cout << "PREV SCORE: " << sol.getScore() << " CURR SCORE: " << bestSol.getScore() << endl;
		sol.copy(bestSol);
		return true;
	}
	return false;
}

void Grasp::perm_assigned(std::list<Candidate> &assigned, std::list<Candidate> &unassigned, Solution &sol, Solution &bestSol, int remain, int perm){
	if(remain > 0){
		for (std::list<Candidate>::iterator it=assigned.begin(); it != assigned.end(); ++it){
			Candidate c = *it;
			it = assigned.erase(it);
			sol.removeCandidate(c);
			perm_assigned(assigned, unassigned, sol, bestSol, remain-1, perm);
			sol.addAssignment(c);
			it = assigned.insert(it,c);
		}
	}
	else {
		cout << ".";	
		perm_unassigned(unassigned, sol, bestSol, perm); 
	}
}

void Grasp::perm_unassigned(std::list<Candidate> &unassigned, Solution &sol, Solution &bestSol, int remain){
	if(remain > 0){
               	 for (std::list<Candidate>::iterator it=unassigned.begin(); it != unassigned.end(); ++it){
                        Candidate c = *it;
                        if(sol.validCandidate(c.nurse, c.hour)>=0){
				it = unassigned.erase(it);
                        	sol.addAssignment(c);
				perm_unassigned(unassigned, sol, bestSol, remain-1);
				sol.removeCandidate(c);
				it = unassigned.insert(it,c);
			}
                }
        }
        else if(sol.getScore()<bestSol.getScore()) bestSol.copy(sol);
}
