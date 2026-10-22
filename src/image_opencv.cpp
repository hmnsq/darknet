#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    ကိန်းပြည့် x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ကိန်းပြည့် step = disp->widthStep;
    အတွက်(y = 0; y < im.h; ++y){
        အတွက်(x = 0; x < im.w; ++x){
            အတွက်(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ပြန်ပေး disp;
}

image ipl_to_image(IplImage* src)
{
    ကိန်းပြည့် h = src->height;
    ကိန်းပြည့် w = src->width;
    ကိန်းပြည့် c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    ကိန်းပြည့် step = src->widthStep;
    ကိန်းပြည့် i, j, k;

    အတွက်(i = 0; i < h; ++i){
        အတွက်(k= 0; k < c; ++k){
            အတွက်(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ပြန်ပေး im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    အကယ်၍(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ပြန်ပေး m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ပြန်ပေး im;
}

void *open_video_stream(const var_na *f, ကိန်းပြည့် c, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် fps)
{
    VideoCapture *cap;
    အကယ်၍(f) cap = new VideoCapture(f);
    မဟုတ်ပါက cap = new VideoCapture(c);
    အကယ်၍(!cap->isOpened()) ပြန်ပေး 0;
    အကယ်၍(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    အကယ်၍(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    အကယ်၍(fps) cap->set(CV_CAP_PROP_FPS, w);
    ပြန်ပေး (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    အကယ်၍(m.empty()) ပြန်ပေး make_empty_image(0,0,0);
    ပြန်ပေး mat_to_image(m);
}

image load_image_cv(var_na *filename, ကိန်းပြည့် channels)
{
    ကိန်းပြည့် flag = -1;
    အကယ်၍ (channels == 0) flag = -1;
    မဟုတ်ပါက အကယ်၍ (channels == 1) flag = 0;
    မဟုတ်ပါက အကယ်၍ (channels == 3) flag = 1;
    မဟုတ်ပါက {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    အကယ်၍(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ပြန်ပေး make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ပြန်ပေး im;
}

ကိန်းပြည့် show_image_cv(image im, const var_na* name, ကိန်းပြည့် ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ကိန်းပြည့် c = waitKey(ms);
    အကယ်၍ (c != -1) c = c%256;
    ပြန်ပေး c;
}

void make_window(var_na *name, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    အကယ်၍ (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } မဟုတ်ပါက {
        resizeWindow(name, w, h);
        အကယ်၍(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
