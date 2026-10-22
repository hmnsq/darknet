#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᰀᰧ(l.type == DROPOUT){
        ᰀᰧ(l.rand)           free(l.rand);
#ifdef GPU
        ᰀᰧ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᰀᰧᰌ;
    }
    ᰀᰧ(l.cweights)           free(l.cweights);
    ᰀᰧ(l.indexes)            free(l.indexes);
    ᰀᰧ(l.input_layers)       free(l.input_layers);
    ᰀᰧ(l.input_sizes)        free(l.input_sizes);
    ᰀᰧ(l.map)                free(l.map);
    ᰀᰧ(l.rand)               free(l.rand);
    ᰀᰧ(l.cost)               free(l.cost);
    ᰀᰧ(l.state)              free(l.state);
    ᰀᰧ(l.prev_state)         free(l.prev_state);
    ᰀᰧ(l.forgot_state)       free(l.forgot_state);
    ᰀᰧ(l.forgot_delta)       free(l.forgot_delta);
    ᰀᰧ(l.state_delta)        free(l.state_delta);
    ᰀᰧ(l.concat)             free(l.concat);
    ᰀᰧ(l.concat_delta)       free(l.concat_delta);
    ᰀᰧ(l.binary_weights)     free(l.binary_weights);
    ᰀᰧ(l.biases)             free(l.biases);
    ᰀᰧ(l.bias_updates)       free(l.bias_updates);
    ᰀᰧ(l.scales)             free(l.scales);
    ᰀᰧ(l.scale_updates)      free(l.scale_updates);
    ᰀᰧ(l.weights)            free(l.weights);
    ᰀᰧ(l.weight_updates)     free(l.weight_updates);
    ᰀᰧ(l.delta)              free(l.delta);
    ᰀᰧ(l.output)             free(l.output);
    ᰀᰧ(l.squared)            free(l.squared);
    ᰀᰧ(l.norms)              free(l.norms);
    ᰀᰧ(l.spatial_mean)       free(l.spatial_mean);
    ᰀᰧ(l.mean)               free(l.mean);
    ᰀᰧ(l.variance)           free(l.variance);
    ᰀᰧ(l.mean_delta)         free(l.mean_delta);
    ᰀᰧ(l.variance_delta)     free(l.variance_delta);
    ᰀᰧ(l.rolling_mean)       free(l.rolling_mean);
    ᰀᰧ(l.rolling_variance)   free(l.rolling_variance);
    ᰀᰧ(l.x)                  free(l.x);
    ᰀᰧ(l.x_norm)             free(l.x_norm);
    ᰀᰧ(l.m)                  free(l.m);
    ᰀᰧ(l.v)                  free(l.v);
    ᰀᰧ(l.z_cpu)              free(l.z_cpu);
    ᰀᰧ(l.r_cpu)              free(l.r_cpu);
    ᰀᰧ(l.h_cpu)              free(l.h_cpu);
    ᰀᰧ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᰀᰧ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᰀᰧ(l.z_gpu)                   cuda_free(l.z_gpu);
    ᰀᰧ(l.r_gpu)                   cuda_free(l.r_gpu);
    ᰀᰧ(l.h_gpu)                   cuda_free(l.h_gpu);
    ᰀᰧ(l.m_gpu)                   cuda_free(l.m_gpu);
    ᰀᰧ(l.v_gpu)                   cuda_free(l.v_gpu);
    ᰀᰧ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᰀᰧ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᰀᰧ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᰀᰧ(l.state_gpu)               cuda_free(l.state_gpu);
    ᰀᰧ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᰀᰧ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᰀᰧ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᰀᰧ(l.save_gpu)                cuda_free(l.save_gpu);
    ᰀᰧ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᰀᰧ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᰀᰧ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᰀᰧ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᰀᰧ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᰀᰧ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᰀᰧ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᰀᰧ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᰀᰧ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᰀᰧ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᰀᰧ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᰀᰧ(l.x_gpu)                   cuda_free(l.x_gpu);
    ᰀᰧ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᰀᰧ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᰀᰧ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᰀᰧ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᰀᰧ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᰀᰧ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᰀᰧ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᰀᰧ(l.output_gpu)              cuda_free(l.output_gpu);
    ᰀᰧ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᰀᰧ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᰀᰧ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᰀᰧ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
