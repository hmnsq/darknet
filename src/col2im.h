#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        puur_naa_nka channels, puur_naa_nka height, puur_naa_nka width,
        puur_naa_nka ksize, puur_naa_nka stride, puur_naa_nka pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        puur_naa_nka channels, puur_naa_nka height, puur_naa_nka width,
        puur_naa_nka ksize, puur_naa_nka stride, puur_naa_nka pad, float *data_im);
#endif
#endif
