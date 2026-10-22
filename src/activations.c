#include "activations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var_na *get_activation_string(ACTIVATION a)
{
    switch(a){
        pak_sa LOGISTIC:
            ආපසු "logistic";
        pak_sa LOGGY:
            ආපසු "loggy";
        pak_sa RELU:
            ආපසු "relu";
        pak_sa ELU:
            ආපසු "elu";
        pak_sa SELU:
            ආපසු "selu";
        pak_sa RELIE:
            ආපසු "relie";
        pak_sa RAMP:
            ආපසු "ramp";
        pak_sa LINEAR:
            ආපසු "linear";
        pak_sa TANH:
            ආපසු "tanh";
        pak_sa PLSE:
            ආපසු "plse";
        pak_sa LEAKY:
            ආපසු "leaky";
        pak_sa STAIR:
            ආපසු "stair";
        pak_sa HARDTAN:
            ආපසු "hardtan";
        pak_sa LHTAN:
            ආපසු "lhtan";
        default:
            bha_nga;
    }
    ආපසු "relu";
}

ACTIVATION get_activation(var_na *s)
{
    නම් (strcmp(s, "logistic")==0) ආපසු LOGISTIC;
    නම් (strcmp(s, "loggy")==0) ආපසු LOGGY;
    නම් (strcmp(s, "relu")==0) ආපසු RELU;
    නම් (strcmp(s, "elu")==0) ආපසු ELU;
    නම් (strcmp(s, "selu")==0) ආපසු SELU;
    නම් (strcmp(s, "relie")==0) ආපසු RELIE;
    නම් (strcmp(s, "plse")==0) ආපසු PLSE;
    නම් (strcmp(s, "hardtan")==0) ආපසු HARDTAN;
    නම් (strcmp(s, "lhtan")==0) ආපසු LHTAN;
    නම් (strcmp(s, "linear")==0) ආපසු LINEAR;
    නම් (strcmp(s, "ramp")==0) ආපසු RAMP;
    නම් (strcmp(s, "leaky")==0) ආපසු LEAKY;
    නම් (strcmp(s, "tanh")==0) ආපසු TANH;
    නම් (strcmp(s, "stair")==0) ආපසු STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ආපසු RELU;
}

float activate(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ආපසු linear_activate(x);
        pak_sa LOGISTIC:
            ආපසු logistic_activate(x);
        pak_sa LOGGY:
            ආපසු loggy_activate(x);
        pak_sa RELU:
            ආපසු relu_activate(x);
        pak_sa ELU:
            ආපසු elu_activate(x);
        pak_sa SELU:
            ආපසු selu_activate(x);
        pak_sa RELIE:
            ආපසු relie_activate(x);
        pak_sa RAMP:
            ආපසු ramp_activate(x);
        pak_sa LEAKY:
            ආපසු leaky_activate(x);
        pak_sa TANH:
            ආපසු tanh_activate(x);
        pak_sa PLSE:
            ආපසු plse_activate(x);
        pak_sa STAIR:
            ආපසු stair_activate(x);
        pak_sa HARDTAN:
            ආපසු hardtan_activate(x);
        pak_sa LHTAN:
            ආපසු lhtan_activate(x);
    }
    ආපසු 0;
}

void activate_array(float *x, const පූර්ණ_සංඛ්‍යා n, const ACTIVATION a)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

float gradient(float x, ACTIVATION a)
{
    switch(a){
        pak_sa LINEAR:
            ආපසු linear_gradient(x);
        pak_sa LOGISTIC:
            ආපසු logistic_gradient(x);
        pak_sa LOGGY:
            ආපසු loggy_gradient(x);
        pak_sa RELU:
            ආපසු relu_gradient(x);
        pak_sa ELU:
            ආපසු elu_gradient(x);
        pak_sa SELU:
            ආපසු selu_gradient(x);
        pak_sa RELIE:
            ආපසු relie_gradient(x);
        pak_sa RAMP:
            ආපසු ramp_gradient(x);
        pak_sa LEAKY:
            ආපසු leaky_gradient(x);
        pak_sa TANH:
            ආපසු tanh_gradient(x);
        pak_sa PLSE:
            ආපසු plse_gradient(x);
        pak_sa STAIR:
            ආපසු stair_gradient(x);
        pak_sa HARDTAN:
            ආපසු hardtan_gradient(x);
        pak_sa LHTAN:
            ආපසු lhtan_gradient(x);
    }
    ආපසු 0;
}

void gradient_array(const float *x, const පූර්ණ_සංඛ්‍යා n, const ACTIVATION a, float *delta)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

