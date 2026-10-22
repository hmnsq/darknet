#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            вернуть "logistic";
        case LOGGY:
            вернуть "loggy";
        case RELU:
            вернуть "relu";
        case ELU:
            вернуть "elu";
        case SELU:
            вернуть "selu";
        case RELIE:
            вернуть "relie";
        case RAMP:
            вернуть "ramp";
        case LINEAR:
            вернуть "linear";
        case TANH:
            вернуть "tanh";
        case PLSE:
            вернуть "plse";
        case LEAKY:
            вернуть "leaky";
        case STAIR:
            вернуть "stair";
        case HARDTAN:
            вернуть "hardtan";
        case LHTAN:
            вернуть "lhtan";
        default:
            break;
    }
    вернуть "relu";
}

ACTIVATION get_activation(char *s)
{
    если (strcmp(s, "logistic")==0) вернуть LOGISTIC;
    если (strcmp(s, "loggy")==0) вернуть LOGGY;
    если (strcmp(s, "relu")==0) вернуть RELU;
    если (strcmp(s, "elu")==0) вернуть ELU;
    если (strcmp(s, "selu")==0) вернуть SELU;
    если (strcmp(s, "relie")==0) вернуть RELIE;
    если (strcmp(s, "plse")==0) вернуть PLSE;
    если (strcmp(s, "hardtan")==0) вернуть HARDTAN;
    если (strcmp(s, "lhtan")==0) вернуть LHTAN;
    если (strcmp(s, "linear")==0) вернуть LINEAR;
    если (strcmp(s, "ramp")==0) вернуть RAMP;
    если (strcmp(s, "leaky")==0) вернуть LEAKY;
    если (strcmp(s, "tanh")==0) вернуть TANH;
    если (strcmp(s, "stair")==0) вернуть STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    вернуть RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            вернуть linear_activate(x);
        case LOGISTIC:
            вернуть logistic_activate(x);
        case LOGGY:
            вернуть loggy_activate(x);
        case RELU:
            вернуть relu_activate(x);
        case ELU:
            вернуть elu_activate(x);
        case SELU:
            вернуть selu_activate(x);
        case RELIE:
            вернуть relie_activate(x);
        case RAMP:
            вернуть ramp_activate(x);
        case LEAKY:
            вернуть leaky_activate(x);
        case TANH:
            вернуть tanh_activate(x);
        case PLSE:
            вернуть plse_activate(x);
        case STAIR:
            вернуть stair_activate(x);
        case HARDTAN:
            вернуть hardtan_activate(x);
        case LHTAN:
            вернуть lhtan_activate(x);
    }
    вернуть 0;
}

void activate_array(float *x, const целое n, const ACTIVATION a)
{
    целое i;
    для(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            вернуть linear_gradient(x);
        case LOGISTIC:
            вернуть logistic_gradient(x);
        case LOGGY:
            вернуть loggy_gradient(x);
        case RELU:
            вернуть relu_gradient(x);
        case ELU:
            вернуть elu_gradient(x);
        case SELU:
            вернуть selu_gradient(x);
        case RELIE:
            вернуть relie_gradient(x);
        case RAMP:
            вернуть ramp_gradient(x);
        case LEAKY:
            вернуть leaky_gradient(x);
        case TANH:
            вернуть tanh_gradient(x);
        case PLSE:
            вернуть plse_gradient(x);
        case STAIR:
            вернуть stair_gradient(x);
        case HARDTAN:
            вернуть hardtan_gradient(x);
        case LHTAN:
            вернуть lhtan_gradient(x);
    }
    вернуть 0;
}

void gradient_array(const float *x, const целое n, const ACTIVATION a, float *delta)
{
    целое i;
    для(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

