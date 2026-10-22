#अन्तर्भाव "activations.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

वर्ण *get_activation_string(ACTIVATION a)
{
    विकल्प(a){
        पक्ष LOGISTIC:
            पटिनिवत्त "logistic";
        पक्ष LOGGY:
            पटिनिवत्त "loggy";
        पक्ष RELU:
            पटिनिवत्त "relu";
        पक्ष ELU:
            पटिनिवत्त "elu";
        पक्ष SELU:
            पटिनिवत्त "selu";
        पक्ष RELIE:
            पटिनिवत्त "relie";
        पक्ष RAMP:
            पटिनिवत्त "ramp";
        पक्ष LINEAR:
            पटिनिवत्त "linear";
        पक्ष TANH:
            पटिनिवत्त "tanh";
        पक्ष PLSE:
            पटिनिवत्त "plse";
        पक्ष LEAKY:
            पटिनिवत्त "leaky";
        पक्ष STAIR:
            पटिनिवत्त "stair";
        पक्ष HARDTAN:
            पटिनिवत्त "hardtan";
        पक्ष LHTAN:
            पटिनिवत्त "lhtan";
        स्वभाव:
            भङ्ग;
    }
    पटिनिवत्त "relu";
}

ACTIVATION get_activation(वर्ण *s)
{
    सचे (strcmp(s, "logistic")==0) पटिनिवत्त LOGISTIC;
    सचे (strcmp(s, "loggy")==0) पटिनिवत्त LOGGY;
    सचे (strcmp(s, "relu")==0) पटिनिवत्त RELU;
    सचे (strcmp(s, "elu")==0) पटिनिवत्त ELU;
    सचे (strcmp(s, "selu")==0) पटिनिवत्त SELU;
    सचे (strcmp(s, "relie")==0) पटिनिवत्त RELIE;
    सचे (strcmp(s, "plse")==0) पटिनिवत्त PLSE;
    सचे (strcmp(s, "hardtan")==0) पटिनिवत्त HARDTAN;
    सचे (strcmp(s, "lhtan")==0) पटिनिवत्त LHTAN;
    सचे (strcmp(s, "linear")==0) पटिनिवत्त LINEAR;
    सचे (strcmp(s, "ramp")==0) पटिनिवत्त RAMP;
    सचे (strcmp(s, "leaky")==0) पटिनिवत्त LEAKY;
    सचे (strcmp(s, "tanh")==0) पटिनिवत्त TANH;
    सचे (strcmp(s, "stair")==0) पटिनिवत्त STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    पटिनिवत्त RELU;
}

भिन्न activate(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            पटिनिवत्त linear_activate(x);
        पक्ष LOGISTIC:
            पटिनिवत्त logistic_activate(x);
        पक्ष LOGGY:
            पटिनिवत्त loggy_activate(x);
        पक्ष RELU:
            पटिनिवत्त relu_activate(x);
        पक्ष ELU:
            पटिनिवत्त elu_activate(x);
        पक्ष SELU:
            पटिनिवत्त selu_activate(x);
        पक्ष RELIE:
            पटिनिवत्त relie_activate(x);
        पक्ष RAMP:
            पटिनिवत्त ramp_activate(x);
        पक्ष LEAKY:
            पटिनिवत्त leaky_activate(x);
        पक्ष TANH:
            पटिनिवत्त tanh_activate(x);
        पक्ष PLSE:
            पटिनिवत्त plse_activate(x);
        पक्ष STAIR:
            पटिनिवत्त stair_activate(x);
        पक्ष HARDTAN:
            पटिनिवत्त hardtan_activate(x);
        पक्ष LHTAN:
            पटिनिवत्त lhtan_activate(x);
    }
    पटिनिवत्त 0;
}

शून्य activate_array(भिन्न *x, ध्रुव पूर्णाङ्क n, ध्रुव ACTIVATION a)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

भिन्न gradient(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            पटिनिवत्त linear_gradient(x);
        पक्ष LOGISTIC:
            पटिनिवत्त logistic_gradient(x);
        पक्ष LOGGY:
            पटिनिवत्त loggy_gradient(x);
        पक्ष RELU:
            पटिनिवत्त relu_gradient(x);
        पक्ष ELU:
            पटिनिवत्त elu_gradient(x);
        पक्ष SELU:
            पटिनिवत्त selu_gradient(x);
        पक्ष RELIE:
            पटिनिवत्त relie_gradient(x);
        पक्ष RAMP:
            पटिनिवत्त ramp_gradient(x);
        पक्ष LEAKY:
            पटिनिवत्त leaky_gradient(x);
        पक्ष TANH:
            पटिनिवत्त tanh_gradient(x);
        पक्ष PLSE:
            पटिनिवत्त plse_gradient(x);
        पक्ष STAIR:
            पटिनिवत्त stair_gradient(x);
        पक्ष HARDTAN:
            पटिनिवत्त hardtan_gradient(x);
        पक्ष LHTAN:
            पटिनिवत्त lhtan_gradient(x);
    }
    पटिनिवत्त 0;
}

शून्य gradient_array(ध्रुव भिन्न *x, ध्रुव पूर्णाङ्क n, ध्रुव ACTIVATION a, भिन्न *delta)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

