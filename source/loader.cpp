#include "../include/loader.hpp"


using namespace std;

void loader(char a, char b, int i){

	//TODO Need to implement full memory check
	// if(Memory::completeMemory == nullptr){
	// 	cout << " memory full " << endl;
	// }
	//cout << a << b[0] << endl;
	string combine(1,a);
	combine += b; //b holds too much info, concatenating.

	Opcode current; //OPCODE is a struct for all objects an instruction holds.
	current.opcode = std::stoi(combine, nullptr, 16);
	//cout << (int)current.opcode << endl;

	Memory::write(i, current.opcode);

}
