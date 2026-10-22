#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᮚᮦᮔ᮪(l.type == DROPOUT){
        ᮚᮦᮔ᮪(l.rand)           free(l.rand);
#ifdef GPU
        ᮚᮦᮔ᮪(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪(l.cweights)           free(l.cweights);
    ᮚᮦᮔ᮪(l.indexes)            free(l.indexes);
    ᮚᮦᮔ᮪(l.input_layers)       free(l.input_layers);
    ᮚᮦᮔ᮪(l.input_sizes)        free(l.input_sizes);
    ᮚᮦᮔ᮪(l.map)                free(l.map);
    ᮚᮦᮔ᮪(l.rand)               free(l.rand);
    ᮚᮦᮔ᮪(l.cost)               free(l.cost);
    ᮚᮦᮔ᮪(l.state)              free(l.state);
    ᮚᮦᮔ᮪(l.prev_state)         free(l.prev_state);
    ᮚᮦᮔ᮪(l.forgot_state)       free(l.forgot_state);
    ᮚᮦᮔ᮪(l.forgot_delta)       free(l.forgot_delta);
    ᮚᮦᮔ᮪(l.state_delta)        free(l.state_delta);
    ᮚᮦᮔ᮪(l.concat)             free(l.concat);
    ᮚᮦᮔ᮪(l.concat_delta)       free(l.concat_delta);
    ᮚᮦᮔ᮪(l.binary_weights)     free(l.binary_weights);
    ᮚᮦᮔ᮪(l.biases)             free(l.biases);
    ᮚᮦᮔ᮪(l.bias_updates)       free(l.bias_updates);
    ᮚᮦᮔ᮪(l.scales)             free(l.scales);
    ᮚᮦᮔ᮪(l.scale_updates)      free(l.scale_updates);
    ᮚᮦᮔ᮪(l.weights)            free(l.weights);
    ᮚᮦᮔ᮪(l.weight_updates)     free(l.weight_updates);
    ᮚᮦᮔ᮪(l.delta)              free(l.delta);
    ᮚᮦᮔ᮪(l.output)             free(l.output);
    ᮚᮦᮔ᮪(l.squared)            free(l.squared);
    ᮚᮦᮔ᮪(l.norms)              free(l.norms);
    ᮚᮦᮔ᮪(l.spatial_mean)       free(l.spatial_mean);
    ᮚᮦᮔ᮪(l.mean)               free(l.mean);
    ᮚᮦᮔ᮪(l.variance)           free(l.variance);
    ᮚᮦᮔ᮪(l.mean_delta)         free(l.mean_delta);
    ᮚᮦᮔ᮪(l.variance_delta)     free(l.variance_delta);
    ᮚᮦᮔ᮪(l.rolling_mean)       free(l.rolling_mean);
    ᮚᮦᮔ᮪(l.rolling_variance)   free(l.rolling_variance);
    ᮚᮦᮔ᮪(l.x)                  free(l.x);
    ᮚᮦᮔ᮪(l.x_norm)             free(l.x_norm);
    ᮚᮦᮔ᮪(l.m)                  free(l.m);
    ᮚᮦᮔ᮪(l.v)                  free(l.v);
    ᮚᮦᮔ᮪(l.z_cpu)              free(l.z_cpu);
    ᮚᮦᮔ᮪(l.r_cpu)              free(l.r_cpu);
    ᮚᮦᮔ᮪(l.h_cpu)              free(l.h_cpu);
    ᮚᮦᮔ᮪(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᮚᮦᮔ᮪(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᮚᮦᮔ᮪(l.z_gpu)                   cuda_free(l.z_gpu);
    ᮚᮦᮔ᮪(l.r_gpu)                   cuda_free(l.r_gpu);
    ᮚᮦᮔ᮪(l.h_gpu)                   cuda_free(l.h_gpu);
    ᮚᮦᮔ᮪(l.m_gpu)                   cuda_free(l.m_gpu);
    ᮚᮦᮔ᮪(l.v_gpu)                   cuda_free(l.v_gpu);
    ᮚᮦᮔ᮪(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᮚᮦᮔ᮪(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᮚᮦᮔ᮪(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᮚᮦᮔ᮪(l.state_gpu)               cuda_free(l.state_gpu);
    ᮚᮦᮔ᮪(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᮚᮦᮔ᮪(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᮚᮦᮔ᮪(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᮚᮦᮔ᮪(l.save_gpu)                cuda_free(l.save_gpu);
    ᮚᮦᮔ᮪(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᮚᮦᮔ᮪(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᮚᮦᮔ᮪(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᮚᮦᮔ᮪(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᮚᮦᮔ᮪(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᮚᮦᮔ᮪(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᮚᮦᮔ᮪(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᮚᮦᮔ᮪(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᮚᮦᮔ᮪(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᮚᮦᮔ᮪(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᮚᮦᮔ᮪(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᮚᮦᮔ᮪(l.x_gpu)                   cuda_free(l.x_gpu);
    ᮚᮦᮔ᮪(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᮚᮦᮔ᮪(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᮚᮦᮔ᮪(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᮚᮦᮔ᮪(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᮚᮦᮔ᮪(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᮚᮦᮔ᮪(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᮚᮦᮔ᮪(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᮚᮦᮔ᮪(l.output_gpu)              cuda_free(l.output_gpu);
    ᮚᮦᮔ᮪(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᮚᮦᮔ᮪(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᮚᮦᮔ᮪(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᮚᮦᮔ᮪(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
