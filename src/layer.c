#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    གལ་སྲིད(l.type == DROPOUT){
        གལ་སྲིད(l.rand)           free(l.rand);
#ifdef GPU
        གལ་སྲིད(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ལོག;
    }
    གལ་སྲིད(l.cweights)           free(l.cweights);
    གལ་སྲིད(l.indexes)            free(l.indexes);
    གལ་སྲིད(l.input_layers)       free(l.input_layers);
    གལ་སྲིད(l.input_sizes)        free(l.input_sizes);
    གལ་སྲིད(l.map)                free(l.map);
    གལ་སྲིད(l.rand)               free(l.rand);
    གལ་སྲིད(l.cost)               free(l.cost);
    གལ་སྲིད(l.state)              free(l.state);
    གལ་སྲིད(l.prev_state)         free(l.prev_state);
    གལ་སྲིད(l.forgot_state)       free(l.forgot_state);
    གལ་སྲིད(l.forgot_delta)       free(l.forgot_delta);
    གལ་སྲིད(l.state_delta)        free(l.state_delta);
    གལ་སྲིད(l.concat)             free(l.concat);
    གལ་སྲིད(l.concat_delta)       free(l.concat_delta);
    གལ་སྲིད(l.binary_weights)     free(l.binary_weights);
    གལ་སྲིད(l.biases)             free(l.biases);
    གལ་སྲིད(l.bias_updates)       free(l.bias_updates);
    གལ་སྲིད(l.scales)             free(l.scales);
    གལ་སྲིད(l.scale_updates)      free(l.scale_updates);
    གལ་སྲིད(l.weights)            free(l.weights);
    གལ་སྲིད(l.weight_updates)     free(l.weight_updates);
    གལ་སྲིད(l.delta)              free(l.delta);
    གལ་སྲིད(l.output)             free(l.output);
    གལ་སྲིད(l.squared)            free(l.squared);
    གལ་སྲིད(l.norms)              free(l.norms);
    གལ་སྲིད(l.spatial_mean)       free(l.spatial_mean);
    གལ་སྲིད(l.mean)               free(l.mean);
    གལ་སྲིད(l.variance)           free(l.variance);
    གལ་སྲིད(l.mean_delta)         free(l.mean_delta);
    གལ་སྲིད(l.variance_delta)     free(l.variance_delta);
    གལ་སྲིད(l.rolling_mean)       free(l.rolling_mean);
    གལ་སྲིད(l.rolling_variance)   free(l.rolling_variance);
    གལ་སྲིད(l.x)                  free(l.x);
    གལ་སྲིད(l.x_norm)             free(l.x_norm);
    གལ་སྲིད(l.m)                  free(l.m);
    གལ་སྲིད(l.v)                  free(l.v);
    གལ་སྲིད(l.z_cpu)              free(l.z_cpu);
    གལ་སྲིད(l.r_cpu)              free(l.r_cpu);
    གལ་སྲིད(l.h_cpu)              free(l.h_cpu);
    གལ་སྲིད(l.binary_input)       free(l.binary_input);

#ifdef GPU
    གལ་སྲིད(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    གལ་སྲིད(l.z_gpu)                   cuda_free(l.z_gpu);
    གལ་སྲིད(l.r_gpu)                   cuda_free(l.r_gpu);
    གལ་སྲིད(l.h_gpu)                   cuda_free(l.h_gpu);
    གལ་སྲིད(l.m_gpu)                   cuda_free(l.m_gpu);
    གལ་སྲིད(l.v_gpu)                   cuda_free(l.v_gpu);
    གལ་སྲིད(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    གལ་སྲིད(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    གལ་སྲིད(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    གལ་སྲིད(l.state_gpu)               cuda_free(l.state_gpu);
    གལ་སྲིད(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    གལ་སྲིད(l.gate_gpu)                cuda_free(l.gate_gpu);
    གལ་སྲིད(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    གལ་སྲིད(l.save_gpu)                cuda_free(l.save_gpu);
    གལ་སྲིད(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    གལ་སྲིད(l.concat_gpu)              cuda_free(l.concat_gpu);
    གལ་སྲིད(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    གལ་སྲིད(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    གལ་སྲིད(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    གལ་སྲིད(l.mean_gpu)                cuda_free(l.mean_gpu);
    གལ་སྲིད(l.variance_gpu)            cuda_free(l.variance_gpu);
    གལ་སྲིད(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    གལ་སྲིད(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    གལ་སྲིད(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    གལ་སྲིད(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    གལ་སྲིད(l.x_gpu)                   cuda_free(l.x_gpu);
    གལ་སྲིད(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    གལ་སྲིད(l.weights_gpu)             cuda_free(l.weights_gpu);
    གལ་སྲིད(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    གལ་སྲིད(l.biases_gpu)              cuda_free(l.biases_gpu);
    གལ་སྲིད(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    གལ་སྲིད(l.scales_gpu)              cuda_free(l.scales_gpu);
    གལ་སྲིད(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    གལ་སྲིད(l.output_gpu)              cuda_free(l.output_gpu);
    གལ་སྲིད(l.delta_gpu)               cuda_free(l.delta_gpu);
    གལ་སྲིད(l.rand_gpu)                cuda_free(l.rand_gpu);
    གལ་སྲིད(l.squared_gpu)             cuda_free(l.squared_gpu);
    གལ་སྲིད(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
