#অন্তর্ভাব "activations.h"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

অক্ষর *get_activation_string(ACTIVATION a)
{
    নির্বাচন(a){
        ক্ষেত্র LOGISTIC:
            ফেরত "logistic";
        ক্ষেত্র LOGGY:
            ফেরত "loggy";
        ক্ষেত্র RELU:
            ফেরত "relu";
        ক্ষেত্র ELU:
            ফেরত "elu";
        ক্ষেত্র SELU:
            ফেরত "selu";
        ক্ষেত্র RELIE:
            ফেরত "relie";
        ক্ষেত্র RAMP:
            ফেরত "ramp";
        ক্ষেত্র LINEAR:
            ফেরত "linear";
        ক্ষেত্র TANH:
            ফেরত "tanh";
        ক্ষেত্র PLSE:
            ফেরত "plse";
        ক্ষেত্র LEAKY:
            ফেরত "leaky";
        ক্ষেত্র STAIR:
            ফেরত "stair";
        ক্ষেত্র HARDTAN:
            ফেরত "hardtan";
        ক্ষেত্র LHTAN:
            ফেরত "lhtan";
        পূর্বনির্ধারিত:
            বিরতি;
    }
    ফেরত "relu";
}

ACTIVATION get_activation(অক্ষর *s)
{
    যদি (strcmp(s, "logistic")==0) ফেরত LOGISTIC;
    যদি (strcmp(s, "loggy")==0) ফেরত LOGGY;
    যদি (strcmp(s, "relu")==0) ফেরত RELU;
    যদি (strcmp(s, "elu")==0) ফেরত ELU;
    যদি (strcmp(s, "selu")==0) ফেরত SELU;
    যদি (strcmp(s, "relie")==0) ফেরত RELIE;
    যদি (strcmp(s, "plse")==0) ফেরত PLSE;
    যদি (strcmp(s, "hardtan")==0) ফেরত HARDTAN;
    যদি (strcmp(s, "lhtan")==0) ফেরত LHTAN;
    যদি (strcmp(s, "linear")==0) ফেরত LINEAR;
    যদি (strcmp(s, "ramp")==0) ফেরত RAMP;
    যদি (strcmp(s, "leaky")==0) ফেরত LEAKY;
    যদি (strcmp(s, "tanh")==0) ফেরত TANH;
    যদি (strcmp(s, "stair")==0) ফেরত STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ফেরত RELU;
}

দশমিক activate(দশমিক x, ACTIVATION a)
{
    নির্বাচন(a){
        ক্ষেত্র LINEAR:
            ফেরত linear_activate(x);
        ক্ষেত্র LOGISTIC:
            ফেরত logistic_activate(x);
        ক্ষেত্র LOGGY:
            ফেরত loggy_activate(x);
        ক্ষেত্র RELU:
            ফেরত relu_activate(x);
        ক্ষেত্র ELU:
            ফেরত elu_activate(x);
        ক্ষেত্র SELU:
            ফেরত selu_activate(x);
        ক্ষেত্র RELIE:
            ফেরত relie_activate(x);
        ক্ষেত্র RAMP:
            ফেরত ramp_activate(x);
        ক্ষেত্র LEAKY:
            ফেরত leaky_activate(x);
        ক্ষেত্র TANH:
            ফেরত tanh_activate(x);
        ক্ষেত্র PLSE:
            ফেরত plse_activate(x);
        ক্ষেত্র STAIR:
            ফেরত stair_activate(x);
        ক্ষেত্র HARDTAN:
            ফেরত hardtan_activate(x);
        ক্ষেত্র LHTAN:
            ফেরত lhtan_activate(x);
    }
    ফেরত 0;
}

শূন্য activate_array(দশমিক *x, ধ্রুবক পূর্ণসংখ্যা n, ধ্রুবক ACTIVATION a)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

দশমিক gradient(দশমিক x, ACTIVATION a)
{
    নির্বাচন(a){
        ক্ষেত্র LINEAR:
            ফেরত linear_gradient(x);
        ক্ষেত্র LOGISTIC:
            ফেরত logistic_gradient(x);
        ক্ষেত্র LOGGY:
            ফেরত loggy_gradient(x);
        ক্ষেত্র RELU:
            ফেরত relu_gradient(x);
        ক্ষেত্র ELU:
            ফেরত elu_gradient(x);
        ক্ষেত্র SELU:
            ফেরত selu_gradient(x);
        ক্ষেত্র RELIE:
            ফেরত relie_gradient(x);
        ক্ষেত্র RAMP:
            ফেরত ramp_gradient(x);
        ক্ষেত্র LEAKY:
            ফেরত leaky_gradient(x);
        ক্ষেত্র TANH:
            ফেরত tanh_gradient(x);
        ক্ষেত্র PLSE:
            ফেরত plse_gradient(x);
        ক্ষেত্র STAIR:
            ফেরত stair_gradient(x);
        ক্ষেত্র HARDTAN:
            ফেরত hardtan_gradient(x);
        ক্ষেত্র LHTAN:
            ফেরত lhtan_gradient(x);
    }
    ফেরত 0;
}

শূন্য gradient_array(ধ্রুবক দশমিক *x, ধ্রুবক পূর্ণসংখ্যা n, ধ্রুবক ACTIVATION a, দশমিক *delta)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

