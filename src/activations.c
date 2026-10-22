#ضمن "activations.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

حرف *get_activation_string(ACTIVATION a)
{
    بدل(a){
        حالة LOGISTIC:
            إرجاع "logistic";
        حالة LOGGY:
            إرجاع "loggy";
        حالة RELU:
            إرجاع "relu";
        حالة ELU:
            إرجاع "elu";
        حالة SELU:
            إرجاع "selu";
        حالة RELIE:
            إرجاع "relie";
        حالة RAMP:
            إرجاع "ramp";
        حالة LINEAR:
            إرجاع "linear";
        حالة TANH:
            إرجاع "tanh";
        حالة PLSE:
            إرجاع "plse";
        حالة LEAKY:
            إرجاع "leaky";
        حالة STAIR:
            إرجاع "stair";
        حالة HARDTAN:
            إرجاع "hardtan";
        حالة LHTAN:
            إرجاع "lhtan";
        افتراضي:
            اكسر;
    }
    إرجاع "relu";
}

ACTIVATION get_activation(حرف *s)
{
    إذا (strcmp(s, "logistic")==0) إرجاع LOGISTIC;
    إذا (strcmp(s, "loggy")==0) إرجاع LOGGY;
    إذا (strcmp(s, "relu")==0) إرجاع RELU;
    إذا (strcmp(s, "elu")==0) إرجاع ELU;
    إذا (strcmp(s, "selu")==0) إرجاع SELU;
    إذا (strcmp(s, "relie")==0) إرجاع RELIE;
    إذا (strcmp(s, "plse")==0) إرجاع PLSE;
    إذا (strcmp(s, "hardtan")==0) إرجاع HARDTAN;
    إذا (strcmp(s, "lhtan")==0) إرجاع LHTAN;
    إذا (strcmp(s, "linear")==0) إرجاع LINEAR;
    إذا (strcmp(s, "ramp")==0) إرجاع RAMP;
    إذا (strcmp(s, "leaky")==0) إرجاع LEAKY;
    إذا (strcmp(s, "tanh")==0) إرجاع TANH;
    إذا (strcmp(s, "stair")==0) إرجاع STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    إرجاع RELU;
}

عائم activate(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة LINEAR:
            إرجاع linear_activate(x);
        حالة LOGISTIC:
            إرجاع logistic_activate(x);
        حالة LOGGY:
            إرجاع loggy_activate(x);
        حالة RELU:
            إرجاع relu_activate(x);
        حالة ELU:
            إرجاع elu_activate(x);
        حالة SELU:
            إرجاع selu_activate(x);
        حالة RELIE:
            إرجاع relie_activate(x);
        حالة RAMP:
            إرجاع ramp_activate(x);
        حالة LEAKY:
            إرجاع leaky_activate(x);
        حالة TANH:
            إرجاع tanh_activate(x);
        حالة PLSE:
            إرجاع plse_activate(x);
        حالة STAIR:
            إرجاع stair_activate(x);
        حالة HARDTAN:
            إرجاع hardtan_activate(x);
        حالة LHTAN:
            إرجاع lhtan_activate(x);
    }
    إرجاع 0;
}

فراغ activate_array(عائم *x, ثابت صحيح n, ثابت ACTIVATION a)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

عائم gradient(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة LINEAR:
            إرجاع linear_gradient(x);
        حالة LOGISTIC:
            إرجاع logistic_gradient(x);
        حالة LOGGY:
            إرجاع loggy_gradient(x);
        حالة RELU:
            إرجاع relu_gradient(x);
        حالة ELU:
            إرجاع elu_gradient(x);
        حالة SELU:
            إرجاع selu_gradient(x);
        حالة RELIE:
            إرجاع relie_gradient(x);
        حالة RAMP:
            إرجاع ramp_gradient(x);
        حالة LEAKY:
            إرجاع leaky_gradient(x);
        حالة TANH:
            إرجاع tanh_gradient(x);
        حالة PLSE:
            إرجاع plse_gradient(x);
        حالة STAIR:
            إرجاع stair_gradient(x);
        حالة HARDTAN:
            إرجاع hardtan_gradient(x);
        حالة LHTAN:
            إرجاع lhtan_gradient(x);
    }
    إرجاع 0;
}

فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

