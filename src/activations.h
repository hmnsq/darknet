#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(char *s);

char *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const số_nguyên n, const ACTIVATION a, float *delta);
void activate_array(float *x, const số_nguyên n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, số_nguyên n, ACTIVATION a);
void gradient_array_gpu(float *x, số_nguyên n, ACTIVATION a, float *delta);
#endif

static inline float stair_activate(float x)
{
    số_nguyên n = floor(x);
    nếu (n%2 == 0) trả_về floor(x/2.);
    không_thì trả_về (x - n) + floor(x/2.);
}
static inline float hardtan_activate(float x)
{
    nếu (x < -1) trả_về -1;
    nếu (x > 1) trả_về 1;
    trả_về x;
}
static inline float linear_activate(float x){trả_về x;}
static inline float logistic_activate(float x){trả_về 1./(1. + exp(-x));}
static inline float loggy_activate(float x){trả_về 2./(1. + exp(-x)) - 1;}
static inline float relu_activate(float x){trả_về x*(x>0);}
static inline float elu_activate(float x){trả_về (x >= 0)*x + (x < 0)*(exp(x)-1);}
static inline float selu_activate(float x){trả_về (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static inline float relie_activate(float x){trả_về (x>0) ? x : .01*x;}
static inline float ramp_activate(float x){trả_về x*(x>0)+.1*x;}
static inline float leaky_activate(float x){trả_về (x>0) ? x : .1*x;}
static inline float tanh_activate(float x){trả_về (exp(2*x)-1)/(exp(2*x)+1);}
static inline float plse_activate(float x)
{
    nếu(x < -4) trả_về .01 * (x + 4);
    nếu(x > 4)  trả_về .01 * (x - 4) + 1;
    trả_về .125*x + .5;
}

static inline float lhtan_activate(float x)
{
    nếu(x < 0) trả_về .001*x;
    nếu(x > 1) trả_về .001*(x-1) + 1;
    trả_về x;
}
static inline float lhtan_gradient(float x)
{
    nếu(x > 0 && x < 1) trả_về 1;
    trả_về .001;
}

static inline float hardtan_gradient(float x)
{
    nếu (x > -1 && x < 1) trả_về 1;
    trả_về 0;
}
static inline float linear_gradient(float x){trả_về 1;}
static inline float logistic_gradient(float x){trả_về (1-x)*x;}
static inline float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    trả_về 2*(1-y)*y;
}
static inline float stair_gradient(float x)
{
    nếu (floor(x) == x) trả_về 0;
    trả_về 1;
}
static inline float relu_gradient(float x){trả_về (x>0);}
static inline float elu_gradient(float x){trả_về (x >= 0) + (x < 0)*(x + 1);}
static inline float selu_gradient(float x){trả_về (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static inline float relie_gradient(float x){trả_về (x>0) ? 1 : .01;}
static inline float ramp_gradient(float x){trả_về (x>0)+.1;}
static inline float leaky_gradient(float x){trả_về (x>0) ? 1 : .1;}
static inline float tanh_gradient(float x){trả_về 1-x*x;}
static inline float plse_gradient(float x){trả_về (x < 0 || x > 1) ? .01 : .125;}

#endif

