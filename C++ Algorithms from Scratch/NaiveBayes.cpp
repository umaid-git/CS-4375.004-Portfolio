#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

// this function returns the total number of times the passed variable survivedOrPerished
int countInstance(vector <double> survived_vector, int survivedOrPerished){
    int counts = 0;

    for(int i=0; i< survived_vector.size() ; i++){
        if(survived_vector.at(i) == survivedOrPerished){
            counts++;
        }
    }

    return counts;
}

// this function returns tha apriori probability
double apriori (int numberOfObservations, int counts_of_survived_or_perished){
    return ((double)counts_of_survived_or_perished) / numberOfObservations;
}

/* this function returns the likelihood for passenger class
return matrix: 
        [pclass1]                   [pclass2]                   [pclasse3]
[0]    prob. perished pclass1   prob. perished pclass2      prob. perished pclass3
[1]    prob. survived pclass1   prob. survived pclass2      prob. survived pclass2
*/
vector <vector<double>>  likelihood_for_pclass(vector <double> pclass_vector, vector <double> survived_vector, double survivedCount, double perishedCount){

    // initializing the counters
    int survived_pclass_1 = 0, survived_pclass_2 = 0, survived_pclass_3 = 0;
    int perished_pclass_1 = 0, perished_pclass_2 = 0, perished_pclass_3 = 0;

    // loops through the array to count number of survivor and perished according to pclass
    for(int i =0; i<pclass_vector.size(); i++){
        // if pclass is 1 and it survived then incrementing the value
        if((pclass_vector.at(i) == 1) && (survived_vector.at(i) == 1)){
            survived_pclass_1++;
        }
        // if pclass is 3 and it survived then incrementing the value
        else if((pclass_vector.at(i) == 2) && (survived_vector.at(i) == 1)){
            survived_pclass_2++;
        }
        // if pclass is 3 and it survived then incrementing the value
        else if((pclass_vector.at(i) == 3) && (survived_vector.at(i) == 1)){
            survived_pclass_3++;
        }
        else if((pclass_vector.at(i) == 1) && (survived_vector.at(i) == 0)){
            perished_pclass_1++;
        }
        // if pclass is 2 and it perished then incrementing the value
        else if((pclass_vector.at(i) == 2) && (survived_vector.at(i) == 0)){
            perished_pclass_2++;
        }
        // if pclass is 3 and it perished then incrementing the value
        else if((pclass_vector.at(i) == 3) && (survived_vector.at(i) == 0)){
            perished_pclass_3++;
        }
    }

    // cout << perished_pclass_1 << " " << perished_pclass_2 << " " << perished_pclass_3 << endl;
    // cout << survived_pclass_1 << " " << survived_pclass_2 << " "  << survived_pclass_3 << endl;

    // calculating likelihood
    vector < vector<double> > likelihood_matrix
    {
        { (perished_pclass_1/perishedCount) , (perished_pclass_2/perishedCount) , (perished_pclass_3/perishedCount) },
        { (survived_pclass_1/survivedCount) , (survived_pclass_2/survivedCount), (survived_pclass_3/survivedCount) }
    };

    return likelihood_matrix;
}


/* this function returns the likelihood of survival base on sex
return matrix: 
        [0]                             [1]              
[0]    prob. perished sex_0   prob. perished sex_1
[1]    prob. survived sex_0   prob. survived sex_1
*/
vector <vector<double>>  likelihood_for_sex(vector <double> sex_vector, vector <double> survived_vector, double survivedCount, double perishedCount){
    // initializing the counters
    int survived_sex_0 = 0, survived_sex_1 = 0;
    int perished_sex_0 = 0, perished_sex_1 = 0;

    // loops through the array to count number of survivor and perished according to pclass
    for(int i =0; i<sex_vector.size(); i++){
        // if pclass is 1 and it survived then incrementing the value
        if((sex_vector.at(i) == 0) && (survived_vector.at(i) == 1)){
            survived_sex_0++;
        }
        // if pclass is 3 and it survived then incrementing the value
        else if((sex_vector.at(i) == 1) && (survived_vector.at(i) == 1)){
            survived_sex_1++;
        }
        else if((sex_vector.at(i) == 0) && (survived_vector.at(i) == 0)){
            perished_sex_0++;
        }
        // if pclass is 2 and it perished then incrementing the value
        else if((sex_vector.at(i) == 1) && (survived_vector.at(i) == 0)){
            perished_sex_1++;
        }
    }

    // calculating likelihood
    vector < vector<double> > likelihood_matrix
    {
        { (perished_sex_0/perishedCount) , (perished_sex_1/perishedCount) },
        { (survived_sex_0/survivedCount) , (survived_sex_1/survivedCount) }
    };

    return likelihood_matrix;

}

