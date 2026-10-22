#که_نه_تعریف_شوی ACTIVATIONS_H
#تعریف_کړه ACTIVATIONS_H
#ضمن "darknet.h"
#ضمن "cuda.h"
#ضمن "math.h"

ACTIVATION get_activation(حرف *s);

حرف *get_activation_string(ACTIVATION a);
عائم activate(عائم x, ACTIVATION a);
عائم gradient(عائم x, ACTIVATION a);
فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta);
فراغ activate_array(عائم *x, ثابت صحيح n, ثابت ACTIVATION a);
#که_تعریف_شوی GPU
فراغ activate_array_gpu(عائم *x, صحيح n, ACTIVATION a);
فراغ gradient_array_gpu(عائم *x, صحيح n, ACTIVATION a, عائم *delta);
#د_که_پای

ساكن مضمن عائم stair_activate(عائم x)
{
    صحيح n = floor(x);
    إذا (n%2 == 0) أرجع floor(x/2.);
    وإلا أرجع (x - n) + floor(x/2.);
}
ساكن مضمن عائم hardtan_activate(عائم x)
{
    إذا (x < -1) أرجع -1;
    إذا (x > 1) أرجع 1;
    أرجع x;
}
ساكن مضمن عائم خطي_activate(عائم x){أرجع x;}
ساكن مضمن عائم logistic_activate(عائم x){أرجع 1./(1. + توانیز_تابع(-x));}
ساكن مضمن عائم loggy_activate(عائم x){أرجع 2./(1. + توانیز_تابع(-x)) - 1;}
ساكن مضمن عائم relu_activate(عائم x){أرجع x*(x>0);}
ساكن مضمن عائم elu_activate(عائم x){أرجع (x >= 0)*x + (x < 0)*(توانیز_تابع(x)-1);}
ساكن مضمن عائم selu_activate(عائم x){أرجع (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(توانیز_تابع(x)-1);}
ساكن مضمن عائم relie_activate(عائم x){أرجع (x>0) ? x : .01*x;}
ساكن مضمن عائم ramp_activate(عائم x){أرجع x*(x>0)+.1*x;}
ساكن مضمن عائم leaky_activate(عائم x){أرجع (x>0) ? x : .1*x;}
ساكن مضمن عائم tanh_activate(عائم x){أرجع (توانیز_تابع(2*x)-1)/(توانیز_تابع(2*x)+1);}
ساكن مضمن عائم plse_activate(عائم x)
{
    إذا(x < -4) أرجع .01 * (x + 4);
    إذا(x > 4)  أرجع .01 * (x - 4) + 1;
    أرجع .125*x + .5;
}

ساكن مضمن عائم lhtan_activate(عائم x)
{
    إذا(x < 0) أرجع .001*x;
    إذا(x > 1) أرجع .001*(x-1) + 1;
    أرجع x;
}
ساكن مضمن عائم lhtan_gradient(عائم x)
{
    إذا(x > 0 && x < 1) أرجع 1;
    أرجع .001;
}

ساكن مضمن عائم hardtan_gradient(عائم x)
{
    إذا (x > -1 && x < 1) أرجع 1;
    أرجع 0;
}
ساكن مضمن عائم خطي_gradient(عائم x){أرجع 1;}
ساكن مضمن عائم logistic_gradient(عائم x){أرجع (1-x)*x;}
ساكن مضمن عائم loggy_gradient(عائم x)
{
    عائم y = (x+1.)/2.;
    أرجع 2*(1-y)*y;
}
ساكن مضمن عائم stair_gradient(عائم x)
{
    إذا (floor(x) == x) أرجع 0;
    أرجع 1;
}
ساكن مضمن عائم relu_gradient(عائم x){أرجع (x>0);}
ساكن مضمن عائم elu_gradient(عائم x){أرجع (x >= 0) + (x < 0)*(x + 1);}
ساكن مضمن عائم selu_gradient(عائم x){أرجع (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
ساكن مضمن عائم relie_gradient(عائم x){أرجع (x>0) ? 1 : .01;}
ساكن مضمن عائم ramp_gradient(عائم x){أرجع (x>0)+.1;}
ساكن مضمن عائم leaky_gradient(عائم x){أرجع (x>0) ? 1 : .1;}
ساكن مضمن عائم tanh_gradient(عائم x){أرجع 1-x*x;}
ساكن مضمن عائم plse_gradient(عائم x){أرجع (x < 0 || x > 1) ? .01 : .125;}

#د_که_پای

