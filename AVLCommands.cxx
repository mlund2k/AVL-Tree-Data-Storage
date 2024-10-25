#include <iostream> // output library
#include <fstream> // file read library
#include <string>  // string library
#include "json.hpp" // json library
#include "AVL.h" // AVL header
using namespace std; // standard namespace

int main(int argc, char** argv) { // command line arguments
    if(argc!=2){ // check if incorrect amount of arguments
        cout << "Usage: ./AVLCommands.exe [file_name].json" << endl; // output usage format
        return 0; // exit
    }
    ifstream f(argv[1]); // read in file from command line
    nlohmann::json data = nlohmann::json::parse(f); // parse the file
    AVL T; // initialize empty AVL tree
    unsigned int totalZeros = (int) floor(log10((double) data["metadata"]["numOps"])) + 1; // used for finding number of zeroes as used in createheapoperationdata.cxx
    
    for(int i=1; i <= data["metadata"]["numOps"]; i++){ // loop through all operations
        int opZeros =  (int) floor(log10((double) i)) + 1; // determine # of necessary 0s for current index operation, used in createheapoperationdata.cxx
        std::string opNum = std::string(totalZeros - opZeros, '0').append(std::to_string(i)); // subtract operation 0s from total 0s to find necessary number of 0s at index
        string name = opNum; // create index name to find data in json file
        if(data[name]["operation"] == "Insert"){ // if command is to remove minimum node
            int key = data[name]["key"]; // turn json element into type int
            T.Insert(key); // then insert current key
        }
    }
    cout << T.JSON() << endl; // dump output
    return 0; // exit program
}