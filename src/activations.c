#شامل "activations.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

حرف *get_activation_string(ACTIVATION a)
{
    انتخاب(a){
        حالت LOGISTIC:
            واپس "logistic";
        حالت LOGGY:
            واپس "loggy";
        حالت RELU:
            واپس "relu";
        حالت ELU:
            واپس "elu";
        حالت SELU:
            واپس "selu";
        حالت RELIE:
            واپس "relie";
        حالت RAMP:
            واپس "ramp";
        حالت خطی:
            واپس "linear";
        حالت TANH:
            واپس "tanh";
        حالت PLSE:
            واپس "plse";
        حالت LEAKY:
            واپس "leaky";
        حالت STAIR:
            واپس "stair";
        حالت HARDTAN:
            واپس "hardtan";
        حالت LHTAN:
            واپس "lhtan";
        طےشدہ:
            توڑو;
    }
    واپس "relu";
}

ACTIVATION get_activation(حرف *s)
{
    اگر (strcmp(s, "logistic")==0) واپس LOGISTIC;
    اگر (strcmp(s, "loggy")==0) واپس LOGGY;
    اگر (strcmp(s, "relu")==0) واپس RELU;
    اگر (strcmp(s, "elu")==0) واپس ELU;
    اگر (strcmp(s, "selu")==0) واپس SELU;
    اگر (strcmp(s, "relie")==0) واپس RELIE;
    اگر (strcmp(s, "plse")==0) واپس PLSE;
    اگر (strcmp(s, "hardtan")==0) واپس HARDTAN;
    اگر (strcmp(s, "lhtan")==0) واپس LHTAN;
    اگر (strcmp(s, "linear")==0) واپس خطی;
    اگر (strcmp(s, "ramp")==0) واپس RAMP;
    اگر (strcmp(s, "leaky")==0) واپس LEAKY;
    اگر (strcmp(s, "tanh")==0) واپس TANH;
    اگر (strcmp(s, "stair")==0) واپس STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    واپس RELU;
}

اعشاری activate(اعشاری x, ACTIVATION a)
{
    انتخاب(a){
        حالت خطی:
            واپس خطی_activate(x);
        حالت LOGISTIC:
            واپس logistic_activate(x);
        حالت LOGGY:
            واپس loggy_activate(x);
        حالت RELU:
            واپس relu_activate(x);
        حالت ELU:
            واپس elu_activate(x);
        حالت SELU:
            واپس selu_activate(x);
        حالت RELIE:
            واپس relie_activate(x);
        حالت RAMP:
            واپس ramp_activate(x);
        حالت LEAKY:
            واپس leaky_activate(x);
        حالت TANH:
            واپس tanh_activate(x);
        حالت PLSE:
            واپس plse_activate(x);
        حالت STAIR:
            واپس stair_activate(x);
        حالت HARDTAN:
            واپس hardtan_activate(x);
        حالت LHTAN:
            واپس lhtan_activate(x);
    }
    واپس 0;
}

خالی activate_array(اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

اعشاری gradient(اعشاری x, ACTIVATION a)
{
    انتخاب(a){
        حالت خطی:
            واپس خطی_gradient(x);
        حالت LOGISTIC:
            واپس logistic_gradient(x);
        حالت LOGGY:
            واپس loggy_gradient(x);
        حالت RELU:
            واپس relu_gradient(x);
        حالت ELU:
            واپس elu_gradient(x);
        حالت SELU:
            واپس selu_gradient(x);
        حالت RELIE:
            واپس relie_gradient(x);
        حالت RAMP:
            واپس ramp_gradient(x);
        حالت LEAKY:
            واپس leaky_gradient(x);
        حالت TANH:
            واپس tanh_gradient(x);
        حالت PLSE:
            واپس plse_gradient(x);
        حالت STAIR:
            واپس stair_gradient(x);
        حالت HARDTAN:
            واپس hardtan_gradient(x);
        حالت LHTAN:
            واپس lhtan_gradient(x);
    }
    واپس 0;
}

خالی gradient_array(ثابت اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a, اعشاری *delta)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

