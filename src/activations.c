#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ສົ່ງຄືນ "logistic";
        pak_sa LOGGY:
            ສົ່ງຄືນ "loggy";
        pak_sa RELU:
            ສົ່ງຄືນ "relu";
        pak_sa ELU:
            ສົ່ງຄືນ "elu";
        pak_sa SELU:
            ສົ່ງຄືນ "selu";
        pak_sa RELIE:
            ສົ່ງຄືນ "relie";
        pak_sa RAMP:
            ສົ່ງຄືນ "ramp";
        pak_sa LINEAR:
            ສົ່ງຄືນ "linear";
        pak_sa TANH:
            ສົ່ງຄືນ "tanh";
        pak_sa PLSE:
            ສົ່ງຄືນ "plse";
        pak_sa LEAKY:
            ສົ່ງຄືນ "leaky";
        pak_sa STAIR:
            ສົ່ງຄືນ "stair";
        pak_sa HARDTAN:
            ສົ່ງຄືນ "hardtan";
        pak_sa LHTAN:
            ສົ່ງຄືນ "lhtan";
        default:
            bha_nga;
    }
    ສົ່ງຄືນ "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ຖ້າ (strcmp(s, "logistic")==0) ສົ່ງຄືນ LOGISTIC;
    ຖ້າ (strcmp(s, "loggy")==0) ສົ່ງຄືນ LOGGY;
    ຖ້າ (strcmp(s, "relu")==0) ສົ່ງຄືນ RELU;
    ຖ້າ (strcmp(s, "elu")==0) ສົ່ງຄືນ ELU;
    ຖ້າ (strcmp(s, "selu")==0) ສົ່ງຄືນ SELU;
    ຖ້າ (strcmp(s, "relie")==0) ສົ່ງຄືນ RELIE;
    ຖ້າ (strcmp(s, "plse")==0) ສົ່ງຄືນ PLSE;
    ຖ້າ (strcmp(s, "hardtan")==0) ສົ່ງຄືນ HARDTAN;
    ຖ້າ (strcmp(s, "lhtan")==0) ສົ່ງຄືນ LHTAN;
    ຖ້າ (strcmp(s, "linear")==0) ສົ່ງຄືນ LINEAR;
    ຖ້າ (strcmp(s, "ramp")==0) ສົ່ງຄືນ RAMP;
    ຖ້າ (strcmp(s, "leaky")==0) ສົ່ງຄືນ LEAKY;
    ຖ້າ (strcmp(s, "tanh")==0) ສົ່ງຄືນ TANH;
    ຖ້າ (strcmp(s, "stair")==0) ສົ່ງຄືນ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ສົ່ງຄືນ RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ສົ່ງຄືນ linear_activate(x);
        pak_sa LOGISTIC:
            ສົ່ງຄືນ logistic_activate(x);
        pak_sa LOGGY:
            ສົ່ງຄືນ loggy_activate(x);
        pak_sa RELU:
            ສົ່ງຄືນ relu_activate(x);
        pak_sa ELU:
            ສົ່ງຄືນ elu_activate(x);
        pak_sa SELU:
            ສົ່ງຄືນ selu_activate(x);
        pak_sa RELIE:
            ສົ່ງຄືນ relie_activate(x);
        pak_sa RAMP:
            ສົ່ງຄືນ ramp_activate(x);
        pak_sa LEAKY:
            ສົ່ງຄືນ leaky_activate(x);
        pak_sa TANH:
            ສົ່ງຄືນ tanh_activate(x);
        pak_sa PLSE:
            ສົ່ງຄືນ plse_activate(x);
        pak_sa STAIR:
            ສົ່ງຄືນ stair_activate(x);
        pak_sa HARDTAN:
            ສົ່ງຄືນ hardtan_activate(x);
        pak_sa LHTAN:
            ສົ່ງຄືນ lhtan_activate(x);
    }
    ສົ່ງຄືນ 0;
}

void activate_array(float *x, const ຈຳນວນເຕັມ n, const ACTIVATION a)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ສົ່ງຄືນ linear_gradient(x);
        pak_sa LOGISTIC:
            ສົ່ງຄືນ logistic_gradient(x);
        pak_sa LOGGY:
            ສົ່ງຄືນ loggy_gradient(x);
        pak_sa RELU:
            ສົ່ງຄືນ relu_gradient(x);
        pak_sa ELU:
            ສົ່ງຄືນ elu_gradient(x);
        pak_sa SELU:
            ສົ່ງຄືນ selu_gradient(x);
        pak_sa RELIE:
            ສົ່ງຄືນ relie_gradient(x);
        pak_sa RAMP:
            ສົ່ງຄືນ ramp_gradient(x);
        pak_sa LEAKY:
            ສົ່ງຄືນ leaky_gradient(x);
        pak_sa TANH:
            ສົ່ງຄືນ tanh_gradient(x);
        pak_sa PLSE:
            ສົ່ງຄືນ plse_gradient(x);
        pak_sa STAIR:
            ສົ່ງຄືນ stair_gradient(x);
        pak_sa HARDTAN:
            ສົ່ງຄືນ hardtan_gradient(x);
        pak_sa LHTAN:
            ສົ່ງຄືນ lhtan_gradient(x);
    }
    ສົ່ງຄືນ 0;
}

void gradient_array(const float *x, const ຈຳນວນເຕັມ n, const ACTIVATION a, float *delta)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

