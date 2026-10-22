#അന്തര്ഭാവ "activations.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

അക്ഷരം *get_activation_string(ACTIVATION a)
{
    തിരഞ്ഞെടുപ്പ്(a){
        അവസ്ഥ LOGISTIC:
            തിരികെ "logistic";
        അവസ്ഥ LOGGY:
            തിരികെ "loggy";
        അവസ്ഥ RELU:
            തിരികെ "relu";
        അവസ്ഥ ELU:
            തിരികെ "elu";
        അവസ്ഥ SELU:
            തിരികെ "selu";
        അവസ്ഥ RELIE:
            തിരികെ "relie";
        അവസ്ഥ RAMP:
            തിരികെ "ramp";
        അവസ്ഥ LINEAR:
            തിരികെ "linear";
        അവസ്ഥ TANH:
            തിരികെ "tanh";
        അവസ്ഥ PLSE:
            തിരികെ "plse";
        അവസ്ഥ LEAKY:
            തിരികെ "leaky";
        അവസ്ഥ STAIR:
            തിരികെ "stair";
        അവസ്ഥ HARDTAN:
            തിരികെ "hardtan";
        അവസ്ഥ LHTAN:
            തിരികെ "lhtan";
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
    }
    തിരികെ "relu";
}

ACTIVATION get_activation(അക്ഷരം *s)
{
    എങ്കിൽ (strcmp(s, "logistic")==0) തിരികെ LOGISTIC;
    എങ്കിൽ (strcmp(s, "loggy")==0) തിരികെ LOGGY;
    എങ്കിൽ (strcmp(s, "relu")==0) തിരികെ RELU;
    എങ്കിൽ (strcmp(s, "elu")==0) തിരികെ ELU;
    എങ്കിൽ (strcmp(s, "selu")==0) തിരികെ SELU;
    എങ്കിൽ (strcmp(s, "relie")==0) തിരികെ RELIE;
    എങ്കിൽ (strcmp(s, "plse")==0) തിരികെ PLSE;
    എങ്കിൽ (strcmp(s, "hardtan")==0) തിരികെ HARDTAN;
    എങ്കിൽ (strcmp(s, "lhtan")==0) തിരികെ LHTAN;
    എങ്കിൽ (strcmp(s, "linear")==0) തിരികെ LINEAR;
    എങ്കിൽ (strcmp(s, "ramp")==0) തിരികെ RAMP;
    എങ്കിൽ (strcmp(s, "leaky")==0) തിരികെ LEAKY;
    എങ്കിൽ (strcmp(s, "tanh")==0) തിരികെ TANH;
    എങ്കിൽ (strcmp(s, "stair")==0) തിരികെ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    തിരികെ RELU;
}

ദശാംശം activate(ദശാംശം x, ACTIVATION a)
{
    തിരഞ്ഞെടുപ്പ്(a){
        അവസ്ഥ LINEAR:
            തിരികെ linear_activate(x);
        അവസ്ഥ LOGISTIC:
            തിരികെ logistic_activate(x);
        അവസ്ഥ LOGGY:
            തിരികെ loggy_activate(x);
        അവസ്ഥ RELU:
            തിരികെ relu_activate(x);
        അവസ്ഥ ELU:
            തിരികെ elu_activate(x);
        അവസ്ഥ SELU:
            തിരികെ selu_activate(x);
        അവസ്ഥ RELIE:
            തിരികെ relie_activate(x);
        അവസ്ഥ RAMP:
            തിരികെ ramp_activate(x);
        അവസ്ഥ LEAKY:
            തിരികെ leaky_activate(x);
        അവസ്ഥ TANH:
            തിരികെ tanh_activate(x);
        അവസ്ഥ PLSE:
            തിരികെ plse_activate(x);
        അവസ്ഥ STAIR:
            തിരികെ stair_activate(x);
        അവസ്ഥ HARDTAN:
            തിരികെ hardtan_activate(x);
        അവസ്ഥ LHTAN:
            തിരികെ lhtan_activate(x);
    }
    തിരികെ 0;
}

ശൂന്യം activate_array(ദശാംശം *x, സ്ഥിരം പൂർണ്ണസംഖ്യ n, സ്ഥിരം ACTIVATION a)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ദശാംശം gradient(ദശാംശം x, ACTIVATION a)
{
    തിരഞ്ഞെടുപ്പ്(a){
        അവസ്ഥ LINEAR:
            തിരികെ linear_gradient(x);
        അവസ്ഥ LOGISTIC:
            തിരികെ logistic_gradient(x);
        അവസ്ഥ LOGGY:
            തിരികെ loggy_gradient(x);
        അവസ്ഥ RELU:
            തിരികെ relu_gradient(x);
        അവസ്ഥ ELU:
            തിരികെ elu_gradient(x);
        അവസ്ഥ SELU:
            തിരികെ selu_gradient(x);
        അവസ്ഥ RELIE:
            തിരികെ relie_gradient(x);
        അവസ്ഥ RAMP:
            തിരികെ ramp_gradient(x);
        അവസ്ഥ LEAKY:
            തിരികെ leaky_gradient(x);
        അവസ്ഥ TANH:
            തിരികെ tanh_gradient(x);
        അവസ്ഥ PLSE:
            തിരികെ plse_gradient(x);
        അവസ്ഥ STAIR:
            തിരികെ stair_gradient(x);
        അവസ്ഥ HARDTAN:
            തിരികെ hardtan_gradient(x);
        അവസ്ഥ LHTAN:
            തിരികെ lhtan_gradient(x);
    }
    തിരികെ 0;
}

ശൂന്യം gradient_array(സ്ഥിരം ദശാംശം *x, സ്ഥിരം പൂർണ്ണസംഖ്യ n, സ്ഥിരം ACTIVATION a, ദശാംശം *delta)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

