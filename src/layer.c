#అన్తర్భావ "layer.h"
#అన్తర్భావ "cuda.h"

#అన్తర్భావ <stdlib.h>

శూన్య free_layer(layer l)
{
    ఒకవేళ(l.type == DROPOUT){
        ఒకవేళ(l.rand)           విడుదల(l.rand);
#నిర్వచితమైతే GPU
        ఒకవేళ(l.rand_gpu)             cuda_free(l.rand_gpu);
#అయితే_ముగింపు
        తిరిగి;
    }
    ఒకవేళ(l.cweights)           విడుదల(l.cweights);
    ఒకవేళ(l.indexes)            విడుదల(l.indexes);
    ఒకవేళ(l.ఆదానం_layers)       విడుదల(l.ఆదానం_layers);
    ఒకవేళ(l.ఆదానం_sizes)        విడుదల(l.ఆదానం_sizes);
    ఒకవేళ(l.map)                విడుదల(l.map);
    ఒకవేళ(l.rand)               విడుదల(l.rand);
    ఒకవేళ(l.ఖర్చు_2)               విడుదల(l.ఖర్చు_2);
    ఒకవేళ(l.state)              విడుదల(l.state);
    ఒకవేళ(l.prev_state)         విడుదల(l.prev_state);
    ఒకవేళ(l.forgot_state)       విడుదల(l.forgot_state);
    ఒకవేళ(l.forgot_delta)       విడుదల(l.forgot_delta);
    ఒకవేళ(l.state_delta)        విడుదల(l.state_delta);
    ఒకవేళ(l.concat)             విడుదల(l.concat);
    ఒకవేళ(l.concat_delta)       విడుదల(l.concat_delta);
    ఒకవేళ(l.ద్విమాన_weights)     విడుదల(l.ద్విమాన_weights);
    ఒకవేళ(l.biases)             విడుదల(l.biases);
    ఒకవేళ(l.bias_updates)       విడుదల(l.bias_updates);
    ఒకవేళ(l.scales)             విడుదల(l.scales);
    ఒకవేళ(l.scale_updates)      విడుదల(l.scale_updates);
    ఒకవేళ(l.weights)            విడుదల(l.weights);
    ఒకవేళ(l.బరువు_updates)     విడుదల(l.బరువు_updates);
    ఒకవేళ(l.delta)              విడుదల(l.delta);
    ఒకవేళ(l.output)             విడుదల(l.output);
    ఒకవేళ(l.squared)            విడుదల(l.squared);
    ఒకవేళ(l.norms)              విడుదల(l.norms);
    ఒకవేళ(l.spatial_సగటు)       విడుదల(l.spatial_సగటు);
    ఒకవేళ(l.సగటు)               విడుదల(l.సగటు);
    ఒకవేళ(l.variance)           విడుదల(l.variance);
    ఒకవేళ(l.సగటు_delta)         విడుదల(l.సగటు_delta);
    ఒకవేళ(l.variance_delta)     విడుదల(l.variance_delta);
    ఒకవేళ(l.rolling_సగటు)       విడుదల(l.rolling_సగటు);
    ఒకవేళ(l.rolling_variance)   విడుదల(l.rolling_variance);
    ఒకవేళ(l.x)                  విడుదల(l.x);
    ఒకవేళ(l.x_norm)             విడుదల(l.x_norm);
    ఒకవేళ(l.m)                  విడుదల(l.m);
    ఒకవేళ(l.v)                  విడుదల(l.v);
    ఒకవేళ(l.z_cpu)              విడుదల(l.z_cpu);
    ఒకవేళ(l.r_cpu)              విడుదల(l.r_cpu);
    ఒకవేళ(l.h_cpu)              విడుదల(l.h_cpu);
    ఒకవేళ(l.ద్విమాన_ఆదానం)       విడుదల(l.ద్విమాన_ఆదానం);

#నిర్వచితమైతే GPU
    ఒకవేళ(l.indexes_gpu)           cuda_free((దశాంశం *)l.indexes_gpu);

    ఒకవేళ(l.z_gpu)                   cuda_free(l.z_gpu);
    ఒకవేళ(l.r_gpu)                   cuda_free(l.r_gpu);
    ఒకవేళ(l.h_gpu)                   cuda_free(l.h_gpu);
    ఒకవేళ(l.m_gpu)                   cuda_free(l.m_gpu);
    ఒకవేళ(l.v_gpu)                   cuda_free(l.v_gpu);
    ఒకవేళ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ఒకవేళ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ఒకవేళ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ఒకవేళ(l.state_gpu)               cuda_free(l.state_gpu);
    ఒకవేళ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ఒకవేళ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ఒకవేళ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ఒకవేళ(l.save_gpu)                cuda_free(l.save_gpu);
    ఒకవేళ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ఒకవేళ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ఒకవేళ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ఒకవేళ(l.ద్విమాన_ఆదానం_gpu)        cuda_free(l.ద్విమాన_ఆదానం_gpu);
    ఒకవేళ(l.ద్విమాన_weights_gpu)      cuda_free(l.ద్విమాన_weights_gpu);
    ఒకవేళ(l.సగటు_gpu)                cuda_free(l.సగటు_gpu);
    ఒకవేళ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ఒకవేళ(l.rolling_సగటు_gpu)        cuda_free(l.rolling_సగటు_gpu);
    ఒకవేళ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ఒకవేళ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ఒకవేళ(l.సగటు_delta_gpu)          cuda_free(l.సగటు_delta_gpu);
    ఒకవేళ(l.x_gpu)                   cuda_free(l.x_gpu);
    ఒకవేళ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ఒకవేళ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ఒకవేళ(l.బరువు_updates_gpu)      cuda_free(l.బరువు_updates_gpu);
    ఒకవేళ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ఒకవేళ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ఒకవేళ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ఒకవేళ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ఒకవేళ(l.output_gpu)              cuda_free(l.output_gpu);
    ఒకవేళ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ఒకవేళ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ఒకవేళ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ఒకవేళ(l.norms_gpu)               cuda_free(l.norms_gpu);
#అయితే_ముగింపు
}
