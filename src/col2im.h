#ifndef COL2IM_H
#define COL2IM_H

ଶୂନ୍ୟ col2im_cpu(ଦଶମିକ* data_col,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ଦଶମିକ* data_im);

#ifdef GPU
ଶୂନ୍ୟ col2im_gpu(ଦଶମିକ *data_col,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ଦଶମିକ *data_im);
#endif
#endif
