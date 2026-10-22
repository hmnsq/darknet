#অন্তর্ভাৱ "activations.h"

#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

ৱর্ণ *get_activation_string(ACTIVATION a)
{
    ৱিকল্প(a){
        পক্ষ LOGISTIC:
            ঘূৰাই "logistic";
        পক্ষ LOGGY:
            ঘূৰাই "loggy";
        পক্ষ RELU:
            ঘূৰাই "relu";
        পক্ষ ELU:
            ঘূৰাই "elu";
        পক্ষ SELU:
            ঘূৰাই "selu";
        পক্ষ RELIE:
            ঘূৰাই "relie";
        পক্ষ RAMP:
            ঘূৰাই "ramp";
        পক্ষ LINEAR:
            ঘূৰাই "linear";
        পক্ষ TANH:
            ঘূৰাই "tanh";
        পক্ষ PLSE:
            ঘূৰাই "plse";
        পক্ষ LEAKY:
            ঘূৰাই "leaky";
        পক্ষ STAIR:
            ঘূৰাই "stair";
        পক্ষ HARDTAN:
            ঘূৰাই "hardtan";
        পক্ষ LHTAN:
            ঘূৰাই "lhtan";
        স্ৱভাৱ:
            ভঙ্গ;
    }
    ঘূৰাই "relu";
}

ACTIVATION get_activation(ৱর্ণ *s)
{
    যদি (strcmp(s, "logistic")==0) ঘূৰাই LOGISTIC;
    যদি (strcmp(s, "loggy")==0) ঘূৰাই LOGGY;
    যদি (strcmp(s, "relu")==0) ঘূৰাই RELU;
    যদি (strcmp(s, "elu")==0) ঘূৰাই ELU;
    যদি (strcmp(s, "selu")==0) ঘূৰাই SELU;
    যদি (strcmp(s, "relie")==0) ঘূৰাই RELIE;
    যদি (strcmp(s, "plse")==0) ঘূৰাই PLSE;
    যদি (strcmp(s, "hardtan")==0) ঘূৰাই HARDTAN;
    যদি (strcmp(s, "lhtan")==0) ঘূৰাই LHTAN;
    যদি (strcmp(s, "linear")==0) ঘূৰাই LINEAR;
    যদি (strcmp(s, "ramp")==0) ঘূৰাই RAMP;
    যদি (strcmp(s, "leaky")==0) ঘূৰাই LEAKY;
    যদি (strcmp(s, "tanh")==0) ঘূৰাই TANH;
    যদি (strcmp(s, "stair")==0) ঘূৰাই STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ঘূৰাই RELU;
}

ভিন্ন activate(ভিন্ন x, ACTIVATION a)
{
    ৱিকল্প(a){
        পক্ষ LINEAR:
            ঘূৰাই linear_activate(x);
        পক্ষ LOGISTIC:
            ঘূৰাই logistic_activate(x);
        পক্ষ LOGGY:
            ঘূৰাই loggy_activate(x);
        পক্ষ RELU:
            ঘূৰাই relu_activate(x);
        পক্ষ ELU:
            ঘূৰাই elu_activate(x);
        পক্ষ SELU:
            ঘূৰাই selu_activate(x);
        পক্ষ RELIE:
            ঘূৰাই relie_activate(x);
        পক্ষ RAMP:
            ঘূৰাই ramp_activate(x);
        পক্ষ LEAKY:
            ঘূৰাই leaky_activate(x);
        পক্ষ TANH:
            ঘূৰাই tanh_activate(x);
        পক্ষ PLSE:
            ঘূৰাই plse_activate(x);
        পক্ষ STAIR:
            ঘূৰাই stair_activate(x);
        পক্ষ HARDTAN:
            ঘূৰাই hardtan_activate(x);
        পক্ষ LHTAN:
            ঘূৰাই lhtan_activate(x);
    }
    ঘূৰাই 0;
}

শূন্য activate_array(ভিন্ন *x, ধ্রুৱ পূর্ণাঙ্ক n, ধ্রুৱ ACTIVATION a)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ভিন্ন gradient(ভিন্ন x, ACTIVATION a)
{
    ৱিকল্প(a){
        পক্ষ LINEAR:
            ঘূৰাই linear_gradient(x);
        পক্ষ LOGISTIC:
            ঘূৰাই logistic_gradient(x);
        পক্ষ LOGGY:
            ঘূৰাই loggy_gradient(x);
        পক্ষ RELU:
            ঘূৰাই relu_gradient(x);
        পক্ষ ELU:
            ঘূৰাই elu_gradient(x);
        পক্ষ SELU:
            ঘূৰাই selu_gradient(x);
        পক্ষ RELIE:
            ঘূৰাই relie_gradient(x);
        পক্ষ RAMP:
            ঘূৰাই ramp_gradient(x);
        পক্ষ LEAKY:
            ঘূৰাই leaky_gradient(x);
        পক্ষ TANH:
            ঘূৰাই tanh_gradient(x);
        পক্ষ PLSE:
            ঘূৰাই plse_gradient(x);
        পক্ষ STAIR:
            ঘূৰাই stair_gradient(x);
        পক্ষ HARDTAN:
            ঘূৰাই hardtan_gradient(x);
        পক্ষ LHTAN:
            ঘূৰাই lhtan_gradient(x);
    }
    ঘূৰাই 0;
}

শূন্য gradient_array(ধ্রুৱ ভিন্ন *x, ধ্রুৱ পূর্ণাঙ্ক n, ধ্রুৱ ACTIVATION a, ভিন্ন *delta)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

