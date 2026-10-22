#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            戻る "logistic";
        case LOGGY:
            戻る "loggy";
        case RELU:
            戻る "relu";
        case ELU:
            戻る "elu";
        case SELU:
            戻る "selu";
        case RELIE:
            戻る "relie";
        case RAMP:
            戻る "ramp";
        case LINEAR:
            戻る "linear";
        case TANH:
            戻る "tanh";
        case PLSE:
            戻る "plse";
        case LEAKY:
            戻る "leaky";
        case STAIR:
            戻る "stair";
        case HARDTAN:
            戻る "hardtan";
        case LHTAN:
            戻る "lhtan";
        default:
            break;
    }
    戻る "relu";
}

ACTIVATION get_activation(char *s)
{
    もし (strcmp(s, "logistic")==0) 戻る LOGISTIC;
    もし (strcmp(s, "loggy")==0) 戻る LOGGY;
    もし (strcmp(s, "relu")==0) 戻る RELU;
    もし (strcmp(s, "elu")==0) 戻る ELU;
    もし (strcmp(s, "selu")==0) 戻る SELU;
    もし (strcmp(s, "relie")==0) 戻る RELIE;
    もし (strcmp(s, "plse")==0) 戻る PLSE;
    もし (strcmp(s, "hardtan")==0) 戻る HARDTAN;
    もし (strcmp(s, "lhtan")==0) 戻る LHTAN;
    もし (strcmp(s, "linear")==0) 戻る LINEAR;
    もし (strcmp(s, "ramp")==0) 戻る RAMP;
    もし (strcmp(s, "leaky")==0) 戻る LEAKY;
    もし (strcmp(s, "tanh")==0) 戻る TANH;
    もし (strcmp(s, "stair")==0) 戻る STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    戻る RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            戻る linear_activate(x);
        case LOGISTIC:
            戻る logistic_activate(x);
        case LOGGY:
            戻る loggy_activate(x);
        case RELU:
            戻る relu_activate(x);
        case ELU:
            戻る elu_activate(x);
        case SELU:
            戻る selu_activate(x);
        case RELIE:
            戻る relie_activate(x);
        case RAMP:
            戻る ramp_activate(x);
        case LEAKY:
            戻る leaky_activate(x);
        case TANH:
            戻る tanh_activate(x);
        case PLSE:
            戻る plse_activate(x);
        case STAIR:
            戻る stair_activate(x);
        case HARDTAN:
            戻る hardtan_activate(x);
        case LHTAN:
            戻る lhtan_activate(x);
    }
    戻る 0;
}

void activate_array(float *x, const 整数 n, const ACTIVATION a)
{
    整数 i;
    ために(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            戻る linear_gradient(x);
        case LOGISTIC:
            戻る logistic_gradient(x);
        case LOGGY:
            戻る loggy_gradient(x);
        case RELU:
            戻る relu_gradient(x);
        case ELU:
            戻る elu_gradient(x);
        case SELU:
            戻る selu_gradient(x);
        case RELIE:
            戻る relie_gradient(x);
        case RAMP:
            戻る ramp_gradient(x);
        case LEAKY:
            戻る leaky_gradient(x);
        case TANH:
            戻る tanh_gradient(x);
        case PLSE:
            戻る plse_gradient(x);
        case STAIR:
            戻る stair_gradient(x);
        case HARDTAN:
            戻る hardtan_gradient(x);
        case LHTAN:
            戻る lhtan_gradient(x);
    }
    戻る 0;
}

void gradient_array(const float *x, const 整数 n, const ACTIVATION a, float *delta)
{
    整数 i;
    ために(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

