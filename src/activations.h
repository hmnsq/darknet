#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#अन्तर्भाव "darknet.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "math.h"

ACTIVATION get_activation(अक्षर *s);

अक्षर *get_activation_string(ACTIVATION a);
दशांश activate(दशांश x, ACTIVATION a);
दशांश gradient(दशांश x, ACTIVATION a);
शून्य gradient_array(स्थिरांक दशांश *x, स्थिरांक पूर्णांक n, स्थिरांक ACTIVATION a, दशांश *delta);
शून्य activate_array(दशांश *x, स्थिरांक पूर्णांक n, स्थिरांक ACTIVATION a);
#ifdef GPU
शून्य activate_array_gpu(दशांश *x, पूर्णांक n, ACTIVATION a);
शून्य gradient_array_gpu(दशांश *x, पूर्णांक n, ACTIVATION a, दशांश *delta);
#endif

स्थिर ओळीत दशांश stair_activate(दशांश x)
{
    पूर्णांक n = न्यूनमान(x);
    जर (n%2 == 0) परत न्यूनमान(x/2.);
    अन्यथा परत (x - n) + न्यूनमान(x/2.);
}
स्थिर ओळीत दशांश hardtan_activate(दशांश x)
{
    जर (x < -1) परत -1;
    जर (x > 1) परत 1;
    परत x;
}
स्थिर ओळीत दशांश linear_activate(दशांश x){परत x;}
स्थिर ओळीत दशांश logistic_activate(दशांश x){परत 1./(1. + exp(-x));}
स्थिर ओळीत दशांश loggy_activate(दशांश x){परत 2./(1. + exp(-x)) - 1;}
स्थिर ओळीत दशांश relu_activate(दशांश x){परत x*(x>0);}
स्थिर ओळीत दशांश elu_activate(दशांश x){परत (x >= 0)*x + (x < 0)*(exp(x)-1);}
स्थिर ओळीत दशांश selu_activate(दशांश x){परत (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
स्थिर ओळीत दशांश relie_activate(दशांश x){परत (x>0) ? x : .01*x;}
स्थिर ओळीत दशांश ramp_activate(दशांश x){परत x*(x>0)+.1*x;}
स्थिर ओळीत दशांश leaky_activate(दशांश x){परत (x>0) ? x : .1*x;}
स्थिर ओळीत दशांश tanh_activate(दशांश x){परत (exp(2*x)-1)/(exp(2*x)+1);}
स्थिर ओळीत दशांश plse_activate(दशांश x)
{
    जर(x < -4) परत .01 * (x + 4);
    जर(x > 4)  परत .01 * (x - 4) + 1;
    परत .125*x + .5;
}

स्थिर ओळीत दशांश lhtan_activate(दशांश x)
{
    जर(x < 0) परत .001*x;
    जर(x > 1) परत .001*(x-1) + 1;
    परत x;
}
स्थिर ओळीत दशांश lhtan_gradient(दशांश x)
{
    जर(x > 0 && x < 1) परत 1;
    परत .001;
}

स्थिर ओळीत दशांश hardtan_gradient(दशांश x)
{
    जर (x > -1 && x < 1) परत 1;
    परत 0;
}
स्थिर ओळीत दशांश linear_gradient(दशांश x){परत 1;}
स्थिर ओळीत दशांश logistic_gradient(दशांश x){परत (1-x)*x;}
स्थिर ओळीत दशांश loggy_gradient(दशांश x)
{
    दशांश y = (x+1.)/2.;
    परत 2*(1-y)*y;
}
स्थिर ओळीत दशांश stair_gradient(दशांश x)
{
    जर (न्यूनमान(x) == x) परत 0;
    परत 1;
}
स्थिर ओळीत दशांश relu_gradient(दशांश x){परत (x>0);}
स्थिर ओळीत दशांश elu_gradient(दशांश x){परत (x >= 0) + (x < 0)*(x + 1);}
स्थिर ओळीत दशांश selu_gradient(दशांश x){परत (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
स्थिर ओळीत दशांश relie_gradient(दशांश x){परत (x>0) ? 1 : .01;}
स्थिर ओळीत दशांश ramp_gradient(दशांश x){परत (x>0)+.1;}
स्थिर ओळीत दशांश leaky_gradient(दशांश x){परत (x>0) ? 1 : .1;}
स्थिर ओळीत दशांश tanh_gradient(दशांश x){परत 1-x*x;}
स्थिर ओळीत दशांश plse_gradient(दशांश x){परत (x < 0 || x > 1) ? .01 : .125;}

#endif

