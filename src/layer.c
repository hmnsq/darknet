#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ถ้า(l.type == DROPOUT){
        ถ้า(l.rand)           free(l.rand);
#ifdef GPU
        ถ้า(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        คืน;
    }
    ถ้า(l.cweights)           free(l.cweights);
    ถ้า(l.indexes)            free(l.indexes);
    ถ้า(l.input_layers)       free(l.input_layers);
    ถ้า(l.input_sizes)        free(l.input_sizes);
    ถ้า(l.map)                free(l.map);
    ถ้า(l.rand)               free(l.rand);
    ถ้า(l.cost)               free(l.cost);
    ถ้า(l.state)              free(l.state);
    ถ้า(l.prev_state)         free(l.prev_state);
    ถ้า(l.forgot_state)       free(l.forgot_state);
    ถ้า(l.forgot_delta)       free(l.forgot_delta);
    ถ้า(l.state_delta)        free(l.state_delta);
    ถ้า(l.concat)             free(l.concat);
    ถ้า(l.concat_delta)       free(l.concat_delta);
    ถ้า(l.binary_weights)     free(l.binary_weights);
    ถ้า(l.biases)             free(l.biases);
    ถ้า(l.bias_updates)       free(l.bias_updates);
    ถ้า(l.scales)             free(l.scales);
    ถ้า(l.scale_updates)      free(l.scale_updates);
    ถ้า(l.weights)            free(l.weights);
    ถ้า(l.weight_updates)     free(l.weight_updates);
    ถ้า(l.delta)              free(l.delta);
    ถ้า(l.output)             free(l.output);
    ถ้า(l.squared)            free(l.squared);
    ถ้า(l.norms)              free(l.norms);
    ถ้า(l.spatial_mean)       free(l.spatial_mean);
    ถ้า(l.mean)               free(l.mean);
    ถ้า(l.variance)           free(l.variance);
    ถ้า(l.mean_delta)         free(l.mean_delta);
    ถ้า(l.variance_delta)     free(l.variance_delta);
    ถ้า(l.rolling_mean)       free(l.rolling_mean);
    ถ้า(l.rolling_variance)   free(l.rolling_variance);
    ถ้า(l.x)                  free(l.x);
    ถ้า(l.x_norm)             free(l.x_norm);
    ถ้า(l.m)                  free(l.m);
    ถ้า(l.v)                  free(l.v);
    ถ้า(l.z_cpu)              free(l.z_cpu);
    ถ้า(l.r_cpu)              free(l.r_cpu);
    ถ้า(l.h_cpu)              free(l.h_cpu);
    ถ้า(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ถ้า(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ถ้า(l.z_gpu)                   cuda_free(l.z_gpu);
    ถ้า(l.r_gpu)                   cuda_free(l.r_gpu);
    ถ้า(l.h_gpu)                   cuda_free(l.h_gpu);
    ถ้า(l.m_gpu)                   cuda_free(l.m_gpu);
    ถ้า(l.v_gpu)                   cuda_free(l.v_gpu);
    ถ้า(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ถ้า(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ถ้า(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ถ้า(l.state_gpu)               cuda_free(l.state_gpu);
    ถ้า(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ถ้า(l.gate_gpu)                cuda_free(l.gate_gpu);
    ถ้า(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ถ้า(l.save_gpu)                cuda_free(l.save_gpu);
    ถ้า(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ถ้า(l.concat_gpu)              cuda_free(l.concat_gpu);
    ถ้า(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ถ้า(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ถ้า(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ถ้า(l.mean_gpu)                cuda_free(l.mean_gpu);
    ถ้า(l.variance_gpu)            cuda_free(l.variance_gpu);
    ถ้า(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ถ้า(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ถ้า(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ถ้า(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ถ้า(l.x_gpu)                   cuda_free(l.x_gpu);
    ถ้า(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ถ้า(l.weights_gpu)             cuda_free(l.weights_gpu);
    ถ้า(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ถ้า(l.biases_gpu)              cuda_free(l.biases_gpu);
    ถ้า(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ถ้า(l.scales_gpu)              cuda_free(l.scales_gpu);
    ถ้า(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ถ้า(l.output_gpu)              cuda_free(l.output_gpu);
    ถ้า(l.delta_gpu)               cuda_free(l.delta_gpu);
    ถ้า(l.rand_gpu)                cuda_free(l.rand_gpu);
    ถ้า(l.squared_gpu)             cuda_free(l.squared_gpu);
    ถ้า(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