// this function returns a vector which contains the mean of age based on if the passenger survived or not
// mean[0] = mean of perish
// mean[1] = mean of survive
vector <double> meanOfAge(vector <double> age_vector, vector<double> survived_vector ,double totalSurvived, double totalPerished){
    int sum_of_survived =0;
    int sum_of_perished = 0;

    for(int i=0; i< age_vector.size(); i++){
        if(survived_vector.at(i) == 1)
            sum_of_survived += age_vector.at(i);
        else
            sum_of_perished += age_vector.at(i);
    }

    vector <double> meanVector;

    // calculating means of survived and perished
    double mean_of_survived = sum_of_survived/totalSurvived;
    double mean_of_perished = sum_of_perished/totalPerished;

    meanVector.push_back(mean_of_perished);
    meanVector.push_back(mean_of_survived);

    return meanVector;
}

// this function calculates variance in the data
double variance(vector <double> age_vector, vector<double> survived_vector, int survived_or_perished, double meanOfVector, double totalCount){
    double deviation_from_mean = 0;
    double sum_of_square_of_deviation_from_mean = 0;

    for(int i=0; i<age_vector.size(); i++){
        if(survived_vector.at(i) == survived_or_perished){
            deviation_from_mean = age_vector.at(i) - meanOfVector;
            sum_of_square_of_deviation_from_mean += deviation_from_mean * deviation_from_mean;
        }
    }

    return sum_of_square_of_deviation_from_mean / (totalCount-1);
}

// this function calculate likelihood for age by using variance and mean function
double likelihood_for_age(double age, double mean, double varaiance){
    double pi = 3.141593;
    return (1 / sqrt(2 * pi * varaiance)) * exp(- (pow(age-mean,2))/ (2 * varaiance) );
}

// debugging code

