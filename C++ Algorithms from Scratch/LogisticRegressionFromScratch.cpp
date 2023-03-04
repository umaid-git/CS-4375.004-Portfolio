
#include <iostream>
#include <fstream>
#include <vector>
#include <valarray>
#include <chrono>
using namespace std; 

vector<vector<double> > multiply_matrices(const vector<vector<double> >& A, const vector<vector<double> >& B) {
    const size_t m = A.size();
    const size_t n = A[0].size();
    const size_t p = B[0].size();

    // Initialize result matrix with zeros
    vector<vector<double> > C(m, vector<double>(p, 0.0));

    // Compute matrix multiplication
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < p; j++) {
            for (size_t k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}


vector<vector<double> > subtract_matrices(const vector<vector<double> > &A, const vector<vector<double> > &B) {
    const size_t m = A.size();
    const size_t n = A[0].size();

    // Initialize result matrix with zeros
    vector<vector<double> > C(m, vector<double>(n, 0.0));

    // Compute matrix subtraction
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }

    return C;
}

vector<vector<double> > transpose_matrix(const vector<vector<double> > &A) {
    const size_t m = A.size();
    const size_t n = A[0].size();

    // Initialize result matrix with zeros
    vector<vector<double> > B(n, vector<double>(m, 0.0));

    // Compute transpose
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            B[j][i] = A[i][j];
        }
    }

    return B;
}

vector<vector<double> > sigmoid(const vector<vector<double> > &A) {
    vector<vector<double> > sigmoid_values(A.size(), vector<double>(1,0));

    for(int i = 0; i < A.size(); i++) {
        sigmoid_values[i][0] = 1.0 / (1 + exp(-A[i][0]));
    }

    return sigmoid_values;
}

vector<vector<double> > glm(const vector<double> &sex, const vector<double> &survived) {
    // Set initial weights
     vector<vector<double> > weights{{1},{1}};

    // Create data matrix
    vector<vector<double> > data_matrix(sex.size(), vector<double>(2, 0));

    for(int i = 0; i < sex.size(); i++) {
        data_matrix[i][0] = 1;
        data_matrix[i][1] = sex[i];
    }

    // Create label vector
    vector<vector<double> > labels(survived.size(), vector<double>(1, 0));

    for(int i = 0; i < survived.size(); i++) {
        labels[i][0] = survived.at(i);
    }

    // Train model using gradient descent
    const double learning_rate = 0.001;
    for (int i = 0; i < 5000; i++) {
        // Compute probabilities using sigmoid function
        const vector<vector<double> > prob_vector = sigmoid(multiply_matrices(data_matrix, weights));

        // Compute error between labels and predicted probabilities
        const vector<vector<double> > error = subtract_matrices(labels, prob_vector);

        // Update weights using gradient descent
        const vector<vector<double> > transpose = transpose_matrix(data_matrix);
        const vector<vector<double> > gradient = multiply_matrices(transpose, error);
        weights[0][0] += learning_rate * gradient[0][0];
        weights[1][0] += learning_rate * gradient[1][0];
    }

    return weights;
}

//the function takes a vector doubles 'sex' and doubles intercept and slope which correspond to w0 and w1 
//the function predicts either a 0 or 1 based on logistic regression and returns those
vector<double> predictFunction(vector<double> &sex, double intercept, double slope) {
    //store the size, create a vector to store outputs and variables to hold the calculation values like logOdds, odds and probability
    int size=sex.size();
    vector<double> output(size);
    double logOdds, odds, probability;
    
    for (int i = 0; i < size; i++) {
        logOdds = (slope * sex[i]) + intercept;
        odds = exp(logOdds);
        probability = odds / (1 + odds);

        // store probabilty as 1 if it's greater than 0.5
        // and 0 if it's less than 0.5
        if (probability > 0.50) {
            output[i] = 1;
        }
        else {
            output[i] = 0;
        }
    }
    return output;
}

