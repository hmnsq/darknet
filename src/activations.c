#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            повернути "logistic";
        case LOGGY:
            повернути "loggy";
        case RELU:
            повернути "relu";
        case ELU:
            повернути "elu";
        case SELU:
            повернути "selu";
        case RELIE:
            повернути "relie";
        case RAMP:
            повернути "ramp";
        case LINEAR:
            повернути "linear";
        case TANH:
            повернути "tanh";
        case PLSE:
            повернути "plse";
        case LEAKY:
            повернути "leaky";
        case STAIR:
            повернути "stair";
        case HARDTAN:
            повернути "hardtan";
        case LHTAN:
            повернути "lhtan";
        default:
            break;
    }
    повернути "relu";
}

ACTIVATION get_activation(char *s)
{
    якщо (strcmp(s, "logistic")==0) повернути LOGISTIC;
    якщо (strcmp(s, "loggy")==0) повернути LOGGY;
    якщо (strcmp(s, "relu")==0) повернути RELU;
    якщо (strcmp(s, "elu")==0) повернути ELU;
    якщо (strcmp(s, "selu")==0) повернути SELU;
    якщо (strcmp(s, "relie")==0) повернути RELIE;
    якщо (strcmp(s, "plse")==0) повернути PLSE;
    якщо (strcmp(s, "hardtan")==0) повернути HARDTAN;
    якщо (strcmp(s, "lhtan")==0) повернути LHTAN;
    якщо (strcmp(s, "linear")==0) повернути LINEAR;
    якщо (strcmp(s, "ramp")==0) повернути RAMP;
    якщо (strcmp(s, "leaky")==0) повернути LEAKY;
    якщо (strcmp(s, "tanh")==0) повернути TANH;
    якщо (strcmp(s, "stair")==0) повернути STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    повернути RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            повернути linear_activate(x);
        case LOGISTIC:
            повернути logistic_activate(x);
        case LOGGY:
            повернути loggy_activate(x);
        case RELU:
            повернути relu_activate(x);
        case ELU:
            повернути elu_activate(x);
        case SELU:
            повернути selu_activate(x);
        case RELIE:
            повернути relie_activate(x);
        case RAMP:
            повернути ramp_activate(x);
        case LEAKY:
            повернути leaky_activate(x);
        case TANH:
            повернути tanh_activate(x);
        case PLSE:
            повернути plse_activate(x);
        case STAIR:
            повернути stair_activate(x);
        case HARDTAN:
            повернути hardtan_activate(x);
        case LHTAN:
            повернути lhtan_activate(x);
    }
    повернути 0;
}

void activate_array(float *x, const ціле n, const ACTIVATION a)
{
    ціле i;
    для(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            повернути linear_gradient(x);
        case LOGISTIC:
            повернути logistic_gradient(x);
        case LOGGY:
            повернути loggy_gradient(x);
        case RELU:
            повернути relu_gradient(x);
        case ELU:
            повернути elu_gradient(x);
        case SELU:
            повернути selu_gradient(x);
        case RELIE:
            повернути relie_gradient(x);
        case RAMP:
            повернути ramp_gradient(x);
        case LEAKY:
            повернути leaky_gradient(x);
        case TANH:
            повернути tanh_gradient(x);
        case PLSE:
            повернути plse_gradient(x);
        case STAIR:
            повернути stair_gradient(x);
        case HARDTAN:
            повернути hardtan_gradient(x);
        case LHTAN:
            повернути lhtan_gradient(x);
    }
    повернути 0;
}

void gradient_array(const float *x, const ціле n, const ACTIVATION a, float *delta)
{
    ціле i;
    для(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