void print2DVector( vector < vector<double> > matrix){

    for(int i=0; i< matrix.size(); i++){
        for(int j=0; j< matrix.at(i).size(); j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

}


// This function calculates the raw probability and return a vector of survived
//      [1]                     [0]
//   prediction survive     prediction perished
vector <vector <double>> calc_raw_prob(int pclass, int sex, double age, vector <vector <double> > likelihood_for_pclass_matrix,
                    vector <vector <double> > likelihood_for_sex_matrix, vector <double> apriori, vector <double> mean,
                    vector <double> variance){
    
    double num_survived = likelihood_for_pclass_matrix[1][pclass-1] *  likelihood_for_sex_matrix[1][sex] * apriori[1] *
                          likelihood_for_age(age,mean[1],variance[1]);

    double num_perished = likelihood_for_pclass_matrix[0][pclass-1] *  likelihood_for_sex_matrix[0][sex] * apriori[0] *
                          likelihood_for_age(age,mean[0],variance[0]);

    double denominator = num_survived + num_perished;
    vector <vector <double> > raw_prob = { 
        {(num_survived/denominator) , (num_perished/denominator) }
        };

    return raw_prob;
}

//this function creates a confusion matrix based on how accurately our model predicted
vector<double> confusionMatrixCalculator(vector<double> predict_vector, vector<double> survived){
    //creating a vector of doubles to hold the result 
    vector<double> results(4);
    //creating integer counters and setting them all to 0
    int truePositive, trueNegative, falsePositive, falseNegative;
    truePositive= trueNegative= falsePositive= falseNegative=0;
    //Using a loop to iterate 
    //incrementing based on whether the model predicted correctly or incorrectly 
    for (int i = 0; i < predict_vector.size(); i++) {
        
        if (predict_vector[i] == 1 and survived[i] == 1) {
            truePositive++;
        }
        else if (predict_vector[i] == 1 and survived[i] == 0) {
            falsePositive++;
        }
        else if (predict_vector[i] == 0 and survived[i] == 1) {
            falseNegative++;
        }
        else {
            trueNegative++;
        }
    }
    //finalizing and returning the results 
    results[0] = truePositive;
    results[1] = falsePositive;
    results[2] = trueNegative;
    results[3] = falseNegative;
    return results;
}


int main(int argc, char** argv) {
    
    ifstream inputFileStream;
    string lineFromFile;
    string X_val_str, pclass_str, survived_str, sex_str, age_str;
    const int MAX_LEN = 2000;
    vector <double> X_val_vector(MAX_LEN);
    vector <double> pclass_vector(MAX_LEN);
    vector <double> survived_vector(MAX_LEN);
    vector <double> sex_vector(MAX_LEN);
    vector <double> age_vector(MAX_LEN);

    cout <<"Opening titanic_project.csv file" << endl;

    inputFileStream.open("titanic_project.csv");

    if(!inputFileStream.is_open()) {
        cout << "Could not open file Boston.csv" << endl;
        return -1;
    }

    cout << "Reading line1 " << endl;
    getline(inputFileStream, lineFromFile);

    int numOfObs = 0;

    while(inputFileStream.good()){
        getline(inputFileStream, X_val_str, ',');
        getline(inputFileStream, pclass_str, ',');
        getline(inputFileStream, survived_str, ',');
        getline(inputFileStream, sex_str, ',');
        getline(inputFileStream, age_str, '\n');

        // X_val_vector.at(numOfObs) = stof(X_val_str);
        pclass_vector.at(numOfObs) = stof(pclass_str);
        survived_vector.at(numOfObs) = stof(survived_str);
        sex_vector.at(numOfObs) = stof(sex_str);
        age_vector.at(numOfObs) = stof(age_str);

        // cout << numOfObs << "\t" << pclass_vector.at(numOfObs) << "\t" << survived_vector.at(numOfObs) << "\t" << sex_vector.at(numOfObs) << "\t" << age_vector.at(numOfObs) << endl; 
        numOfObs++;
    }

    pclass_vector.resize(numOfObs);
    survived_vector.resize(numOfObs);
    sex_vector.resize(numOfObs);
    age_vector.resize(numOfObs);

    vector <double> training_pclass_vector(800);
    vector <double> training_survived_vector(800);
    vector <double> training_sex_vector(800);
    vector <double> training_age_vector(800);

    const int TESTING_COUNT = numOfObs - 800;
    vector <double> testing_pclass_vector(TESTING_COUNT);
    vector <double> testing_survived_vector(TESTING_COUNT);
    vector <double> testing_sex_vector(TESTING_COUNT);
    vector <double> testing_age_vector(TESTING_COUNT);
    int index=0;

    for(int i=0; i<numOfObs; i++){
        if(i<800){
            training_pclass_vector.at(i) = pclass_vector.at(i);
            training_survived_vector.at(i) = survived_vector.at(i);
            training_sex_vector.at(i) = sex_vector.at(i);
            training_age_vector.at(i) = age_vector.at(i);
        }
        else{
            index = i-800;
            testing_pclass_vector.at(index) = pclass_vector.at(i);
            testing_survived_vector.at(index) = survived_vector.at(i);
            testing_sex_vector.at(index) = sex_vector.at(i);
            testing_age_vector.at(index) = age_vector.at(i);
        }
    }

    cout<< "Starting to build Naive Bayes Model" << endl;
    auto begin = chrono::high_resolution_clock::now();      // begins the count for training the model

    int count_survive = countInstance(training_survived_vector,1);
    int count_perish =  countInstance(training_survived_vector,0);

    // stores apriori for survived and perished
    vector <double> apriori_vector;
    apriori_vector.push_back(apriori(training_survived_vector.size(), count_perish));
    apriori_vector.push_back(apriori(training_survived_vector.size(), count_survive));
    

    // cout << "not \t survived" << endl;
    // cout << perished_apriori_prob << "\t" << survived_apriori_prob <<endl;

    // stores the likelihood of survived or perished based on passenger class
    vector < vector <double> > likelihood_for_pclass_matrix = likelihood_for_pclass(training_pclass_vector,training_survived_vector, count_survive, count_perish);
    // print2DVector(likelihood_for_pclass_matrix);

    // stores the likelihood of survived or perished based on sex
    vector < vector <double> > likelihood_for_sex_matrix = likelihood_for_sex(training_sex_vector,training_survived_vector,count_survive,count_perish);
    // print2DVector(likelihood_for_sex_matrix);

    // age_mean[0] = age mean of perished
    // age_mean[1] = age mean of survived
    vector <double> age_mean = meanOfAge(training_age_vector,training_survived_vector,count_survive,count_perish);
    
    // age_variance[0] = age variance of perished
    // age_variance[1] = age variance of survived
    vector <double> age_variance;
    age_variance.push_back(variance(training_age_vector,training_survived_vector,0, age_mean[0], count_perish));
    age_variance.push_back(variance(training_age_vector,training_survived_vector,1,age_mean[1], count_survive));

    // cout << age_mean[0] << " " << age_mean[1];
    // cout << age_variance[0] << " " << age_variance[1] << endl;

    // cout << likelihood_for_age(6,25.9,138) << endl;
    
    auto finish= chrono::high_resolution_clock::now();      // ends the count for training the model
    cout<< "The training for Naive Bayes Model have ended!" << endl;

    // prints the total duration for the training model
    cout<<endl;
    cout<<"The training time taken was "<< chrono::duration<double, milli>(finish - begin).count() << " milliseconds.";
    cout<<endl << endl;
    
    vector <vector <double>> raw;
    vector <double> predict;

    cout << "     0" << "\t\t    1" << endl;
    for(int i=0; i<testing_survived_vector.size(); i++){
        raw = calc_raw_prob(testing_pclass_vector.at(i), testing_sex_vector.at(i), testing_age_vector.at(i), likelihood_for_pclass_matrix,likelihood_for_sex_matrix, apriori_vector, age_mean, age_variance);

        // if probablilty of perished is greater than survived than push 0 into the vector
        // as either one of the probability of survived or perished would be greater or less than each other
        if(raw[0][1] > raw[0][0]){
            predict.push_back(0);
        }
        else
            predict.push_back(1);

        if(i < 6)
            cout << raw[0][1] << "\t" << raw[0][0] << endl;
    }


    //next we calculate the accuracy, sensitivity and specificity
    vector <double> resultsFromConfusionMatrix;
    resultsFromConfusionMatrix= confusionMatrixCalculator(predict,testing_survived_vector);

    double truePositive=resultsFromConfusionMatrix[0];
    double falsePositive=resultsFromConfusionMatrix[1];
    double trueNegative=resultsFromConfusionMatrix[2];
    double falseNegative=resultsFromConfusionMatrix[3];
   
    //using formulas for accuracy specificity and sensitivity
    double sensitivity = truePositive / (truePositive + falseNegative);
    double specificity = trueNegative / (trueNegative + falsePositive);
    double accuracyNumerator = truePositive + trueNegative;
    double accuracyDenominator = truePositive + trueNegative + falsePositive + falseNegative;
    double accuracy=accuracyNumerator/accuracyDenominator;

    //display the calculated metrics such as accuracy, specificity and sensitivity
    cout << endl;
    cout << "The metrics are as follows:  " << endl;
    cout << "Accuracy: " << accuracy << endl;
    cout << "Sensitivity: " << sensitivity << endl;
    cout << "Specificity: " << specificity << endl;


    return 0;
}