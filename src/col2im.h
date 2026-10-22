#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        ចំនួនគត់ channels, ចំនួនគត់ height, ចំនួនគត់ width,
        ចំនួនគត់ ksize, ចំនួនគត់ stride, ចំនួនគត់ pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        ចំនួនគត់ channels, ចំនួនគត់ height, ចំនួនគត់ width,
        ចំនួនគត់ ksize, ចំនួនគត់ stride, ចំនួនគត់ pad, float *data_im);
#endif
#endif
