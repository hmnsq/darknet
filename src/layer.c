#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᱡᱩᱫᱤ(l.type == DROPOUT){
        ᱡᱩᱫᱤ(l.rand)           free(l.rand);
#ifdef GPU
        ᱡᱩᱫᱤ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᱨᱩᱣᱟᱹᱲ;
    }
    ᱡᱩᱫᱤ(l.cweights)           free(l.cweights);
    ᱡᱩᱫᱤ(l.indexes)            free(l.indexes);
    ᱡᱩᱫᱤ(l.input_layers)       free(l.input_layers);
    ᱡᱩᱫᱤ(l.input_sizes)        free(l.input_sizes);
    ᱡᱩᱫᱤ(l.map)                free(l.map);
    ᱡᱩᱫᱤ(l.rand)               free(l.rand);
    ᱡᱩᱫᱤ(l.cost)               free(l.cost);
    ᱡᱩᱫᱤ(l.state)              free(l.state);
    ᱡᱩᱫᱤ(l.prev_state)         free(l.prev_state);
    ᱡᱩᱫᱤ(l.forgot_state)       free(l.forgot_state);
    ᱡᱩᱫᱤ(l.forgot_delta)       free(l.forgot_delta);
    ᱡᱩᱫᱤ(l.state_delta)        free(l.state_delta);
    ᱡᱩᱫᱤ(l.concat)             free(l.concat);
    ᱡᱩᱫᱤ(l.concat_delta)       free(l.concat_delta);
    ᱡᱩᱫᱤ(l.binary_weights)     free(l.binary_weights);
    ᱡᱩᱫᱤ(l.biases)             free(l.biases);
    ᱡᱩᱫᱤ(l.bias_updates)       free(l.bias_updates);
    ᱡᱩᱫᱤ(l.scales)             free(l.scales);
    ᱡᱩᱫᱤ(l.scale_updates)      free(l.scale_updates);
    ᱡᱩᱫᱤ(l.weights)            free(l.weights);
    ᱡᱩᱫᱤ(l.weight_updates)     free(l.weight_updates);
    ᱡᱩᱫᱤ(l.delta)              free(l.delta);
    ᱡᱩᱫᱤ(l.output)             free(l.output);
    ᱡᱩᱫᱤ(l.squared)            free(l.squared);
    ᱡᱩᱫᱤ(l.norms)              free(l.norms);
    ᱡᱩᱫᱤ(l.spatial_mean)       free(l.spatial_mean);
    ᱡᱩᱫᱤ(l.mean)               free(l.mean);
    ᱡᱩᱫᱤ(l.variance)           free(l.variance);
    ᱡᱩᱫᱤ(l.mean_delta)         free(l.mean_delta);
    ᱡᱩᱫᱤ(l.variance_delta)     free(l.variance_delta);
    ᱡᱩᱫᱤ(l.rolling_mean)       free(l.rolling_mean);
    ᱡᱩᱫᱤ(l.rolling_variance)   free(l.rolling_variance);
    ᱡᱩᱫᱤ(l.x)                  free(l.x);
    ᱡᱩᱫᱤ(l.x_norm)             free(l.x_norm);
    ᱡᱩᱫᱤ(l.m)                  free(l.m);
    ᱡᱩᱫᱤ(l.v)                  free(l.v);
    ᱡᱩᱫᱤ(l.z_cpu)              free(l.z_cpu);
    ᱡᱩᱫᱤ(l.r_cpu)              free(l.r_cpu);
    ᱡᱩᱫᱤ(l.h_cpu)              free(l.h_cpu);
    ᱡᱩᱫᱤ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᱡᱩᱫᱤ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᱡᱩᱫᱤ(l.z_gpu)                   cuda_free(l.z_gpu);
    ᱡᱩᱫᱤ(l.r_gpu)                   cuda_free(l.r_gpu);
    ᱡᱩᱫᱤ(l.h_gpu)                   cuda_free(l.h_gpu);
    ᱡᱩᱫᱤ(l.m_gpu)                   cuda_free(l.m_gpu);
    ᱡᱩᱫᱤ(l.v_gpu)                   cuda_free(l.v_gpu);
    ᱡᱩᱫᱤ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᱡᱩᱫᱤ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᱡᱩᱫᱤ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᱡᱩᱫᱤ(l.state_gpu)               cuda_free(l.state_gpu);
    ᱡᱩᱫᱤ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᱡᱩᱫᱤ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᱡᱩᱫᱤ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᱡᱩᱫᱤ(l.save_gpu)                cuda_free(l.save_gpu);
    ᱡᱩᱫᱤ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᱡᱩᱫᱤ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᱡᱩᱫᱤ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᱡᱩᱫᱤ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᱡᱩᱫᱤ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᱡᱩᱫᱤ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᱡᱩᱫᱤ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᱡᱩᱫᱤ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᱡᱩᱫᱤ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᱡᱩᱫᱤ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᱡᱩᱫᱤ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᱡᱩᱫᱤ(l.x_gpu)                   cuda_free(l.x_gpu);
    ᱡᱩᱫᱤ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᱡᱩᱫᱤ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᱡᱩᱫᱤ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᱡᱩᱫᱤ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᱡᱩᱫᱤ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᱡᱩᱫᱤ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᱡᱩᱫᱤ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᱡᱩᱫᱤ(l.output_gpu)              cuda_free(l.output_gpu);
    ᱡᱩᱫᱤ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᱡᱩᱫᱤ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᱡᱩᱫᱤ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᱡᱩᱫᱤ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
