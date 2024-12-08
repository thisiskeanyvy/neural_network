#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define INPUT_SIZE 2744 // 56x49 pixels
#define HIDDEN_SIZE 256
#define OUTPUT_SIZE 26  // lettres alphabet
#define LEARNING_RATE 0.01
#define MAX_EPOCHS 10000
#define MAX_EXAMPLES 260 // max exemples

typedef struct {
    double weights[INPUT_SIZE][HIDDEN_SIZE];
    double hidden_bias[HIDDEN_SIZE];
    double output_weights[HIDDEN_SIZE][OUTPUT_SIZE];
    double output_bias[OUTPUT_SIZE];
} NeuralNetwork;

typedef struct {
    double input[INPUT_SIZE];
    double target[OUTPUT_SIZE];
} Example;

Example examples[MAX_EXAMPLES];
int num_examples = 0;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1 - x);
}

void initialize_network(NeuralNetwork* nn) {
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        nn->hidden_bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            nn->output_weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        nn->output_bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

void forward_propagation(double input[], NeuralNetwork* nn, double hidden[], double output[]) {
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        hidden[i] = nn->hidden_bias[i];
        for (int j = 0; j < INPUT_SIZE; j++) {
            hidden[i] += input[j] * nn->weights[j][i];
        }
        hidden[i] = sigmoid(hidden[i]);
    }

    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] = nn->output_bias[i];
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            output[i] += hidden[j] * nn->output_weights[j][i];
        }
        output[i] = sigmoid(output[i]);
    }
}

void backpropagation(double input[], double hidden[], double output[], double target[], NeuralNetwork* nn) {
    double output_error[OUTPUT_SIZE];
    double hidden_error[HIDDEN_SIZE];

    // calcul des erreurs de sortie
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output_error[i] = (target[i] - output[i]) * sigmoid_derivative(output[i]);
        nn->output_bias[i] += LEARNING_RATE * output_error[i];
    }

    // calcul des erreurs cachées
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        hidden_error[i] = 0;
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            hidden_error[i] += output_error[j] * nn->output_weights[i][j];
            nn->output_weights[i][j] += LEARNING_RATE * output_error[j] * hidden[i];
        }
        hidden_error[i] *= sigmoid_derivative(hidden[i]);
        nn->hidden_bias[i] += LEARNING_RATE * hidden_error[i];
    }

    // mise à jour des poids d'entrée vers cachés
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->weights[i][j] += LEARNING_RATE * hidden_error[j] * input[i];
        }
    }
}

void train_network(NeuralNetwork* nn, double input[], double target[]) {
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];

    forward_propagation(input, nn, hidden, output);
    backpropagation(input, hidden, output, target, nn);
}

void save_network(NeuralNetwork* nn, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }
    fwrite(nn, sizeof(NeuralNetwork), 1, file);
    fclose(file);
}

int load_network(NeuralNetwork* nn, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return 0;
    
    fread(nn, sizeof(NeuralNetwork), 1, file);
    fclose(file);
    
    return 1;
}

void save_examples(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return;

    fwrite(&num_examples, sizeof(int), 1, file);
    fwrite(examples, sizeof(Example), num_examples, file);
    
    fclose(file);
}

void load_examples(const char* filename) {
    FILE* file = fopen(filename, "rb");
    
    if (!file) return;

    fread(&num_examples, sizeof(int), 1, file);
    fread(examples, sizeof(Example), num_examples, file);

    fclose(file);
}

void add_example(double input[], double target[]) {
    if (num_examples >= MAX_EXAMPLES) return;

    memcpy(examples[num_examples].input, input, sizeof(double) * INPUT_SIZE);
    memcpy(examples[num_examples].target, target, sizeof(double) * OUTPUT_SIZE);

    num_examples++;
}

void train_on_examples(NeuralNetwork* nn) {
    for (int epoch = 0; epoch < MAX_EPOCHS / num_examples; epoch++) { 
        for (int i = 0; i < num_examples; i++) { 
            train_network(nn, examples[i].input, examples[i].target); 
        } 
     } 
}

void preprocess_image(SDL_Surface* surface, double input[]) {
    SDL_Surface* scaled_surface = SDL_CreateRGBSurface(0, 56, 49, 32, 0, 0, 0, 0);
    SDL_BlitScaled(surface, NULL, scaled_surface, NULL);

    SDL_LockSurface(scaled_surface);
    Uint32* pixels = (Uint32*)scaled_surface->pixels;

    for (int i = 0; i < INPUT_SIZE; i++) {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], scaled_surface->format, &r, &g, &b);
        input[i] = (r + g + b) / (3.0 * 255.0);
    }

    SDL_UnlockSurface(scaled_surface);
    SDL_FreeSurface(scaled_surface);
}

char recognize_letter(NeuralNetwork* nn, double input[]) {
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];

    forward_propagation(input, nn, hidden, output);

    int max_index = 0;
    for (int i = 1; i < OUTPUT_SIZE; i++) {
        if (output[i] > output[max_index]) {
            max_index = i;
        }
    }

    return 'A' + max_index;
}

void train_single_image(NeuralNetwork* nn, const char* image_path) {
    SDL_Surface* surface = IMG_Load(image_path);
    if (!surface) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    double input[INPUT_SIZE];
    preprocess_image(surface, input);

    double target[OUTPUT_SIZE] = {0};
    char letter = image_path[strlen(image_path) - 5];
    target[letter - 'A'] = 1;

    add_example(input, target);
    train_on_examples(nn);

    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <train/recognize/train_single> <file_path>\n", argv[0]);
        return 1;
    }

    NeuralNetwork nn;
    const char* network_file = "network_data.bin";
    const char* examples_file = "train_data.bin";

    // init SDL et SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur d'initialisation SDL/SDL_image: %s\n", SDL_GetError());
        return 1;
    }

    // chargement du réseau et des exemples
    if (!load_network(&nn, network_file)) {
        initialize_network(&nn);
    }
    load_examples(examples_file);

    if (strcmp(argv[1], "train") == 0) {
        train_on_examples(&nn);
        save_network(&nn, network_file);
        save_examples(examples_file);
        printf("Entraînement terminé et réseau sauvegardé.\n");
    } else if (strcmp(argv[1], "recognize") == 0) {
        SDL_Surface* surface = IMG_Load(argv[2]);
        if (!surface) {
            printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
            return 1;
        }

        double input[INPUT_SIZE];
        preprocess_image(surface, input);

        char letter = recognize_letter(&nn, input);
        printf("Lettre reconnue : %c\n", letter);

        SDL_FreeSurface(surface);
    } else if (strcmp(argv[1], "train_single") == 0) {
        train_single_image(&nn, argv[2]);
        save_network(&nn, network_file);
        save_examples(examples_file);
        printf("Entraînement sur une image terminé et réseau sauvegardé.\n");
    } else {
        printf("Commande non reconnue. Utilisez 'train', 'recognize' ou 'train_single'.\n");
        return 1;
    }

    IMG_Quit();
    SDL_Quit();
    return 0;
}
