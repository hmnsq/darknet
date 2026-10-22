#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        ཧྲིལ channels, ཧྲིལ height, ཧྲིལ width,
        ཧྲིལ ksize, ཧྲིལ stride, ཧྲིལ pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        ཧྲིལ channels, ཧྲིལ height, ཧྲིལ width,
        ཧྲིལ ksize, ཧྲིལ stride, ཧྲིལ pad, float *data_im);
#endif
#endif
