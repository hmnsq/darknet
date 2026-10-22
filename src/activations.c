#ضمن "activations.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

حرف *get_activation_string(ACTIVATION a)
{
    بدل(a){
        حالة LOGISTIC:
            أرجع "logistic";
        حالة LOGGY:
            أرجع "loggy";
        حالة RELU:
            أرجع "relu";
        حالة ELU:
            أرجع "elu";
        حالة SELU:
            أرجع "selu";
        حالة RELIE:
            أرجع "relie";
        حالة RAMP:
            أرجع "ramp";
        حالة خطی:
            أرجع "linear";
        حالة TANH:
            أرجع "tanh";
        حالة PLSE:
            أرجع "plse";
        حالة LEAKY:
            أرجع "leaky";
        حالة STAIR:
            أرجع "stair";
        حالة HARDTAN:
            أرجع "hardtan";
        حالة LHTAN:
            أرجع "lhtan";
        افتراضي:
            اكسر;
    }
    أرجع "relu";
}

ACTIVATION get_activation(حرف *s)
{
    إذا (strcmp(s, "logistic")==0) أرجع LOGISTIC;
    إذا (strcmp(s, "loggy")==0) أرجع LOGGY;
    إذا (strcmp(s, "relu")==0) أرجع RELU;
    إذا (strcmp(s, "elu")==0) أرجع ELU;
    إذا (strcmp(s, "selu")==0) أرجع SELU;
    إذا (strcmp(s, "relie")==0) أرجع RELIE;
    إذا (strcmp(s, "plse")==0) أرجع PLSE;
    إذا (strcmp(s, "hardtan")==0) أرجع HARDTAN;
    إذا (strcmp(s, "lhtan")==0) أرجع LHTAN;
    إذا (strcmp(s, "linear")==0) أرجع خطی;
    إذا (strcmp(s, "ramp")==0) أرجع RAMP;
    إذا (strcmp(s, "leaky")==0) أرجع LEAKY;
    إذا (strcmp(s, "tanh")==0) أرجع TANH;
    إذا (strcmp(s, "stair")==0) أرجع STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    أرجع RELU;
}

عائم activate(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة خطی:
            أرجع خطی_activate(x);
        حالة LOGISTIC:
            أرجع logistic_activate(x);
        حالة LOGGY:
            أرجع loggy_activate(x);
        حالة RELU:
            أرجع relu_activate(x);
        حالة ELU:
            أرجع elu_activate(x);
        حالة SELU:
            أرجع selu_activate(x);
        حالة RELIE:
            أرجع relie_activate(x);
        حالة RAMP:
            أرجع ramp_activate(x);
        حالة LEAKY:
            أرجع leaky_activate(x);
        حالة TANH:
            أرجع tanh_activate(x);
        حالة PLSE:
            أرجع plse_activate(x);
        حالة STAIR:
            أرجع stair_activate(x);
        حالة HARDTAN:
            أرجع hardtan_activate(x);
        حالة LHTAN:
            أرجع lhtan_activate(x);
    }
    أرجع 0;
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
        حالة خطی:
            أرجع خطی_gradient(x);
        حالة LOGISTIC:
            أرجع logistic_gradient(x);
        حالة LOGGY:
            أرجع loggy_gradient(x);
        حالة RELU:
            أرجع relu_gradient(x);
        حالة ELU:
            أرجع elu_gradient(x);
        حالة SELU:
            أرجع selu_gradient(x);
        حالة RELIE:
            أرجع relie_gradient(x);
        حالة RAMP:
            أرجع ramp_gradient(x);
        حالة LEAKY:
            أرجع leaky_gradient(x);
        حالة TANH:
            أرجع tanh_gradient(x);
        حالة PLSE:
            أرجع plse_gradient(x);
        حالة STAIR:
            أرجع stair_gradient(x);
        حالة HARDTAN:
            أرجع hardtan_gradient(x);
        حالة LHTAN:
            أرجع lhtan_gradient(x);
    }
    أرجع 0;
}

فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

