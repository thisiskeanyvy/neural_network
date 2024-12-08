#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INPUT_SIZE 2
#define HIDDEN_SIZE 2
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.1 // valeur par défaut
#define EPOCHS 10000 // entrainement sur 10000 époques (fonctionne)

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) { // dérivée de sigmoid
    return x * (1.0 - x);
}

void initialize_weights(double weights[], int size) { // initialisation aléatoire [-1, 1]
    for (int i = 0; i < size; i++) {
        weights[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

int train() {
    srand(time(NULL));

    double hidden_weights[INPUT_SIZE * HIDDEN_SIZE];
    double output_weights[HIDDEN_SIZE * OUTPUT_SIZE];
    double hidden_bias[HIDDEN_SIZE];
    double output_bias[OUTPUT_SIZE];

    initialize_weights(hidden_weights, INPUT_SIZE * HIDDEN_SIZE);
    initialize_weights(output_weights, HIDDEN_SIZE * OUTPUT_SIZE);
    initialize_weights(hidden_bias, HIDDEN_SIZE);
    initialize_weights(output_bias, OUTPUT_SIZE);

    // Inputs for non A. non B + A.B
    double inputs[4][INPUT_SIZE] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    // Targets for non A . non B + A.B
    double targets[4] = {1, 0, 0, 1};

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        for (int i = 0; i < 4; i++) {
            double hidden_layer[HIDDEN_SIZE];
            double output_layer[OUTPUT_SIZE];

            // Propagation avant
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                double sum = hidden_bias[j];
                for (int k = 0; k < INPUT_SIZE; k++) {
                    sum += inputs[i][k] * hidden_weights[k * HIDDEN_SIZE + j];
                }
                hidden_layer[j] = sigmoid(sum);
            }

            for (int k = 0; k < OUTPUT_SIZE; k++) {
                double sum = output_bias[k];
                for (int l = 0; l < HIDDEN_SIZE; l++) {
                    sum += hidden_layer[l] * output_weights[l * OUTPUT_SIZE + k];
                }
                output_layer[k] = sigmoid(sum);
            }

            // Rétropropagation
            double output_error = targets[i] - output_layer[0];
            double output_delta = output_error * sigmoid_derivative(output_layer[0]);

            double hidden_errors[HIDDEN_SIZE];
            double hidden_deltas[HIDDEN_SIZE];
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                hidden_errors[j] = output_delta * output_weights[j];
                hidden_deltas[j] = hidden_errors[j] * sigmoid_derivative(hidden_layer[j]);
            }

            // Update poids et biais
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                output_weights[j] += LEARNING_RATE * output_delta * hidden_layer[j];
                for (int k = 0; k < INPUT_SIZE; k++) {
                    hidden_weights[k * HIDDEN_SIZE + j] += LEARNING_RATE * hidden_deltas[j] * inputs[i][k];
                }
                hidden_bias[j] += LEARNING_RATE * hidden_deltas[j];
            }
            output_bias[0] += LEARNING_RATE * output_delta;
        }
    }

    // Test the network
    printf("POC -> Testing the network for XNOR :\n");
    for (int i = 0; i < 4; i++) {
        double hidden_layer[HIDDEN_SIZE];
        double output_layer[OUTPUT_SIZE];

        for (int j = 0; j < HIDDEN_SIZE; j++) {
            double sum = hidden_bias[j];
            for (int k = 0; k < INPUT_SIZE; k++) {
                sum += inputs[i][k] * hidden_weights[k * HIDDEN_SIZE + j];
            }
            hidden_layer[j] = sigmoid(sum);
        }

        for (int j = 0; j < OUTPUT_SIZE; j++) {
            double sum = output_bias[j];
            for (int k = 0; k < HIDDEN_SIZE; k++) {
                sum += hidden_layer[k] * output_weights[k * OUTPUT_SIZE + j];
            }
            output_layer[j] = sigmoid(sum);
        }

        printf("Input XNOR: %.0f %.0f, Output XNOR: %.4f, Result: %.0f\n", 
               inputs[i][0], inputs[i][1], output_layer[0], targets[i]);
    }

    return 0;
}