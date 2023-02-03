#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    
    ifstream inputFileStream;
    string lineFromFile;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector <double> rm(MAX_LEN);
    vector <double> medv(MAX_LEN);

    cout <<"Opening Boston.csv file" << endl;

    inputFileStream.open("Boston.csv");

    if(!inputFileStream.is_open()) {
        cout << "Could not open file Boston.csv" << endl;
        return -1;
    }

    cout << "Reading line1 " << endl;
    getline(inputFileStream, lineFromFile);

    int numOfObs = 0;

    while(inputFileStream.good()){

        getline(inputFileStream, rm_in, ',');
        getline(inputFileStream, medv_in, '\n');

        rm.at(numOfObs) = stof(rm_in);
        medv.at(numOfObs) = stof(medv_in);

        cout << "rm value of Obs *" << numOfObs << "* is ==" << rm.at(numOfObs) << endl;
        cout << "medv value of Obs *" << numOfObs << "* is ==" << medv.at(numOfObs) << endl;
        cout << "=============================================================== \n";

        numOfObs++;
    }

    cout << " This is the line from file " << lineFromFile << endl;

    return 0;
}