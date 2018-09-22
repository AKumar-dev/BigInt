#include "main.h"

using namespace std;

int main(int argc, char *argv[]){	
	try{
		ifstream iFile(argv[1]);
		
		if(!iFile.is_open()){
			cerr << "Could not open " << argv[1] << endl;
			exit(EXIT_FAILURE);
		}

		char traverse; //for reading/storing operation_to_be_performed on BIGINTS
		
		BigInt<int,List> one;
		BigInt<int,List> two;
		
		iFile >> one;
		iFile >> two;
		
		iFile >> traverse;
		if(traverse != '+' && traverse != '-')
			throw logic_error("traverse can only be an addition or subtraction symbol");

		BigInt<int,List> three;
		three = (traverse == '+'? one + two: one - two);

	  	cout << "one < two? " << (one < two) << endl;
		cout << "one > two? " << (one > two) << endl;
		cout << "one == two? " << (one == two) << endl;
		cout << "one != two? " << (one != two) << endl << endl;

		cout << one << endl;
		cout << two << endl;
		cout << traverse << endl;
		cout << "___________________________" << endl;
		cout << three << endl;


		iFile.get(traverse); //to get rid of \n character at end of line
		iFile.close();

	}catch(logic_error &e){
			cout << e.what() << endl;
	}
	cout << endl << "# # # # # # # # # # # # # # # # # # # # # # # # # #" << endl << endl;
	try{
		ifstream iFile(argv[1]);
		
		if(!iFile.is_open()){
			cerr << "Could not open " << argv[1] << endl;
			exit(EXIT_FAILURE);
		}

		char traverse; //for reading/storing operation_to_be_performed on BIGINTS
		
		for(int i = 0; i < 1; i++){
		
		BigInt<char,List> one;
		BigInt<char,List> two;
		
		iFile >> one;
		iFile >> two;
		
		iFile >> traverse;
		if(traverse != '+' && traverse != '-')
			throw logic_error("traverse can only be an addition or subtraction symbol");

		BigInt<char,List> three;
		three = one - two; 

	  	cout << "one < two? " << (one < two) << endl;
		cout << "one > two? " << (one > two) << endl;
		cout << "one == two? " << (one == two) << endl;
		cout << "one != two? " << (one != two) << endl << endl;

		cout << one << endl; 
		cout << two << endl;
		cout << traverse << endl;
		cout << "___________________________" << endl;
		cout << three << endl;

		iFile.get(traverse); //to get rid of \n character at end of line
		}
	}catch(logic_error &e){
			cout << e.what() << endl;
	}
	return 0;
}
