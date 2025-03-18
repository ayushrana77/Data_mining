#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}


class NeuralNetwork {
public:
  
    vector<vector<double>> weights_input_hidden1;
    vector<vector<double>> weights_input_hidden2;
    vector<double> weights_hidden_output;
    double bias_hidden1[2];
    double bias_hidden2[2];
    double bias_output;

   
    NeuralNetwork() {
        srand(time(0));
        weights_input_hidden1 = {
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX},    
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX}     
        };

		weights_input_hidden2 = {
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX},    
            {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX}     
        };
       
        weights_hidden_output = {(double)rand()/RAND_MAX,(double)rand()/RAND_MAX};

       
        bias_hidden1[0] = (double)rand()/RAND_MAX;  
        bias_hidden1[1] = (double)rand()/RAND_MAX;
        
        bias_hidden2[0] = (double)rand()/RAND_MAX; 
        bias_hidden2[1] = (double)rand()/RAND_MAX;

       
        bias_output = (double)rand()/RAND_MAX;     
    }


    double forward(vector<double> &inputs)  {
        
        
        // first hiddenl layer
        vector<double> hidden_activations1(2, 0.0);
        for(int i = 0; i < 2; ++i){
            double activation = bias_hidden1[i];
            for(int j = 0; j < 2; ++j){
                activation += weights_input_hidden1[i][j] * inputs[j];
            }
            hidden_activations1[i] = sigmoid(activation);
        }
        
        //second hidden layer
        vector<double> hidden_activations2(2, 0.0);
        for(int i = 0; i < 2; ++i){
            double activation = bias_hidden2[i];
            for(int j = 0; j < 2; ++j){
                activation += weights_input_hidden2[i][j] * hidden_activations2[j];
            }
            hidden_activations2[i] = sigmoid(activation);
        }

      
        double output = bias_output;
        for(int i = 0; i < 2; ++i){
            output += weights_hidden_output[i] * hidden_activations2[i];
        }
        output = sigmoid(output);

        return output;
    }
};

int main(){
   
    vector<double>input(2);

   	cout<<"\nEnter the first value : ";
   	cin>>input[0];
   		
   	cout<<"\nEnter the second value : ";
	cin>>input[1];   	
	

    NeuralNetwork nn;

	cout<<endl;
	
    double output = nn.forward(input);
    int predicted = (output > 0.75) ? 1 : 0;
    cout << "Input: {" << static_cast<int>(input[0]) << ", " << static_cast<int>(input[1])<< "}-> Output: " << output<< " -> Predicted: " << predicted;
    return 0;
}