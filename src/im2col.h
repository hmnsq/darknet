#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        ຈຳນວນເຕັມ channels, ຈຳນວນເຕັມ height, ຈຳນວນເຕັມ width,
        ຈຳນວນເຕັມ ksize, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         ຈຳນວນເຕັມ channels, ຈຳນວນເຕັມ height, ຈຳນວນເຕັມ width,
         ຈຳນວນເຕັມ ksize, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ pad,float *data_col);

#endif
#endif
