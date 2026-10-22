#অন্তর্ভাব "activations.h"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

বর্ণ *get_activation_string(ACTIVATION a)
{
    বিকল্প(a){
        পক্ষ LOGISTIC:
            প্রত্যাগম "logistic";
        পক্ষ LOGGY:
            প্রত্যাগম "loggy";
        পক্ষ RELU:
            প্রত্যাগম "relu";
        পক্ষ ELU:
            প্রত্যাগম "elu";
        পক্ষ SELU:
            প্রত্যাগম "selu";
        পক্ষ RELIE:
            প্রত্যাগম "relie";
        পক্ষ RAMP:
            প্রত্যাগম "ramp";
        পক্ষ LINEAR:
            প্রত্যাগম "linear";
        পক্ষ TANH:
            প্রত্যাগম "tanh";
        পক্ষ PLSE:
            প্রত্যাগম "plse";
        পক্ষ LEAKY:
            প্রত্যাগম "leaky";
        পক্ষ STAIR:
            প্রত্যাগম "stair";
        পক্ষ HARDTAN:
            প্রত্যাগম "hardtan";
        পক্ষ LHTAN:
            প্রত্যাগম "lhtan";
        স্বভাব:
            ভঙ্গ;
    }
    প্রত্যাগম "relu";
}

ACTIVATION get_activation(বর্ণ *s)
{
    যদি (strcmp(s, "logistic")==0) প্রত্যাগম LOGISTIC;
    যদি (strcmp(s, "loggy")==0) প্রত্যাগম LOGGY;
    যদি (strcmp(s, "relu")==0) প্রত্যাগম RELU;
    যদি (strcmp(s, "elu")==0) প্রত্যাগম ELU;
    যদি (strcmp(s, "selu")==0) প্রত্যাগম SELU;
    যদি (strcmp(s, "relie")==0) প্রত্যাগম RELIE;
    যদি (strcmp(s, "plse")==0) প্রত্যাগম PLSE;
    যদি (strcmp(s, "hardtan")==0) প্রত্যাগম HARDTAN;
    যদি (strcmp(s, "lhtan")==0) প্রত্যাগম LHTAN;
    যদি (strcmp(s, "linear")==0) প্রত্যাগম LINEAR;
    যদি (strcmp(s, "ramp")==0) প্রত্যাগম RAMP;
    যদি (strcmp(s, "leaky")==0) প্রত্যাগম LEAKY;
    যদি (strcmp(s, "tanh")==0) প্রত্যাগম TANH;
    যদি (strcmp(s, "stair")==0) প্রত্যাগম STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    প্রত্যাগম RELU;
}

ভিন্ন activate(ভিন্ন x, ACTIVATION a)
{
    বিকল্প(a){
        পক্ষ LINEAR:
            প্রত্যাগম linear_activate(x);
        পক্ষ LOGISTIC:
            প্রত্যাগম logistic_activate(x);
        পক্ষ LOGGY:
            প্রত্যাগম loggy_activate(x);
        পক্ষ RELU:
            প্রত্যাগম relu_activate(x);
        পক্ষ ELU:
            প্রত্যাগম elu_activate(x);
        পক্ষ SELU:
            প্রত্যাগম selu_activate(x);
        পক্ষ RELIE:
            প্রত্যাগম relie_activate(x);
        পক্ষ RAMP:
            প্রত্যাগম ramp_activate(x);
        পক্ষ LEAKY:
            প্রত্যাগম leaky_activate(x);
        পক্ষ TANH:
            প্রত্যাগম tanh_activate(x);
        পক্ষ PLSE:
            প্রত্যাগম plse_activate(x);
        পক্ষ STAIR:
            প্রত্যাগম stair_activate(x);
        পক্ষ HARDTAN:
            প্রত্যাগম hardtan_activate(x);
        পক্ষ LHTAN:
            প্রত্যাগম lhtan_activate(x);
    }
    প্রত্যাগম 0;
}

শূন্য activate_array(ভিন্ন *x, ধ্রুব পূর্ণাঙ্ক n, ধ্রুব ACTIVATION a)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ভিন্ন gradient(ভিন্ন x, ACTIVATION a)
{
    বিকল্প(a){
        পক্ষ LINEAR:
            প্রত্যাগম linear_gradient(x);
        পক্ষ LOGISTIC:
            প্রত্যাগম logistic_gradient(x);
        পক্ষ LOGGY:
            প্রত্যাগম loggy_gradient(x);
        পক্ষ RELU:
            প্রত্যাগম relu_gradient(x);
        পক্ষ ELU:
            প্রত্যাগম elu_gradient(x);
        পক্ষ SELU:
            প্রত্যাগম selu_gradient(x);
        পক্ষ RELIE:
            প্রত্যাগম relie_gradient(x);
        পক্ষ RAMP:
            প্রত্যাগম ramp_gradient(x);
        পক্ষ LEAKY:
            প্রত্যাগম leaky_gradient(x);
        পক্ষ TANH:
            প্রত্যাগম tanh_gradient(x);
        পক্ষ PLSE:
            প্রত্যাগম plse_gradient(x);
        পক্ষ STAIR:
            প্রত্যাগম stair_gradient(x);
        পক্ষ HARDTAN:
            প্রত্যাগম hardtan_gradient(x);
        পক্ষ LHTAN:
            প্রত্যাগম lhtan_gradient(x);
    }
    প্রত্যাগম 0;
}

শূন্য gradient_array(ধ্রুব ভিন্ন *x, ধ্রুব পূর্ণাঙ্ক n, ধ্রুব ACTIVATION a, ভিন্ন *delta)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

