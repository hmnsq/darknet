#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    số_nguyên x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    số_nguyên step = disp->widthStep;
    cho(y = 0; y < im.h; ++y){
        cho(x = 0; x < im.w; ++x){
            cho(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    trả_về disp;
}

image ipl_to_image(IplImage* src)
{
    số_nguyên h = src->height;
    số_nguyên w = src->width;
    số_nguyên c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    số_nguyên step = src->widthStep;
    số_nguyên i, j, k;

    cho(i = 0; i < h; ++i){
        cho(k= 0; k < c; ++k){
            cho(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    trả_về im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    nếu(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    trả_về m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    trả_về im;
}

void *open_video_stream(const char *f, số_nguyên c, số_nguyên w, số_nguyên h, số_nguyên fps)
{
    VideoCapture *cap;
    nếu(f) cap = new VideoCapture(f);
    không_thì cap = new VideoCapture(c);
    nếu(!cap->isOpened()) trả_về 0;
    nếu(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    nếu(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    nếu(fps) cap->set(CV_CAP_PROP_FPS, w);
    trả_về (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    nếu(m.empty()) trả_về make_empty_image(0,0,0);
    trả_về mat_to_image(m);
}

image load_image_cv(char *filename, số_nguyên channels)
{
    số_nguyên flag = -1;
    nếu (channels == 0) flag = -1;
    không_thì nếu (channels == 1) flag = 0;
    không_thì nếu (channels == 3) flag = 1;
    không_thì {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    nếu(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        trả_về make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    trả_về im;
}

số_nguyên show_image_cv(image im, const char* name, số_nguyên ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    số_nguyên c = waitKey(ms);
    nếu (c != -1) c = c%256;
    trả_về c;
}

void make_window(char *name, số_nguyên w, số_nguyên h, số_nguyên fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    nếu (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } không_thì {
        resizeWindow(name, w, h);
        nếu(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
