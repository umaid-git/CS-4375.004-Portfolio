#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>

using namespace std;

ifstream readFile(string fileName){
    ifstream inputFileStream;

    cout <<"Opening file" << fileName << endl;
    inputFileStream.open(fileName);

    if(!inputFileStream.is_open()) {
        cout << "Could not open file Boston.csv" << endl;
    }

    return inputFileStream;
}

// this function returns the sum of all the values in a numeric vector
double sumOfNumericVector(vector <double> vectorVariableName){
    double sum=0;

    // loops through the whole array and add the value
    for(int i=0; i<vectorVariableName.size(); i++){
        sum += vectorVariableName.at(i);
    }

    return sum;
}

// this function returns the mean of a numeric vector
double meanOfNumericVector(vector <double> vectorVariableName){
    return sumOfNumericVector(vectorVariableName)/vectorVariableName.size();
}

// this function returns the median of a numeric vector
double medianOfNumericVector(vector <double> vectorVariableName){
    bool even = false;

    // checks if the vector is even or odd. 
    if(vectorVariableName.size() % 2 == 0)
        even = true;

    sort(execution::par, vectorVariableName.begin(), vectorVariableName.end());
    int middleIndex = vectorVariableName.size() / 2;

    // if the vector size is even then get the left index and right index of the middle value 
    if(even){
        int leftIndex = middleIndex;
        int rigtIndex = middleIndex+1;

        double median = (vectorVariableName.at(leftIndex) + vectorVariableName.at(rigtIndex))/2;    // stores the average of left and right value of middle index

        return median;
    }
    else {
        return vectorVariableName.at(ceil(middleIndex));
    }
}

// this function returns the range of numeric vector
double rangeOfNumericVector(vector <double> vectorVariableName){
    sort(execution::par, vectorVariableName.begin(), vectorVariableName.end());
    return vectorVariableName.at(vectorVariableName.size()-1)-vectorVariableName.at(0);
}

// this function computes and returns the covariance between to features of a table
// the formula below is take from Machine Learning Handbook Using R and Python by Dr. Karen Mazidi pg.74
double covarianceBetweenTwoFeatures(vector <double> vectorX, vector <double> vectorY){
    double meanOfX = meanOfNumericVector(vectorX);
    double meanOfY = meanOfNumericVector(vectorY);
    int numberOfObservations = vectorX.size();

    double sumOfProduct = 0;
    double differenceInXValues = 0;
    double differenceInYValues = 0;

    for (int index=0; index < numberOfObservations; index++){
        differenceInXValues = vectorX.at(index) - meanOfX;
        differenceInYValues = vectorY.at(index) - meanOfY;
        sumOfProduct += differenceInXValues * differenceInYValues;
    }

    double covariance = sumOfProduct / (numberOfObservations-1);

    return covariance;
}

// this function calculates standard deviation in given set of values
double standardDeviation(vector <double> vectorVariableName){
    double mean = meanOfNumericVector(vectorVariableName);
    double squareOfSumOfDifference = 0;
    double numberOfObservations = vectorVariableName.size();
    double difference = 0;

    for(int index = 0; index < numberOfObservations; index++){
        difference = vectorVariableName.at(index) - mean;
        squareOfSumOfDifference += difference * difference;
    }

    double standardDeviation = sqrt(squareOfSumOfDifference/ (numberOfObservations-1));

    return standardDeviation;
}

double correlationBetweenTwoFeatures(vector <double> vectorX, vector <double> vectorY){
    double covariance = covarianceBetweenTwoFeatures(vectorX,vectorY);
    double standardDeviationInX = standardDeviation(vectorX);
    double standardDeviationInY = standardDeviation(vectorY);

    double correlation = covariance / (standardDeviationInX * standardDeviationInY);

    return correlation;

}



// The main function
// few of the code snippets are taken from the professor provided starter code from Portfolio Component 1: Data Exploration
int main(int argc, char** argv) {
    
    // variable declaration
    ifstream inputFileStream;
    string lineFromFile;
    string rm_feature_string_input_from_file, medv_feature_string_input_from_file;
    const int MAX_LEN = 1000;
    vector <double> rm_feature_vector(MAX_LEN);
    vector <double> medv_feature_vector(MAX_LEN);

    inputFileStream = readFile("Boston.csv");

    // reads and display the features/ column name
    cout << "Reading line1 " << endl;
    getline(inputFileStream, lineFromFile);
    cout << " This is the line from file " << lineFromFile << endl;

    int numOfObservation = 0;

    while(inputFileStream.good()){

        getline(inputFileStream, rm_feature_string_input_from_file, ',');
        getline(inputFileStream, medv_feature_string_input_from_file, '\n');

        // convert the string into a float and store into respective vectors
        rm_feature_vector.at(numOfObservation) = stof(rm_feature_string_input_from_file);
        medv_feature_vector.at(numOfObservation) = stof(medv_feature_string_input_from_file);

        // cout << "rm value of Obs *" << numOfObservation << "* is ==" << rm_feature_vector.at(numOfObservation) << endl;
        // cout << "medv value of Obs *" << numOfObservation << "* is ==" << medv_feature_vector.at(numOfObservation) << endl;
        // cout << "=============================================================== \n";

        numOfObservation++;
    }

    rm_feature_vector.resize(numOfObservation);
    medv_feature_vector.resize(numOfObservation);

    // closing the file
    cout << "Closing the file" << endl;
    inputFileStream.close();

    cout << "The sum of rm is == " << sumOfNumericVector(rm_feature_vector) << endl;
    cout << "The sum of medv is == " << sumOfNumericVector(medv_feature_vector) << endl;
    
    cout << "The mean of rm is == " << meanOfNumericVector(rm_feature_vector) << endl;
    cout << "The mean of medv is == " << meanOfNumericVector(medv_feature_vector) << endl;

    cout << "The median of rm is == " << medianOfNumericVector(rm_feature_vector) << endl;
    cout << "The median of medv is == " << medianOfNumericVector(medv_feature_vector) << endl;

    cout << "The range of rm is == " << rangeOfNumericVector(rm_feature_vector) << endl;
    cout << "The range of medv is == " << rangeOfNumericVector(medv_feature_vector) << endl;

    cout << "The covariance between rm and medv is == " <<  covarianceBetweenTwoFeatures(rm_feature_vector, medv_feature_vector) << endl;

    cout << " Standard deviation of rm == " << standardDeviation(rm_feature_vector) << endl;
    cout << " Standard deviation of medv == " << standardDeviation(medv_feature_vector) << endl;

    cout << "The correlation between rm and medv is == " << correlationBetweenTwoFeatures(rm_feature_vector,medv_feature_vector);


    return 0;
}