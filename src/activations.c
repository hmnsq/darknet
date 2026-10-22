#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            คืน "logistic";
        pak_sa LOGGY:
            คืน "loggy";
        pak_sa RELU:
            คืน "relu";
        pak_sa ELU:
            คืน "elu";
        pak_sa SELU:
            คืน "selu";
        pak_sa RELIE:
            คืน "relie";
        pak_sa RAMP:
            คืน "ramp";
        pak_sa LINEAR:
            คืน "linear";
        pak_sa TANH:
            คืน "tanh";
        pak_sa PLSE:
            คืน "plse";
        pak_sa LEAKY:
            คืน "leaky";
        pak_sa STAIR:
            คืน "stair";
        pak_sa HARDTAN:
            คืน "hardtan";
        pak_sa LHTAN:
            คืน "lhtan";
        default:
            bha_nga;
    }
    คืน "relu";
}

ACTIVATION get_activation(var_na *s)
{
    ถ้า (strcmp(s, "logistic")==0) คืน LOGISTIC;
    ถ้า (strcmp(s, "loggy")==0) คืน LOGGY;
    ถ้า (strcmp(s, "relu")==0) คืน RELU;
    ถ้า (strcmp(s, "elu")==0) คืน ELU;
    ถ้า (strcmp(s, "selu")==0) คืน SELU;
    ถ้า (strcmp(s, "relie")==0) คืน RELIE;
    ถ้า (strcmp(s, "plse")==0) คืน PLSE;
    ถ้า (strcmp(s, "hardtan")==0) คืน HARDTAN;
    ถ้า (strcmp(s, "lhtan")==0) คืน LHTAN;
    ถ้า (strcmp(s, "linear")==0) คืน LINEAR;
    ถ้า (strcmp(s, "ramp")==0) คืน RAMP;
    ถ้า (strcmp(s, "leaky")==0) คืน LEAKY;
    ถ้า (strcmp(s, "tanh")==0) คืน TANH;
    ถ้า (strcmp(s, "stair")==0) คืน STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    คืน RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            คืน linear_activate(x);
        pak_sa LOGISTIC:
            คืน logistic_activate(x);
        pak_sa LOGGY:
            คืน loggy_activate(x);
        pak_sa RELU:
            คืน relu_activate(x);
        pak_sa ELU:
            คืน elu_activate(x);
        pak_sa SELU:
            คืน selu_activate(x);
        pak_sa RELIE:
            คืน relie_activate(x);
        pak_sa RAMP:
            คืน ramp_activate(x);
        pak_sa LEAKY:
            คืน leaky_activate(x);
        pak_sa TANH:
            คืน tanh_activate(x);
        pak_sa PLSE:
            คืน plse_activate(x);
        pak_sa STAIR:
            คืน stair_activate(x);
        pak_sa HARDTAN:
            คืน hardtan_activate(x);
        pak_sa LHTAN:
            คืน lhtan_activate(x);
    }
    คืน 0;
}

void activate_array(float *x, const จำนวนเต็ม n, const ACTIVATION a)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            คืน linear_gradient(x);
        pak_sa LOGISTIC:
            คืน logistic_gradient(x);
        pak_sa LOGGY:
            คืน loggy_gradient(x);
        pak_sa RELU:
            คืน relu_gradient(x);
        pak_sa ELU:
            คืน elu_gradient(x);
        pak_sa SELU:
            คืน selu_gradient(x);
        pak_sa RELIE:
            คืน relie_gradient(x);
        pak_sa RAMP:
            คืน ramp_gradient(x);
        pak_sa LEAKY:
            คืน leaky_gradient(x);
        pak_sa TANH:
            คืน tanh_gradient(x);
        pak_sa PLSE:
            คืน plse_gradient(x);
        pak_sa STAIR:
            คืน stair_gradient(x);
        pak_sa HARDTAN:
            คืน hardtan_gradient(x);
        pak_sa LHTAN:
            คืน lhtan_gradient(x);
    }
    คืน 0;
}

void gradient_array(const float *x, const จำนวนเต็ม n, const ACTIVATION a, float *delta)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

