#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ལོག "logistic";
        pak_sa LOGGY:
            ལོག "loggy";
        pak_sa RELU:
            ལོག "relu";
        pak_sa ELU:
            ལོག "elu";
        pak_sa SELU:
            ལོག "selu";
        pak_sa RELIE:
            ལོག "relie";
        pak_sa RAMP:
            ལོག "ramp";
        pak_sa LINEAR:
            ལོག "linear";
        pak_sa TANH:
            ལོག "tanh";
        pak_sa PLSE:
            ལོག "plse";
        pak_sa LEAKY:
            ལོག "leaky";
        pak_sa STAIR:
            ལོག "stair";
        pak_sa HARDTAN:
            ལོག "hardtan";
        pak_sa LHTAN:
            ལོག "lhtan";
        default:
            bha_nga;
    }
    ལོག "relu";
}

ACTIVATION get_activation(var_na *s)
{
    གལ་སྲིད (strcmp(s, "logistic")==0) ལོག LOGISTIC;
    གལ་སྲིད (strcmp(s, "loggy")==0) ལོག LOGGY;
    གལ་སྲིད (strcmp(s, "relu")==0) ལོག RELU;
    གལ་སྲིད (strcmp(s, "elu")==0) ལོག ELU;
    གལ་སྲིད (strcmp(s, "selu")==0) ལོག SELU;
    གལ་སྲིད (strcmp(s, "relie")==0) ལོག RELIE;
    གལ་སྲིད (strcmp(s, "plse")==0) ལོག PLSE;
    གལ་སྲིད (strcmp(s, "hardtan")==0) ལོག HARDTAN;
    གལ་སྲིད (strcmp(s, "lhtan")==0) ལོག LHTAN;
    གལ་སྲིད (strcmp(s, "linear")==0) ལོག LINEAR;
    གལ་སྲིད (strcmp(s, "ramp")==0) ལོག RAMP;
    གལ་སྲིད (strcmp(s, "leaky")==0) ལོག LEAKY;
    གལ་སྲིད (strcmp(s, "tanh")==0) ལོག TANH;
    གལ་སྲིད (strcmp(s, "stair")==0) ལོག STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ལོག RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ལོག linear_activate(x);
        pak_sa LOGISTIC:
            ལོག logistic_activate(x);
        pak_sa LOGGY:
            ལོག loggy_activate(x);
        pak_sa RELU:
            ལོག relu_activate(x);
        pak_sa ELU:
            ལོག elu_activate(x);
        pak_sa SELU:
            ལོག selu_activate(x);
        pak_sa RELIE:
            ལོག relie_activate(x);
        pak_sa RAMP:
            ལོག ramp_activate(x);
        pak_sa LEAKY:
            ལོག leaky_activate(x);
        pak_sa TANH:
            ལོག tanh_activate(x);
        pak_sa PLSE:
            ལོག plse_activate(x);
        pak_sa STAIR:
            ལོག stair_activate(x);
        pak_sa HARDTAN:
            ལོག hardtan_activate(x);
        pak_sa LHTAN:
            ལོག lhtan_activate(x);
    }
    ལོག 0;
}

void activate_array(float *x, const ཧྲིལ n, const ACTIVATION a)
{
    ཧྲིལ i;
    དོན་ལུ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ལོག linear_gradient(x);
        pak_sa LOGISTIC:
            ལོག logistic_gradient(x);
        pak_sa LOGGY:
            ལོག loggy_gradient(x);
        pak_sa RELU:
            ལོག relu_gradient(x);
        pak_sa ELU:
            ལོག elu_gradient(x);
        pak_sa SELU:
            ལོག selu_gradient(x);
        pak_sa RELIE:
            ལོག relie_gradient(x);
        pak_sa RAMP:
            ལོག ramp_gradient(x);
        pak_sa LEAKY:
            ལོག leaky_gradient(x);
        pak_sa TANH:
            ལོག tanh_gradient(x);
        pak_sa PLSE:
            ལོག plse_gradient(x);
        pak_sa STAIR:
            ལོག stair_gradient(x);
        pak_sa HARDTAN:
            ལོག hardtan_gradient(x);
        pak_sa LHTAN:
            ལོག lhtan_gradient(x);
    }
    ལོག 0;
}

void gradient_array(const float *x, const ཧྲིལ n, const ACTIVATION a, float *delta)
{
    ཧྲིལ i;
    དོན་ལུ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

