#అన్తర్భావ "activations.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

అక్షరం *get_activation_string(ACTIVATION a)
{
    ఎంపిక(a){
        స్థితి LOGISTIC:
            తిరిగి "logistic";
        స్థితి LOGGY:
            తిరిగి "loggy";
        స్థితి RELU:
            తిరిగి "relu";
        స్థితి ELU:
            తిరిగి "elu";
        స్థితి SELU:
            తిరిగి "selu";
        స్థితి RELIE:
            తిరిగి "relie";
        స్థితి RAMP:
            తిరిగి "ramp";
        స్థితి రేఖీయ:
            తిరిగి "linear";
        స్థితి TANH:
            తిరిగి "tanh";
        స్థితి PLSE:
            తిరిగి "plse";
        స్థితి LEAKY:
            తిరిగి "leaky";
        స్థితి STAIR:
            తిరిగి "stair";
        స్థితి HARDTAN:
            తిరిగి "hardtan";
        స్థితి LHTAN:
            తిరిగి "lhtan";
        అప్రమేయ:
            ఆపు;
    }
    తిరిగి "relu";
}

ACTIVATION get_activation(అక్షరం *s)
{
    ఒకవేళ (strcmp(s, "logistic")==0) తిరిగి LOGISTIC;
    ఒకవేళ (strcmp(s, "loggy")==0) తిరిగి LOGGY;
    ఒకవేళ (strcmp(s, "relu")==0) తిరిగి RELU;
    ఒకవేళ (strcmp(s, "elu")==0) తిరిగి ELU;
    ఒకవేళ (strcmp(s, "selu")==0) తిరిగి SELU;
    ఒకవేళ (strcmp(s, "relie")==0) తిరిగి RELIE;
    ఒకవేళ (strcmp(s, "plse")==0) తిరిగి PLSE;
    ఒకవేళ (strcmp(s, "hardtan")==0) తిరిగి HARDTAN;
    ఒకవేళ (strcmp(s, "lhtan")==0) తిరిగి LHTAN;
    ఒకవేళ (strcmp(s, "linear")==0) తిరిగి రేఖీయ;
    ఒకవేళ (strcmp(s, "ramp")==0) తిరిగి RAMP;
    ఒకవేళ (strcmp(s, "leaky")==0) తిరిగి LEAKY;
    ఒకవేళ (strcmp(s, "tanh")==0) తిరిగి TANH;
    ఒకవేళ (strcmp(s, "stair")==0) తిరిగి STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    తిరిగి RELU;
}

దశాంశం activate(దశాంశం x, ACTIVATION a)
{
    ఎంపిక(a){
        స్థితి రేఖీయ:
            తిరిగి రేఖీయ_activate(x);
        స్థితి LOGISTIC:
            తిరిగి logistic_activate(x);
        స్థితి LOGGY:
            తిరిగి loggy_activate(x);
        స్థితి RELU:
            తిరిగి relu_activate(x);
        స్థితి ELU:
            తిరిగి elu_activate(x);
        స్థితి SELU:
            తిరిగి selu_activate(x);
        స్థితి RELIE:
            తిరిగి relie_activate(x);
        స్థితి RAMP:
            తిరిగి ramp_activate(x);
        స్థితి LEAKY:
            తిరిగి leaky_activate(x);
        స్థితి TANH:
            తిరిగి tanh_activate(x);
        స్థితి PLSE:
            తిరిగి plse_activate(x);
        స్థితి STAIR:
            తిరిగి stair_activate(x);
        స్థితి HARDTAN:
            తిరిగి hardtan_activate(x);
        స్థితి LHTAN:
            తిరిగి lhtan_activate(x);
    }
    తిరిగి 0;
}

శూన్య activate_array(దశాంశం *x, స్థిరరాశి పూర్ణాంకం n, స్థిరరాశి ACTIVATION a)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

దశాంశం gradient(దశాంశం x, ACTIVATION a)
{
    ఎంపిక(a){
        స్థితి రేఖీయ:
            తిరిగి రేఖీయ_gradient(x);
        స్థితి LOGISTIC:
            తిరిగి logistic_gradient(x);
        స్థితి LOGGY:
            తిరిగి loggy_gradient(x);
        స్థితి RELU:
            తిరిగి relu_gradient(x);
        స్థితి ELU:
            తిరిగి elu_gradient(x);
        స్థితి SELU:
            తిరిగి selu_gradient(x);
        స్థితి RELIE:
            తిరిగి relie_gradient(x);
        స్థితి RAMP:
            తిరిగి ramp_gradient(x);
        స్థితి LEAKY:
            తిరిగి leaky_gradient(x);
        స్థితి TANH:
            తిరిగి tanh_gradient(x);
        స్థితి PLSE:
            తిరిగి plse_gradient(x);
        స్థితి STAIR:
            తిరిగి stair_gradient(x);
        స్థితి HARDTAN:
            తిరిగి hardtan_gradient(x);
        స్థితి LHTAN:
            తిరిగి lhtan_gradient(x);
    }
    తిరిగి 0;
}

శూన్య gradient_array(స్థిరరాశి దశాంశం *x, స్థిరరాశి పూర్ణాంకం n, స్థిరరాశి ACTIVATION a, దశాంశం *delta)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

