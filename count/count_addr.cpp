#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main(int argc, char *argv[]) {
	ifstream inFile(argv[1]);
	string task, cpu, flag, time, fname, tmp1, tmp2, tmp3, addr;
	map <string, int> m;

	while (!inFile.eof()) {
		inFile >> task >> cpu >> flag >> time >> fname >> tmp1 >> tmp2 >> tmp3;
		if (fname == "rbtree_addr:")
			inFile >> addr;
		else 
			continue;
		m[addr]++;
	}

	for (map<string,int>::iterator i = m.begin(); i != m.end(); i++) {
		cout << i->first << " " << i->second << '\n';
	}

	return 0;
}
