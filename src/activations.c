#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᰀᰧᰌ "logistic";
        pak_sa LOGGY:
            ᰀᰧᰌ "loggy";
        pak_sa RELU:
            ᰀᰧᰌ "relu";
        pak_sa ELU:
            ᰀᰧᰌ "elu";
        pak_sa SELU:
            ᰀᰧᰌ "selu";
        pak_sa RELIE:
            ᰀᰧᰌ "relie";
        pak_sa RAMP:
            ᰀᰧᰌ "ramp";
        pak_sa LINEAR:
            ᰀᰧᰌ "linear";
        pak_sa TANH:
            ᰀᰧᰌ "tanh";
        pak_sa PLSE:
            ᰀᰧᰌ "plse";
        pak_sa LEAKY:
            ᰀᰧᰌ "leaky";
        pak_sa STAIR:
            ᰀᰧᰌ "stair";
        pak_sa HARDTAN:
            ᰀᰧᰌ "hardtan";
        pak_sa LHTAN:
            ᰀᰧᰌ "lhtan";
        default:
            bha_nga;
    }
    ᰀᰧᰌ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᰀᰧ (strcmp(s, "logistic")==0) ᰀᰧᰌ LOGISTIC;
    ᰀᰧ (strcmp(s, "loggy")==0) ᰀᰧᰌ LOGGY;
    ᰀᰧ (strcmp(s, "relu")==0) ᰀᰧᰌ RELU;
    ᰀᰧ (strcmp(s, "elu")==0) ᰀᰧᰌ ELU;
    ᰀᰧ (strcmp(s, "selu")==0) ᰀᰧᰌ SELU;
    ᰀᰧ (strcmp(s, "relie")==0) ᰀᰧᰌ RELIE;
    ᰀᰧ (strcmp(s, "plse")==0) ᰀᰧᰌ PLSE;
    ᰀᰧ (strcmp(s, "hardtan")==0) ᰀᰧᰌ HARDTAN;
    ᰀᰧ (strcmp(s, "lhtan")==0) ᰀᰧᰌ LHTAN;
    ᰀᰧ (strcmp(s, "linear")==0) ᰀᰧᰌ LINEAR;
    ᰀᰧ (strcmp(s, "ramp")==0) ᰀᰧᰌ RAMP;
    ᰀᰧ (strcmp(s, "leaky")==0) ᰀᰧᰌ LEAKY;
    ᰀᰧ (strcmp(s, "tanh")==0) ᰀᰧᰌ TANH;
    ᰀᰧ (strcmp(s, "stair")==0) ᰀᰧᰌ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᰀᰧᰌ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᰀᰧᰌ linear_activate(x);
        pak_sa LOGISTIC:
            ᰀᰧᰌ logistic_activate(x);
        pak_sa LOGGY:
            ᰀᰧᰌ loggy_activate(x);
        pak_sa RELU:
            ᰀᰧᰌ relu_activate(x);
        pak_sa ELU:
            ᰀᰧᰌ elu_activate(x);
        pak_sa SELU:
            ᰀᰧᰌ selu_activate(x);
        pak_sa RELIE:
            ᰀᰧᰌ relie_activate(x);
        pak_sa RAMP:
            ᰀᰧᰌ ramp_activate(x);
        pak_sa LEAKY:
            ᰀᰧᰌ leaky_activate(x);
        pak_sa TANH:
            ᰀᰧᰌ tanh_activate(x);
        pak_sa PLSE:
            ᰀᰧᰌ plse_activate(x);
        pak_sa STAIR:
            ᰀᰧᰌ stair_activate(x);
        pak_sa HARDTAN:
            ᰀᰧᰌ hardtan_activate(x);
        pak_sa LHTAN:
            ᰀᰧᰌ lhtan_activate(x);
    }
    ᰀᰧᰌ 0;
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
            ᰀᰧᰌ linear_gradient(x);
        pak_sa LOGISTIC:
            ᰀᰧᰌ logistic_gradient(x);
        pak_sa LOGGY:
            ᰀᰧᰌ loggy_gradient(x);
        pak_sa RELU:
            ᰀᰧᰌ relu_gradient(x);
        pak_sa ELU:
            ᰀᰧᰌ elu_gradient(x);
        pak_sa SELU:
            ᰀᰧᰌ selu_gradient(x);
        pak_sa RELIE:
            ᰀᰧᰌ relie_gradient(x);
        pak_sa RAMP:
            ᰀᰧᰌ ramp_gradient(x);
        pak_sa LEAKY:
            ᰀᰧᰌ leaky_gradient(x);
        pak_sa TANH:
            ᰀᰧᰌ tanh_gradient(x);
        pak_sa PLSE:
            ᰀᰧᰌ plse_gradient(x);
        pak_sa STAIR:
            ᰀᰧᰌ stair_gradient(x);
        pak_sa HARDTAN:
            ᰀᰧᰌ hardtan_gradient(x);
        pak_sa LHTAN:
            ᰀᰧᰌ lhtan_gradient(x);
    }
    ᰀᰧᰌ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

