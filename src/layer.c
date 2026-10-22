#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ако(l.type == DROPOUT){
        ако(l.rand)           free(l.rand);
#ifdef GPU
        ако(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        върни;
    }
    ако(l.cweights)           free(l.cweights);
    ако(l.indexes)            free(l.indexes);
    ако(l.input_layers)       free(l.input_layers);
    ако(l.input_sizes)        free(l.input_sizes);
    ако(l.map)                free(l.map);
    ако(l.rand)               free(l.rand);
    ако(l.cost)               free(l.cost);
    ако(l.state)              free(l.state);
    ако(l.prev_state)         free(l.prev_state);
    ако(l.forgot_state)       free(l.forgot_state);
    ако(l.forgot_delta)       free(l.forgot_delta);
    ако(l.state_delta)        free(l.state_delta);
    ако(l.concat)             free(l.concat);
    ако(l.concat_delta)       free(l.concat_delta);
    ако(l.binary_weights)     free(l.binary_weights);
    ако(l.biases)             free(l.biases);
    ако(l.bias_updates)       free(l.bias_updates);
    ако(l.scales)             free(l.scales);
    ако(l.scale_updates)      free(l.scale_updates);
    ако(l.weights)            free(l.weights);
    ако(l.weight_updates)     free(l.weight_updates);
    ако(l.delta)              free(l.delta);
    ако(l.output)             free(l.output);
    ако(l.squared)            free(l.squared);
    ако(l.norms)              free(l.norms);
    ако(l.spatial_mean)       free(l.spatial_mean);
    ако(l.mean)               free(l.mean);
    ако(l.variance)           free(l.variance);
    ако(l.mean_delta)         free(l.mean_delta);
    ако(l.variance_delta)     free(l.variance_delta);
    ако(l.rolling_mean)       free(l.rolling_mean);
    ако(l.rolling_variance)   free(l.rolling_variance);
    ако(l.x)                  free(l.x);
    ако(l.x_norm)             free(l.x_norm);
    ако(l.m)                  free(l.m);
    ако(l.v)                  free(l.v);
    ако(l.z_cpu)              free(l.z_cpu);
    ако(l.r_cpu)              free(l.r_cpu);
    ако(l.h_cpu)              free(l.h_cpu);
    ако(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ако(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ако(l.z_gpu)                   cuda_free(l.z_gpu);
    ако(l.r_gpu)                   cuda_free(l.r_gpu);
    ако(l.h_gpu)                   cuda_free(l.h_gpu);
    ако(l.m_gpu)                   cuda_free(l.m_gpu);
    ако(l.v_gpu)                   cuda_free(l.v_gpu);
    ако(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ако(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ако(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ако(l.state_gpu)               cuda_free(l.state_gpu);
    ако(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ако(l.gate_gpu)                cuda_free(l.gate_gpu);
    ако(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ако(l.save_gpu)                cuda_free(l.save_gpu);
    ако(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ако(l.concat_gpu)              cuda_free(l.concat_gpu);
    ако(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ако(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ако(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ако(l.mean_gpu)                cuda_free(l.mean_gpu);
    ако(l.variance_gpu)            cuda_free(l.variance_gpu);
    ако(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ако(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ако(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ако(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ако(l.x_gpu)                   cuda_free(l.x_gpu);
    ако(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ако(l.weights_gpu)             cuda_free(l.weights_gpu);
    ако(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ако(l.biases_gpu)              cuda_free(l.biases_gpu);
    ако(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ако(l.scales_gpu)              cuda_free(l.scales_gpu);
    ако(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ако(l.output_gpu)              cuda_free(l.output_gpu);
    ако(l.delta_gpu)               cuda_free(l.delta_gpu);
    ако(l.rand_gpu)                cuda_free(l.rand_gpu);
    ако(l.squared_gpu)             cuda_free(l.squared_gpu);
    ако(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
