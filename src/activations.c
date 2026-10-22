#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᨀᨘᨒ "logistic";
        pak_sa LOGGY:
            ᨀᨘᨒ "loggy";
        pak_sa RELU:
            ᨀᨘᨒ "relu";
        pak_sa ELU:
            ᨀᨘᨒ "elu";
        pak_sa SELU:
            ᨀᨘᨒ "selu";
        pak_sa RELIE:
            ᨀᨘᨒ "relie";
        pak_sa RAMP:
            ᨀᨘᨒ "ramp";
        pak_sa LINEAR:
            ᨀᨘᨒ "linear";
        pak_sa TANH:
            ᨀᨘᨒ "tanh";
        pak_sa PLSE:
            ᨀᨘᨒ "plse";
        pak_sa LEAKY:
            ᨀᨘᨒ "leaky";
        pak_sa STAIR:
            ᨀᨘᨒ "stair";
        pak_sa HARDTAN:
            ᨀᨘᨒ "hardtan";
        pak_sa LHTAN:
            ᨀᨘᨒ "lhtan";
        default:
            bha_nga;
    }
    ᨀᨘᨒ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᨀᨘ (strcmp(s, "logistic")==0) ᨀᨘᨒ LOGISTIC;
    ᨀᨘ (strcmp(s, "loggy")==0) ᨀᨘᨒ LOGGY;
    ᨀᨘ (strcmp(s, "relu")==0) ᨀᨘᨒ RELU;
    ᨀᨘ (strcmp(s, "elu")==0) ᨀᨘᨒ ELU;
    ᨀᨘ (strcmp(s, "selu")==0) ᨀᨘᨒ SELU;
    ᨀᨘ (strcmp(s, "relie")==0) ᨀᨘᨒ RELIE;
    ᨀᨘ (strcmp(s, "plse")==0) ᨀᨘᨒ PLSE;
    ᨀᨘ (strcmp(s, "hardtan")==0) ᨀᨘᨒ HARDTAN;
    ᨀᨘ (strcmp(s, "lhtan")==0) ᨀᨘᨒ LHTAN;
    ᨀᨘ (strcmp(s, "linear")==0) ᨀᨘᨒ LINEAR;
    ᨀᨘ (strcmp(s, "ramp")==0) ᨀᨘᨒ RAMP;
    ᨀᨘ (strcmp(s, "leaky")==0) ᨀᨘᨒ LEAKY;
    ᨀᨘ (strcmp(s, "tanh")==0) ᨀᨘᨒ TANH;
    ᨀᨘ (strcmp(s, "stair")==0) ᨀᨘᨒ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᨀᨘᨒ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᨀᨘᨒ linear_activate(x);
        pak_sa LOGISTIC:
            ᨀᨘᨒ logistic_activate(x);
        pak_sa LOGGY:
            ᨀᨘᨒ loggy_activate(x);
        pak_sa RELU:
            ᨀᨘᨒ relu_activate(x);
        pak_sa ELU:
            ᨀᨘᨒ elu_activate(x);
        pak_sa SELU:
            ᨀᨘᨒ selu_activate(x);
        pak_sa RELIE:
            ᨀᨘᨒ relie_activate(x);
        pak_sa RAMP:
            ᨀᨘᨒ ramp_activate(x);
        pak_sa LEAKY:
            ᨀᨘᨒ leaky_activate(x);
        pak_sa TANH:
            ᨀᨘᨒ tanh_activate(x);
        pak_sa PLSE:
            ᨀᨘᨒ plse_activate(x);
        pak_sa STAIR:
            ᨀᨘᨒ stair_activate(x);
        pak_sa HARDTAN:
            ᨀᨘᨒ hardtan_activate(x);
        pak_sa LHTAN:
            ᨀᨘᨒ lhtan_activate(x);
    }
    ᨀᨘᨒ 0;
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
            ᨀᨘᨒ linear_gradient(x);
        pak_sa LOGISTIC:
            ᨀᨘᨒ logistic_gradient(x);
        pak_sa LOGGY:
            ᨀᨘᨒ loggy_gradient(x);
        pak_sa RELU:
            ᨀᨘᨒ relu_gradient(x);
        pak_sa ELU:
            ᨀᨘᨒ elu_gradient(x);
        pak_sa SELU:
            ᨀᨘᨒ selu_gradient(x);
        pak_sa RELIE:
            ᨀᨘᨒ relie_gradient(x);
        pak_sa RAMP:
            ᨀᨘᨒ ramp_gradient(x);
        pak_sa LEAKY:
            ᨀᨘᨒ leaky_gradient(x);
        pak_sa TANH:
            ᨀᨘᨒ tanh_gradient(x);
        pak_sa PLSE:
            ᨀᨘᨒ plse_gradient(x);
        pak_sa STAIR:
            ᨀᨘᨒ stair_gradient(x);
        pak_sa HARDTAN:
            ᨀᨘᨒ hardtan_gradient(x);
        pak_sa LHTAN:
            ᨀᨘᨒ lhtan_gradient(x);
    }
    ᨀᨘᨒ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

