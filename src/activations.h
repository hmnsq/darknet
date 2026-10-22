#अगर_अघोषित ACTIVATIONS_H
#घोषणा ACTIVATIONS_H
#समावेश "darknet.h"
#समावेश "cuda.h"
#समावेश "math.h"

ACTIVATION get_activation(अक्षर *s);

अक्षर *get_activation_string(ACTIVATION a);
भग्न activate(भग्न x, ACTIVATION a);
भग्न gradient(भग्न x, ACTIVATION a);
व्योम gradient_array(स्थिर भग्न *x, स्थिर पूर्णांक n, स्थिर ACTIVATION a, भग्न *delta);
व्योम activate_array(भग्न *x, स्थिर पूर्णांक n, स्थिर ACTIVATION a);
#अगर_घोषित GPU
व्योम activate_array_gpu(भग्न *x, पूर्णांक n, ACTIVATION a);
व्योम gradient_array_gpu(भग्न *x, पूर्णांक n, ACTIVATION a, भग्न *delta);
#पूर्ण_अगर

अटल अंतरभूत भग्न stair_activate(भग्न x)
{
    पूर्णांक n = न्यूनमान(x);
    अगर (n%2 == 0) वापस न्यूनमान(x/2.);
    अन्यथा वापस (x - n) + न्यूनमान(x/2.);
}
अटल अंतरभूत भग्न hardtan_activate(भग्न x)
{
    अगर (x < -1) वापस -1;
    अगर (x > 1) वापस 1;
    वापस x;
}
अटल अंतरभूत भग्न linear_activate(भग्न x){वापस x;}
अटल अंतरभूत भग्न logistic_activate(भग्न x){वापस 1./(1. + exp(-x));}
अटल अंतरभूत भग्न loggy_activate(भग्न x){वापस 2./(1. + exp(-x)) - 1;}
अटल अंतरभूत भग्न relu_activate(भग्न x){वापस x*(x>0);}
अटल अंतरभूत भग्न elu_activate(भग्न x){वापस (x >= 0)*x + (x < 0)*(exp(x)-1);}
अटल अंतरभूत भग्न selu_activate(भग्न x){वापस (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
अटल अंतरभूत भग्न relie_activate(भग्न x){वापस (x>0) ? x : .01*x;}
अटल अंतरभूत भग्न ramp_activate(भग्न x){वापस x*(x>0)+.1*x;}
अटल अंतरभूत भग्न leaky_activate(भग्न x){वापस (x>0) ? x : .1*x;}
अटल अंतरभूत भग्न tanh_activate(भग्न x){वापस (exp(2*x)-1)/(exp(2*x)+1);}
अटल अंतरभूत भग्न plse_activate(भग्न x)
{
    अगर(x < -4) वापस .01 * (x + 4);
    अगर(x > 4)  वापस .01 * (x - 4) + 1;
    वापस .125*x + .5;
}

अटल अंतरभूत भग्न lhtan_activate(भग्न x)
{
    अगर(x < 0) वापस .001*x;
    अगर(x > 1) वापस .001*(x-1) + 1;
    वापस x;
}
अटल अंतरभूत भग्न lhtan_gradient(भग्न x)
{
    अगर(x > 0 && x < 1) वापस 1;
    वापस .001;
}

अटल अंतरभूत भग्न hardtan_gradient(भग्न x)
{
    अगर (x > -1 && x < 1) वापस 1;
    वापस 0;
}
अटल अंतरभूत भग्न linear_gradient(भग्न x){वापस 1;}
अटल अंतरभूत भग्न logistic_gradient(भग्न x){वापस (1-x)*x;}
अटल अंतरभूत भग्न loggy_gradient(भग्न x)
{
    भग्न y = (x+1.)/2.;
    वापस 2*(1-y)*y;
}
अटल अंतरभूत भग्न stair_gradient(भग्न x)
{
    अगर (न्यूनमान(x) == x) वापस 0;
    वापस 1;
}
अटल अंतरभूत भग्न relu_gradient(भग्न x){वापस (x>0);}
अटल अंतरभूत भग्न elu_gradient(भग्न x){वापस (x >= 0) + (x < 0)*(x + 1);}
अटल अंतरभूत भग्न selu_gradient(भग्न x){वापस (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
अटल अंतरभूत भग्न relie_gradient(भग्न x){वापस (x>0) ? 1 : .01;}
अटल अंतरभूत भग्न ramp_gradient(भग्न x){वापस (x>0)+.1;}
अटल अंतरभूत भग्न leaky_gradient(भग्न x){वापस (x>0) ? 1 : .1;}
अटल अंतरभूत भग्न tanh_gradient(भग्न x){वापस 1-x*x;}
अटल अंतरभूत भग्न plse_gradient(भग्न x){वापस (x < 0 || x > 1) ? .01 : .125;}

#पूर्ण_अगर

