#ifndef COL2IM_H
#define COL2IM_H

શૂન્ય col2im_cpu(દશાંશ* data_col,
        પૂર્ણાંક channels, પૂર્ણાંક height, પૂર્ણાંક width,
        પૂર્ણાંક ksize, પૂર્ણાંક stride, પૂર્ણાંક pad, દશાંશ* data_im);

#ifdef GPU
શૂન્ય col2im_gpu(દશાંશ *data_col,
        પૂર્ણાંક channels, પૂર્ણાંક height, પૂર્ણાંક width,
        પૂર્ણાંક ksize, પૂર્ણાંક stride, પૂર્ણાંક pad, દશાંશ *data_im);
#endif
#endif
