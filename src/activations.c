#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᤀᤡᤴ "logistic";
        pak_sa LOGGY:
            ᤀᤡᤴ "loggy";
        pak_sa RELU:
            ᤀᤡᤴ "relu";
        pak_sa ELU:
            ᤀᤡᤴ "elu";
        pak_sa SELU:
            ᤀᤡᤴ "selu";
        pak_sa RELIE:
            ᤀᤡᤴ "relie";
        pak_sa RAMP:
            ᤀᤡᤴ "ramp";
        pak_sa LINEAR:
            ᤀᤡᤴ "linear";
        pak_sa TANH:
            ᤀᤡᤴ "tanh";
        pak_sa PLSE:
            ᤀᤡᤴ "plse";
        pak_sa LEAKY:
            ᤀᤡᤴ "leaky";
        pak_sa STAIR:
            ᤀᤡᤴ "stair";
        pak_sa HARDTAN:
            ᤀᤡᤴ "hardtan";
        pak_sa LHTAN:
            ᤀᤡᤴ "lhtan";
        default:
            bha_nga;
    }
    ᤀᤡᤴ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᤀᤡ᤺ (strcmp(s, "logistic")==0) ᤀᤡᤴ LOGISTIC;
    ᤀᤡ᤺ (strcmp(s, "loggy")==0) ᤀᤡᤴ LOGGY;
    ᤀᤡ᤺ (strcmp(s, "relu")==0) ᤀᤡᤴ RELU;
    ᤀᤡ᤺ (strcmp(s, "elu")==0) ᤀᤡᤴ ELU;
    ᤀᤡ᤺ (strcmp(s, "selu")==0) ᤀᤡᤴ SELU;
    ᤀᤡ᤺ (strcmp(s, "relie")==0) ᤀᤡᤴ RELIE;
    ᤀᤡ᤺ (strcmp(s, "plse")==0) ᤀᤡᤴ PLSE;
    ᤀᤡ᤺ (strcmp(s, "hardtan")==0) ᤀᤡᤴ HARDTAN;
    ᤀᤡ᤺ (strcmp(s, "lhtan")==0) ᤀᤡᤴ LHTAN;
    ᤀᤡ᤺ (strcmp(s, "linear")==0) ᤀᤡᤴ LINEAR;
    ᤀᤡ᤺ (strcmp(s, "ramp")==0) ᤀᤡᤴ RAMP;
    ᤀᤡ᤺ (strcmp(s, "leaky")==0) ᤀᤡᤴ LEAKY;
    ᤀᤡ᤺ (strcmp(s, "tanh")==0) ᤀᤡᤴ TANH;
    ᤀᤡ᤺ (strcmp(s, "stair")==0) ᤀᤡᤴ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᤀᤡᤴ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᤀᤡᤴ linear_activate(x);
        pak_sa LOGISTIC:
            ᤀᤡᤴ logistic_activate(x);
        pak_sa LOGGY:
            ᤀᤡᤴ loggy_activate(x);
        pak_sa RELU:
            ᤀᤡᤴ relu_activate(x);
        pak_sa ELU:
            ᤀᤡᤴ elu_activate(x);
        pak_sa SELU:
            ᤀᤡᤴ selu_activate(x);
        pak_sa RELIE:
            ᤀᤡᤴ relie_activate(x);
        pak_sa RAMP:
            ᤀᤡᤴ ramp_activate(x);
        pak_sa LEAKY:
            ᤀᤡᤴ leaky_activate(x);
        pak_sa TANH:
            ᤀᤡᤴ tanh_activate(x);
        pak_sa PLSE:
            ᤀᤡᤴ plse_activate(x);
        pak_sa STAIR:
            ᤀᤡᤴ stair_activate(x);
        pak_sa HARDTAN:
            ᤀᤡᤴ hardtan_activate(x);
        pak_sa LHTAN:
            ᤀᤡᤴ lhtan_activate(x);
    }
    ᤀᤡᤴ 0;
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
            ᤀᤡᤴ linear_gradient(x);
        pak_sa LOGISTIC:
            ᤀᤡᤴ logistic_gradient(x);
        pak_sa LOGGY:
            ᤀᤡᤴ loggy_gradient(x);
        pak_sa RELU:
            ᤀᤡᤴ relu_gradient(x);
        pak_sa ELU:
            ᤀᤡᤴ elu_gradient(x);
        pak_sa SELU:
            ᤀᤡᤴ selu_gradient(x);
        pak_sa RELIE:
            ᤀᤡᤴ relie_gradient(x);
        pak_sa RAMP:
            ᤀᤡᤴ ramp_gradient(x);
        pak_sa LEAKY:
            ᤀᤡᤴ leaky_gradient(x);
        pak_sa TANH:
            ᤀᤡᤴ tanh_gradient(x);
        pak_sa PLSE:
            ᤀᤡᤴ plse_gradient(x);
        pak_sa STAIR:
            ᤀᤡᤴ stair_gradient(x);
        pak_sa HARDTAN:
            ᤀᤡᤴ hardtan_gradient(x);
        pak_sa LHTAN:
            ᤀᤡᤴ lhtan_gradient(x);
    }
    ᤀᤡᤴ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

