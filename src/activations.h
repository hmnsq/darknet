#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#அந்தர்பாவ "darknet.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "math.h"

ACTIVATION get_activation(எழுத்து *s);

எழுத்து *get_activation_string(ACTIVATION a);
தசம activate(தசம x, ACTIVATION a);
தசம gradient(தசம x, ACTIVATION a);
வெறுமை gradient_array(மாறிலி தசம *x, மாறிலி முழு n, மாறிலி ACTIVATION a, தசம *delta);
வெறுமை activate_array(தசம *x, மாறிலி முழு n, மாறிலி ACTIVATION a);
#ifdef GPU
வெறுமை activate_array_gpu(தசம *x, முழு n, ACTIVATION a);
வெறுமை gradient_array_gpu(தசம *x, முழு n, ACTIVATION a, தசம *delta);
#endif

நிலையான உள்வரி தசம stair_activate(தசம x)
{
    முழு n = floor(x);
    எனில் (n%2 == 0) திரும்பு floor(x/2.);
    இல்லையெனில் திரும்பு (x - n) + floor(x/2.);
}
நிலையான உள்வரி தசம hardtan_activate(தசம x)
{
    எனில் (x < -1) திரும்பு -1;
    எனில் (x > 1) திரும்பு 1;
    திரும்பு x;
}
நிலையான உள்வரி தசம linear_activate(தசம x){திரும்பு x;}
நிலையான உள்வரி தசம logistic_activate(தசம x){திரும்பு 1./(1. + exp(-x));}
நிலையான உள்வரி தசம loggy_activate(தசம x){திரும்பு 2./(1. + exp(-x)) - 1;}
நிலையான உள்வரி தசம relu_activate(தசம x){திரும்பு x*(x>0);}
நிலையான உள்வரி தசம elu_activate(தசம x){திரும்பு (x >= 0)*x + (x < 0)*(exp(x)-1);}
நிலையான உள்வரி தசம selu_activate(தசம x){திரும்பு (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
நிலையான உள்வரி தசம relie_activate(தசம x){திரும்பு (x>0) ? x : .01*x;}
நிலையான உள்வரி தசம ramp_activate(தசம x){திரும்பு x*(x>0)+.1*x;}
நிலையான உள்வரி தசம leaky_activate(தசம x){திரும்பு (x>0) ? x : .1*x;}
நிலையான உள்வரி தசம tanh_activate(தசம x){திரும்பு (exp(2*x)-1)/(exp(2*x)+1);}
நிலையான உள்வரி தசம plse_activate(தசம x)
{
    எனில்(x < -4) திரும்பு .01 * (x + 4);
    எனில்(x > 4)  திரும்பு .01 * (x - 4) + 1;
    திரும்பு .125*x + .5;
}

நிலையான உள்வரி தசம lhtan_activate(தசம x)
{
    எனில்(x < 0) திரும்பு .001*x;
    எனில்(x > 1) திரும்பு .001*(x-1) + 1;
    திரும்பு x;
}
நிலையான உள்வரி தசம lhtan_gradient(தசம x)
{
    எனில்(x > 0 && x < 1) திரும்பு 1;
    திரும்பு .001;
}

நிலையான உள்வரி தசம hardtan_gradient(தசம x)
{
    எனில் (x > -1 && x < 1) திரும்பு 1;
    திரும்பு 0;
}
நிலையான உள்வரி தசம linear_gradient(தசம x){திரும்பு 1;}
நிலையான உள்வரி தசம logistic_gradient(தசம x){திரும்பு (1-x)*x;}
நிலையான உள்வரி தசம loggy_gradient(தசம x)
{
    தசம y = (x+1.)/2.;
    திரும்பு 2*(1-y)*y;
}
நிலையான உள்வரி தசம stair_gradient(தசம x)
{
    எனில் (floor(x) == x) திரும்பு 0;
    திரும்பு 1;
}
நிலையான உள்வரி தசம relu_gradient(தசம x){திரும்பு (x>0);}
நிலையான உள்வரி தசம elu_gradient(தசம x){திரும்பு (x >= 0) + (x < 0)*(x + 1);}
நிலையான உள்வரி தசம selu_gradient(தசம x){திரும்பு (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
நிலையான உள்வரி தசம relie_gradient(தசம x){திரும்பு (x>0) ? 1 : .01;}
நிலையான உள்வரி தசம ramp_gradient(தசம x){திரும்பு (x>0)+.1;}
நிலையான உள்வரி தசம leaky_gradient(தசம x){திரும்பு (x>0) ? 1 : .1;}
நிலையான உள்வரி தசம tanh_gradient(தசம x){திரும்பு 1-x*x;}
நிலையான உள்வரி தசம plse_gradient(தசம x){திரும்பு (x < 0 || x > 1) ? .01 : .125;}

#endif

