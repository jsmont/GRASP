#ifndef _H_GRASP_
#define _H_GRASP_

#include <vector>
#include <list>
#include <Solution.h>

class Grasp {	
	SolutionParams params;
	
	public:
		Grasp(SolutionParams params);
		Solution executeGrasp(int maxiter, float alpha);

	private:	
		void construct(Solution &sol, float alpha);
		void initCandidates(std::list<Candidate> &C, Solution &sol);
		void updateCandidates(std::list<Candidate> &C, Solution &sol);
		Candidate RCL(float alpha, Solution &sol, std::list<Candidate> &C);
		void local(Solution &sol);
		bool findNeighbours(Solution &sol);
		//static void perm_assigned(std::list<Candidate> &assigned, std::list<Candidate> &unassigned, Solution &sol, Solution &bestSol, int remain, int perm);
		//static void perm_unassigned(std::list<Candidate> &unassigned, Solution &sol, Solution &bestSol, int remain);
};
#endif
