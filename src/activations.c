#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᬯᬮᬶ "logistic";
        pak_sa LOGGY:
            ᬯᬮᬶ "loggy";
        pak_sa RELU:
            ᬯᬮᬶ "relu";
        pak_sa ELU:
            ᬯᬮᬶ "elu";
        pak_sa SELU:
            ᬯᬮᬶ "selu";
        pak_sa RELIE:
            ᬯᬮᬶ "relie";
        pak_sa RAMP:
            ᬯᬮᬶ "ramp";
        pak_sa LINEAR:
            ᬯᬮᬶ "linear";
        pak_sa TANH:
            ᬯᬮᬶ "tanh";
        pak_sa PLSE:
            ᬯᬮᬶ "plse";
        pak_sa LEAKY:
            ᬯᬮᬶ "leaky";
        pak_sa STAIR:
            ᬯᬮᬶ "stair";
        pak_sa HARDTAN:
            ᬯᬮᬶ "hardtan";
        pak_sa LHTAN:
            ᬯᬮᬶ "lhtan";
        default:
            bha_nga;
    }
    ᬯᬮᬶ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᬬᬾᬦ᭄ (strcmp(s, "logistic")==0) ᬯᬮᬶ LOGISTIC;
    ᬬᬾᬦ᭄ (strcmp(s, "loggy")==0) ᬯᬮᬶ LOGGY;
    ᬬᬾᬦ᭄ (strcmp(s, "relu")==0) ᬯᬮᬶ RELU;
    ᬬᬾᬦ᭄ (strcmp(s, "elu")==0) ᬯᬮᬶ ELU;
    ᬬᬾᬦ᭄ (strcmp(s, "selu")==0) ᬯᬮᬶ SELU;
    ᬬᬾᬦ᭄ (strcmp(s, "relie")==0) ᬯᬮᬶ RELIE;
    ᬬᬾᬦ᭄ (strcmp(s, "plse")==0) ᬯᬮᬶ PLSE;
    ᬬᬾᬦ᭄ (strcmp(s, "hardtan")==0) ᬯᬮᬶ HARDTAN;
    ᬬᬾᬦ᭄ (strcmp(s, "lhtan")==0) ᬯᬮᬶ LHTAN;
    ᬬᬾᬦ᭄ (strcmp(s, "linear")==0) ᬯᬮᬶ LINEAR;
    ᬬᬾᬦ᭄ (strcmp(s, "ramp")==0) ᬯᬮᬶ RAMP;
    ᬬᬾᬦ᭄ (strcmp(s, "leaky")==0) ᬯᬮᬶ LEAKY;
    ᬬᬾᬦ᭄ (strcmp(s, "tanh")==0) ᬯᬮᬶ TANH;
    ᬬᬾᬦ᭄ (strcmp(s, "stair")==0) ᬯᬮᬶ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᬯᬮᬶ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᬯᬮᬶ linear_activate(x);
        pak_sa LOGISTIC:
            ᬯᬮᬶ logistic_activate(x);
        pak_sa LOGGY:
            ᬯᬮᬶ loggy_activate(x);
        pak_sa RELU:
            ᬯᬮᬶ relu_activate(x);
        pak_sa ELU:
            ᬯᬮᬶ elu_activate(x);
        pak_sa SELU:
            ᬯᬮᬶ selu_activate(x);
        pak_sa RELIE:
            ᬯᬮᬶ relie_activate(x);
        pak_sa RAMP:
            ᬯᬮᬶ ramp_activate(x);
        pak_sa LEAKY:
            ᬯᬮᬶ leaky_activate(x);
        pak_sa TANH:
            ᬯᬮᬶ tanh_activate(x);
        pak_sa PLSE:
            ᬯᬮᬶ plse_activate(x);
        pak_sa STAIR:
            ᬯᬮᬶ stair_activate(x);
        pak_sa HARDTAN:
            ᬯᬮᬶ hardtan_activate(x);
        pak_sa LHTAN:
            ᬯᬮᬶ lhtan_activate(x);
    }
    ᬯᬮᬶ 0;
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
            ᬯᬮᬶ linear_gradient(x);
        pak_sa LOGISTIC:
            ᬯᬮᬶ logistic_gradient(x);
        pak_sa LOGGY:
            ᬯᬮᬶ loggy_gradient(x);
        pak_sa RELU:
            ᬯᬮᬶ relu_gradient(x);
        pak_sa ELU:
            ᬯᬮᬶ elu_gradient(x);
        pak_sa SELU:
            ᬯᬮᬶ selu_gradient(x);
        pak_sa RELIE:
            ᬯᬮᬶ relie_gradient(x);
        pak_sa RAMP:
            ᬯᬮᬶ ramp_gradient(x);
        pak_sa LEAKY:
            ᬯᬮᬶ leaky_gradient(x);
        pak_sa TANH:
            ᬯᬮᬶ tanh_gradient(x);
        pak_sa PLSE:
            ᬯᬮᬶ plse_gradient(x);
        pak_sa STAIR:
            ᬯᬮᬶ stair_gradient(x);
        pak_sa HARDTAN:
            ᬯᬮᬶ hardtan_gradient(x);
        pak_sa LHTAN:
            ᬯᬮᬶ lhtan_gradient(x);
    }
    ᬯᬮᬶ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

