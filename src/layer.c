#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    အကယ်၍(l.type == DROPOUT){
        အကယ်၍(l.rand)           free(l.rand);
#ifdef GPU
        အကယ်၍(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ပြန်ပေး;
    }
    အကယ်၍(l.cweights)           free(l.cweights);
    အကယ်၍(l.indexes)            free(l.indexes);
    အကယ်၍(l.input_layers)       free(l.input_layers);
    အကယ်၍(l.input_sizes)        free(l.input_sizes);
    အကယ်၍(l.map)                free(l.map);
    အကယ်၍(l.rand)               free(l.rand);
    အကယ်၍(l.cost)               free(l.cost);
    အကယ်၍(l.state)              free(l.state);
    အကယ်၍(l.prev_state)         free(l.prev_state);
    အကယ်၍(l.forgot_state)       free(l.forgot_state);
    အကယ်၍(l.forgot_delta)       free(l.forgot_delta);
    အကယ်၍(l.state_delta)        free(l.state_delta);
    အကယ်၍(l.concat)             free(l.concat);
    အကယ်၍(l.concat_delta)       free(l.concat_delta);
    အကယ်၍(l.binary_weights)     free(l.binary_weights);
    အကယ်၍(l.biases)             free(l.biases);
    အကယ်၍(l.bias_updates)       free(l.bias_updates);
    အကယ်၍(l.scales)             free(l.scales);
    အကယ်၍(l.scale_updates)      free(l.scale_updates);
    အကယ်၍(l.weights)            free(l.weights);
    အကယ်၍(l.weight_updates)     free(l.weight_updates);
    အကယ်၍(l.delta)              free(l.delta);
    အကယ်၍(l.output)             free(l.output);
    အကယ်၍(l.squared)            free(l.squared);
    အကယ်၍(l.norms)              free(l.norms);
    အကယ်၍(l.spatial_mean)       free(l.spatial_mean);
    အကယ်၍(l.mean)               free(l.mean);
    အကယ်၍(l.variance)           free(l.variance);
    အကယ်၍(l.mean_delta)         free(l.mean_delta);
    အကယ်၍(l.variance_delta)     free(l.variance_delta);
    အကယ်၍(l.rolling_mean)       free(l.rolling_mean);
    အကယ်၍(l.rolling_variance)   free(l.rolling_variance);
    အကယ်၍(l.x)                  free(l.x);
    အကယ်၍(l.x_norm)             free(l.x_norm);
    အကယ်၍(l.m)                  free(l.m);
    အကယ်၍(l.v)                  free(l.v);
    အကယ်၍(l.z_cpu)              free(l.z_cpu);
    အကယ်၍(l.r_cpu)              free(l.r_cpu);
    အကယ်၍(l.h_cpu)              free(l.h_cpu);
    အကယ်၍(l.binary_input)       free(l.binary_input);

#ifdef GPU
    အကယ်၍(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    အကယ်၍(l.z_gpu)                   cuda_free(l.z_gpu);
    အကယ်၍(l.r_gpu)                   cuda_free(l.r_gpu);
    အကယ်၍(l.h_gpu)                   cuda_free(l.h_gpu);
    အကယ်၍(l.m_gpu)                   cuda_free(l.m_gpu);
    အကယ်၍(l.v_gpu)                   cuda_free(l.v_gpu);
    အကယ်၍(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    အကယ်၍(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    အကယ်၍(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    အကယ်၍(l.state_gpu)               cuda_free(l.state_gpu);
    အကယ်၍(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    အကယ်၍(l.gate_gpu)                cuda_free(l.gate_gpu);
    အကယ်၍(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    အကယ်၍(l.save_gpu)                cuda_free(l.save_gpu);
    အကယ်၍(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    အကယ်၍(l.concat_gpu)              cuda_free(l.concat_gpu);
    အကယ်၍(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    အကယ်၍(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    အကယ်၍(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    အကယ်၍(l.mean_gpu)                cuda_free(l.mean_gpu);
    အကယ်၍(l.variance_gpu)            cuda_free(l.variance_gpu);
    အကယ်၍(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    အကယ်၍(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    အကယ်၍(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    အကယ်၍(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    အကယ်၍(l.x_gpu)                   cuda_free(l.x_gpu);
    အကယ်၍(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    အကယ်၍(l.weights_gpu)             cuda_free(l.weights_gpu);
    အကယ်၍(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    အကယ်၍(l.biases_gpu)              cuda_free(l.biases_gpu);
    အကယ်၍(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    အကယ်၍(l.scales_gpu)              cuda_free(l.scales_gpu);
    အကယ်၍(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    အကယ်၍(l.output_gpu)              cuda_free(l.output_gpu);
    အကယ်၍(l.delta_gpu)               cuda_free(l.delta_gpu);
    အကယ်၍(l.rand_gpu)                cuda_free(l.rand_gpu);
    အကယ်၍(l.squared_gpu)             cuda_free(l.squared_gpu);
    အကယ်၍(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
