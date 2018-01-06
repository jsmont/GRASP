#include "Grasp.h"

int main(int argc, char* argv[]){

	cout << "READING INPUT DATA" << endl;
		
	int nNurses, nHours;
	cin >> nNurses >> nHours;

	vector<int> demand = vector<int>(nHours);
	for(int i = 0; i < demand.size(); ++i) cin >> demand[i];
	int minHours, maxHours, maxConsec, maxPresence;
	cin >> minHours >> maxHours >> maxConsec >> maxPresence;
	
	SolutionParams solparams;
	solparams.numHours=nHours;
	solparams.numNurses=nNurses;
	solparams.demand=demand;
	solparams.maxHours=maxHours;
	solparams.maxConsec=maxConsec;
	solparams.maxPresence=maxPresence;

	cout << "INPUT DATA READ" << endl;

	int iterations=5;
	float alpha=0.2;
	int score, bestScore;
	//int patience = 2;	
        Solution sol(solparams);
	Solution bestSol(solparams);


	cout << "GRASP INIT" << endl;

	Grasp g(solparams); //do a better iterating system, changing alpha and stuff
	//int best=g.getBest();
	bestSol=g.executeGrasp(iterations,alpha); 
	bestScore=bestSol.getScore();

	/*while(patience > 0){

		cout << " -------------------------------------- " << endl;
		cout << " ------------ RUNNING GRASP ----------- " << endl;
		cout << " -------------------------------------- " << endl;

		sol=g.executeGrasp(iterations,alpha);
	        score=sol.getScore();	
		if(score<bestScore){
			bestScore=score;
			bestSol.copy(sol);
			patience++;
		}
		else{
			patience --;
			if(alpha<1) alpha+=0.1;
		}
		sol.reset();
		if(iterations>2) iterations--; //minimum 2 iterations
	}*/

    	
    cout << endl << endl;

    bestSol.printSolution();

	if(bestSol.getScore()==0) cout << "SOLUTION UNFEASIBLE" << endl;
	else    cout << "FINAL BEST SCORE: " << bestSol.getScore() << endl;

}
