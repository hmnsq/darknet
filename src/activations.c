#अन्तर्भाव "activations.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

अक्षर *get_activation_string(ACTIVATION a)
{
    निवड(a){
        स्थिती LOGISTIC:
            परत "logistic";
        स्थिती LOGGY:
            परत "loggy";
        स्थिती RELU:
            परत "relu";
        स्थिती ELU:
            परत "elu";
        स्थिती SELU:
            परत "selu";
        स्थिती RELIE:
            परत "relie";
        स्थिती RAMP:
            परत "ramp";
        स्थिती LINEAR:
            परत "linear";
        स्थिती TANH:
            परत "tanh";
        स्थिती PLSE:
            परत "plse";
        स्थिती LEAKY:
            परत "leaky";
        स्थिती STAIR:
            परत "stair";
        स्थिती HARDTAN:
            परत "hardtan";
        स्थिती LHTAN:
            परत "lhtan";
        पूर्वनिर्धारित:
            थांबा;
    }
    परत "relu";
}

ACTIVATION get_activation(अक्षर *s)
{
    जर (म_भेद(s, "logistic")==0) परत LOGISTIC;
    जर (म_भेद(s, "loggy")==0) परत LOGGY;
    जर (म_भेद(s, "relu")==0) परत RELU;
    जर (म_भेद(s, "elu")==0) परत ELU;
    जर (म_भेद(s, "selu")==0) परत SELU;
    जर (म_भेद(s, "relie")==0) परत RELIE;
    जर (म_भेद(s, "plse")==0) परत PLSE;
    जर (म_भेद(s, "hardtan")==0) परत HARDTAN;
    जर (म_भेद(s, "lhtan")==0) परत LHTAN;
    जर (म_भेद(s, "linear")==0) परत LINEAR;
    जर (म_भेद(s, "ramp")==0) परत RAMP;
    जर (म_भेद(s, "leaky")==0) परत LEAKY;
    जर (म_भेद(s, "tanh")==0) परत TANH;
    जर (म_भेद(s, "stair")==0) परत STAIR;
    ख_लिखो(मानक_त्रुटि, "Couldn't find activation function %s, going with ReLU\n", s);
    परत RELU;
}

दशांश activate(दशांश x, ACTIVATION a)
{
    निवड(a){
        स्थिती LINEAR:
            परत linear_activate(x);
        स्थिती LOGISTIC:
            परत logistic_activate(x);
        स्थिती LOGGY:
            परत loggy_activate(x);
        स्थिती RELU:
            परत relu_activate(x);
        स्थिती ELU:
            परत elu_activate(x);
        स्थिती SELU:
            परत selu_activate(x);
        स्थिती RELIE:
            परत relie_activate(x);
        स्थिती RAMP:
            परत ramp_activate(x);
        स्थिती LEAKY:
            परत leaky_activate(x);
        स्थिती TANH:
            परत tanh_activate(x);
        स्थिती PLSE:
            परत plse_activate(x);
        स्थिती STAIR:
            परत stair_activate(x);
        स्थिती HARDTAN:
            परत hardtan_activate(x);
        स्थिती LHTAN:
            परत lhtan_activate(x);
    }
    परत 0;
}

शून्य activate_array(दशांश *x, स्थिरांक पूर्णांक n, स्थिरांक ACTIVATION a)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

दशांश gradient(दशांश x, ACTIVATION a)
{
    निवड(a){
        स्थिती LINEAR:
            परत linear_gradient(x);
        स्थिती LOGISTIC:
            परत logistic_gradient(x);
        स्थिती LOGGY:
            परत loggy_gradient(x);
        स्थिती RELU:
            परत relu_gradient(x);
        स्थिती ELU:
            परत elu_gradient(x);
        स्थिती SELU:
            परत selu_gradient(x);
        स्थिती RELIE:
            परत relie_gradient(x);
        स्थिती RAMP:
            परत ramp_gradient(x);
        स्थिती LEAKY:
            परत leaky_gradient(x);
        स्थिती TANH:
            परत tanh_gradient(x);
        स्थिती PLSE:
            परत plse_gradient(x);
        स्थिती STAIR:
            परत stair_gradient(x);
        स्थिती HARDTAN:
            परत hardtan_gradient(x);
        स्थिती LHTAN:
            परत lhtan_gradient(x);
    }
    परत 0;
}

शून्य gradient_array(स्थिरांक दशांश *x, स्थिरांक पूर्णांक n, स्थिरांक ACTIVATION a, दशांश *delta)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

