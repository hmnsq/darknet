#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            trả_về "logistic";
        case LOGGY:
            trả_về "loggy";
        case RELU:
            trả_về "relu";
        case ELU:
            trả_về "elu";
        case SELU:
            trả_về "selu";
        case RELIE:
            trả_về "relie";
        case RAMP:
            trả_về "ramp";
        case LINEAR:
            trả_về "linear";
        case TANH:
            trả_về "tanh";
        case PLSE:
            trả_về "plse";
        case LEAKY:
            trả_về "leaky";
        case STAIR:
            trả_về "stair";
        case HARDTAN:
            trả_về "hardtan";
        case LHTAN:
            trả_về "lhtan";
        default:
            break;
    }
    trả_về "relu";
}

ACTIVATION get_activation(char *s)
{
    nếu (strcmp(s, "logistic")==0) trả_về LOGISTIC;
    nếu (strcmp(s, "loggy")==0) trả_về LOGGY;
    nếu (strcmp(s, "relu")==0) trả_về RELU;
    nếu (strcmp(s, "elu")==0) trả_về ELU;
    nếu (strcmp(s, "selu")==0) trả_về SELU;
    nếu (strcmp(s, "relie")==0) trả_về RELIE;
    nếu (strcmp(s, "plse")==0) trả_về PLSE;
    nếu (strcmp(s, "hardtan")==0) trả_về HARDTAN;
    nếu (strcmp(s, "lhtan")==0) trả_về LHTAN;
    nếu (strcmp(s, "linear")==0) trả_về LINEAR;
    nếu (strcmp(s, "ramp")==0) trả_về RAMP;
    nếu (strcmp(s, "leaky")==0) trả_về LEAKY;
    nếu (strcmp(s, "tanh")==0) trả_về TANH;
    nếu (strcmp(s, "stair")==0) trả_về STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    trả_về RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            trả_về linear_activate(x);
        case LOGISTIC:
            trả_về logistic_activate(x);
        case LOGGY:
            trả_về loggy_activate(x);
        case RELU:
            trả_về relu_activate(x);
        case ELU:
            trả_về elu_activate(x);
        case SELU:
            trả_về selu_activate(x);
        case RELIE:
            trả_về relie_activate(x);
        case RAMP:
            trả_về ramp_activate(x);
        case LEAKY:
            trả_về leaky_activate(x);
        case TANH:
            trả_về tanh_activate(x);
        case PLSE:
            trả_về plse_activate(x);
        case STAIR:
            trả_về stair_activate(x);
        case HARDTAN:
            trả_về hardtan_activate(x);
        case LHTAN:
            trả_về lhtan_activate(x);
    }
    trả_về 0;
}

void activate_array(float *x, const số_nguyên n, const ACTIVATION a)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            trả_về linear_gradient(x);
        case LOGISTIC:
            trả_về logistic_gradient(x);
        case LOGGY:
            trả_về loggy_gradient(x);
        case RELU:
            trả_về relu_gradient(x);
        case ELU:
            trả_về elu_gradient(x);
        case SELU:
            trả_về selu_gradient(x);
        case RELIE:
            trả_về relie_gradient(x);
        case RAMP:
            trả_về ramp_gradient(x);
        case LEAKY:
            trả_về leaky_gradient(x);
        case TANH:
            trả_về tanh_gradient(x);
        case PLSE:
            trả_về plse_gradient(x);
        case STAIR:
            trả_về stair_gradient(x);
        case HARDTAN:
            trả_về hardtan_gradient(x);
        case LHTAN:
            trả_về lhtan_gradient(x);
    }
    trả_về 0;
}

void gradient_array(const float *x, const số_nguyên n, const ACTIVATION a, float *delta)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

