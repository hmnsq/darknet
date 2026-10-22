#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            반환 "logistic";
        case LOGGY:
            반환 "loggy";
        case RELU:
            반환 "relu";
        case ELU:
            반환 "elu";
        case SELU:
            반환 "selu";
        case RELIE:
            반환 "relie";
        case RAMP:
            반환 "ramp";
        case LINEAR:
            반환 "linear";
        case TANH:
            반환 "tanh";
        case PLSE:
            반환 "plse";
        case LEAKY:
            반환 "leaky";
        case STAIR:
            반환 "stair";
        case HARDTAN:
            반환 "hardtan";
        case LHTAN:
            반환 "lhtan";
        default:
            break;
    }
    반환 "relu";
}

ACTIVATION get_activation(char *s)
{
    만약 (strcmp(s, "logistic")==0) 반환 LOGISTIC;
    만약 (strcmp(s, "loggy")==0) 반환 LOGGY;
    만약 (strcmp(s, "relu")==0) 반환 RELU;
    만약 (strcmp(s, "elu")==0) 반환 ELU;
    만약 (strcmp(s, "selu")==0) 반환 SELU;
    만약 (strcmp(s, "relie")==0) 반환 RELIE;
    만약 (strcmp(s, "plse")==0) 반환 PLSE;
    만약 (strcmp(s, "hardtan")==0) 반환 HARDTAN;
    만약 (strcmp(s, "lhtan")==0) 반환 LHTAN;
    만약 (strcmp(s, "linear")==0) 반환 LINEAR;
    만약 (strcmp(s, "ramp")==0) 반환 RAMP;
    만약 (strcmp(s, "leaky")==0) 반환 LEAKY;
    만약 (strcmp(s, "tanh")==0) 반환 TANH;
    만약 (strcmp(s, "stair")==0) 반환 STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    반환 RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            반환 linear_activate(x);
        case LOGISTIC:
            반환 logistic_activate(x);
        case LOGGY:
            반환 loggy_activate(x);
        case RELU:
            반환 relu_activate(x);
        case ELU:
            반환 elu_activate(x);
        case SELU:
            반환 selu_activate(x);
        case RELIE:
            반환 relie_activate(x);
        case RAMP:
            반환 ramp_activate(x);
        case LEAKY:
            반환 leaky_activate(x);
        case TANH:
            반환 tanh_activate(x);
        case PLSE:
            반환 plse_activate(x);
        case STAIR:
            반환 stair_activate(x);
        case HARDTAN:
            반환 hardtan_activate(x);
        case LHTAN:
            반환 lhtan_activate(x);
    }
    반환 0;
}

void activate_array(float *x, const 정수 n, const ACTIVATION a)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            반환 linear_gradient(x);
        case LOGISTIC:
            반환 logistic_gradient(x);
        case LOGGY:
            반환 loggy_gradient(x);
        case RELU:
            반환 relu_gradient(x);
        case ELU:
            반환 elu_gradient(x);
        case SELU:
            반환 selu_gradient(x);
        case RELIE:
            반환 relie_gradient(x);
        case RAMP:
            반환 ramp_gradient(x);
        case LEAKY:
            반환 leaky_gradient(x);
        case TANH:
            반환 tanh_gradient(x);
        case PLSE:
            반환 plse_gradient(x);
        case STAIR:
            반환 stair_gradient(x);
        case HARDTAN:
            반환 hardtan_gradient(x);
        case LHTAN:
            반환 lhtan_gradient(x);
    }
    반환 0;
}

void gradient_array(const float *x, const 정수 n, const ACTIVATION a, float *delta)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

