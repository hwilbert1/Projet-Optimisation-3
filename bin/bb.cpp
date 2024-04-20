#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main ( int argc , char ** argv ) {
    double f = 1e20; 
    double c1 = 1e20;
    double Q[5]; 

    double ElevAm = 135;
    double Debitotal = 500;


    if (argc >= 2) {

        ifstream in(argv[1]);

        for (int i = 0; i < 5; i++) {
            in >> Q[i];
        }

        in >> ElevAm;
        in >> Debitotal;

        if (in.fail())
        {
            f = 1e20; c1 = -1e20;
        }
        else {

            c1 = -1;

            f = 100;


        }
        in.close();

    }

    cout << f << " " << c1 << endl;
  return 0;
}
