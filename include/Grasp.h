#include <vector>
#include <list>

class Grasp {
	typedef std::vector<bool> Candidate;
	
	public:
		Grasp();
		Solution executeGrasp(int maxiter, float alpha);

	private:	
		void construct(Solution sol, float alpha);
		void initCandidates(std::list<Candidate> &C, Solution sol);
		void updateCandidates(std::list<Candidate> &C, Solution sol);
		void generateCandidates(std::list<Candidate> &C, Candidate c, int remaining, int pos
		Candidate RCL(float alpha, Solution sol, std::list<Candidate> &C);
		void local(Solution sol);
		void findNeighbours(Solution sol, std::list<Solution> neighbours)	

}
