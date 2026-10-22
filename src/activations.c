#અન્તર્ભાવ "activations.h"

#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

અક્ષર *get_activation_string(ACTIVATION a)
{
    પસંદગી(a){
        સ્થિતિ LOGISTIC:
            પાછા "logistic";
        સ્થિતિ LOGGY:
            પાછા "loggy";
        સ્થિતિ RELU:
            પાછા "relu";
        સ્થિતિ ELU:
            પાછા "elu";
        સ્થિતિ SELU:
            પાછા "selu";
        સ્થિતિ RELIE:
            પાછા "relie";
        સ્થિતિ RAMP:
            પાછા "ramp";
        સ્થિતિ LINEAR:
            પાછા "linear";
        સ્થિતિ TANH:
            પાછા "tanh";
        સ્થિતિ PLSE:
            પાછા "plse";
        સ્થિતિ LEAKY:
            પાછા "leaky";
        સ્થિતિ STAIR:
            પાછા "stair";
        સ્થિતિ HARDTAN:
            પાછા "hardtan";
        સ્થિતિ LHTAN:
            પાછા "lhtan";
        મૂળભૂત:
            તોડો;
    }
    પાછા "relu";
}

ACTIVATION get_activation(અક્ષર *s)
{
    જો (strcmp(s, "logistic")==0) પાછા LOGISTIC;
    જો (strcmp(s, "loggy")==0) પાછા LOGGY;
    જો (strcmp(s, "relu")==0) પાછા RELU;
    જો (strcmp(s, "elu")==0) પાછા ELU;
    જો (strcmp(s, "selu")==0) પાછા SELU;
    જો (strcmp(s, "relie")==0) પાછા RELIE;
    જો (strcmp(s, "plse")==0) પાછા PLSE;
    જો (strcmp(s, "hardtan")==0) પાછા HARDTAN;
    જો (strcmp(s, "lhtan")==0) પાછા LHTAN;
    જો (strcmp(s, "linear")==0) પાછા LINEAR;
    જો (strcmp(s, "ramp")==0) પાછા RAMP;
    જો (strcmp(s, "leaky")==0) પાછા LEAKY;
    જો (strcmp(s, "tanh")==0) પાછા TANH;
    જો (strcmp(s, "stair")==0) પાછા STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    પાછા RELU;
}

દશાંશ activate(દશાંશ x, ACTIVATION a)
{
    પસંદગી(a){
        સ્થિતિ LINEAR:
            પાછા linear_activate(x);
        સ્થિતિ LOGISTIC:
            પાછા logistic_activate(x);
        સ્થિતિ LOGGY:
            પાછા loggy_activate(x);
        સ્થિતિ RELU:
            પાછા relu_activate(x);
        સ્થિતિ ELU:
            પાછા elu_activate(x);
        સ્થિતિ SELU:
            પાછા selu_activate(x);
        સ્થિતિ RELIE:
            પાછા relie_activate(x);
        સ્થિતિ RAMP:
            પાછા ramp_activate(x);
        સ્થિતિ LEAKY:
            પાછા leaky_activate(x);
        સ્થિતિ TANH:
            પાછા tanh_activate(x);
        સ્થિતિ PLSE:
            પાછા plse_activate(x);
        સ્થિતિ STAIR:
            પાછા stair_activate(x);
        સ્થિતિ HARDTAN:
            પાછા hardtan_activate(x);
        સ્થિતિ LHTAN:
            પાછા lhtan_activate(x);
    }
    પાછા 0;
}

શૂન્ય activate_array(દશાંશ *x, અચલ પૂર્ણાંક n, અચલ ACTIVATION a)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

દશાંશ gradient(દશાંશ x, ACTIVATION a)
{
    પસંદગી(a){
        સ્થિતિ LINEAR:
            પાછા linear_gradient(x);
        સ્થિતિ LOGISTIC:
            પાછા logistic_gradient(x);
        સ્થિતિ LOGGY:
            પાછા loggy_gradient(x);
        સ્થિતિ RELU:
            પાછા relu_gradient(x);
        સ્થિતિ ELU:
            પાછા elu_gradient(x);
        સ્થિતિ SELU:
            પાછા selu_gradient(x);
        સ્થિતિ RELIE:
            પાછા relie_gradient(x);
        સ્થિતિ RAMP:
            પાછા ramp_gradient(x);
        સ્થિતિ LEAKY:
            પાછા leaky_gradient(x);
        સ્થિતિ TANH:
            પાછા tanh_gradient(x);
        સ્થિતિ PLSE:
            પાછા plse_gradient(x);
        સ્થિતિ STAIR:
            પાછા stair_gradient(x);
        સ્થિતિ HARDTAN:
            પાછા hardtan_gradient(x);
        સ્થિતિ LHTAN:
            પાછા lhtan_gradient(x);
    }
    પાછા 0;
}

શૂન્ય gradient_array(અચલ દશાંશ *x, અચલ પૂર્ણાંક n, અચલ ACTIVATION a, દશાંશ *delta)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

