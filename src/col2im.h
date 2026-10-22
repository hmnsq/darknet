#ifndef COL2IM_H
#define COL2IM_H

வெறுமை col2im_cpu(தசம* data_col,
        முழு channels, முழு height, முழு width,
        முழு ksize, முழு stride, முழு pad, தசம* data_im);

#ifdef GPU
வெறுமை col2im_gpu(தசம *data_col,
        முழு channels, முழு height, முழு width,
        முழு ksize, முழு stride, முழு pad, தசம *data_im);
#endif
#endif
