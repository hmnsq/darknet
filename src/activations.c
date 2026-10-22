#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ཕྱིར "logistic";
        pak_sa LOGGY:
            ཕྱིར "loggy";
        pak_sa RELU:
            ཕྱིར "relu";
        pak_sa ELU:
            ཕྱིར "elu";
        pak_sa SELU:
            ཕྱིར "selu";
        pak_sa RELIE:
            ཕྱིར "relie";
        pak_sa RAMP:
            ཕྱིར "ramp";
        pak_sa LINEAR:
            ཕྱིར "linear";
        pak_sa TANH:
            ཕྱིར "tanh";
        pak_sa PLSE:
            ཕྱིར "plse";
        pak_sa LEAKY:
            ཕྱིར "leaky";
        pak_sa STAIR:
            ཕྱིར "stair";
        pak_sa HARDTAN:
            ཕྱིར "hardtan";
        pak_sa LHTAN:
            ཕྱིར "lhtan";
        default:
            bha_nga;
    }
    ཕྱིར "relu";
}

ACTIVATION get_activation(var_na *s)
{
    གལ་ཏེ (strcmp(s, "logistic")==0) ཕྱིར LOGISTIC;
    གལ་ཏེ (strcmp(s, "loggy")==0) ཕྱིར LOGGY;
    གལ་ཏེ (strcmp(s, "relu")==0) ཕྱིར RELU;
    གལ་ཏེ (strcmp(s, "elu")==0) ཕྱིར ELU;
    གལ་ཏེ (strcmp(s, "selu")==0) ཕྱིར SELU;
    གལ་ཏེ (strcmp(s, "relie")==0) ཕྱིར RELIE;
    གལ་ཏེ (strcmp(s, "plse")==0) ཕྱིར PLSE;
    གལ་ཏེ (strcmp(s, "hardtan")==0) ཕྱིར HARDTAN;
    གལ་ཏེ (strcmp(s, "lhtan")==0) ཕྱིར LHTAN;
    གལ་ཏེ (strcmp(s, "linear")==0) ཕྱིར LINEAR;
    གལ་ཏེ (strcmp(s, "ramp")==0) ཕྱིར RAMP;
    གལ་ཏེ (strcmp(s, "leaky")==0) ཕྱིར LEAKY;
    གལ་ཏེ (strcmp(s, "tanh")==0) ཕྱིར TANH;
    གལ་ཏེ (strcmp(s, "stair")==0) ཕྱིར STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ཕྱིར RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ཕྱིར linear_activate(x);
        pak_sa LOGISTIC:
            ཕྱིར logistic_activate(x);
        pak_sa LOGGY:
            ཕྱིར loggy_activate(x);
        pak_sa RELU:
            ཕྱིར relu_activate(x);
        pak_sa ELU:
            ཕྱིར elu_activate(x);
        pak_sa SELU:
            ཕྱིར selu_activate(x);
        pak_sa RELIE:
            ཕྱིར relie_activate(x);
        pak_sa RAMP:
            ཕྱིར ramp_activate(x);
        pak_sa LEAKY:
            ཕྱིར leaky_activate(x);
        pak_sa TANH:
            ཕྱིར tanh_activate(x);
        pak_sa PLSE:
            ཕྱིར plse_activate(x);
        pak_sa STAIR:
            ཕྱིར stair_activate(x);
        pak_sa HARDTAN:
            ཕྱིར hardtan_activate(x);
        pak_sa LHTAN:
            ཕྱིར lhtan_activate(x);
    }
    ཕྱིར 0;
}

void activate_array(float *x, const ཧྲིལ n, const ACTIVATION a)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ཕྱིར linear_gradient(x);
        pak_sa LOGISTIC:
            ཕྱིར logistic_gradient(x);
        pak_sa LOGGY:
            ཕྱིར loggy_gradient(x);
        pak_sa RELU:
            ཕྱིར relu_gradient(x);
        pak_sa ELU:
            ཕྱིར elu_gradient(x);
        pak_sa SELU:
            ཕྱིར selu_gradient(x);
        pak_sa RELIE:
            ཕྱིར relie_gradient(x);
        pak_sa RAMP:
            ཕྱིར ramp_gradient(x);
        pak_sa LEAKY:
            ཕྱིར leaky_gradient(x);
        pak_sa TANH:
            ཕྱིར tanh_gradient(x);
        pak_sa PLSE:
            ཕྱིར plse_gradient(x);
        pak_sa STAIR:
            ཕྱིར stair_gradient(x);
        pak_sa HARDTAN:
            ཕྱིར hardtan_gradient(x);
        pak_sa LHTAN:
            ཕྱིར lhtan_gradient(x);
    }
    ཕྱིར 0;
}

void gradient_array(const float *x, const ཧྲིལ n, const ACTIVATION a, float *delta)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

