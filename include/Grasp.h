#include <vector>
#include <list>

class Grasp {
	typedef std::vector<bool> Candidate;
	
	public:
		Grasp();
		void executeGrasp(int maxiter, float alpha);

	private:
		
		void construct(Solution sol, float alpha);
		void initCandidates(std::list<Candidate> &C);
		void updateCandidates(std::list<Candidate> &C);
		Candidate RCL(float alpha, Solution sol, std::list<Candidate> &C);
		void local(Solution sol);
		void findNeighbours(Solution sol, std::list<Solution> neighbours)	

}
