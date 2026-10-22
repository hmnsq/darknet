#ضمن "activations.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

حرف *get_activation_string(ACTIVATION a)
{
    بدل(a){
        حالة LOGISTIC:
            החזר "logistic";
        حالة LOGGY:
            החזר "loggy";
        حالة RELU:
            החזר "relu";
        حالة ELU:
            החזר "elu";
        حالة SELU:
            החזר "selu";
        حالة RELIE:
            החזר "relie";
        حالة RAMP:
            החזר "ramp";
        حالة LINEAR:
            החזר "linear";
        حالة TANH:
            החזר "tanh";
        حالة PLSE:
            החזר "plse";
        حالة LEAKY:
            החזר "leaky";
        حالة STAIR:
            החזר "stair";
        حالة HARDTAN:
            החזר "hardtan";
        حالة LHTAN:
            החזר "lhtan";
        افتراضي:
            اكسر;
    }
    החזר "relu";
}

ACTIVATION get_activation(حرف *s)
{
    אם (strcmp(s, "logistic")==0) החזר LOGISTIC;
    אם (strcmp(s, "loggy")==0) החזר LOGGY;
    אם (strcmp(s, "relu")==0) החזר RELU;
    אם (strcmp(s, "elu")==0) החזר ELU;
    אם (strcmp(s, "selu")==0) החזר SELU;
    אם (strcmp(s, "relie")==0) החזר RELIE;
    אם (strcmp(s, "plse")==0) החזר PLSE;
    אם (strcmp(s, "hardtan")==0) החזר HARDTAN;
    אם (strcmp(s, "lhtan")==0) החזר LHTAN;
    אם (strcmp(s, "linear")==0) החזר LINEAR;
    אם (strcmp(s, "ramp")==0) החזר RAMP;
    אם (strcmp(s, "leaky")==0) החזר LEAKY;
    אם (strcmp(s, "tanh")==0) החזר TANH;
    אם (strcmp(s, "stair")==0) החזר STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    החזר RELU;
}

عائم activate(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة LINEAR:
            החזר linear_activate(x);
        حالة LOGISTIC:
            החזר logistic_activate(x);
        حالة LOGGY:
            החזר loggy_activate(x);
        حالة RELU:
            החזר relu_activate(x);
        حالة ELU:
            החזר elu_activate(x);
        حالة SELU:
            החזר selu_activate(x);
        حالة RELIE:
            החזר relie_activate(x);
        حالة RAMP:
            החזר ramp_activate(x);
        حالة LEAKY:
            החזר leaky_activate(x);
        حالة TANH:
            החזר tanh_activate(x);
        حالة PLSE:
            החזר plse_activate(x);
        حالة STAIR:
            החזר stair_activate(x);
        حالة HARDTAN:
            החזר hardtan_activate(x);
        حالة LHTAN:
            החזר lhtan_activate(x);
    }
    החזר 0;
}

فراغ activate_array(عائم *x, ثابت صحيح n, ثابت ACTIVATION a)
{
    صحيح i;
    עבור(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

عائم gradient(عائم x, ACTIVATION a)
{
    بدل(a){
        حالة LINEAR:
            החזר linear_gradient(x);
        حالة LOGISTIC:
            החזר logistic_gradient(x);
        حالة LOGGY:
            החזר loggy_gradient(x);
        حالة RELU:
            החזר relu_gradient(x);
        حالة ELU:
            החזר elu_gradient(x);
        حالة SELU:
            החזר selu_gradient(x);
        حالة RELIE:
            החזר relie_gradient(x);
        حالة RAMP:
            החזר ramp_gradient(x);
        حالة LEAKY:
            החזר leaky_gradient(x);
        حالة TANH:
            החזר tanh_gradient(x);
        حالة PLSE:
            החזר plse_gradient(x);
        حالة STAIR:
            החזר stair_gradient(x);
        حالة HARDTAN:
            החזר hardtan_gradient(x);
        حالة LHTAN:
            החזר lhtan_gradient(x);
    }
    החזר 0;
}

فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta)
{
    صحيح i;
    עבור(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

