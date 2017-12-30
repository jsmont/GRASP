#include <vector>
#include <list>

class Grasp {
	typedef std::vector<bool> Candidate;
	
	public:
		Grasp(int maxiter, float alpha);
		void executeGrasp();

	private:
		int maxiter;
		float alpha;
		
		void construct(Solution sol, float alpha);
		void initCandidates(std::list<Candidate> &C, Solution sol);
		void updateCandidates(std::list<Candidate> &C, Solution sol);
		Candidate RCL(float alpha, Solution sol, std::list<Candidate> &C);
		void local(Solution sol);
		void findNeighbours(Solution sol, std::list<Solution> neighbours)	

}
