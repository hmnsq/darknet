#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    якщо(l.type == DROPOUT){
        якщо(l.rand)           free(l.rand);
#ifdef GPU
        якщо(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        повернути;
    }
    якщо(l.cweights)           free(l.cweights);
    якщо(l.indexes)            free(l.indexes);
    якщо(l.input_layers)       free(l.input_layers);
    якщо(l.input_sizes)        free(l.input_sizes);
    якщо(l.map)                free(l.map);
    якщо(l.rand)               free(l.rand);
    якщо(l.cost)               free(l.cost);
    якщо(l.state)              free(l.state);
    якщо(l.prev_state)         free(l.prev_state);
    якщо(l.forgot_state)       free(l.forgot_state);
    якщо(l.forgot_delta)       free(l.forgot_delta);
    якщо(l.state_delta)        free(l.state_delta);
    якщо(l.concat)             free(l.concat);
    якщо(l.concat_delta)       free(l.concat_delta);
    якщо(l.binary_weights)     free(l.binary_weights);
    якщо(l.biases)             free(l.biases);
    якщо(l.bias_updates)       free(l.bias_updates);
    якщо(l.scales)             free(l.scales);
    якщо(l.scale_updates)      free(l.scale_updates);
    якщо(l.weights)            free(l.weights);
    якщо(l.weight_updates)     free(l.weight_updates);
    якщо(l.delta)              free(l.delta);
    якщо(l.output)             free(l.output);
    якщо(l.squared)            free(l.squared);
    якщо(l.norms)              free(l.norms);
    якщо(l.spatial_mean)       free(l.spatial_mean);
    якщо(l.mean)               free(l.mean);
    якщо(l.variance)           free(l.variance);
    якщо(l.mean_delta)         free(l.mean_delta);
    якщо(l.variance_delta)     free(l.variance_delta);
    якщо(l.rolling_mean)       free(l.rolling_mean);
    якщо(l.rolling_variance)   free(l.rolling_variance);
    якщо(l.x)                  free(l.x);
    якщо(l.x_norm)             free(l.x_norm);
    якщо(l.m)                  free(l.m);
    якщо(l.v)                  free(l.v);
    якщо(l.z_cpu)              free(l.z_cpu);
    якщо(l.r_cpu)              free(l.r_cpu);
    якщо(l.h_cpu)              free(l.h_cpu);
    якщо(l.binary_input)       free(l.binary_input);

#ifdef GPU
    якщо(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    якщо(l.z_gpu)                   cuda_free(l.z_gpu);
    якщо(l.r_gpu)                   cuda_free(l.r_gpu);
    якщо(l.h_gpu)                   cuda_free(l.h_gpu);
    якщо(l.m_gpu)                   cuda_free(l.m_gpu);
    якщо(l.v_gpu)                   cuda_free(l.v_gpu);
    якщо(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    якщо(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    якщо(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    якщо(l.state_gpu)               cuda_free(l.state_gpu);
    якщо(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    якщо(l.gate_gpu)                cuda_free(l.gate_gpu);
    якщо(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    якщо(l.save_gpu)                cuda_free(l.save_gpu);
    якщо(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    якщо(l.concat_gpu)              cuda_free(l.concat_gpu);
    якщо(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    якщо(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    якщо(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    якщо(l.mean_gpu)                cuda_free(l.mean_gpu);
    якщо(l.variance_gpu)            cuda_free(l.variance_gpu);
    якщо(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    якщо(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    якщо(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    якщо(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    якщо(l.x_gpu)                   cuda_free(l.x_gpu);
    якщо(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    якщо(l.weights_gpu)             cuda_free(l.weights_gpu);
    якщо(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    якщо(l.biases_gpu)              cuda_free(l.biases_gpu);
    якщо(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    якщо(l.scales_gpu)              cuda_free(l.scales_gpu);
    якщо(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    якщо(l.output_gpu)              cuda_free(l.output_gpu);
    якщо(l.delta_gpu)               cuda_free(l.delta_gpu);
    якщо(l.rand_gpu)                cuda_free(l.rand_gpu);
    якщо(l.squared_gpu)             cuda_free(l.squared_gpu);
    якщо(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
