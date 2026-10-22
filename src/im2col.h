#ifndef IM2COL_H
#define IM2COL_H

শূন্য im2col_cpu(দশমিক* তথ্য_im,
        পূর্ণসংখ্যা channels, পূর্ণসংখ্যা height, পূর্ণসংখ্যা width,
        পূর্ণসংখ্যা ksize, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, দশমিক* তথ্য_col);

#ifdef GPU

শূন্য im2col_gpu(দশমিক *im,
         পূর্ণসংখ্যা channels, পূর্ণসংখ্যা height, পূর্ণসংখ্যা width,
         পূর্ণসংখ্যা ksize, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad,দশমিক *তথ্য_col);

#endif
#endif
