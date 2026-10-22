#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ပြန်ပေး "logistic";
        pak_sa LOGGY:
            ပြန်ပေး "loggy";
        pak_sa RELU:
            ပြန်ပေး "relu";
        pak_sa ELU:
            ပြန်ပေး "elu";
        pak_sa SELU:
            ပြန်ပေး "selu";
        pak_sa RELIE:
            ပြန်ပေး "relie";
        pak_sa RAMP:
            ပြန်ပေး "ramp";
        pak_sa LINEAR:
            ပြန်ပေး "linear";
        pak_sa TANH:
            ပြန်ပေး "tanh";
        pak_sa PLSE:
            ပြန်ပေး "plse";
        pak_sa LEAKY:
            ပြန်ပေး "leaky";
        pak_sa STAIR:
            ပြန်ပေး "stair";
        pak_sa HARDTAN:
            ပြန်ပေး "hardtan";
        pak_sa LHTAN:
            ပြန်ပေး "lhtan";
        default:
            bha_nga;
    }
    ပြန်ပေး "relu";
}

ACTIVATION get_activation(var_na *s)
{
    အကယ်၍ (strcmp(s, "logistic")==0) ပြန်ပေး LOGISTIC;
    အကယ်၍ (strcmp(s, "loggy")==0) ပြန်ပေး LOGGY;
    အကယ်၍ (strcmp(s, "relu")==0) ပြန်ပေး RELU;
    အကယ်၍ (strcmp(s, "elu")==0) ပြန်ပေး ELU;
    အကယ်၍ (strcmp(s, "selu")==0) ပြန်ပေး SELU;
    အကယ်၍ (strcmp(s, "relie")==0) ပြန်ပေး RELIE;
    အကယ်၍ (strcmp(s, "plse")==0) ပြန်ပေး PLSE;
    အကယ်၍ (strcmp(s, "hardtan")==0) ပြန်ပေး HARDTAN;
    အကယ်၍ (strcmp(s, "lhtan")==0) ပြန်ပေး LHTAN;
    အကယ်၍ (strcmp(s, "linear")==0) ပြန်ပေး LINEAR;
    အကယ်၍ (strcmp(s, "ramp")==0) ပြန်ပေး RAMP;
    အကယ်၍ (strcmp(s, "leaky")==0) ပြန်ပေး LEAKY;
    အကယ်၍ (strcmp(s, "tanh")==0) ပြန်ပေး TANH;
    အကယ်၍ (strcmp(s, "stair")==0) ပြန်ပေး STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ပြန်ပေး RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ပြန်ပေး linear_activate(x);
        pak_sa LOGISTIC:
            ပြန်ပေး logistic_activate(x);
        pak_sa LOGGY:
            ပြန်ပေး loggy_activate(x);
        pak_sa RELU:
            ပြန်ပေး relu_activate(x);
        pak_sa ELU:
            ပြန်ပေး elu_activate(x);
        pak_sa SELU:
            ပြန်ပေး selu_activate(x);
        pak_sa RELIE:
            ပြန်ပေး relie_activate(x);
        pak_sa RAMP:
            ပြန်ပေး ramp_activate(x);
        pak_sa LEAKY:
            ပြန်ပေး leaky_activate(x);
        pak_sa TANH:
            ပြန်ပေး tanh_activate(x);
        pak_sa PLSE:
            ပြန်ပေး plse_activate(x);
        pak_sa STAIR:
            ပြန်ပေး stair_activate(x);
        pak_sa HARDTAN:
            ပြန်ပေး hardtan_activate(x);
        pak_sa LHTAN:
            ပြန်ပေး lhtan_activate(x);
    }
    ပြန်ပေး 0;
}

void activate_array(float *x, const ကိန်းပြည့် n, const ACTIVATION a)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ပြန်ပေး linear_gradient(x);
        pak_sa LOGISTIC:
            ပြန်ပေး logistic_gradient(x);
        pak_sa LOGGY:
            ပြန်ပေး loggy_gradient(x);
        pak_sa RELU:
            ပြန်ပေး relu_gradient(x);
        pak_sa ELU:
            ပြန်ပေး elu_gradient(x);
        pak_sa SELU:
            ပြန်ပေး selu_gradient(x);
        pak_sa RELIE:
            ပြန်ပေး relie_gradient(x);
        pak_sa RAMP:
            ပြန်ပေး ramp_gradient(x);
        pak_sa LEAKY:
            ပြန်ပေး leaky_gradient(x);
        pak_sa TANH:
            ပြန်ပေး tanh_gradient(x);
        pak_sa PLSE:
            ပြန်ပေး plse_gradient(x);
        pak_sa STAIR:
            ပြန်ပေး stair_gradient(x);
        pak_sa HARDTAN:
            ပြန်ပေး hardtan_gradient(x);
        pak_sa LHTAN:
            ပြန်ပေး lhtan_gradient(x);
    }
    ပြန်ပေး 0;
}

void gradient_array(const float *x, const ကိန်းပြည့် n, const ACTIVATION a, float *delta)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

