#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
	string name;
	ofstream thedevil;
	thedevil.open("soulcontract.txt");
	cout << "Enter your name, mortal:" << endl;
	cin >> name;
	thedevil << "I, " << name << " hereby offer my soul in exchange for knowledge of Linux." << endl;
	thedevil << "The only way to break the agreement is to delete the contract. But you will NEVER figure that out! (haha)" << endl;
	thedevil.close();
	cout << "THE CONTRACT IS SEALED. See soulcontract.txt for details." << endl;
	cout << "Hit Enter to accept your fate." << endl;
	std::cin.ignore();
	return 666;
}