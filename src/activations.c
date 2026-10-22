#ଅନ୍ତର୍ଭାଵ "activations.h"

#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

ଅକ୍ଷର *get_activation_string(ACTIVATION a)
{
    ବାଛ(a){
        ସ୍ଥିତି LOGISTIC:
            ଫେରନ୍ତୁ "logistic";
        ସ୍ଥିତି LOGGY:
            ଫେରନ୍ତୁ "loggy";
        ସ୍ଥିତି RELU:
            ଫେରନ୍ତୁ "relu";
        ସ୍ଥିତି ELU:
            ଫେରନ୍ତୁ "elu";
        ସ୍ଥିତି SELU:
            ଫେରନ୍ତୁ "selu";
        ସ୍ଥିତି RELIE:
            ଫେରନ୍ତୁ "relie";
        ସ୍ଥିତି RAMP:
            ଫେରନ୍ତୁ "ramp";
        ସ୍ଥିତି LINEAR:
            ଫେରନ୍ତୁ "linear";
        ସ୍ଥିତି TANH:
            ଫେରନ୍ତୁ "tanh";
        ସ୍ଥିତି PLSE:
            ଫେରନ୍ତୁ "plse";
        ସ୍ଥିତି LEAKY:
            ଫେରନ୍ତୁ "leaky";
        ସ୍ଥିତି STAIR:
            ଫେରନ୍ତୁ "stair";
        ସ୍ଥିତି HARDTAN:
            ଫେରନ୍ତୁ "hardtan";
        ସ୍ଥିତି LHTAN:
            ଫେରନ୍ତୁ "lhtan";
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ "relu";
}

ACTIVATION get_activation(ଅକ୍ଷର *s)
{
    ଯଦି (strcmp(s, "logistic")==0) ଫେରନ୍ତୁ LOGISTIC;
    ଯଦି (strcmp(s, "loggy")==0) ଫେରନ୍ତୁ LOGGY;
    ଯଦି (strcmp(s, "relu")==0) ଫେରନ୍ତୁ RELU;
    ଯଦି (strcmp(s, "elu")==0) ଫେରନ୍ତୁ ELU;
    ଯଦି (strcmp(s, "selu")==0) ଫେରନ୍ତୁ SELU;
    ଯଦି (strcmp(s, "relie")==0) ଫେରନ୍ତୁ RELIE;
    ଯଦି (strcmp(s, "plse")==0) ଫେରନ୍ତୁ PLSE;
    ଯଦି (strcmp(s, "hardtan")==0) ଫେରନ୍ତୁ HARDTAN;
    ଯଦି (strcmp(s, "lhtan")==0) ଫେରନ୍ତୁ LHTAN;
    ଯଦି (strcmp(s, "linear")==0) ଫେରନ୍ତୁ LINEAR;
    ଯଦି (strcmp(s, "ramp")==0) ଫେରନ୍ତୁ RAMP;
    ଯଦି (strcmp(s, "leaky")==0) ଫେରନ୍ତୁ LEAKY;
    ଯଦି (strcmp(s, "tanh")==0) ଫେରନ୍ତୁ TANH;
    ଯଦି (strcmp(s, "stair")==0) ଫେରନ୍ତୁ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ଫେରନ୍ତୁ RELU;
}

ଦଶମିକ activate(ଦଶମିକ x, ACTIVATION a)
{
    ବାଛ(a){
        ସ୍ଥିତି LINEAR:
            ଫେରନ୍ତୁ linear_activate(x);
        ସ୍ଥିତି LOGISTIC:
            ଫେରନ୍ତୁ logistic_activate(x);
        ସ୍ଥିତି LOGGY:
            ଫେରନ୍ତୁ loggy_activate(x);
        ସ୍ଥିତି RELU:
            ଫେରନ୍ତୁ relu_activate(x);
        ସ୍ଥିତି ELU:
            ଫେରନ୍ତୁ elu_activate(x);
        ସ୍ଥିତି SELU:
            ଫେରନ୍ତୁ selu_activate(x);
        ସ୍ଥିତି RELIE:
            ଫେରନ୍ତୁ relie_activate(x);
        ସ୍ଥିତି RAMP:
            ଫେରନ୍ତୁ ramp_activate(x);
        ସ୍ଥିତି LEAKY:
            ଫେରନ୍ତୁ leaky_activate(x);
        ସ୍ଥିତି TANH:
            ଫେରନ୍ତୁ tanh_activate(x);
        ସ୍ଥିତି PLSE:
            ଫେରନ୍ତୁ plse_activate(x);
        ସ୍ଥିତି STAIR:
            ଫେରନ୍ତୁ stair_activate(x);
        ସ୍ଥିତି HARDTAN:
            ଫେରନ୍ତୁ hardtan_activate(x);
        ସ୍ଥିତି LHTAN:
            ଫେରନ୍ତୁ lhtan_activate(x);
    }
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ activate_array(ଦଶମିକ *x, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ACTIVATION a)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ଦଶମିକ gradient(ଦଶମିକ x, ACTIVATION a)
{
    ବାଛ(a){
        ସ୍ଥିତି LINEAR:
            ଫେରନ୍ତୁ linear_gradient(x);
        ସ୍ଥିତି LOGISTIC:
            ଫେରନ୍ତୁ logistic_gradient(x);
        ସ୍ଥିତି LOGGY:
            ଫେରନ୍ତୁ loggy_gradient(x);
        ସ୍ଥିତି RELU:
            ଫେରନ୍ତୁ relu_gradient(x);
        ସ୍ଥିତି ELU:
            ଫେରନ୍ତୁ elu_gradient(x);
        ସ୍ଥିତି SELU:
            ଫେରନ୍ତୁ selu_gradient(x);
        ସ୍ଥିତି RELIE:
            ଫେରନ୍ତୁ relie_gradient(x);
        ସ୍ଥିତି RAMP:
            ଫେରନ୍ତୁ ramp_gradient(x);
        ସ୍ଥିତି LEAKY:
            ଫେରନ୍ତୁ leaky_gradient(x);
        ସ୍ଥିତି TANH:
            ଫେରନ୍ତୁ tanh_gradient(x);
        ସ୍ଥିତି PLSE:
            ଫେରନ୍ତୁ plse_gradient(x);
        ସ୍ଥିତି STAIR:
            ଫେରନ୍ତୁ stair_gradient(x);
        ସ୍ଥିତି HARDTAN:
            ଫେରନ୍ତୁ hardtan_gradient(x);
        ସ୍ଥିତି LHTAN:
            ଫେରନ୍ତୁ lhtan_gradient(x);
    }
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ gradient_array(ଅଚଳ ଦଶମିକ *x, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ACTIVATION a, ଦଶମିକ *delta)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

