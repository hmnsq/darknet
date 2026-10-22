#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_activation_string(ACTIVATION a)
{
    switch(a){
        case LOGISTIC:
            返回 "logistic";
        case LOGGY:
            返回 "loggy";
        case RELU:
            返回 "relu";
        case ELU:
            返回 "elu";
        case SELU:
            返回 "selu";
        case RELIE:
            返回 "relie";
        case RAMP:
            返回 "ramp";
        case LINEAR:
            返回 "linear";
        case TANH:
            返回 "tanh";
        case PLSE:
            返回 "plse";
        case LEAKY:
            返回 "leaky";
        case STAIR:
            返回 "stair";
        case HARDTAN:
            返回 "hardtan";
        case LHTAN:
            返回 "lhtan";
        default:
            break;
    }
    返回 "relu";
}

ACTIVATION get_activation(char *s)
{
    如果 (strcmp(s, "logistic")==0) 返回 LOGISTIC;
    如果 (strcmp(s, "loggy")==0) 返回 LOGGY;
    如果 (strcmp(s, "relu")==0) 返回 RELU;
    如果 (strcmp(s, "elu")==0) 返回 ELU;
    如果 (strcmp(s, "selu")==0) 返回 SELU;
    如果 (strcmp(s, "relie")==0) 返回 RELIE;
    如果 (strcmp(s, "plse")==0) 返回 PLSE;
    如果 (strcmp(s, "hardtan")==0) 返回 HARDTAN;
    如果 (strcmp(s, "lhtan")==0) 返回 LHTAN;
    如果 (strcmp(s, "linear")==0) 返回 LINEAR;
    如果 (strcmp(s, "ramp")==0) 返回 RAMP;
    如果 (strcmp(s, "leaky")==0) 返回 LEAKY;
    如果 (strcmp(s, "tanh")==0) 返回 TANH;
    如果 (strcmp(s, "stair")==0) 返回 STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    返回 RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            返回 linear_activate(x);
        case LOGISTIC:
            返回 logistic_activate(x);
        case LOGGY:
            返回 loggy_activate(x);
        case RELU:
            返回 relu_activate(x);
        case ELU:
            返回 elu_activate(x);
        case SELU:
            返回 selu_activate(x);
        case RELIE:
            返回 relie_activate(x);
        case RAMP:
            返回 ramp_activate(x);
        case LEAKY:
            返回 leaky_activate(x);
        case TANH:
            返回 tanh_activate(x);
        case PLSE:
            返回 plse_activate(x);
        case STAIR:
            返回 stair_activate(x);
        case HARDTAN:
            返回 hardtan_activate(x);
        case LHTAN:
            返回 lhtan_activate(x);
    }
    返回 0;
}

void activate_array(float *x, const 整数 n, const ACTIVATION a)
{
    整数 i;
    为(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        case LINEAR:
            返回 linear_gradient(x);
        case LOGISTIC:
            返回 logistic_gradient(x);
        case LOGGY:
            返回 loggy_gradient(x);
        case RELU:
            返回 relu_gradient(x);
        case ELU:
            返回 elu_gradient(x);
        case SELU:
            返回 selu_gradient(x);
        case RELIE:
            返回 relie_gradient(x);
        case RAMP:
            返回 ramp_gradient(x);
        case LEAKY:
            返回 leaky_gradient(x);
        case TANH:
            返回 tanh_gradient(x);
        case PLSE:
            返回 plse_gradient(x);
        case STAIR:
            返回 stair_gradient(x);
        case HARDTAN:
            返回 hardtan_gradient(x);
        case LHTAN:
            返回 lhtan_gradient(x);
    }
    返回 0;
}

void gradient_array(const float *x, const 整数 n, const ACTIVATION a, float *delta)
{
    整数 i;
    为(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

