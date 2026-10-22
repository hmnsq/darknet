#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᱨᱩᱣᱟᱹᱲ "logistic";
        pak_sa LOGGY:
            ᱨᱩᱣᱟᱹᱲ "loggy";
        pak_sa RELU:
            ᱨᱩᱣᱟᱹᱲ "relu";
        pak_sa ELU:
            ᱨᱩᱣᱟᱹᱲ "elu";
        pak_sa SELU:
            ᱨᱩᱣᱟᱹᱲ "selu";
        pak_sa RELIE:
            ᱨᱩᱣᱟᱹᱲ "relie";
        pak_sa RAMP:
            ᱨᱩᱣᱟᱹᱲ "ramp";
        pak_sa LINEAR:
            ᱨᱩᱣᱟᱹᱲ "linear";
        pak_sa TANH:
            ᱨᱩᱣᱟᱹᱲ "tanh";
        pak_sa PLSE:
            ᱨᱩᱣᱟᱹᱲ "plse";
        pak_sa LEAKY:
            ᱨᱩᱣᱟᱹᱲ "leaky";
        pak_sa STAIR:
            ᱨᱩᱣᱟᱹᱲ "stair";
        pak_sa HARDTAN:
            ᱨᱩᱣᱟᱹᱲ "hardtan";
        pak_sa LHTAN:
            ᱨᱩᱣᱟᱹᱲ "lhtan";
        default:
            bha_nga;
    }
    ᱨᱩᱣᱟᱹᱲ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᱡᱩᱫᱤ (strcmp(s, "logistic")==0) ᱨᱩᱣᱟᱹᱲ LOGISTIC;
    ᱡᱩᱫᱤ (strcmp(s, "loggy")==0) ᱨᱩᱣᱟᱹᱲ LOGGY;
    ᱡᱩᱫᱤ (strcmp(s, "relu")==0) ᱨᱩᱣᱟᱹᱲ RELU;
    ᱡᱩᱫᱤ (strcmp(s, "elu")==0) ᱨᱩᱣᱟᱹᱲ ELU;
    ᱡᱩᱫᱤ (strcmp(s, "selu")==0) ᱨᱩᱣᱟᱹᱲ SELU;
    ᱡᱩᱫᱤ (strcmp(s, "relie")==0) ᱨᱩᱣᱟᱹᱲ RELIE;
    ᱡᱩᱫᱤ (strcmp(s, "plse")==0) ᱨᱩᱣᱟᱹᱲ PLSE;
    ᱡᱩᱫᱤ (strcmp(s, "hardtan")==0) ᱨᱩᱣᱟᱹᱲ HARDTAN;
    ᱡᱩᱫᱤ (strcmp(s, "lhtan")==0) ᱨᱩᱣᱟᱹᱲ LHTAN;
    ᱡᱩᱫᱤ (strcmp(s, "linear")==0) ᱨᱩᱣᱟᱹᱲ LINEAR;
    ᱡᱩᱫᱤ (strcmp(s, "ramp")==0) ᱨᱩᱣᱟᱹᱲ RAMP;
    ᱡᱩᱫᱤ (strcmp(s, "leaky")==0) ᱨᱩᱣᱟᱹᱲ LEAKY;
    ᱡᱩᱫᱤ (strcmp(s, "tanh")==0) ᱨᱩᱣᱟᱹᱲ TANH;
    ᱡᱩᱫᱤ (strcmp(s, "stair")==0) ᱨᱩᱣᱟᱹᱲ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᱨᱩᱣᱟᱹᱲ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᱨᱩᱣᱟᱹᱲ linear_activate(x);
        pak_sa LOGISTIC:
            ᱨᱩᱣᱟᱹᱲ logistic_activate(x);
        pak_sa LOGGY:
            ᱨᱩᱣᱟᱹᱲ loggy_activate(x);
        pak_sa RELU:
            ᱨᱩᱣᱟᱹᱲ relu_activate(x);
        pak_sa ELU:
            ᱨᱩᱣᱟᱹᱲ elu_activate(x);
        pak_sa SELU:
            ᱨᱩᱣᱟᱹᱲ selu_activate(x);
        pak_sa RELIE:
            ᱨᱩᱣᱟᱹᱲ relie_activate(x);
        pak_sa RAMP:
            ᱨᱩᱣᱟᱹᱲ ramp_activate(x);
        pak_sa LEAKY:
            ᱨᱩᱣᱟᱹᱲ leaky_activate(x);
        pak_sa TANH:
            ᱨᱩᱣᱟᱹᱲ tanh_activate(x);
        pak_sa PLSE:
            ᱨᱩᱣᱟᱹᱲ plse_activate(x);
        pak_sa STAIR:
            ᱨᱩᱣᱟᱹᱲ stair_activate(x);
        pak_sa HARDTAN:
            ᱨᱩᱣᱟᱹᱲ hardtan_activate(x);
        pak_sa LHTAN:
            ᱨᱩᱣᱟᱹᱲ lhtan_activate(x);
    }
    ᱨᱩᱣᱟᱹᱲ 0;
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
            ᱨᱩᱣᱟᱹᱲ linear_gradient(x);
        pak_sa LOGISTIC:
            ᱨᱩᱣᱟᱹᱲ logistic_gradient(x);
        pak_sa LOGGY:
            ᱨᱩᱣᱟᱹᱲ loggy_gradient(x);
        pak_sa RELU:
            ᱨᱩᱣᱟᱹᱲ relu_gradient(x);
        pak_sa ELU:
            ᱨᱩᱣᱟᱹᱲ elu_gradient(x);
        pak_sa SELU:
            ᱨᱩᱣᱟᱹᱲ selu_gradient(x);
        pak_sa RELIE:
            ᱨᱩᱣᱟᱹᱲ relie_gradient(x);
        pak_sa RAMP:
            ᱨᱩᱣᱟᱹᱲ ramp_gradient(x);
        pak_sa LEAKY:
            ᱨᱩᱣᱟᱹᱲ leaky_gradient(x);
        pak_sa TANH:
            ᱨᱩᱣᱟᱹᱲ tanh_gradient(x);
        pak_sa PLSE:
            ᱨᱩᱣᱟᱹᱲ plse_gradient(x);
        pak_sa STAIR:
            ᱨᱩᱣᱟᱹᱲ stair_gradient(x);
        pak_sa HARDTAN:
            ᱨᱩᱣᱟᱹᱲ hardtan_gradient(x);
        pak_sa LHTAN:
            ᱨᱩᱣᱟᱹᱲ lhtan_gradient(x);
    }
    ᱨᱩᱣᱟᱹᱲ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

