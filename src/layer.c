#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ꦪꦼꦤ꧀(l.type == DROPOUT){
        ꦪꦼꦤ꧀(l.rand)           free(l.rand);
#ifdef GPU
        ꦪꦼꦤ꧀(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀(l.cweights)           free(l.cweights);
    ꦪꦼꦤ꧀(l.indexes)            free(l.indexes);
    ꦪꦼꦤ꧀(l.input_layers)       free(l.input_layers);
    ꦪꦼꦤ꧀(l.input_sizes)        free(l.input_sizes);
    ꦪꦼꦤ꧀(l.map)                free(l.map);
    ꦪꦼꦤ꧀(l.rand)               free(l.rand);
    ꦪꦼꦤ꧀(l.cost)               free(l.cost);
    ꦪꦼꦤ꧀(l.state)              free(l.state);
    ꦪꦼꦤ꧀(l.prev_state)         free(l.prev_state);
    ꦪꦼꦤ꧀(l.forgot_state)       free(l.forgot_state);
    ꦪꦼꦤ꧀(l.forgot_delta)       free(l.forgot_delta);
    ꦪꦼꦤ꧀(l.state_delta)        free(l.state_delta);
    ꦪꦼꦤ꧀(l.concat)             free(l.concat);
    ꦪꦼꦤ꧀(l.concat_delta)       free(l.concat_delta);
    ꦪꦼꦤ꧀(l.binary_weights)     free(l.binary_weights);
    ꦪꦼꦤ꧀(l.biases)             free(l.biases);
    ꦪꦼꦤ꧀(l.bias_updates)       free(l.bias_updates);
    ꦪꦼꦤ꧀(l.scales)             free(l.scales);
    ꦪꦼꦤ꧀(l.scale_updates)      free(l.scale_updates);
    ꦪꦼꦤ꧀(l.weights)            free(l.weights);
    ꦪꦼꦤ꧀(l.weight_updates)     free(l.weight_updates);
    ꦪꦼꦤ꧀(l.delta)              free(l.delta);
    ꦪꦼꦤ꧀(l.output)             free(l.output);
    ꦪꦼꦤ꧀(l.squared)            free(l.squared);
    ꦪꦼꦤ꧀(l.norms)              free(l.norms);
    ꦪꦼꦤ꧀(l.spatial_mean)       free(l.spatial_mean);
    ꦪꦼꦤ꧀(l.mean)               free(l.mean);
    ꦪꦼꦤ꧀(l.variance)           free(l.variance);
    ꦪꦼꦤ꧀(l.mean_delta)         free(l.mean_delta);
    ꦪꦼꦤ꧀(l.variance_delta)     free(l.variance_delta);
    ꦪꦼꦤ꧀(l.rolling_mean)       free(l.rolling_mean);
    ꦪꦼꦤ꧀(l.rolling_variance)   free(l.rolling_variance);
    ꦪꦼꦤ꧀(l.x)                  free(l.x);
    ꦪꦼꦤ꧀(l.x_norm)             free(l.x_norm);
    ꦪꦼꦤ꧀(l.m)                  free(l.m);
    ꦪꦼꦤ꧀(l.v)                  free(l.v);
    ꦪꦼꦤ꧀(l.z_cpu)              free(l.z_cpu);
    ꦪꦼꦤ꧀(l.r_cpu)              free(l.r_cpu);
    ꦪꦼꦤ꧀(l.h_cpu)              free(l.h_cpu);
    ꦪꦼꦤ꧀(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ꦪꦼꦤ꧀(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ꦪꦼꦤ꧀(l.z_gpu)                   cuda_free(l.z_gpu);
    ꦪꦼꦤ꧀(l.r_gpu)                   cuda_free(l.r_gpu);
    ꦪꦼꦤ꧀(l.h_gpu)                   cuda_free(l.h_gpu);
    ꦪꦼꦤ꧀(l.m_gpu)                   cuda_free(l.m_gpu);
    ꦪꦼꦤ꧀(l.v_gpu)                   cuda_free(l.v_gpu);
    ꦪꦼꦤ꧀(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ꦪꦼꦤ꧀(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ꦪꦼꦤ꧀(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ꦪꦼꦤ꧀(l.state_gpu)               cuda_free(l.state_gpu);
    ꦪꦼꦤ꧀(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ꦪꦼꦤ꧀(l.gate_gpu)                cuda_free(l.gate_gpu);
    ꦪꦼꦤ꧀(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ꦪꦼꦤ꧀(l.save_gpu)                cuda_free(l.save_gpu);
    ꦪꦼꦤ꧀(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ꦪꦼꦤ꧀(l.concat_gpu)              cuda_free(l.concat_gpu);
    ꦪꦼꦤ꧀(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ꦪꦼꦤ꧀(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ꦪꦼꦤ꧀(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ꦪꦼꦤ꧀(l.mean_gpu)                cuda_free(l.mean_gpu);
    ꦪꦼꦤ꧀(l.variance_gpu)            cuda_free(l.variance_gpu);
    ꦪꦼꦤ꧀(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ꦪꦼꦤ꧀(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ꦪꦼꦤ꧀(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ꦪꦼꦤ꧀(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ꦪꦼꦤ꧀(l.x_gpu)                   cuda_free(l.x_gpu);
    ꦪꦼꦤ꧀(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ꦪꦼꦤ꧀(l.weights_gpu)             cuda_free(l.weights_gpu);
    ꦪꦼꦤ꧀(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ꦪꦼꦤ꧀(l.biases_gpu)              cuda_free(l.biases_gpu);
    ꦪꦼꦤ꧀(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ꦪꦼꦤ꧀(l.scales_gpu)              cuda_free(l.scales_gpu);
    ꦪꦼꦤ꧀(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ꦪꦼꦤ꧀(l.output_gpu)              cuda_free(l.output_gpu);
    ꦪꦼꦤ꧀(l.delta_gpu)               cuda_free(l.delta_gpu);
    ꦪꦼꦤ꧀(l.rand_gpu)                cuda_free(l.rand_gpu);
    ꦪꦼꦤ꧀(l.squared_gpu)             cuda_free(l.squared_gpu);
    ꦪꦼꦤ꧀(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
