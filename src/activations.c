#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            врати "logistic";
        case LOGGY:
            врати "loggy";
        case RELU:
            врати "relu";
        case ELU:
            врати "elu";
        case SELU:
            врати "selu";
        case RELIE:
            врати "relie";
        case RAMP:
            врати "ramp";
        case LINEAR:
            врати "linear";
        case TANH:
            врати "tanh";
        case PLSE:
            врати "plse";
        case LEAKY:
            врати "leaky";
        case STAIR:
            врати "stair";
        case HARDTAN:
            врати "hardtan";
        case LHTAN:
            врати "lhtan";
        default:
            break;
    }
    врати "relu";
}

ACTIVATION get_activation(char *s)
{
    ако (strcmp(s, "logistic")==0) врати LOGISTIC;
    ако (strcmp(s, "loggy")==0) врати LOGGY;
    ако (strcmp(s, "relu")==0) врати RELU;
    ако (strcmp(s, "elu")==0) врати ELU;
    ако (strcmp(s, "selu")==0) врати SELU;
    ако (strcmp(s, "relie")==0) врати RELIE;
    ако (strcmp(s, "plse")==0) врати PLSE;
    ако (strcmp(s, "hardtan")==0) врати HARDTAN;
    ако (strcmp(s, "lhtan")==0) врати LHTAN;
    ако (strcmp(s, "linear")==0) врати LINEAR;
    ако (strcmp(s, "ramp")==0) врати RAMP;
    ако (strcmp(s, "leaky")==0) врати LEAKY;
    ако (strcmp(s, "tanh")==0) врати TANH;
    ако (strcmp(s, "stair")==0) врати STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    врати RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            врати linear_activate(x);
        case LOGISTIC:
            врати logistic_activate(x);
        case LOGGY:
            врати loggy_activate(x);
        case RELU:
            врати relu_activate(x);
        case ELU:
            врати elu_activate(x);
        case SELU:
            врати selu_activate(x);
        case RELIE:
            врати relie_activate(x);
        case RAMP:
            врати ramp_activate(x);
        case LEAKY:
            врати leaky_activate(x);
        case TANH:
            врати tanh_activate(x);
        case PLSE:
            врати plse_activate(x);
        case STAIR:
            врати stair_activate(x);
        case HARDTAN:
            врати hardtan_activate(x);
        case LHTAN:
            врати lhtan_activate(x);
    }
    врати 0;
}

void activate_array(float *x, const цео n, const ACTIVATION a)
{
    цео i;
    за(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            врати linear_gradient(x);
        case LOGISTIC:
            врати logistic_gradient(x);
        case LOGGY:
            врати loggy_gradient(x);
        case RELU:
            врати relu_gradient(x);
        case ELU:
            врати elu_gradient(x);
        case SELU:
            врати selu_gradient(x);
        case RELIE:
            врати relie_gradient(x);
        case RAMP:
            врати ramp_gradient(x);
        case LEAKY:
            врати leaky_gradient(x);
        case TANH:
            врати tanh_gradient(x);
        case PLSE:
            врати plse_gradient(x);
        case STAIR:
            врати stair_gradient(x);
        case HARDTAN:
            врати hardtan_gradient(x);
        case LHTAN:
            врати lhtan_gradient(x);
    }
    врати 0;
}

void gradient_array(const float *x, const цео n, const ACTIVATION a, float *delta)
{
    цео i;
    за(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

