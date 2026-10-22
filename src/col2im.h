#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        ကိန်းပြည့် channels, ကိန်းပြည့် height, ကိန်းပြည့် width,
        ကိန်းပြည့် ksize, ကိန်းပြည့် stride, ကိန်းပြည့် pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        ကိန်းပြည့် channels, ကိန်းပြည့် height, ကိန်းပြည့် width,
        ကိန်းပြည့် ksize, ကိန်းပြည့် stride, ကိန်းပြည့် pad, float *data_im);
#endif
#endif
