#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            върни "logistic";
        case LOGGY:
            върни "loggy";
        case RELU:
            върни "relu";
        case ELU:
            върни "elu";
        case SELU:
            върни "selu";
        case RELIE:
            върни "relie";
        case RAMP:
            върни "ramp";
        case LINEAR:
            върни "linear";
        case TANH:
            върни "tanh";
        case PLSE:
            върни "plse";
        case LEAKY:
            върни "leaky";
        case STAIR:
            върни "stair";
        case HARDTAN:
            върни "hardtan";
        case LHTAN:
            върни "lhtan";
        default:
            break;
    }
    върни "relu";
}

ACTIVATION get_activation(char *s)
{
    ако (strcmp(s, "logistic")==0) върни LOGISTIC;
    ако (strcmp(s, "loggy")==0) върни LOGGY;
    ако (strcmp(s, "relu")==0) върни RELU;
    ако (strcmp(s, "elu")==0) върни ELU;
    ако (strcmp(s, "selu")==0) върни SELU;
    ако (strcmp(s, "relie")==0) върни RELIE;
    ако (strcmp(s, "plse")==0) върни PLSE;
    ако (strcmp(s, "hardtan")==0) върни HARDTAN;
    ако (strcmp(s, "lhtan")==0) върни LHTAN;
    ако (strcmp(s, "linear")==0) върни LINEAR;
    ако (strcmp(s, "ramp")==0) върни RAMP;
    ако (strcmp(s, "leaky")==0) върни LEAKY;
    ако (strcmp(s, "tanh")==0) върни TANH;
    ако (strcmp(s, "stair")==0) върни STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    върни RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            върни linear_activate(x);
        case LOGISTIC:
            върни logistic_activate(x);
        case LOGGY:
            върни loggy_activate(x);
        case RELU:
            върни relu_activate(x);
        case ELU:
            върни elu_activate(x);
        case SELU:
            върни selu_activate(x);
        case RELIE:
            върни relie_activate(x);
        case RAMP:
            върни ramp_activate(x);
        case LEAKY:
            върни leaky_activate(x);
        case TANH:
            върни tanh_activate(x);
        case PLSE:
            върни plse_activate(x);
        case STAIR:
            върни stair_activate(x);
        case HARDTAN:
            върни hardtan_activate(x);
        case LHTAN:
            върни lhtan_activate(x);
    }
    върни 0;
}

void activate_array(float *x, const цяло n, const ACTIVATION a)
{
    цяло i;
    за(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            върни linear_gradient(x);
        case LOGISTIC:
            върни logistic_gradient(x);
        case LOGGY:
            върни loggy_gradient(x);
        case RELU:
            върни relu_gradient(x);
        case ELU:
            върни elu_gradient(x);
        case SELU:
            върни selu_gradient(x);
        case RELIE:
            върни relie_gradient(x);
        case RAMP:
            върни ramp_gradient(x);
        case LEAKY:
            върни leaky_gradient(x);
        case TANH:
            върни tanh_gradient(x);
        case PLSE:
            върни plse_gradient(x);
        case STAIR:
            върни stair_gradient(x);
        case HARDTAN:
            върни hardtan_gradient(x);
        case LHTAN:
            върни lhtan_gradient(x);
    }
    върни 0;
}

void gradient_array(const float *x, const цяло n, const ACTIVATION a, float *delta)
{
    цяло i;
    за(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

