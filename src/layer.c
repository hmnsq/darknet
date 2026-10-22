#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    གལ་ཏེ(l.type == DROPOUT){
        གལ་ཏེ(l.rand)           free(l.rand);
#ifdef GPU
        གལ་ཏེ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ཕྱིར;
    }
    གལ་ཏེ(l.cweights)           free(l.cweights);
    གལ་ཏེ(l.indexes)            free(l.indexes);
    གལ་ཏེ(l.input_layers)       free(l.input_layers);
    གལ་ཏེ(l.input_sizes)        free(l.input_sizes);
    གལ་ཏེ(l.map)                free(l.map);
    གལ་ཏེ(l.rand)               free(l.rand);
    གལ་ཏེ(l.cost)               free(l.cost);
    གལ་ཏེ(l.state)              free(l.state);
    གལ་ཏེ(l.prev_state)         free(l.prev_state);
    གལ་ཏེ(l.forgot_state)       free(l.forgot_state);
    གལ་ཏེ(l.forgot_delta)       free(l.forgot_delta);
    གལ་ཏེ(l.state_delta)        free(l.state_delta);
    གལ་ཏེ(l.concat)             free(l.concat);
    གལ་ཏེ(l.concat_delta)       free(l.concat_delta);
    གལ་ཏེ(l.binary_weights)     free(l.binary_weights);
    གལ་ཏེ(l.biases)             free(l.biases);
    གལ་ཏེ(l.bias_updates)       free(l.bias_updates);
    གལ་ཏེ(l.scales)             free(l.scales);
    གལ་ཏེ(l.scale_updates)      free(l.scale_updates);
    གལ་ཏེ(l.weights)            free(l.weights);
    གལ་ཏེ(l.weight_updates)     free(l.weight_updates);
    གལ་ཏེ(l.delta)              free(l.delta);
    གལ་ཏེ(l.output)             free(l.output);
    གལ་ཏེ(l.squared)            free(l.squared);
    གལ་ཏེ(l.norms)              free(l.norms);
    གལ་ཏེ(l.spatial_mean)       free(l.spatial_mean);
    གལ་ཏེ(l.mean)               free(l.mean);
    གལ་ཏེ(l.variance)           free(l.variance);
    གལ་ཏེ(l.mean_delta)         free(l.mean_delta);
    གལ་ཏེ(l.variance_delta)     free(l.variance_delta);
    གལ་ཏེ(l.rolling_mean)       free(l.rolling_mean);
    གལ་ཏེ(l.rolling_variance)   free(l.rolling_variance);
    གལ་ཏེ(l.x)                  free(l.x);
    གལ་ཏེ(l.x_norm)             free(l.x_norm);
    གལ་ཏེ(l.m)                  free(l.m);
    གལ་ཏེ(l.v)                  free(l.v);
    གལ་ཏེ(l.z_cpu)              free(l.z_cpu);
    གལ་ཏེ(l.r_cpu)              free(l.r_cpu);
    གལ་ཏེ(l.h_cpu)              free(l.h_cpu);
    གལ་ཏེ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    གལ་ཏེ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    གལ་ཏེ(l.z_gpu)                   cuda_free(l.z_gpu);
    གལ་ཏེ(l.r_gpu)                   cuda_free(l.r_gpu);
    གལ་ཏེ(l.h_gpu)                   cuda_free(l.h_gpu);
    གལ་ཏེ(l.m_gpu)                   cuda_free(l.m_gpu);
    གལ་ཏེ(l.v_gpu)                   cuda_free(l.v_gpu);
    གལ་ཏེ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    གལ་ཏེ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    གལ་ཏེ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    གལ་ཏེ(l.state_gpu)               cuda_free(l.state_gpu);
    གལ་ཏེ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    གལ་ཏེ(l.gate_gpu)                cuda_free(l.gate_gpu);
    གལ་ཏེ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    གལ་ཏེ(l.save_gpu)                cuda_free(l.save_gpu);
    གལ་ཏེ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    གལ་ཏེ(l.concat_gpu)              cuda_free(l.concat_gpu);
    གལ་ཏེ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    གལ་ཏེ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    གལ་ཏེ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    གལ་ཏེ(l.mean_gpu)                cuda_free(l.mean_gpu);
    གལ་ཏེ(l.variance_gpu)            cuda_free(l.variance_gpu);
    གལ་ཏེ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    གལ་ཏེ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    གལ་ཏེ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    གལ་ཏེ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    གལ་ཏེ(l.x_gpu)                   cuda_free(l.x_gpu);
    གལ་ཏེ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    གལ་ཏེ(l.weights_gpu)             cuda_free(l.weights_gpu);
    གལ་ཏེ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    གལ་ཏེ(l.biases_gpu)              cuda_free(l.biases_gpu);
    གལ་ཏེ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    གལ་ཏེ(l.scales_gpu)              cuda_free(l.scales_gpu);
    གལ་ཏེ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    གལ་ཏེ(l.output_gpu)              cuda_free(l.output_gpu);
    གལ་ཏེ(l.delta_gpu)               cuda_free(l.delta_gpu);
    གལ་ཏེ(l.rand_gpu)                cuda_free(l.rand_gpu);
    གལ་ཏེ(l.squared_gpu)             cuda_free(l.squared_gpu);
    གལ་ཏེ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
