#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ပွၵ်ႈ "logistic";
        pak_sa LOGGY:
            ပွၵ်ႈ "loggy";
        pak_sa RELU:
            ပွၵ်ႈ "relu";
        pak_sa ELU:
            ပွၵ်ႈ "elu";
        pak_sa SELU:
            ပွၵ်ႈ "selu";
        pak_sa RELIE:
            ပွၵ်ႈ "relie";
        pak_sa RAMP:
            ပွၵ်ႈ "ramp";
        pak_sa LINEAR:
            ပွၵ်ႈ "linear";
        pak_sa TANH:
            ပွၵ်ႈ "tanh";
        pak_sa PLSE:
            ပွၵ်ႈ "plse";
        pak_sa LEAKY:
            ပွၵ်ႈ "leaky";
        pak_sa STAIR:
            ပွၵ်ႈ "stair";
        pak_sa HARDTAN:
            ပွၵ်ႈ "hardtan";
        pak_sa LHTAN:
            ပွၵ်ႈ "lhtan";
        default:
            bha_nga;
    }
    ပွၵ်ႈ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ပေႃး (strcmp(s, "logistic")==0) ပွၵ်ႈ LOGISTIC;
    ပေႃး (strcmp(s, "loggy")==0) ပွၵ်ႈ LOGGY;
    ပေႃး (strcmp(s, "relu")==0) ပွၵ်ႈ RELU;
    ပေႃး (strcmp(s, "elu")==0) ပွၵ်ႈ ELU;
    ပေႃး (strcmp(s, "selu")==0) ပွၵ်ႈ SELU;
    ပေႃး (strcmp(s, "relie")==0) ပွၵ်ႈ RELIE;
    ပေႃး (strcmp(s, "plse")==0) ပွၵ်ႈ PLSE;
    ပေႃး (strcmp(s, "hardtan")==0) ပွၵ်ႈ HARDTAN;
    ပေႃး (strcmp(s, "lhtan")==0) ပွၵ်ႈ LHTAN;
    ပေႃး (strcmp(s, "linear")==0) ပွၵ်ႈ LINEAR;
    ပေႃး (strcmp(s, "ramp")==0) ပွၵ်ႈ RAMP;
    ပေႃး (strcmp(s, "leaky")==0) ပွၵ်ႈ LEAKY;
    ပေႃး (strcmp(s, "tanh")==0) ပွၵ်ႈ TANH;
    ပေႃး (strcmp(s, "stair")==0) ပွၵ်ႈ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ပွၵ်ႈ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ပွၵ်ႈ linear_activate(x);
        pak_sa LOGISTIC:
            ပွၵ်ႈ logistic_activate(x);
        pak_sa LOGGY:
            ပွၵ်ႈ loggy_activate(x);
        pak_sa RELU:
            ပွၵ်ႈ relu_activate(x);
        pak_sa ELU:
            ပွၵ်ႈ elu_activate(x);
        pak_sa SELU:
            ပွၵ်ႈ selu_activate(x);
        pak_sa RELIE:
            ပွၵ်ႈ relie_activate(x);
        pak_sa RAMP:
            ပွၵ်ႈ ramp_activate(x);
        pak_sa LEAKY:
            ပွၵ်ႈ leaky_activate(x);
        pak_sa TANH:
            ပွၵ်ႈ tanh_activate(x);
        pak_sa PLSE:
            ပွၵ်ႈ plse_activate(x);
        pak_sa STAIR:
            ပွၵ်ႈ stair_activate(x);
        pak_sa HARDTAN:
            ပွၵ်ႈ hardtan_activate(x);
        pak_sa LHTAN:
            ပွၵ်ႈ lhtan_activate(x);
    }
    ပွၵ်ႈ 0;
}

void activate_array(float *x, const puur_naa_nka n, const ACTIVATION a)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ပွၵ်ႈ linear_gradient(x);
        pak_sa LOGISTIC:
            ပွၵ်ႈ logistic_gradient(x);
        pak_sa LOGGY:
            ပွၵ်ႈ loggy_gradient(x);
        pak_sa RELU:
            ပွၵ်ႈ relu_gradient(x);
        pak_sa ELU:
            ပွၵ်ႈ elu_gradient(x);
        pak_sa SELU:
            ပွၵ်ႈ selu_gradient(x);
        pak_sa RELIE:
            ပွၵ်ႈ relie_gradient(x);
        pak_sa RAMP:
            ပွၵ်ႈ ramp_gradient(x);
        pak_sa LEAKY:
            ပွၵ်ႈ leaky_gradient(x);
        pak_sa TANH:
            ပွၵ်ႈ tanh_gradient(x);
        pak_sa PLSE:
            ပွၵ်ႈ plse_gradient(x);
        pak_sa STAIR:
            ပွၵ်ႈ stair_gradient(x);
        pak_sa HARDTAN:
            ပွၵ်ႈ hardtan_gradient(x);
        pak_sa LHTAN:
            ပွၵ်ႈ lhtan_gradient(x);
    }
    ပွၵ်ႈ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

