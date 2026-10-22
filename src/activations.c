#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᯀᯮᯞ "logistic";
        pak_sa LOGGY:
            ᯀᯮᯞ "loggy";
        pak_sa RELU:
            ᯀᯮᯞ "relu";
        pak_sa ELU:
            ᯀᯮᯞ "elu";
        pak_sa SELU:
            ᯀᯮᯞ "selu";
        pak_sa RELIE:
            ᯀᯮᯞ "relie";
        pak_sa RAMP:
            ᯀᯮᯞ "ramp";
        pak_sa LINEAR:
            ᯀᯮᯞ "linear";
        pak_sa TANH:
            ᯀᯮᯞ "tanh";
        pak_sa PLSE:
            ᯀᯮᯞ "plse";
        pak_sa LEAKY:
            ᯀᯮᯞ "leaky";
        pak_sa STAIR:
            ᯀᯮᯞ "stair";
        pak_sa HARDTAN:
            ᯀᯮᯞ "hardtan";
        pak_sa LHTAN:
            ᯀᯮᯞ "lhtan";
        default:
            bha_nga;
    }
    ᯀᯮᯞ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᯀᯮᯝ (strcmp(s, "logistic")==0) ᯀᯮᯞ LOGISTIC;
    ᯀᯮᯝ (strcmp(s, "loggy")==0) ᯀᯮᯞ LOGGY;
    ᯀᯮᯝ (strcmp(s, "relu")==0) ᯀᯮᯞ RELU;
    ᯀᯮᯝ (strcmp(s, "elu")==0) ᯀᯮᯞ ELU;
    ᯀᯮᯝ (strcmp(s, "selu")==0) ᯀᯮᯞ SELU;
    ᯀᯮᯝ (strcmp(s, "relie")==0) ᯀᯮᯞ RELIE;
    ᯀᯮᯝ (strcmp(s, "plse")==0) ᯀᯮᯞ PLSE;
    ᯀᯮᯝ (strcmp(s, "hardtan")==0) ᯀᯮᯞ HARDTAN;
    ᯀᯮᯝ (strcmp(s, "lhtan")==0) ᯀᯮᯞ LHTAN;
    ᯀᯮᯝ (strcmp(s, "linear")==0) ᯀᯮᯞ LINEAR;
    ᯀᯮᯝ (strcmp(s, "ramp")==0) ᯀᯮᯞ RAMP;
    ᯀᯮᯝ (strcmp(s, "leaky")==0) ᯀᯮᯞ LEAKY;
    ᯀᯮᯝ (strcmp(s, "tanh")==0) ᯀᯮᯞ TANH;
    ᯀᯮᯝ (strcmp(s, "stair")==0) ᯀᯮᯞ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᯀᯮᯞ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᯀᯮᯞ linear_activate(x);
        pak_sa LOGISTIC:
            ᯀᯮᯞ logistic_activate(x);
        pak_sa LOGGY:
            ᯀᯮᯞ loggy_activate(x);
        pak_sa RELU:
            ᯀᯮᯞ relu_activate(x);
        pak_sa ELU:
            ᯀᯮᯞ elu_activate(x);
        pak_sa SELU:
            ᯀᯮᯞ selu_activate(x);
        pak_sa RELIE:
            ᯀᯮᯞ relie_activate(x);
        pak_sa RAMP:
            ᯀᯮᯞ ramp_activate(x);
        pak_sa LEAKY:
            ᯀᯮᯞ leaky_activate(x);
        pak_sa TANH:
            ᯀᯮᯞ tanh_activate(x);
        pak_sa PLSE:
            ᯀᯮᯞ plse_activate(x);
        pak_sa STAIR:
            ᯀᯮᯞ stair_activate(x);
        pak_sa HARDTAN:
            ᯀᯮᯞ hardtan_activate(x);
        pak_sa LHTAN:
            ᯀᯮᯞ lhtan_activate(x);
    }
    ᯀᯮᯞ 0;
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
            ᯀᯮᯞ linear_gradient(x);
        pak_sa LOGISTIC:
            ᯀᯮᯞ logistic_gradient(x);
        pak_sa LOGGY:
            ᯀᯮᯞ loggy_gradient(x);
        pak_sa RELU:
            ᯀᯮᯞ relu_gradient(x);
        pak_sa ELU:
            ᯀᯮᯞ elu_gradient(x);
        pak_sa SELU:
            ᯀᯮᯞ selu_gradient(x);
        pak_sa RELIE:
            ᯀᯮᯞ relie_gradient(x);
        pak_sa RAMP:
            ᯀᯮᯞ ramp_gradient(x);
        pak_sa LEAKY:
            ᯀᯮᯞ leaky_gradient(x);
        pak_sa TANH:
            ᯀᯮᯞ tanh_gradient(x);
        pak_sa PLSE:
            ᯀᯮᯞ plse_gradient(x);
        pak_sa STAIR:
            ᯀᯮᯞ stair_gradient(x);
        pak_sa HARDTAN:
            ᯀᯮᯞ hardtan_gradient(x);
        pak_sa LHTAN:
            ᯀᯮᯞ lhtan_gradient(x);
    }
    ᯀᯮᯞ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

