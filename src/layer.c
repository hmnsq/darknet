#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

ਖਾਲੀ free_layer(layer l)
{
    ਜੇ(l.type == DROPOUT){
        ਜੇ(l.rand)           ਮੁਕਤ(l.rand);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
        ਜੇ(l.rand_gpu)             cuda_free(l.rand_gpu);
#ਜੇ_ਅੰਤ
        ਵਾਪਸ;
    }
    ਜੇ(l.cweights)           ਮੁਕਤ(l.cweights);
    ਜੇ(l.indexes)            ਮੁਕਤ(l.indexes);
    ਜੇ(l.ਨਿਵੇਸ਼_layers)       ਮੁਕਤ(l.ਨਿਵੇਸ਼_layers);
    ਜੇ(l.ਨਿਵੇਸ਼_sizes)        ਮੁਕਤ(l.ਨਿਵੇਸ਼_sizes);
    ਜੇ(l.map)                ਮੁਕਤ(l.map);
    ਜੇ(l.rand)               ਮੁਕਤ(l.rand);
    ਜੇ(l.ਲਾਗਤ_2)               ਮੁਕਤ(l.ਲਾਗਤ_2);
    ਜੇ(l.state)              ਮੁਕਤ(l.state);
    ਜੇ(l.prev_state)         ਮੁਕਤ(l.prev_state);
    ਜੇ(l.forgot_state)       ਮੁਕਤ(l.forgot_state);
    ਜੇ(l.forgot_delta)       ਮੁਕਤ(l.forgot_delta);
    ਜੇ(l.state_delta)        ਮੁਕਤ(l.state_delta);
    ਜੇ(l.concat)             ਮੁਕਤ(l.concat);
    ਜੇ(l.concat_delta)       ਮੁਕਤ(l.concat_delta);
    ਜੇ(l.ਦੁਆਧਾਰੀ_weights)     ਮੁਕਤ(l.ਦੁਆਧਾਰੀ_weights);
    ਜੇ(l.biases)             ਮੁਕਤ(l.biases);
    ਜੇ(l.bias_updates)       ਮੁਕਤ(l.bias_updates);
    ਜੇ(l.scales)             ਮੁਕਤ(l.scales);
    ਜੇ(l.scale_updates)      ਮੁਕਤ(l.scale_updates);
    ਜੇ(l.weights)            ਮੁਕਤ(l.weights);
    ਜੇ(l.ਭਾਰ_updates)     ਮੁਕਤ(l.ਭਾਰ_updates);
    ਜੇ(l.delta)              ਮੁਕਤ(l.delta);
    ਜੇ(l.output)             ਮੁਕਤ(l.output);
    ਜੇ(l.squared)            ਮੁਕਤ(l.squared);
    ਜੇ(l.norms)              ਮੁਕਤ(l.norms);
    ਜੇ(l.spatial_ਔਸਤ)       ਮੁਕਤ(l.spatial_ਔਸਤ);
    ਜੇ(l.ਔਸਤ)               ਮੁਕਤ(l.ਔਸਤ);
    ਜੇ(l.variance)           ਮੁਕਤ(l.variance);
    ਜੇ(l.ਔਸਤ_delta)         ਮੁਕਤ(l.ਔਸਤ_delta);
    ਜੇ(l.variance_delta)     ਮੁਕਤ(l.variance_delta);
    ਜੇ(l.rolling_ਔਸਤ)       ਮੁਕਤ(l.rolling_ਔਸਤ);
    ਜੇ(l.rolling_variance)   ਮੁਕਤ(l.rolling_variance);
    ਜੇ(l.x)                  ਮੁਕਤ(l.x);
    ਜੇ(l.x_norm)             ਮੁਕਤ(l.x_norm);
    ਜੇ(l.m)                  ਮੁਕਤ(l.m);
    ਜੇ(l.v)                  ਮੁਕਤ(l.v);
    ਜੇ(l.z_cpu)              ਮੁਕਤ(l.z_cpu);
    ਜੇ(l.r_cpu)              ਮੁਕਤ(l.r_cpu);
    ਜੇ(l.h_cpu)              ਮੁਕਤ(l.h_cpu);
    ਜੇ(l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼)       ਮੁਕਤ(l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(l.indexes_gpu)           cuda_free((ਦਸ਼ਮਲਵ *)l.indexes_gpu);

    ਜੇ(l.z_gpu)                   cuda_free(l.z_gpu);
    ਜੇ(l.r_gpu)                   cuda_free(l.r_gpu);
    ਜੇ(l.h_gpu)                   cuda_free(l.h_gpu);
    ਜੇ(l.m_gpu)                   cuda_free(l.m_gpu);
    ਜੇ(l.v_gpu)                   cuda_free(l.v_gpu);
    ਜੇ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ਜੇ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ਜੇ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ਜੇ(l.state_gpu)               cuda_free(l.state_gpu);
    ਜੇ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ਜੇ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ਜੇ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ਜੇ(l.save_gpu)                cuda_free(l.save_gpu);
    ਜੇ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ਜੇ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ਜੇ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ਜੇ(l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼_gpu)        cuda_free(l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼_gpu);
    ਜੇ(l.ਦੁਆਧਾਰੀ_weights_gpu)      cuda_free(l.ਦੁਆਧਾਰੀ_weights_gpu);
    ਜੇ(l.ਔਸਤ_gpu)                cuda_free(l.ਔਸਤ_gpu);
    ਜੇ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ਜੇ(l.rolling_ਔਸਤ_gpu)        cuda_free(l.rolling_ਔਸਤ_gpu);
    ਜੇ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ਜੇ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ਜੇ(l.ਔਸਤ_delta_gpu)          cuda_free(l.ਔਸਤ_delta_gpu);
    ਜੇ(l.x_gpu)                   cuda_free(l.x_gpu);
    ਜੇ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ਜੇ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ਜੇ(l.ਭਾਰ_updates_gpu)      cuda_free(l.ਭਾਰ_updates_gpu);
    ਜੇ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ਜੇ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ਜੇ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ਜੇ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ਜੇ(l.output_gpu)              cuda_free(l.output_gpu);
    ਜੇ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ਜੇ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ਜੇ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ਜੇ(l.norms_gpu)               cuda_free(l.norms_gpu);
#ਜੇ_ਅੰਤ
}
