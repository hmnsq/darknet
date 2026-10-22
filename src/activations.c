#समावेश "activations.h"

#समावेश <math.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

अक्षर *get_activation_string(ACTIVATION a)
{
    चयन(a){
        हाल LOGISTIC:
            वापस "logistic";
        हाल LOGGY:
            वापस "loggy";
        हाल RELU:
            वापस "relu";
        हाल ELU:
            वापस "elu";
        हाल SELU:
            वापस "selu";
        हाल RELIE:
            वापस "relie";
        हाल RAMP:
            वापस "ramp";
        हाल LINEAR:
            वापस "linear";
        हाल TANH:
            वापस "tanh";
        हाल PLSE:
            वापस "plse";
        हाल LEAKY:
            वापस "leaky";
        हाल STAIR:
            वापस "stair";
        हाल HARDTAN:
            वापस "hardtan";
        हाल LHTAN:
            वापस "lhtan";
        शेष:
            अवरोध;
    }
    वापस "relu";
}

ACTIVATION get_activation(अक्षर *s)
{
    अगर (म_भेद(s, "logistic")==0) वापस LOGISTIC;
    अगर (म_भेद(s, "loggy")==0) वापस LOGGY;
    अगर (म_भेद(s, "relu")==0) वापस RELU;
    अगर (म_भेद(s, "elu")==0) वापस ELU;
    अगर (म_भेद(s, "selu")==0) वापस SELU;
    अगर (म_भेद(s, "relie")==0) वापस RELIE;
    अगर (म_भेद(s, "plse")==0) वापस PLSE;
    अगर (म_भेद(s, "hardtan")==0) वापस HARDTAN;
    अगर (म_भेद(s, "lhtan")==0) वापस LHTAN;
    अगर (म_भेद(s, "linear")==0) वापस LINEAR;
    अगर (म_भेद(s, "ramp")==0) वापस RAMP;
    अगर (म_भेद(s, "leaky")==0) वापस LEAKY;
    अगर (म_भेद(s, "tanh")==0) वापस TANH;
    अगर (म_भेद(s, "stair")==0) वापस STAIR;
    ख_लिखो(मानक_त्रुटि, "Couldn't find activation function %s, going with ReLU\n", s);
    वापस RELU;
}

भग्न activate(भग्न x, ACTIVATION a)
{
    चयन(a){
        हाल LINEAR:
            वापस linear_activate(x);
        हाल LOGISTIC:
            वापस logistic_activate(x);
        हाल LOGGY:
            वापस loggy_activate(x);
        हाल RELU:
            वापस relu_activate(x);
        हाल ELU:
            वापस elu_activate(x);
        हाल SELU:
            वापस selu_activate(x);
        हाल RELIE:
            वापस relie_activate(x);
        हाल RAMP:
            वापस ramp_activate(x);
        हाल LEAKY:
            वापस leaky_activate(x);
        हाल TANH:
            वापस tanh_activate(x);
        हाल PLSE:
            वापस plse_activate(x);
        हाल STAIR:
            वापस stair_activate(x);
        हाल HARDTAN:
            वापस hardtan_activate(x);
        हाल LHTAN:
            वापस lhtan_activate(x);
    }
    वापस 0;
}

व्योम activate_array(भग्न *x, स्थिर पूर्णांक n, स्थिर ACTIVATION a)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

भग्न gradient(भग्न x, ACTIVATION a)
{
    चयन(a){
        हाल LINEAR:
            वापस linear_gradient(x);
        हाल LOGISTIC:
            वापस logistic_gradient(x);
        हाल LOGGY:
            वापस loggy_gradient(x);
        हाल RELU:
            वापस relu_gradient(x);
        हाल ELU:
            वापस elu_gradient(x);
        हाल SELU:
            वापस selu_gradient(x);
        हाल RELIE:
            वापस relie_gradient(x);
        हाल RAMP:
            वापस ramp_gradient(x);
        हाल LEAKY:
            वापस leaky_gradient(x);
        हाल TANH:
            वापस tanh_gradient(x);
        हाल PLSE:
            वापस plse_gradient(x);
        हाल STAIR:
            वापस stair_gradient(x);
        हाल HARDTAN:
            वापस hardtan_gradient(x);
        हाल LHTAN:
            वापस lhtan_gradient(x);
    }
    वापस 0;
}

व्योम gradient_array(स्थिर भग्न *x, स्थिर पूर्णांक n, स्थिर ACTIVATION a, भग्न *delta)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

