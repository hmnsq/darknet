#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    จำนวนเต็ม x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    จำนวนเต็ม step = disp->widthStep;
    สำหรับ(y = 0; y < im.h; ++y){
        สำหรับ(x = 0; x < im.w; ++x){
            สำหรับ(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    คืน disp;
}

image ipl_to_image(IplImage* src)
{
    จำนวนเต็ม h = src->height;
    จำนวนเต็ม w = src->width;
    จำนวนเต็ม c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    จำนวนเต็ม step = src->widthStep;
    จำนวนเต็ม i, j, k;

    สำหรับ(i = 0; i < h; ++i){
        สำหรับ(k= 0; k < c; ++k){
            สำหรับ(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    คืน im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ถ้า(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    คืน m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    คืน im;
}

void *open_video_stream(const var_na *f, จำนวนเต็ม c, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม fps)
{
    VideoCapture *cap;
    ถ้า(f) cap = new VideoCapture(f);
    มิฉะนั้น cap = new VideoCapture(c);
    ถ้า(!cap->isOpened()) คืน 0;
    ถ้า(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ถ้า(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ถ้า(fps) cap->set(CV_CAP_PROP_FPS, w);
    คืน (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ถ้า(m.empty()) คืน make_empty_image(0,0,0);
    คืน mat_to_image(m);
}

image load_image_cv(var_na *filename, จำนวนเต็ม channels)
{
    จำนวนเต็ม flag = -1;
    ถ้า (channels == 0) flag = -1;
    มิฉะนั้น ถ้า (channels == 1) flag = 0;
    มิฉะนั้น ถ้า (channels == 3) flag = 1;
    มิฉะนั้น {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ถ้า(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        คืน make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    คืน im;
}

จำนวนเต็ม show_image_cv(image im, const var_na* name, จำนวนเต็ม ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    จำนวนเต็ม c = waitKey(ms);
    ถ้า (c != -1) c = c%256;
    คืน c;
}

void make_window(var_na *name, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ถ้า (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } มิฉะนั้น {
        resizeWindow(name, w, h);
        ถ้า(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