//this function creates a confusion matrix based on how accurately our model predicted
vector<double> confusionMatrixCalculator(vector<double> sex, vector<double> survived){
    //creating a vector of doubles to hold the result 
    vector<double> results(4);
    //creating integer counters and setting them all to 0
    int truePositive, trueNegative, falsePositive, falseNegative;
    truePositive= trueNegative= falsePositive= falseNegative=0;
    //Using a loop to iterate 
    //incrementing based on whether the model predicted correctly or incorrectly 
    for (int i = 0; i < sex.size(); i++) {
        
        if (sex[i] == 1 and survived[i] == 1) {
            truePositive++;
        }
        else if (sex[i] == 1 and survived[i] == 0) {
            falsePositive++;
        }
        else if (sex[i] == 0 and survived[i] == 1) {
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

int main(){
    //in order to read the data from the file titanic.csv
    ifstream input;
    string s;
    const int MAX_LEN=1400;
    string PassengerID, PClass, Survived, Sex, Age;
    int numOfObservations=0;

    vector<double> pclass(MAX_LEN), survived(MAX_LEN), sex(MAX_LEN), age(MAX_LEN);

    cout<<"Starting to build the logistic regression model"<<endl;
    cout<<"Opening titanic.csv";

    input.open("titanic_project.csv");

    //check if the file doesnt open
    if(!input.is_open()) {
        cout << "Could not open titanic_project.csv file" << endl;
        return 1;
    }

    getline(input, s);

    while(input.good()){
        getline(input, PassengerID, ',');
        getline(input, PClass, ',');
        getline(input, Survived, ',');
        getline(input, Sex, ',');
        getline(input, Age, '\n');

        pclass.at(numOfObservations) = stof(PClass);
        survived.at(numOfObservations) = stof(Survived);
        sex.at(numOfObservations) = stof(Sex);
        age.at(numOfObservations) = stof(Age);

        numOfObservations=numOfObservations+1;
    }
    //resize the vectors
    pclass.resize(numOfObservations);
    survived.resize(numOfObservations);
    sex.resize(numOfObservations);
    age.resize(numOfObservations);
    //close the file as we are done reading
    input.close();

    //split data into training and testing data
    //use first 800 observations for training and the rest for testing 
    vector<double> training_sex(800);
    vector<double> training_survived(800);
    int a=numOfObservations-800;
    vector<double> testing_sex(a);
    vector<double> testing_survived(a);

    //use nested loops to split the data 
    for(int i = 0; i < numOfObservations; i++){
        if (i < 800) {
            training_sex.at(i) = sex.at(i);
            training_survived.at(i) = survived.at(i);
        }
        else {
            testing_sex.at(i - 800) = sex.at(i);
            testing_survived.at( i - 800) = survived.at(i);
        }
    }

    //using the training data to build a logistic regression and output the coefficients
    //calculate the time taken as well

    auto begin = chrono::steady_clock::now();
    vector<vector<double> > weights = glm(training_sex, training_survived);
    auto finish= chrono::steady_clock::now();

    //calculate and output the training time
    cout<<endl;
    cout<<"The training time taken was "<<chrono::duration_cast<chrono::milliseconds>(finish - begin).count() << " milliseconds.";
    cout<<endl;

    //Output the results of glm and display the coefficients 
    cout<<endl<<"Coefficients: " <<endl;
    double intercept=weights[0][0]; 
    double slope=weights[1][0]; 
    cout<<"intercept: "<< intercept <<" slope:  " <<slope<< endl;

    //next we check the model using the test data 
    //we pass the testing_sex, the intercept and the slope.
    testing_sex=predictFunction(testing_sex,intercept,slope);

    //next we calculate the accuracy, sensitivity and specificity

    vector <double> resultsFromConfusionMatrix;
    resultsFromConfusionMatrix= confusionMatrixCalculator(testing_sex, testing_survived);

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