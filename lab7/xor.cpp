#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}


double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

class NeuralNetwork {
public:
   
    double learning_rate;

    vector<vector<double>> weights_input_hidden;
    vector<double> weights_hidden_output;             
   
    vector<double> bias_hidden; 
    double bias_output;
    
    NeuralNetwork() 
    {
        learning_rate = 0.1;
        weights_input_hidden = {
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX},    
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX}     
        };

        weights_hidden_output = {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX};

        bias_hidden.resize(2, 0.0);

        bias_output = 0.0;
    }

  
    double forward(const std::vector<double> &inputs, std::vector<double> &hidden_activations) const {
   
        for(int i = 0; i < 2; ++i){
            double activation = bias_hidden[i];
            for(int j = 0; j < 2; ++j){
                activation += weights_input_hidden[i][j] * inputs[j];
            }
            hidden_activations[i] = sigmoid(activation);
        }

       
        double output = bias_output;
        for(int i = 0; i < 2; ++i){
            output += weights_hidden_output[i] * hidden_activations[i];
        }
        output = sigmoid(output);

        return output;
    }

    
    void train(const std::vector<double> &inputs, double target) {
    
        std::vector<double> hidden_activations(2, 0.0);
        double output = forward(inputs, hidden_activations);

     
        double error_output = target - output;
        double delta_output = error_output * sigmoid_derivative(output);

       
        std::vector<double> delta_hidden(2, 0.0);
        for(int i = 0; i < 2; ++i){
            delta_hidden[i] = weights_hidden_output[i] * delta_output * sigmoid_derivative(hidden_activations[i]);
        }

       
        for(int i = 0; i < 2; ++i){
            weights_hidden_output[i] += learning_rate * delta_output * hidden_activations[i];
        }
        bias_output += learning_rate * delta_output;

        
        for(int i = 0; i < 2; ++i){
            for(int j = 0; j < 2; ++j){
                weights_input_hidden[i][j] += learning_rate * delta_hidden[i] * inputs[j];
            }
            bias_hidden[i] += learning_rate * delta_hidden[i];
        }
    }

  
    double predict(double a,double b)  {
        vector<double> hidden_activations(2, 0.0);
        vector<double> inputs;
        inputs.push_back(a);
        inputs.push_back(b);
        return forward(inputs, hidden_activations);
    }
};

int main(){
   
    vector<vector<double>> training_inputs = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };
    std::vector<double> training_outputs = {0.0, 1.0, 1.0, 0.0};

   
    NeuralNetwork nn; 

    int epochs = 100000; 

    for(int epoch = 1; epoch <= epochs; ++epoch){
        for(int i = 0; i < training_inputs.size(); ++i){
            nn.train(training_inputs[i], training_outputs[i]);
            double prediction = nn.predict(training_inputs[i][0],training_inputs[i][1]);
        }

    }

	ifstream file1;
	file1.open("input.txt");
	string str;
	getline(file1,str);

	int n;
	n = stoi(str);
	vector<vector<int>>check(2,vector<int>(n));
	
	
	for(int i=0;i<2;i++)
	{
		getline(file1,str);
		stringstream ss(str);
		string token;
		for(int j=0;j<n;j++)
		{
			ss >> token;
			check[i][j] = stoi(token);
		}
	}
	
   
   ofstream file2;
   file2.open("output.txt");
    for(int i = 0; i < n; ++i){
        double output = nn.predict(check[0][i],check[1][i]);
        int predicted = (output > 0.5) ? 1 : 0;
        file2 << "Input: " << static_cast<int>(check[0][i]) << ", " 
                  << static_cast<int>(check[1][i])
                  << " -> Output: " << output 
                  << " -> Predicted: " << predicted  << "\n";
    }

	file1.close();
	file2.close();
	
    return 0;
}
