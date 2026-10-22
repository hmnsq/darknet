#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(char *s);

char *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const 整数 n, const ACTIVATION a, float *delta);
void activate_array(float *x, const 整数 n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, 整数 n, ACTIVATION a);
void gradient_array_gpu(float *x, 整数 n, ACTIVATION a, float *delta);
#endif

static inline float stair_activate(float x)
{
    整数 n = floor(x);
    もし (n%2 == 0) 戻る floor(x/2.);
    否则 戻る (x - n) + floor(x/2.);
}
static inline float hardtan_activate(float x)
{
    もし (x < -1) 戻る -1;
    もし (x > 1) 戻る 1;
    戻る x;
}
static inline float linear_activate(float x){戻る x;}
static inline float logistic_activate(float x){戻る 1./(1. + exp(-x));}
static inline float loggy_activate(float x){戻る 2./(1. + exp(-x)) - 1;}
static inline float relu_activate(float x){戻る x*(x>0);}
static inline float elu_activate(float x){戻る (x >= 0)*x + (x < 0)*(exp(x)-1);}
static inline float selu_activate(float x){戻る (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static inline float relie_activate(float x){戻る (x>0) ? x : .01*x;}
static inline float ramp_activate(float x){戻る x*(x>0)+.1*x;}
static inline float leaky_activate(float x){戻る (x>0) ? x : .1*x;}
static inline float tanh_activate(float x){戻る (exp(2*x)-1)/(exp(2*x)+1);}
static inline float plse_activate(float x)
{
    もし(x < -4) 戻る .01 * (x + 4);
    もし(x > 4)  戻る .01 * (x - 4) + 1;
    戻る .125*x + .5;
}

static inline float lhtan_activate(float x)
{
    もし(x < 0) 戻る .001*x;
    もし(x > 1) 戻る .001*(x-1) + 1;
    戻る x;
}
static inline float lhtan_gradient(float x)
{
    もし(x > 0 && x < 1) 戻る 1;
    戻る .001;
}

static inline float hardtan_gradient(float x)
{
    もし (x > -1 && x < 1) 戻る 1;
    戻る 0;
}
static inline float linear_gradient(float x){戻る 1;}
static inline float logistic_gradient(float x){戻る (1-x)*x;}
static inline float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    戻る 2*(1-y)*y;
}
static inline float stair_gradient(float x)
{
    もし (floor(x) == x) 戻る 0;
    戻る 1;
}
static inline float relu_gradient(float x){戻る (x>0);}
static inline float elu_gradient(float x){戻る (x >= 0) + (x < 0)*(x + 1);}
static inline float selu_gradient(float x){戻る (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static inline float relie_gradient(float x){戻る (x>0) ? 1 : .01;}
static inline float ramp_gradient(float x){戻る (x>0)+.1;}
static inline float leaky_gradient(float x){戻る (x>0) ? 1 : .1;}
static inline float tanh_gradient(float x){戻る 1-x*x;}
static inline float plse_gradient(float x){戻る (x < 0 || x > 1) ? .01 : .125;}

#endif

