#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#അന്തര്ഭാവ "darknet.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "math.h"

ACTIVATION get_activation(അക്ഷരം *s);

അക്ഷരം *get_activation_string(ACTIVATION a);
ദശാംശം activate(ദശാംശം x, ACTIVATION a);
ദശാംശം gradient(ദശാംശം x, ACTIVATION a);
ശൂന്യം gradient_array(സ്ഥിരം ദശാംശം *x, സ്ഥിരം പൂർണ്ണസംഖ്യ n, സ്ഥിരം ACTIVATION a, ദശാംശം *delta);
ശൂന്യം activate_array(ദശാംശം *x, സ്ഥിരം പൂർണ്ണസംഖ്യ n, സ്ഥിരം ACTIVATION a);
#ifdef GPU
ശൂന്യം activate_array_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ n, ACTIVATION a);
ശൂന്യം gradient_array_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ n, ACTIVATION a, ദശാംശം *delta);
#endif

സ്ഥിതം വരിയിൽ ദശാംശം stair_activate(ദശാംശം x)
{
    പൂർണ്ണസംഖ്യ n = floor(x);
    എങ്കിൽ (n%2 == 0) തിരികെ floor(x/2.);
    അല്ലെങ്കിൽ തിരികെ (x - n) + floor(x/2.);
}
സ്ഥിതം വരിയിൽ ദശാംശം hardtan_activate(ദശാംശം x)
{
    എങ്കിൽ (x < -1) തിരികെ -1;
    എങ്കിൽ (x > 1) തിരികെ 1;
    തിരികെ x;
}
സ്ഥിതം വരിയിൽ ദശാംശം linear_activate(ദശാംശം x){തിരികെ x;}
സ്ഥിതം വരിയിൽ ദശാംശം logistic_activate(ദശാംശം x){തിരികെ 1./(1. + exp(-x));}
സ്ഥിതം വരിയിൽ ദശാംശം loggy_activate(ദശാംശം x){തിരികെ 2./(1. + exp(-x)) - 1;}
സ്ഥിതം വരിയിൽ ദശാംശം relu_activate(ദശാംശം x){തിരികെ x*(x>0);}
സ്ഥിതം വരിയിൽ ദശാംശം elu_activate(ദശാംശം x){തിരികെ (x >= 0)*x + (x < 0)*(exp(x)-1);}
സ്ഥിതം വരിയിൽ ദശാംശം selu_activate(ദശാംശം x){തിരികെ (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
സ്ഥിതം വരിയിൽ ദശാംശം relie_activate(ദശാംശം x){തിരികെ (x>0) ? x : .01*x;}
സ്ഥിതം വരിയിൽ ദശാംശം ramp_activate(ദശാംശം x){തിരികെ x*(x>0)+.1*x;}
സ്ഥിതം വരിയിൽ ദശാംശം leaky_activate(ദശാംശം x){തിരികെ (x>0) ? x : .1*x;}
സ്ഥിതം വരിയിൽ ദശാംശം tanh_activate(ദശാംശം x){തിരികെ (exp(2*x)-1)/(exp(2*x)+1);}
സ്ഥിതം വരിയിൽ ദശാംശം plse_activate(ദശാംശം x)
{
    എങ്കിൽ(x < -4) തിരികെ .01 * (x + 4);
    എങ്കിൽ(x > 4)  തിരികെ .01 * (x - 4) + 1;
    തിരികെ .125*x + .5;
}

സ്ഥിതം വരിയിൽ ദശാംശം lhtan_activate(ദശാംശം x)
{
    എങ്കിൽ(x < 0) തിരികെ .001*x;
    എങ്കിൽ(x > 1) തിരികെ .001*(x-1) + 1;
    തിരികെ x;
}
സ്ഥിതം വരിയിൽ ദശാംശം lhtan_gradient(ദശാംശം x)
{
    എങ്കിൽ(x > 0 && x < 1) തിരികെ 1;
    തിരികെ .001;
}

സ്ഥിതം വരിയിൽ ദശാംശം hardtan_gradient(ദശാംശം x)
{
    എങ്കിൽ (x > -1 && x < 1) തിരികെ 1;
    തിരികെ 0;
}
സ്ഥിതം വരിയിൽ ദശാംശം linear_gradient(ദശാംശം x){തിരികെ 1;}
സ്ഥിതം വരിയിൽ ദശാംശം logistic_gradient(ദശാംശം x){തിരികെ (1-x)*x;}
സ്ഥിതം വരിയിൽ ദശാംശം loggy_gradient(ദശാംശം x)
{
    ദശാംശം y = (x+1.)/2.;
    തിരികെ 2*(1-y)*y;
}
സ്ഥിതം വരിയിൽ ദശാംശം stair_gradient(ദശാംശം x)
{
    എങ്കിൽ (floor(x) == x) തിരികെ 0;
    തിരികെ 1;
}
സ്ഥിതം വരിയിൽ ദശാംശം relu_gradient(ദശാംശം x){തിരികെ (x>0);}
സ്ഥിതം വരിയിൽ ദശാംശം elu_gradient(ദശാംശം x){തിരികെ (x >= 0) + (x < 0)*(x + 1);}
സ്ഥിതം വരിയിൽ ദശാംശം selu_gradient(ദശാംശം x){തിരികെ (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
സ്ഥിതം വരിയിൽ ദശാംശം relie_gradient(ദശാംശം x){തിരികെ (x>0) ? 1 : .01;}
സ്ഥിതം വരിയിൽ ദശാംശം ramp_gradient(ദശാംശം x){തിരികെ (x>0)+.1;}
സ്ഥിതം വരിയിൽ ദശാംശം leaky_gradient(ദശാംശം x){തിരികെ (x>0) ? 1 : .1;}
സ്ഥിതം വരിയിൽ ദശാംശം tanh_gradient(ദശാംശം x){തിരികെ 1-x*x;}
സ്ഥിതം വരിയിൽ ദശാംശം plse_gradient(ദശാംശം x){തിരികെ (x < 0 || x > 1) ? .01 : .125;}

#endif

