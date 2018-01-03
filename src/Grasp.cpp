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
		//local(sol);
		cout << "Score: " << sol.getScore() << endl;
		if (sol.getScore() < bestSol.getScore()) bestSol=sol;		
		cout << "Current best score: " << bestSol.getScore() << endl;
	}
	return bestSol;
}

void Grasp::construct(Solution sol, float alpha){
	std::list<Candidate> C;	
	initCandidates(C,sol);
	int n=0;
	while(!C.empty()){
		Candidate c;
		c=RCL(alpha,sol, C);
		sol.addAssignment(c);
		updateCandidates(C,sol);
		n++;
	}
}


void Grasp::initCandidates(std::list<Candidate> &C, Solution sol){ 
	int numNurses = sol.getNumNurses();
	int numHours = sol.getNumHours();
	for(int n=0; n<numNurses; ++n){
		for(int h=0; h<numHours; ++h){
			int g = sol.validCandidate(n, h);
			if(g>0){
				Candidate c;
				c.nurse=n;
				c.hour=h;
				c.greed=g;
				C.push_back(c);
			}
		}
	}
}

void Grasp::updateCandidates(std::list<Candidate> &C, Solution sol){	
	for (std::list<Candidate>::iterator it=C.begin(); it != C.end(); ){
		int g = sol.validCandidate((*it).nurse,(*it).hour);
		(*it).greed=g;
		if(g<0) it = C.erase(it);
		else it++;
	}
}

/*void Grasp::generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos, int h, Solution sol){	
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
}*/

Candidate Grasp::RCL(float alpha, Solution sol, std::list<Candidate> &C){
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
	int id = rand()%RCL.size();
	std::list<Candidate>::iterator it = RCL.begin();
        std::advance(it, id);
        c = (*it);
	C.erase(it);
	RCL.clear();
        return c;
}

/*void Grasp::local(Solution sol){
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
