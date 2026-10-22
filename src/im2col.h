#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        puur_naa_nka channels, puur_naa_nka height, puur_naa_nka width,
        puur_naa_nka ksize, puur_naa_nka stride, puur_naa_nka pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         puur_naa_nka channels, puur_naa_nka height, puur_naa_nka width,
         puur_naa_nka ksize, puur_naa_nka stride, puur_naa_nka pad,float *data_col);

#endif
#endif
