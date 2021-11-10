#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cerr << "This programs requires exactly one argument." << endl;
	} else {
		cout << "Hello world, " << argv[1] << "!" << endl;
	}
	return 0;
}
