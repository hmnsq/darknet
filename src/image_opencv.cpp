#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV

#ਅਨ੍ਤਰ੍ਭਾਵ "stdio.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdlib.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/opencv.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;

ਬਾਹਰੀ "C" {

IplImage *image_ਮੰਜ਼ਿਲ_ipl(image im)
{
    ਪੂਰਨ x,y,c;
    IplImage *disp = cvCreateImage(cv_ਮਾਪ(im.w,im.h), IPL_ਡੂੰਘਾਈ_8_U, im.c);
    ਪੂਰਨ ਕਦਮ = disp->ਚੌੜਾਈ_ਕਦਮ;
    ਲਈ(y = 0; y < im.h; ++y){
        ਲਈ(x = 0; x < im.w; ++x){
            ਲਈ(c= 0; c < im.c; ++c){
                ਦਸ਼ਮਲਵ val = im.ਅੰਕੜੇ[c*im.h*im.w + y*im.w + x];
                disp->image_ਅੰਕੜੇ[y*ਕਦਮ + x*im.c + c] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)(val*255);
            }
        }
    }
    ਵਾਪਸ disp;
}

image ipl_ਮੰਜ਼ਿਲ_image(IplImage* src)
{
    ਪੂਰਨ h = src->ਉਚਾਈ;
    ਪੂਰਨ w = src->ਚੌੜਾਈ;
    ਪੂਰਨ c = src->nChannels;
    image im = make_image(w, h, c);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)src->image_ਅੰਕੜੇ;
    ਪੂਰਨ ਕਦਮ = src->ਚੌੜਾਈ_ਕਦਮ;
    ਪੂਰਨ i, j, k;

    ਲਈ(i = 0; i < h; ++i){
        ਲਈ(k= 0; k < c; ++k){
            ਲਈ(j = 0; j < w; ++j){
                im.ਅੰਕੜੇ[k*w*h + i*w + j] = ਅੰਕੜੇ[i*ਕਦਮ + j*c + k]/255.;
            }
        }
    }
    ਵਾਪਸ im;
}

Mat image_ਮੰਜ਼ਿਲ_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ਜੇ(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_ਮੰਜ਼ਿਲ_ipl(copy);
    Mat m = cvarr_ਮੰਜ਼ਿਲ_Mat(ipl, ਸੱਚ);
    cvReleaseImage(&ipl);
    free_image(copy);
    ਵਾਪਸ m;
}

image mat_ਮੰਜ਼ਿਲ_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_ਮੰਜ਼ਿਲ_image(&ipl);
    rgbgr_image(im);
    ਵਾਪਸ im;
}

ਖਾਲੀ *open_video_ਧਾਰਾ(ਅਚਲ ਅੱਖਰ *f, ਪੂਰਨ c, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ fps)
{
    VideoCapture *cap;
    ਜੇ(f) cap = ਨਵਾਂ VideoCapture(f);
    ਨਹੀਂਤਾਂ cap = ਨਵਾਂ VideoCapture(c);
    ਜੇ(!cap->isOpened()) ਵਾਪਸ 0;
    ਜੇ(w) cap->ਸੈੱਟ(CV_CAP_PROP_ਚੌਖਟਾ_ਚੌੜਾਈ, w);
    ਜੇ(h) cap->ਸੈੱਟ(CV_CAP_PROP_ਚੌਖਟਾ_ਉਚਾਈ, w);
    ਜੇ(fps) cap->ਸੈੱਟ(CV_CAP_PROP_FPS, w);
    ਵਾਪਸ (ਖਾਲੀ *) cap;
}

image get_image_from_ਧਾਰਾ(ਖਾਲੀ *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ਜੇ(m.ਖਾਲੀ_ਥਾਵਾਂ()) ਵਾਪਸ make_ਖਾਲੀ_ਥਾਵਾਂ_image(0,0,0);
    ਵਾਪਸ mat_ਮੰਜ਼ਿਲ_image(m);
}

image ਲੱਦੋ_image_cv(ਅੱਖਰ *filename, ਪੂਰਨ channels)
{
    ਪੂਰਨ flag = -1;
    ਜੇ (channels == 0) flag = -1;
    ਨਹੀਂਤਾਂ ਜੇ (channels == 1) flag = 0;
    ਨਹੀਂਤਾਂ ਜੇ (channels == 3) flag = 1;
    ਨਹੀਂਤਾਂ {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ਜੇ(!m.ਅੰਕੜੇ){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        ਅੱਖਰ buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ਵਾਪਸ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_ਮੰਜ਼ਿਲ_image(m);
    ਵਾਪਸ im;
}

ਪੂਰਨ show_image_cv(image im, ਅਚਲ ਅੱਖਰ* name, ਪੂਰਨ ms)
{
    Mat m = image_ਮੰਜ਼ਿਲ_mat(im);
    imshow(name, m);
    ਪੂਰਨ c = wait_ਕੁੰਜੀ(ms);
    ਜੇ (c != -1) c = c%256;
    ਵਾਪਸ c;
}

ਖਾਲੀ make_ਖਿੜਕੀ(ਅੱਖਰ *name, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ fullscreen)
{
    named_ਖਿੜਕੀ(name, ਖਿੜਕੀ_ਸਾਧਾਰਨ); 
    ਜੇ (fullscreen) {
        ਸੈੱਟ_ਖਿੜਕੀ_Property(name, CV_WND_PROP_FULLSCREEN, CV_ਖਿੜਕੀ_FULLSCREEN);
    } ਨਹੀਂਤਾਂ {
        resize_ਖਿੜਕੀ(name, w, h);
        ਜੇ(strcmp(name, "Demo") == 0) move_ਖਿੜਕੀ(name, 0, 0);
    }
}

}

#ਜੇ_ਅੰਤ
