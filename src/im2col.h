#ifndef IM2COL_H
#define IM2COL_H

શૂન્ય im2col_cpu(દશાંશ* data_im,
        પૂર્ણાંક channels, પૂર્ણાંક height, પૂર્ણાંક width,
        પૂર્ણાંક ksize, પૂર્ણાંક stride, પૂર્ણાંક pad, દશાંશ* data_col);

#ifdef GPU

શૂન્ય im2col_gpu(દશાંશ *im,
         પૂર્ણાંક channels, પૂર્ણાંક height, પૂર્ણાંક width,
         પૂર્ણાંક ksize, પૂર્ણાંક stride, પૂર્ણાંક pad,દશાંશ *data_col);

#endif
#endif
