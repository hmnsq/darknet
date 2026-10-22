#ifndef IM2COL_H
#define IM2COL_H

ଶୂନ୍ୟ im2col_cpu(ଦଶମିକ* data_im,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ଦଶମିକ* data_col);

#ifdef GPU

ଶୂନ୍ୟ im2col_gpu(ଦଶମିକ *im,
         ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
         ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad,ଦଶମିକ *data_col);

#endif
#endif
