#include "Grasp.h"

int main(int argc, char* argv[]){
	

	if(argc!=4){
		std::cerr << "Usage: grasp filename iterations alpha" << std::endl; //Or whatever
		std::exit(1);
	}
	
	//read input and stuff
	int iterations=argv[2];
	float alpha=argv[3];

	Grasp g; //do a better iterating system, changing alpha and stuff
	g.executeGrasp(iterations,alpha); 
		
}
