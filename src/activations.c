#அந்தர்பாவ "activations.h"

#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

எழுத்து *get_activation_string(ACTIVATION a)
{
    தேர்வு(a){
        நிலை LOGISTIC:
            திரும்பு "logistic";
        நிலை LOGGY:
            திரும்பு "loggy";
        நிலை RELU:
            திரும்பு "relu";
        நிலை ELU:
            திரும்பு "elu";
        நிலை SELU:
            திரும்பு "selu";
        நிலை RELIE:
            திரும்பு "relie";
        நிலை RAMP:
            திரும்பு "ramp";
        நிலை LINEAR:
            திரும்பு "linear";
        நிலை TANH:
            திரும்பு "tanh";
        நிலை PLSE:
            திரும்பு "plse";
        நிலை LEAKY:
            திரும்பு "leaky";
        நிலை STAIR:
            திரும்பு "stair";
        நிலை HARDTAN:
            திரும்பு "hardtan";
        நிலை LHTAN:
            திரும்பு "lhtan";
        இயல்பு:
            நிறுத்து;
    }
    திரும்பு "relu";
}

ACTIVATION get_activation(எழுத்து *s)
{
    எனில் (strcmp(s, "logistic")==0) திரும்பு LOGISTIC;
    எனில் (strcmp(s, "loggy")==0) திரும்பு LOGGY;
    எனில் (strcmp(s, "relu")==0) திரும்பு RELU;
    எனில் (strcmp(s, "elu")==0) திரும்பு ELU;
    எனில் (strcmp(s, "selu")==0) திரும்பு SELU;
    எனில் (strcmp(s, "relie")==0) திரும்பு RELIE;
    எனில் (strcmp(s, "plse")==0) திரும்பு PLSE;
    எனில் (strcmp(s, "hardtan")==0) திரும்பு HARDTAN;
    எனில் (strcmp(s, "lhtan")==0) திரும்பு LHTAN;
    எனில் (strcmp(s, "linear")==0) திரும்பு LINEAR;
    எனில் (strcmp(s, "ramp")==0) திரும்பு RAMP;
    எனில் (strcmp(s, "leaky")==0) திரும்பு LEAKY;
    எனில் (strcmp(s, "tanh")==0) திரும்பு TANH;
    எனில் (strcmp(s, "stair")==0) திரும்பு STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    திரும்பு RELU;
}

தசம activate(தசம x, ACTIVATION a)
{
    தேர்வு(a){
        நிலை LINEAR:
            திரும்பு linear_activate(x);
        நிலை LOGISTIC:
            திரும்பு logistic_activate(x);
        நிலை LOGGY:
            திரும்பு loggy_activate(x);
        நிலை RELU:
            திரும்பு relu_activate(x);
        நிலை ELU:
            திரும்பு elu_activate(x);
        நிலை SELU:
            திரும்பு selu_activate(x);
        நிலை RELIE:
            திரும்பு relie_activate(x);
        நிலை RAMP:
            திரும்பு ramp_activate(x);
        நிலை LEAKY:
            திரும்பு leaky_activate(x);
        நிலை TANH:
            திரும்பு tanh_activate(x);
        நிலை PLSE:
            திரும்பு plse_activate(x);
        நிலை STAIR:
            திரும்பு stair_activate(x);
        நிலை HARDTAN:
            திரும்பு hardtan_activate(x);
        நிலை LHTAN:
            திரும்பு lhtan_activate(x);
    }
    திரும்பு 0;
}

வெறுமை activate_array(தசம *x, மாறிலி முழு n, மாறிலி ACTIVATION a)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

தசம gradient(தசம x, ACTIVATION a)
{
    தேர்வு(a){
        நிலை LINEAR:
            திரும்பு linear_gradient(x);
        நிலை LOGISTIC:
            திரும்பு logistic_gradient(x);
        நிலை LOGGY:
            திரும்பு loggy_gradient(x);
        நிலை RELU:
            திரும்பு relu_gradient(x);
        நிலை ELU:
            திரும்பு elu_gradient(x);
        நிலை SELU:
            திரும்பு selu_gradient(x);
        நிலை RELIE:
            திரும்பு relie_gradient(x);
        நிலை RAMP:
            திரும்பு ramp_gradient(x);
        நிலை LEAKY:
            திரும்பு leaky_gradient(x);
        நிலை TANH:
            திரும்பு tanh_gradient(x);
        நிலை PLSE:
            திரும்பு plse_gradient(x);
        நிலை STAIR:
            திரும்பு stair_gradient(x);
        நிலை HARDTAN:
            திரும்பு hardtan_gradient(x);
        நிலை LHTAN:
            திரும்பு lhtan_gradient(x);
    }
    திரும்பு 0;
}

வெறுமை gradient_array(மாறிலி தசம *x, மாறிலி முழு n, மாறிலி ACTIVATION a, தசம *delta)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

