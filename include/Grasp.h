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
		//void generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos, int h, Solution sol);
		Candidate RCL(float alpha, Solution &sol, std::list<Candidate> &C);
		//void local(Solution sol);
		//void findNeighbours(Solution sol, std::list<Solution> neighbours);
		//void generateNeighbours(Solution sol, std::list<Solution> neighbours, int h1, int h2, int h);
		//void generateNeighbours(Solution sol, std::list<Solution> neighbours, int h1, int h2);	
//		void generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos, std::vector<bool> nurse_works;
};
#endif
