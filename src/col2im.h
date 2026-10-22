#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        số_nguyên channels, số_nguyên height, số_nguyên width,
        số_nguyên ksize, số_nguyên stride, số_nguyên pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        số_nguyên channels, số_nguyên height, số_nguyên width,
        số_nguyên ksize, số_nguyên stride, số_nguyên pad, float *data_im);
#endif
#endif
