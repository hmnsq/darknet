#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    ចំនួនគត់ x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ចំនួនគត់ step = disp->widthStep;
    សម្រាប់(y = 0; y < im.h; ++y){
        សម្រាប់(x = 0; x < im.w; ++x){
            សម្រាប់(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ត្រឡប់ disp;
}

image ipl_to_image(IplImage* src)
{
    ចំនួនគត់ h = src->height;
    ចំនួនគត់ w = src->width;
    ចំនួនគត់ c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    ចំនួនគត់ step = src->widthStep;
    ចំនួនគត់ i, j, k;

    សម្រាប់(i = 0; i < h; ++i){
        សម្រាប់(k= 0; k < c; ++k){
            សម្រាប់(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ត្រឡប់ im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ប្រសិនបើ(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ត្រឡប់ m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ត្រឡប់ im;
}

void *open_video_stream(const var_na *f, ចំនួនគត់ c, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ fps)
{
    VideoCapture *cap;
    ប្រសិនបើ(f) cap = new VideoCapture(f);
    បើមិនដូច្នេះ cap = new VideoCapture(c);
    ប្រសិនបើ(!cap->isOpened()) ត្រឡប់ 0;
    ប្រសិនបើ(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ប្រសិនបើ(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ប្រសិនបើ(fps) cap->set(CV_CAP_PROP_FPS, w);
    ត្រឡប់ (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ប្រសិនបើ(m.empty()) ត្រឡប់ make_empty_image(0,0,0);
    ត្រឡប់ mat_to_image(m);
}

image load_image_cv(var_na *filename, ចំនួនគត់ channels)
{
    ចំនួនគត់ flag = -1;
    ប្រសិនបើ (channels == 0) flag = -1;
    បើមិនដូច្នេះ ប្រសិនបើ (channels == 1) flag = 0;
    បើមិនដូច្នេះ ប្រសិនបើ (channels == 3) flag = 1;
    បើមិនដូច្នេះ {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ប្រសិនបើ(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ត្រឡប់ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ត្រឡប់ im;
}

ចំនួនគត់ show_image_cv(image im, const var_na* name, ចំនួនគត់ ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ចំនួនគត់ c = waitKey(ms);
    ប្រសិនបើ (c != -1) c = c%256;
    ត្រឡប់ c;
}

void make_window(var_na *name, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ប្រសិនបើ (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } បើមិនដូច្នេះ {
        resizeWindow(name, w, h);
        ប្រសិនបើ(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
