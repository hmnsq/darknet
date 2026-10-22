#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ᮝᮜᮤ "logistic";
        pak_sa LOGGY:
            ᮝᮜᮤ "loggy";
        pak_sa RELU:
            ᮝᮜᮤ "relu";
        pak_sa ELU:
            ᮝᮜᮤ "elu";
        pak_sa SELU:
            ᮝᮜᮤ "selu";
        pak_sa RELIE:
            ᮝᮜᮤ "relie";
        pak_sa RAMP:
            ᮝᮜᮤ "ramp";
        pak_sa LINEAR:
            ᮝᮜᮤ "linear";
        pak_sa TANH:
            ᮝᮜᮤ "tanh";
        pak_sa PLSE:
            ᮝᮜᮤ "plse";
        pak_sa LEAKY:
            ᮝᮜᮤ "leaky";
        pak_sa STAIR:
            ᮝᮜᮤ "stair";
        pak_sa HARDTAN:
            ᮝᮜᮤ "hardtan";
        pak_sa LHTAN:
            ᮝᮜᮤ "lhtan";
        default:
            bha_nga;
    }
    ᮝᮜᮤ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ᮚᮦᮔ᮪ (strcmp(s, "logistic")==0) ᮝᮜᮤ LOGISTIC;
    ᮚᮦᮔ᮪ (strcmp(s, "loggy")==0) ᮝᮜᮤ LOGGY;
    ᮚᮦᮔ᮪ (strcmp(s, "relu")==0) ᮝᮜᮤ RELU;
    ᮚᮦᮔ᮪ (strcmp(s, "elu")==0) ᮝᮜᮤ ELU;
    ᮚᮦᮔ᮪ (strcmp(s, "selu")==0) ᮝᮜᮤ SELU;
    ᮚᮦᮔ᮪ (strcmp(s, "relie")==0) ᮝᮜᮤ RELIE;
    ᮚᮦᮔ᮪ (strcmp(s, "plse")==0) ᮝᮜᮤ PLSE;
    ᮚᮦᮔ᮪ (strcmp(s, "hardtan")==0) ᮝᮜᮤ HARDTAN;
    ᮚᮦᮔ᮪ (strcmp(s, "lhtan")==0) ᮝᮜᮤ LHTAN;
    ᮚᮦᮔ᮪ (strcmp(s, "linear")==0) ᮝᮜᮤ LINEAR;
    ᮚᮦᮔ᮪ (strcmp(s, "ramp")==0) ᮝᮜᮤ RAMP;
    ᮚᮦᮔ᮪ (strcmp(s, "leaky")==0) ᮝᮜᮤ LEAKY;
    ᮚᮦᮔ᮪ (strcmp(s, "tanh")==0) ᮝᮜᮤ TANH;
    ᮚᮦᮔ᮪ (strcmp(s, "stair")==0) ᮝᮜᮤ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ᮝᮜᮤ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ᮝᮜᮤ linear_activate(x);
        pak_sa LOGISTIC:
            ᮝᮜᮤ logistic_activate(x);
        pak_sa LOGGY:
            ᮝᮜᮤ loggy_activate(x);
        pak_sa RELU:
            ᮝᮜᮤ relu_activate(x);
        pak_sa ELU:
            ᮝᮜᮤ elu_activate(x);
        pak_sa SELU:
            ᮝᮜᮤ selu_activate(x);
        pak_sa RELIE:
            ᮝᮜᮤ relie_activate(x);
        pak_sa RAMP:
            ᮝᮜᮤ ramp_activate(x);
        pak_sa LEAKY:
            ᮝᮜᮤ leaky_activate(x);
        pak_sa TANH:
            ᮝᮜᮤ tanh_activate(x);
        pak_sa PLSE:
            ᮝᮜᮤ plse_activate(x);
        pak_sa STAIR:
            ᮝᮜᮤ stair_activate(x);
        pak_sa HARDTAN:
            ᮝᮜᮤ hardtan_activate(x);
        pak_sa LHTAN:
            ᮝᮜᮤ lhtan_activate(x);
    }
    ᮝᮜᮤ 0;
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
            ᮝᮜᮤ linear_gradient(x);
        pak_sa LOGISTIC:
            ᮝᮜᮤ logistic_gradient(x);
        pak_sa LOGGY:
            ᮝᮜᮤ loggy_gradient(x);
        pak_sa RELU:
            ᮝᮜᮤ relu_gradient(x);
        pak_sa ELU:
            ᮝᮜᮤ elu_gradient(x);
        pak_sa SELU:
            ᮝᮜᮤ selu_gradient(x);
        pak_sa RELIE:
            ᮝᮜᮤ relie_gradient(x);
        pak_sa RAMP:
            ᮝᮜᮤ ramp_gradient(x);
        pak_sa LEAKY:
            ᮝᮜᮤ leaky_gradient(x);
        pak_sa TANH:
            ᮝᮜᮤ tanh_gradient(x);
        pak_sa PLSE:
            ᮝᮜᮤ plse_gradient(x);
        pak_sa STAIR:
            ᮝᮜᮤ stair_gradient(x);
        pak_sa HARDTAN:
            ᮝᮜᮤ hardtan_gradient(x);
        pak_sa LHTAN:
            ᮝᮜᮤ lhtan_gradient(x);
    }
    ᮝᮜᮤ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

