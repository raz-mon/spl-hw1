#include <iostream>
#include "../include/json.hpp"
#include <fstream>
#include <vector>
#include "../include/Session.h"
#include <string>

//test
//test
//test grd
//test hgx
//test  rdf
using json = nlohmann::json;
using namespace std;

int main(int argc, char** argv){

    if(argc != 2){
        cout << "usage cTrace <config_path>" << endl;
        return 0;
    }
    Session sess(argv[1]);
    sess.simulate();
    sess.outputConfig();
    return 0;
}
