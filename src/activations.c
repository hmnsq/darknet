#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਅੱਖਰ *get_activation_string(ACTIVATION a)
{
    ਚੋਣ(a){
        ਹਾਲਤ LOGISTIC:
            ਵਾਪਸ "logistic";
        ਹਾਲਤ LOGGY:
            ਵਾਪਸ "loggy";
        ਹਾਲਤ RELU:
            ਵਾਪਸ "relu";
        ਹਾਲਤ ELU:
            ਵਾਪਸ "elu";
        ਹਾਲਤ SELU:
            ਵਾਪਸ "selu";
        ਹਾਲਤ RELIE:
            ਵਾਪਸ "relie";
        ਹਾਲਤ RAMP:
            ਵਾਪਸ "ramp";
        ਹਾਲਤ ਰੇਖਿਕ:
            ਵਾਪਸ "linear";
        ਹਾਲਤ TANH:
            ਵਾਪਸ "tanh";
        ਹਾਲਤ PLSE:
            ਵਾਪਸ "plse";
        ਹਾਲਤ LEAKY:
            ਵਾਪਸ "leaky";
        ਹਾਲਤ STAIR:
            ਵਾਪਸ "stair";
        ਹਾਲਤ HARDTAN:
            ਵਾਪਸ "hardtan";
        ਹਾਲਤ LHTAN:
            ਵਾਪਸ "lhtan";
        ਮੂਲ:
            ਰੋਕੋ;
    }
    ਵਾਪਸ "relu";
}

ACTIVATION get_activation(ਅੱਖਰ *s)
{
    ਜੇ (strcmp(s, "logistic")==0) ਵਾਪਸ LOGISTIC;
    ਜੇ (strcmp(s, "loggy")==0) ਵਾਪਸ LOGGY;
    ਜੇ (strcmp(s, "relu")==0) ਵਾਪਸ RELU;
    ਜੇ (strcmp(s, "elu")==0) ਵਾਪਸ ELU;
    ਜੇ (strcmp(s, "selu")==0) ਵਾਪਸ SELU;
    ਜੇ (strcmp(s, "relie")==0) ਵਾਪਸ RELIE;
    ਜੇ (strcmp(s, "plse")==0) ਵਾਪਸ PLSE;
    ਜੇ (strcmp(s, "hardtan")==0) ਵਾਪਸ HARDTAN;
    ਜੇ (strcmp(s, "lhtan")==0) ਵਾਪਸ LHTAN;
    ਜੇ (strcmp(s, "linear")==0) ਵਾਪਸ ਰੇਖਿਕ;
    ਜੇ (strcmp(s, "ramp")==0) ਵਾਪਸ RAMP;
    ਜੇ (strcmp(s, "leaky")==0) ਵਾਪਸ LEAKY;
    ਜੇ (strcmp(s, "tanh")==0) ਵਾਪਸ TANH;
    ਜੇ (strcmp(s, "stair")==0) ਵਾਪਸ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ਵਾਪਸ RELU;
}

ਦਸ਼ਮਲਵ activate(ਦਸ਼ਮਲਵ x, ACTIVATION a)
{
    ਚੋਣ(a){
        ਹਾਲਤ ਰੇਖਿਕ:
            ਵਾਪਸ ਰੇਖਿਕ_activate(x);
        ਹਾਲਤ LOGISTIC:
            ਵਾਪਸ logistic_activate(x);
        ਹਾਲਤ LOGGY:
            ਵਾਪਸ loggy_activate(x);
        ਹਾਲਤ RELU:
            ਵਾਪਸ relu_activate(x);
        ਹਾਲਤ ELU:
            ਵਾਪਸ elu_activate(x);
        ਹਾਲਤ SELU:
            ਵਾਪਸ selu_activate(x);
        ਹਾਲਤ RELIE:
            ਵਾਪਸ relie_activate(x);
        ਹਾਲਤ RAMP:
            ਵਾਪਸ ramp_activate(x);
        ਹਾਲਤ LEAKY:
            ਵਾਪਸ leaky_activate(x);
        ਹਾਲਤ TANH:
            ਵਾਪਸ tanh_activate(x);
        ਹਾਲਤ PLSE:
            ਵਾਪਸ plse_activate(x);
        ਹਾਲਤ STAIR:
            ਵਾਪਸ stair_activate(x);
        ਹਾਲਤ HARDTAN:
            ਵਾਪਸ hardtan_activate(x);
        ਹਾਲਤ LHTAN:
            ਵਾਪਸ lhtan_activate(x);
    }
    ਵਾਪਸ 0;
}

ਖਾਲੀ activate_array(ਦਸ਼ਮਲਵ *x, ਅਚਲ ਪੂਰਨ n, ਅਚਲ ACTIVATION a)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ਦਸ਼ਮਲਵ gradient(ਦਸ਼ਮਲਵ x, ACTIVATION a)
{
    ਚੋਣ(a){
        ਹਾਲਤ ਰੇਖਿਕ:
            ਵਾਪਸ ਰੇਖਿਕ_gradient(x);
        ਹਾਲਤ LOGISTIC:
            ਵਾਪਸ logistic_gradient(x);
        ਹਾਲਤ LOGGY:
            ਵਾਪਸ loggy_gradient(x);
        ਹਾਲਤ RELU:
            ਵਾਪਸ relu_gradient(x);
        ਹਾਲਤ ELU:
            ਵਾਪਸ elu_gradient(x);
        ਹਾਲਤ SELU:
            ਵਾਪਸ selu_gradient(x);
        ਹਾਲਤ RELIE:
            ਵਾਪਸ relie_gradient(x);
        ਹਾਲਤ RAMP:
            ਵਾਪਸ ramp_gradient(x);
        ਹਾਲਤ LEAKY:
            ਵਾਪਸ leaky_gradient(x);
        ਹਾਲਤ TANH:
            ਵਾਪਸ tanh_gradient(x);
        ਹਾਲਤ PLSE:
            ਵਾਪਸ plse_gradient(x);
        ਹਾਲਤ STAIR:
            ਵਾਪਸ stair_gradient(x);
        ਹਾਲਤ HARDTAN:
            ਵਾਪਸ hardtan_gradient(x);
        ਹਾਲਤ LHTAN:
            ਵਾਪਸ lhtan_gradient(x);
    }
    ਵਾਪਸ 0;
}

ਖਾਲੀ gradient_array(ਅਚਲ ਦਸ਼ਮਲਵ *x, ਅਚਲ ਪੂਰਨ n, ਅਚਲ ACTIVATION a, ਦਸ਼ਮਲਵ *delta)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

