#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(char *s);

char *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const цео n, const ACTIVATION a, float *delta);
void activate_array(float *x, const цео n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, цео n, ACTIVATION a);
void gradient_array_gpu(float *x, цео n, ACTIVATION a, float *delta);
#endif

static inline float stair_activate(float x)
{
    цео n = floor(x);
    ако (n%2 == 0) врати floor(x/2.);
    инако врати (x - n) + floor(x/2.);
}
static inline float hardtan_activate(float x)
{
    ако (x < -1) врати -1;
    ако (x > 1) врати 1;
    врати x;
}
static inline float linear_activate(float x){врати x;}
static inline float logistic_activate(float x){врати 1./(1. + exp(-x));}
static inline float loggy_activate(float x){врати 2./(1. + exp(-x)) - 1;}
static inline float relu_activate(float x){врати x*(x>0);}
static inline float elu_activate(float x){врати (x >= 0)*x + (x < 0)*(exp(x)-1);}
static inline float selu_activate(float x){врати (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static inline float relie_activate(float x){врати (x>0) ? x : .01*x;}
static inline float ramp_activate(float x){врати x*(x>0)+.1*x;}
static inline float leaky_activate(float x){врати (x>0) ? x : .1*x;}
static inline float tanh_activate(float x){врати (exp(2*x)-1)/(exp(2*x)+1);}
static inline float plse_activate(float x)
{
    ако(x < -4) врати .01 * (x + 4);
    ако(x > 4)  врати .01 * (x - 4) + 1;
    врати .125*x + .5;
}

static inline float lhtan_activate(float x)
{
    ако(x < 0) врати .001*x;
    ако(x > 1) врати .001*(x-1) + 1;
    врати x;
}
static inline float lhtan_gradient(float x)
{
    ако(x > 0 && x < 1) врати 1;
    врати .001;
}

static inline float hardtan_gradient(float x)
{
    ако (x > -1 && x < 1) врати 1;
    врати 0;
}
static inline float linear_gradient(float x){врати 1;}
static inline float logistic_gradient(float x){врати (1-x)*x;}
static inline float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    врати 2*(1-y)*y;
}
static inline float stair_gradient(float x)
{
    ако (floor(x) == x) врати 0;
    врати 1;
}
static inline float relu_gradient(float x){врати (x>0);}
static inline float elu_gradient(float x){врати (x >= 0) + (x < 0)*(x + 1);}
static inline float selu_gradient(float x){врати (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static inline float relie_gradient(float x){врати (x>0) ? 1 : .01;}
static inline float ramp_gradient(float x){врати (x>0)+.1;}
static inline float leaky_gradient(float x){врати (x>0) ? 1 : .1;}
static inline float tanh_gradient(float x){врати 1-x*x;}
static inline float plse_gradient(float x){врати (x < 0 || x > 1) ? .01 : .125;}

#endif

