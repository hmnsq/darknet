#شامل "activations.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

نویسه *get_activation_string(ACTIVATION a)
{
    انتخاب(a){
        حالت LOGISTIC:
            بازگشت "logistic";
        حالت LOGGY:
            بازگشت "loggy";
        حالت RELU:
            بازگشت "relu";
        حالت ELU:
            بازگشت "elu";
        حالت SELU:
            بازگشت "selu";
        حالت RELIE:
            بازگشت "relie";
        حالت RAMP:
            بازگشت "ramp";
        حالت LINEAR:
            بازگشت "linear";
        حالت TANH:
            بازگشت "tanh";
        حالت PLSE:
            بازگشت "plse";
        حالت LEAKY:
            بازگشت "leaky";
        حالت STAIR:
            بازگشت "stair";
        حالت HARDTAN:
            بازگشت "hardtan";
        حالت LHTAN:
            بازگشت "lhtan";
        پیشفرض:
            بشکن;
    }
    بازگشت "relu";
}

ACTIVATION get_activation(نویسه *s)
{
    اگر (strcmp(s, "logistic")==0) بازگشت LOGISTIC;
    اگر (strcmp(s, "loggy")==0) بازگشت LOGGY;
    اگر (strcmp(s, "relu")==0) بازگشت RELU;
    اگر (strcmp(s, "elu")==0) بازگشت ELU;
    اگر (strcmp(s, "selu")==0) بازگشت SELU;
    اگر (strcmp(s, "relie")==0) بازگشت RELIE;
    اگر (strcmp(s, "plse")==0) بازگشت PLSE;
    اگر (strcmp(s, "hardtan")==0) بازگشت HARDTAN;
    اگر (strcmp(s, "lhtan")==0) بازگشت LHTAN;
    اگر (strcmp(s, "linear")==0) بازگشت LINEAR;
    اگر (strcmp(s, "ramp")==0) بازگشت RAMP;
    اگر (strcmp(s, "leaky")==0) بازگشت LEAKY;
    اگر (strcmp(s, "tanh")==0) بازگشت TANH;
    اگر (strcmp(s, "stair")==0) بازگشت STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    بازگشت RELU;
}

اعشاری activate(اعشاری x, ACTIVATION a)
{
    انتخاب(a){
        حالت LINEAR:
            بازگشت linear_activate(x);
        حالت LOGISTIC:
            بازگشت logistic_activate(x);
        حالت LOGGY:
            بازگشت loggy_activate(x);
        حالت RELU:
            بازگشت relu_activate(x);
        حالت ELU:
            بازگشت elu_activate(x);
        حالت SELU:
            بازگشت selu_activate(x);
        حالت RELIE:
            بازگشت relie_activate(x);
        حالت RAMP:
            بازگشت ramp_activate(x);
        حالت LEAKY:
            بازگشت leaky_activate(x);
        حالت TANH:
            بازگشت tanh_activate(x);
        حالت PLSE:
            بازگشت plse_activate(x);
        حالت STAIR:
            بازگشت stair_activate(x);
        حالت HARDTAN:
            بازگشت hardtan_activate(x);
        حالت LHTAN:
            بازگشت lhtan_activate(x);
    }
    بازگشت 0;
}

تهی activate_array(اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

اعشاری gradient(اعشاری x, ACTIVATION a)
{
    انتخاب(a){
        حالت LINEAR:
            بازگشت linear_gradient(x);
        حالت LOGISTIC:
            بازگشت logistic_gradient(x);
        حالت LOGGY:
            بازگشت loggy_gradient(x);
        حالت RELU:
            بازگشت relu_gradient(x);
        حالت ELU:
            بازگشت elu_gradient(x);
        حالت SELU:
            بازگشت selu_gradient(x);
        حالت RELIE:
            بازگشت relie_gradient(x);
        حالت RAMP:
            بازگشت ramp_gradient(x);
        حالت LEAKY:
            بازگشت leaky_gradient(x);
        حالت TANH:
            بازگشت tanh_gradient(x);
        حالت PLSE:
            بازگشت plse_gradient(x);
        حالت STAIR:
            بازگشت stair_gradient(x);
        حالت HARDTAN:
            بازگشت hardtan_gradient(x);
        حالت LHTAN:
            بازگشت lhtan_gradient(x);
    }
    بازگشت 0;
}

تهی gradient_array(ثابت اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a, اعشاری *delta)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

