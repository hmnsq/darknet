#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ត្រឡប់ "logistic";
        pak_sa LOGGY:
            ត្រឡប់ "loggy";
        pak_sa RELU:
            ត្រឡប់ "relu";
        pak_sa ELU:
            ត្រឡប់ "elu";
        pak_sa SELU:
            ត្រឡប់ "selu";
        pak_sa RELIE:
            ត្រឡប់ "relie";
        pak_sa RAMP:
            ត្រឡប់ "ramp";
        pak_sa LINEAR:
            ត្រឡប់ "linear";
        pak_sa TANH:
            ត្រឡប់ "tanh";
        pak_sa PLSE:
            ត្រឡប់ "plse";
        pak_sa LEAKY:
            ត្រឡប់ "leaky";
        pak_sa STAIR:
            ត្រឡប់ "stair";
        pak_sa HARDTAN:
            ត្រឡប់ "hardtan";
        pak_sa LHTAN:
            ត្រឡប់ "lhtan";
        default:
            bha_nga;
    }
    ត្រឡប់ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ប្រសិនបើ (strcmp(s, "logistic")==0) ត្រឡប់ LOGISTIC;
    ប្រសិនបើ (strcmp(s, "loggy")==0) ត្រឡប់ LOGGY;
    ប្រសិនបើ (strcmp(s, "relu")==0) ត្រឡប់ RELU;
    ប្រសិនបើ (strcmp(s, "elu")==0) ត្រឡប់ ELU;
    ប្រសិនបើ (strcmp(s, "selu")==0) ត្រឡប់ SELU;
    ប្រសិនបើ (strcmp(s, "relie")==0) ត្រឡប់ RELIE;
    ប្រសិនបើ (strcmp(s, "plse")==0) ត្រឡប់ PLSE;
    ប្រសិនបើ (strcmp(s, "hardtan")==0) ត្រឡប់ HARDTAN;
    ប្រសិនបើ (strcmp(s, "lhtan")==0) ត្រឡប់ LHTAN;
    ប្រសិនបើ (strcmp(s, "linear")==0) ត្រឡប់ LINEAR;
    ប្រសិនបើ (strcmp(s, "ramp")==0) ត្រឡប់ RAMP;
    ប្រសិនបើ (strcmp(s, "leaky")==0) ត្រឡប់ LEAKY;
    ប្រសិនបើ (strcmp(s, "tanh")==0) ត្រឡប់ TANH;
    ប្រសិនបើ (strcmp(s, "stair")==0) ត្រឡប់ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ត្រឡប់ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ត្រឡប់ linear_activate(x);
        pak_sa LOGISTIC:
            ត្រឡប់ logistic_activate(x);
        pak_sa LOGGY:
            ត្រឡប់ loggy_activate(x);
        pak_sa RELU:
            ត្រឡប់ relu_activate(x);
        pak_sa ELU:
            ត្រឡប់ elu_activate(x);
        pak_sa SELU:
            ត្រឡប់ selu_activate(x);
        pak_sa RELIE:
            ត្រឡប់ relie_activate(x);
        pak_sa RAMP:
            ត្រឡប់ ramp_activate(x);
        pak_sa LEAKY:
            ត្រឡប់ leaky_activate(x);
        pak_sa TANH:
            ត្រឡប់ tanh_activate(x);
        pak_sa PLSE:
            ត្រឡប់ plse_activate(x);
        pak_sa STAIR:
            ត្រឡប់ stair_activate(x);
        pak_sa HARDTAN:
            ត្រឡប់ hardtan_activate(x);
        pak_sa LHTAN:
            ត្រឡប់ lhtan_activate(x);
    }
    ត្រឡប់ 0;
}

void activate_array(float *x, const ចំនួនគត់ n, const ACTIVATION a)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ត្រឡប់ linear_gradient(x);
        pak_sa LOGISTIC:
            ត្រឡប់ logistic_gradient(x);
        pak_sa LOGGY:
            ត្រឡប់ loggy_gradient(x);
        pak_sa RELU:
            ត្រឡប់ relu_gradient(x);
        pak_sa ELU:
            ត្រឡប់ elu_gradient(x);
        pak_sa SELU:
            ត្រឡប់ selu_gradient(x);
        pak_sa RELIE:
            ត្រឡប់ relie_gradient(x);
        pak_sa RAMP:
            ត្រឡប់ ramp_gradient(x);
        pak_sa LEAKY:
            ត្រឡប់ leaky_gradient(x);
        pak_sa TANH:
            ត្រឡប់ tanh_gradient(x);
        pak_sa PLSE:
            ត្រឡប់ plse_gradient(x);
        pak_sa STAIR:
            ត្រឡប់ stair_gradient(x);
        pak_sa HARDTAN:
            ត្រឡប់ hardtan_gradient(x);
        pak_sa LHTAN:
            ត្រឡប់ lhtan_gradient(x);
    }
    ត្រឡប់ 0;
}

void gradient_array(const float *x, const ចំនួនគត់ n, const ACTIVATION a, float *delta)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

