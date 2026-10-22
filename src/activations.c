#ضمن "activations.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

حرف *get_activation_string(ACTIVATION a)
{
    بدل(a){
        حالة LOGISTIC:
            ܦܢܐ "logistic";
        حالة LOGGY:
            ܦܢܐ "loggy";
        حالة RELU:
            ܦܢܐ "relu";
        حالة ELU:
            ܦܢܐ "elu";
        حالة SELU:
            ܦܢܐ "selu";
        حالة RELIE:
            ܦܢܐ "relie";
        حالة RAMP:
            ܦܢܐ "ramp";
        حالة LINEAR:
            ܦܢܐ "linear";
        حالة TANH:
            ܦܢܐ "tanh";
        حالة PLSE:
            ܦܢܐ "plse";
        حالة LEAKY:
            ܦܢܐ "leaky";
        حالة STAIR:
            ܦܢܐ "stair";
        حالة HARDTAN:
            ܦܢܐ "hardtan";
        حالة LHTAN:
            ܦܢܐ "lhtan";
        افتراضي:
            اكسر;
    }
    ܦܢܐ "relu";
}

ACTIVATION get_activation(حرف *s)
{
    ܐܢ (strcmp(s, "logistic")==0) ܦܢܐ LOGISTIC;
    ܐܢ (strcmp(s, "loggy")==0) ܦܢܐ LOGGY;
    ܐܢ (strcmp(s, "relu")==0) ܦܢܐ RELU;
    ܐܢ (strcmp(s, "elu")==0) ܦܢܐ ELU;
    ܐܢ (strcmp(s, "selu")==0) ܦܢܐ SELU;
    ܐܢ (strcmp(s, "relie")==0) ܦܢܐ RELIE;
    ܐܢ (strcmp(s, "plse")==0) ܦܢܐ PLSE;
    ܐܢ (strcmp(s, "hardtan")==0) ܦܢܐ HARDTAN;
    ܐܢ (strcmp(s, "lhtan")==0) ܦܢܐ LHTAN;
    ܐܢ (strcmp(s, "linear")==0) ܦܢܐ LINEAR;
    ܐܢ (strcmp(s, "ramp")==0) ܦܢܐ RAMP;
    ܐܢ (strcmp(s, "leaky")==0) ܦܢܐ LEAKY;
    ܐܢ (strcmp(s, "tanh")==0) ܦܢܐ TANH;
    ܐܢ (strcmp(s, "stair")==0) ܦܢܐ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ܦܢܐ RELU;
}

عائم activate(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة LINEAR:
            ܦܢܐ linear_activate(x);
        حالة LOGISTIC:
            ܦܢܐ logistic_activate(x);
        حالة LOGGY:
            ܦܢܐ loggy_activate(x);
        حالة RELU:
            ܦܢܐ relu_activate(x);
        حالة ELU:
            ܦܢܐ elu_activate(x);
        حالة SELU:
            ܦܢܐ selu_activate(x);
        حالة RELIE:
            ܦܢܐ relie_activate(x);
        حالة RAMP:
            ܦܢܐ ramp_activate(x);
        حالة LEAKY:
            ܦܢܐ leaky_activate(x);
        حالة TANH:
            ܦܢܐ tanh_activate(x);
        حالة PLSE:
            ܦܢܐ plse_activate(x);
        حالة STAIR:
            ܦܢܐ stair_activate(x);
        حالة HARDTAN:
            ܦܢܐ hardtan_activate(x);
        حالة LHTAN:
            ܦܢܐ lhtan_activate(x);
    }
    ܦܢܐ 0;
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
            ܦܢܐ linear_gradient(x);
        حالة LOGISTIC:
            ܦܢܐ logistic_gradient(x);
        حالة LOGGY:
            ܦܢܐ loggy_gradient(x);
        حالة RELU:
            ܦܢܐ relu_gradient(x);
        حالة ELU:
            ܦܢܐ elu_gradient(x);
        حالة SELU:
            ܦܢܐ selu_gradient(x);
        حالة RELIE:
            ܦܢܐ relie_gradient(x);
        حالة RAMP:
            ܦܢܐ ramp_gradient(x);
        حالة LEAKY:
            ܦܢܐ leaky_gradient(x);
        حالة TANH:
            ܦܢܐ tanh_gradient(x);
        حالة PLSE:
            ܦܢܐ plse_gradient(x);
        حالة STAIR:
            ܦܢܐ stair_gradient(x);
        حالة HARDTAN:
            ܦܢܐ hardtan_gradient(x);
        حالة LHTAN:
            ܦܢܐ lhtan_gradient(x);
    }
    ܦܢܐ 0;
}

فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

