#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ꦮꦭꦶ "logistic";
        pak_sa LOGGY:
            ꦮꦭꦶ "loggy";
        pak_sa RELU:
            ꦮꦭꦶ "relu";
        pak_sa ELU:
            ꦮꦭꦶ "elu";
        pak_sa SELU:
            ꦮꦭꦶ "selu";
        pak_sa RELIE:
            ꦮꦭꦶ "relie";
        pak_sa RAMP:
            ꦮꦭꦶ "ramp";
        pak_sa LINEAR:
            ꦮꦭꦶ "linear";
        pak_sa TANH:
            ꦮꦭꦶ "tanh";
        pak_sa PLSE:
            ꦮꦭꦶ "plse";
        pak_sa LEAKY:
            ꦮꦭꦶ "leaky";
        pak_sa STAIR:
            ꦮꦭꦶ "stair";
        pak_sa HARDTAN:
            ꦮꦭꦶ "hardtan";
        pak_sa LHTAN:
            ꦮꦭꦶ "lhtan";
        default:
            bha_nga;
    }
    ꦮꦭꦶ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ꦪꦼꦤ꧀ (strcmp(s, "logistic")==0) ꦮꦭꦶ LOGISTIC;
    ꦪꦼꦤ꧀ (strcmp(s, "loggy")==0) ꦮꦭꦶ LOGGY;
    ꦪꦼꦤ꧀ (strcmp(s, "relu")==0) ꦮꦭꦶ RELU;
    ꦪꦼꦤ꧀ (strcmp(s, "elu")==0) ꦮꦭꦶ ELU;
    ꦪꦼꦤ꧀ (strcmp(s, "selu")==0) ꦮꦭꦶ SELU;
    ꦪꦼꦤ꧀ (strcmp(s, "relie")==0) ꦮꦭꦶ RELIE;
    ꦪꦼꦤ꧀ (strcmp(s, "plse")==0) ꦮꦭꦶ PLSE;
    ꦪꦼꦤ꧀ (strcmp(s, "hardtan")==0) ꦮꦭꦶ HARDTAN;
    ꦪꦼꦤ꧀ (strcmp(s, "lhtan")==0) ꦮꦭꦶ LHTAN;
    ꦪꦼꦤ꧀ (strcmp(s, "linear")==0) ꦮꦭꦶ LINEAR;
    ꦪꦼꦤ꧀ (strcmp(s, "ramp")==0) ꦮꦭꦶ RAMP;
    ꦪꦼꦤ꧀ (strcmp(s, "leaky")==0) ꦮꦭꦶ LEAKY;
    ꦪꦼꦤ꧀ (strcmp(s, "tanh")==0) ꦮꦭꦶ TANH;
    ꦪꦼꦤ꧀ (strcmp(s, "stair")==0) ꦮꦭꦶ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ꦮꦭꦶ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ꦮꦭꦶ linear_activate(x);
        pak_sa LOGISTIC:
            ꦮꦭꦶ logistic_activate(x);
        pak_sa LOGGY:
            ꦮꦭꦶ loggy_activate(x);
        pak_sa RELU:
            ꦮꦭꦶ relu_activate(x);
        pak_sa ELU:
            ꦮꦭꦶ elu_activate(x);
        pak_sa SELU:
            ꦮꦭꦶ selu_activate(x);
        pak_sa RELIE:
            ꦮꦭꦶ relie_activate(x);
        pak_sa RAMP:
            ꦮꦭꦶ ramp_activate(x);
        pak_sa LEAKY:
            ꦮꦭꦶ leaky_activate(x);
        pak_sa TANH:
            ꦮꦭꦶ tanh_activate(x);
        pak_sa PLSE:
            ꦮꦭꦶ plse_activate(x);
        pak_sa STAIR:
            ꦮꦭꦶ stair_activate(x);
        pak_sa HARDTAN:
            ꦮꦭꦶ hardtan_activate(x);
        pak_sa LHTAN:
            ꦮꦭꦶ lhtan_activate(x);
    }
    ꦮꦭꦶ 0;
}

void activate_array(float *x, const puur_naa_nka n, const ACTIVATION a)
{
    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ꦮꦭꦶ linear_gradient(x);
        pak_sa LOGISTIC:
            ꦮꦭꦶ logistic_gradient(x);
        pak_sa LOGGY:
            ꦮꦭꦶ loggy_gradient(x);
        pak_sa RELU:
            ꦮꦭꦶ relu_gradient(x);
        pak_sa ELU:
            ꦮꦭꦶ elu_gradient(x);
        pak_sa SELU:
            ꦮꦭꦶ selu_gradient(x);
        pak_sa RELIE:
            ꦮꦭꦶ relie_gradient(x);
        pak_sa RAMP:
            ꦮꦭꦶ ramp_gradient(x);
        pak_sa LEAKY:
            ꦮꦭꦶ leaky_gradient(x);
        pak_sa TANH:
            ꦮꦭꦶ tanh_gradient(x);
        pak_sa PLSE:
            ꦮꦭꦶ plse_gradient(x);
        pak_sa STAIR:
            ꦮꦭꦶ stair_gradient(x);
        pak_sa HARDTAN:
            ꦮꦭꦶ hardtan_gradient(x);
        pak_sa LHTAN:
            ꦮꦭꦶ lhtan_gradient(x);
    }
    ꦮꦭꦶ 0;
}

void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta)
{
    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

