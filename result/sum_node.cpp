#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main(int argc, char *argv[]) {
	ifstream inFile(argv[1]);
	string addr;
	int count;
	map <string, int> m;

	while (!inFile.eof()) {
		inFile >> addr >> count;
		m[addr] += count;
	}

	for (map<string, int>::iterator i = m.begin(); i != m.end(); i++) {
		cout << i->first <<" " << i->second << '\n';
	}

	return 0;
}
