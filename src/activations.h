#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#ضمن "darknet.h"
#ضمن "cuda.h"
#ضمن "math.h"

ACTIVATION get_activation(حرف *s);

حرف *get_activation_string(ACTIVATION a);
عائم activate(عائم x, ACTIVATION a);
عائم gradient(عائم x, ACTIVATION a);
فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta);
فراغ activate_array(عائم *x, ثابت صحيح n, ثابت ACTIVATION a);
#ifdef GPU
فراغ activate_array_gpu(عائم *x, صحيح n, ACTIVATION a);
فراغ gradient_array_gpu(عائم *x, صحيح n, ACTIVATION a, عائم *delta);
#endif

ساكن مضمن عائم stair_activate(عائم x)
{
    صحيح n = floor(x);
    אם (n%2 == 0) החזר floor(x/2.);
    אחרת החזר (x - n) + floor(x/2.);
}
ساكن مضمن عائم hardtan_activate(عائم x)
{
    אם (x < -1) החזר -1;
    אם (x > 1) החזר 1;
    החזר x;
}
ساكن مضمن عائم linear_activate(عائم x){החזר x;}
ساكن مضمن عائم logistic_activate(عائم x){החזר 1./(1. + exp(-x));}
ساكن مضمن عائم loggy_activate(عائم x){החזר 2./(1. + exp(-x)) - 1;}
ساكن مضمن عائم relu_activate(عائم x){החזר x*(x>0);}
ساكن مضمن عائم elu_activate(عائم x){החזר (x >= 0)*x + (x < 0)*(exp(x)-1);}
ساكن مضمن عائم selu_activate(عائم x){החזר (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
ساكن مضمن عائم relie_activate(عائم x){החזר (x>0) ? x : .01*x;}
ساكن مضمن عائم ramp_activate(عائم x){החזר x*(x>0)+.1*x;}
ساكن مضمن عائم leaky_activate(عائم x){החזר (x>0) ? x : .1*x;}
ساكن مضمن عائم tanh_activate(عائم x){החזר (exp(2*x)-1)/(exp(2*x)+1);}
ساكن مضمن عائم plse_activate(عائم x)
{
    אם(x < -4) החזר .01 * (x + 4);
    אם(x > 4)  החזר .01 * (x - 4) + 1;
    החזר .125*x + .5;
}

ساكن مضمن عائم lhtan_activate(عائم x)
{
    אם(x < 0) החזר .001*x;
    אם(x > 1) החזר .001*(x-1) + 1;
    החזר x;
}
ساكن مضمن عائم lhtan_gradient(عائم x)
{
    אם(x > 0 && x < 1) החזר 1;
    החזר .001;
}

ساكن مضمن عائم hardtan_gradient(عائم x)
{
    אם (x > -1 && x < 1) החזר 1;
    החזר 0;
}
ساكن مضمن عائم linear_gradient(عائم x){החזר 1;}
ساكن مضمن عائم logistic_gradient(عائم x){החזר (1-x)*x;}
ساكن مضمن عائم loggy_gradient(عائم x)
{
    عائم y = (x+1.)/2.;
    החזר 2*(1-y)*y;
}
ساكن مضمن عائم stair_gradient(عائم x)
{
    אם (floor(x) == x) החזר 0;
    החזר 1;
}
ساكن مضمن عائم relu_gradient(عائم x){החזר (x>0);}
ساكن مضمن عائم elu_gradient(عائم x){החזר (x >= 0) + (x < 0)*(x + 1);}
ساكن مضمن عائم selu_gradient(عائم x){החזר (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
ساكن مضمن عائم relie_gradient(عائم x){החזר (x>0) ? 1 : .01;}
ساكن مضمن عائم ramp_gradient(عائم x){החזר (x>0)+.1;}
ساكن مضمن عائم leaky_gradient(عائم x){החזר (x>0) ? 1 : .1;}
ساكن مضمن عائم tanh_gradient(عائم x){החזר 1-x*x;}
ساكن مضمن عائم plse_gradient(عائم x){החזר (x < 0 || x > 1) ? .01 : .125;}

#endif

