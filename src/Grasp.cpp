#include "Grasp.h"

Grasp::Grasp(){

}

void Grasp::executeGrasp(int maxiter, float alpha){

	Solution bestSol;

	for(int k = 0; k < maxiter; k++){
		Solution sol;
		construct(sol,alpha);
		local();
		if (evaluate(sol) < evaluate(bestSol)) bestSol=sol; //Can keep value for bestSol
	}
}

void Grasp::construct(Solution sol, float alpha){

	std::list<Candidate> C;	


	initCandidates(C);
	while(!sol.isComplete()){	
		RCL(alpha, C);				
		sol.addAssignment(selectRCL(C));
		updateCandidates(C);
	}
}


void initCandidates(std::list<Candidate> &C){

}

void updateCandidates(std::list<Candidate> &C){

}

void Grasp::RCL(float alpha, std::list<Candidate> &C){
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
}

Candidate Grasp::selectRCL(std::list<Candidate> &C){
	Candidate *c;
	int id = std::rand()%C.size();
        c = *C[id];
        C.clear();
        return c;
}

void Grasp::local(){
	

}
