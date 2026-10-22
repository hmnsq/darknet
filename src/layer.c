#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    nếu(l.type == DROPOUT){
        nếu(l.rand)           free(l.rand);
#ifdef GPU
        nếu(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        trả_về;
    }
    nếu(l.cweights)           free(l.cweights);
    nếu(l.indexes)            free(l.indexes);
    nếu(l.input_layers)       free(l.input_layers);
    nếu(l.input_sizes)        free(l.input_sizes);
    nếu(l.map)                free(l.map);
    nếu(l.rand)               free(l.rand);
    nếu(l.cost)               free(l.cost);
    nếu(l.state)              free(l.state);
    nếu(l.prev_state)         free(l.prev_state);
    nếu(l.forgot_state)       free(l.forgot_state);
    nếu(l.forgot_delta)       free(l.forgot_delta);
    nếu(l.state_delta)        free(l.state_delta);
    nếu(l.concat)             free(l.concat);
    nếu(l.concat_delta)       free(l.concat_delta);
    nếu(l.binary_weights)     free(l.binary_weights);
    nếu(l.biases)             free(l.biases);
    nếu(l.bias_updates)       free(l.bias_updates);
    nếu(l.scales)             free(l.scales);
    nếu(l.scale_updates)      free(l.scale_updates);
    nếu(l.weights)            free(l.weights);
    nếu(l.weight_updates)     free(l.weight_updates);
    nếu(l.delta)              free(l.delta);
    nếu(l.output)             free(l.output);
    nếu(l.squared)            free(l.squared);
    nếu(l.norms)              free(l.norms);
    nếu(l.spatial_mean)       free(l.spatial_mean);
    nếu(l.mean)               free(l.mean);
    nếu(l.variance)           free(l.variance);
    nếu(l.mean_delta)         free(l.mean_delta);
    nếu(l.variance_delta)     free(l.variance_delta);
    nếu(l.rolling_mean)       free(l.rolling_mean);
    nếu(l.rolling_variance)   free(l.rolling_variance);
    nếu(l.x)                  free(l.x);
    nếu(l.x_norm)             free(l.x_norm);
    nếu(l.m)                  free(l.m);
    nếu(l.v)                  free(l.v);
    nếu(l.z_cpu)              free(l.z_cpu);
    nếu(l.r_cpu)              free(l.r_cpu);
    nếu(l.h_cpu)              free(l.h_cpu);
    nếu(l.binary_input)       free(l.binary_input);

#ifdef GPU
    nếu(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    nếu(l.z_gpu)                   cuda_free(l.z_gpu);
    nếu(l.r_gpu)                   cuda_free(l.r_gpu);
    nếu(l.h_gpu)                   cuda_free(l.h_gpu);
    nếu(l.m_gpu)                   cuda_free(l.m_gpu);
    nếu(l.v_gpu)                   cuda_free(l.v_gpu);
    nếu(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    nếu(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    nếu(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    nếu(l.state_gpu)               cuda_free(l.state_gpu);
    nếu(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    nếu(l.gate_gpu)                cuda_free(l.gate_gpu);
    nếu(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    nếu(l.save_gpu)                cuda_free(l.save_gpu);
    nếu(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    nếu(l.concat_gpu)              cuda_free(l.concat_gpu);
    nếu(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    nếu(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    nếu(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    nếu(l.mean_gpu)                cuda_free(l.mean_gpu);
    nếu(l.variance_gpu)            cuda_free(l.variance_gpu);
    nếu(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    nếu(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    nếu(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    nếu(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    nếu(l.x_gpu)                   cuda_free(l.x_gpu);
    nếu(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    nếu(l.weights_gpu)             cuda_free(l.weights_gpu);
    nếu(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    nếu(l.biases_gpu)              cuda_free(l.biases_gpu);
    nếu(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    nếu(l.scales_gpu)              cuda_free(l.scales_gpu);
    nếu(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    nếu(l.output_gpu)              cuda_free(l.output_gpu);
    nếu(l.delta_gpu)               cuda_free(l.delta_gpu);
    nếu(l.rand_gpu)                cuda_free(l.rand_gpu);
    nếu(l.squared_gpu)             cuda_free(l.squared_gpu);
    nếu(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
