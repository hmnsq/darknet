#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"

#ଅନ୍ତର୍ଭାଵ <stdlib.h>

ଶୂନ୍ୟ free_layer(layer l)
{
    ଯଦି(l.type == DROPOUT){
        ଯଦି(l.rand)           ମୁକ୍ତ(l.rand);
#ifdef GPU
        ଯଦି(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ଫେରନ୍ତୁ;
    }
    ଯଦି(l.cweights)           ମୁକ୍ତ(l.cweights);
    ଯଦି(l.indexes)            ମୁକ୍ତ(l.indexes);
    ଯଦି(l.ନିବେଶ_layers)       ମୁକ୍ତ(l.ନିବେଶ_layers);
    ଯଦି(l.ନିବେଶ_sizes)        ମୁକ୍ତ(l.ନିବେଶ_sizes);
    ଯଦି(l.map)                ମୁକ୍ତ(l.map);
    ଯଦି(l.rand)               ମୁକ୍ତ(l.rand);
    ଯଦି(l.cost)               ମୁକ୍ତ(l.cost);
    ଯଦି(l.state)              ମୁକ୍ତ(l.state);
    ଯଦି(l.prev_state)         ମୁକ୍ତ(l.prev_state);
    ଯଦି(l.forgot_state)       ମୁକ୍ତ(l.forgot_state);
    ଯଦି(l.forgot_delta)       ମୁକ୍ତ(l.forgot_delta);
    ଯଦି(l.state_delta)        ମୁକ୍ତ(l.state_delta);
    ଯଦି(l.concat)             ମୁକ୍ତ(l.concat);
    ଯଦି(l.concat_delta)       ମୁକ୍ତ(l.concat_delta);
    ଯଦି(l.ଦ୍ୱିମିକ_weights)     ମୁକ୍ତ(l.ଦ୍ୱିମିକ_weights);
    ଯଦି(l.biases)             ମୁକ୍ତ(l.biases);
    ଯଦି(l.bias_updates)       ମୁକ୍ତ(l.bias_updates);
    ଯଦି(l.scales)             ମୁକ୍ତ(l.scales);
    ଯଦି(l.scale_updates)      ମୁକ୍ତ(l.scale_updates);
    ଯଦି(l.weights)            ମୁକ୍ତ(l.weights);
    ଯଦି(l.weight_updates)     ମୁକ୍ତ(l.weight_updates);
    ଯଦି(l.delta)              ମୁକ୍ତ(l.delta);
    ଯଦି(l.output)             ମୁକ୍ତ(l.output);
    ଯଦି(l.squared)            ମୁକ୍ତ(l.squared);
    ଯଦି(l.norms)              ମୁକ୍ତ(l.norms);
    ଯଦି(l.spatial_mean)       ମୁକ୍ତ(l.spatial_mean);
    ଯଦି(l.mean)               ମୁକ୍ତ(l.mean);
    ଯଦି(l.variance)           ମୁକ୍ତ(l.variance);
    ଯଦି(l.mean_delta)         ମୁକ୍ତ(l.mean_delta);
    ଯଦି(l.variance_delta)     ମୁକ୍ତ(l.variance_delta);
    ଯଦି(l.rolling_mean)       ମୁକ୍ତ(l.rolling_mean);
    ଯଦି(l.rolling_variance)   ମୁକ୍ତ(l.rolling_variance);
    ଯଦି(l.x)                  ମୁକ୍ତ(l.x);
    ଯଦି(l.x_norm)             ମୁକ୍ତ(l.x_norm);
    ଯଦି(l.m)                  ମୁକ୍ତ(l.m);
    ଯଦି(l.v)                  ମୁକ୍ତ(l.v);
    ଯଦି(l.z_cpu)              ମୁକ୍ତ(l.z_cpu);
    ଯଦି(l.r_cpu)              ମୁକ୍ତ(l.r_cpu);
    ଯଦି(l.h_cpu)              ମୁକ୍ତ(l.h_cpu);
    ଯଦି(l.ଦ୍ୱିମିକ_ନିବେଶ)       ମୁକ୍ତ(l.ଦ୍ୱିମିକ_ନିବେଶ);

#ifdef GPU
    ଯଦି(l.indexes_gpu)           cuda_free((ଦଶମିକ *)l.indexes_gpu);

    ଯଦି(l.z_gpu)                   cuda_free(l.z_gpu);
    ଯଦି(l.r_gpu)                   cuda_free(l.r_gpu);
    ଯଦି(l.h_gpu)                   cuda_free(l.h_gpu);
    ଯଦି(l.m_gpu)                   cuda_free(l.m_gpu);
    ଯଦି(l.v_gpu)                   cuda_free(l.v_gpu);
    ଯଦି(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ଯଦି(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ଯଦି(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ଯଦି(l.state_gpu)               cuda_free(l.state_gpu);
    ଯଦି(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ଯଦି(l.gate_gpu)                cuda_free(l.gate_gpu);
    ଯଦି(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ଯଦି(l.save_gpu)                cuda_free(l.save_gpu);
    ଯଦି(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ଯଦି(l.concat_gpu)              cuda_free(l.concat_gpu);
    ଯଦି(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ଯଦି(l.ଦ୍ୱିମିକ_ନିବେଶ_gpu)        cuda_free(l.ଦ୍ୱିମିକ_ନିବେଶ_gpu);
    ଯଦି(l.ଦ୍ୱିମିକ_weights_gpu)      cuda_free(l.ଦ୍ୱିମିକ_weights_gpu);
    ଯଦି(l.mean_gpu)                cuda_free(l.mean_gpu);
    ଯଦି(l.variance_gpu)            cuda_free(l.variance_gpu);
    ଯଦି(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ଯଦି(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ଯଦି(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ଯଦି(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ଯଦି(l.x_gpu)                   cuda_free(l.x_gpu);
    ଯଦି(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ଯଦି(l.weights_gpu)             cuda_free(l.weights_gpu);
    ଯଦି(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ଯଦି(l.biases_gpu)              cuda_free(l.biases_gpu);
    ଯଦି(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ଯଦି(l.scales_gpu)              cuda_free(l.scales_gpu);
    ଯଦି(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ଯଦି(l.output_gpu)              cuda_free(l.output_gpu);
    ଯଦି(l.delta_gpu)               cuda_free(l.delta_gpu);
    ଯଦି(l.rand_gpu)                cuda_free(l.rand_gpu);
    ଯଦି(l.squared_gpu)             cuda_free(l.squared_gpu);
    ଯଦି(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
