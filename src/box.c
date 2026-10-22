#অন্তর্ভাব "box.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdlib.h>

পূর্ণসংখ্যা nms_comparator(ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    দশমিক diff = 0;
    যদি(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } অন্যথা {
        diff = a.objectness - b.objectness;
    }
    যদি(diff < 0) ফেরত 1;
    অন্যথা যদি(diff > 0) ফেরত -1;
    ফেরত 0;
}

শূন্য do_nms_obj(detection *dets, পূর্ণসংখ্যা যোগ_2, পূর্ণসংখ্যা classes, দশমিক thresh)
{
    পূর্ণসংখ্যা i, j, k;
    k = যোগ_2-1;
    জন্য(i = 0; i <= k; ++i){
        যদি(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    যোগ_2 = k+1;

    জন্য(i = 0; i < যোগ_2; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, যোগ_2, আকার(detection), nms_comparator);
    জন্য(i = 0; i < যোগ_2; ++i){
        যদি(dets[i].objectness == 0) চালু;
        box a = dets[i].bbox;
        জন্য(j = i+1; j < যোগ_2; ++j){
            যদি(dets[j].objectness == 0) চালু;
            box b = dets[j].bbox;
            যদি (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                জন্য(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


শূন্য do_nms_sort(detection *dets, পূর্ণসংখ্যা যোগ_2, পূর্ণসংখ্যা classes, দশমিক thresh)
{
    পূর্ণসংখ্যা i, j, k;
    k = যোগ_2-1;
    জন্য(i = 0; i <= k; ++i){
        যদি(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    যোগ_2 = k+1;

    জন্য(k = 0; k < classes; ++k){
        জন্য(i = 0; i < যোগ_2; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, যোগ_2, আকার(detection), nms_comparator);
        জন্য(i = 0; i < যোগ_2; ++i){
            যদি(dets[i].prob[k] == 0) চালু;
            box a = dets[i].bbox;
            জন্য(j = i+1; j < যোগ_2; ++j){
                box b = dets[j].bbox;
                যদি (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(দশমিক *f, পূর্ণসংখ্যা stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    ফেরত b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    দশমিক l1 = a.x - a.w/2;
    দশমিক l2 = b.x - b.w/2;
    যদি (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    দশমিক r1 = a.x + a.w/2;
    দশমিক r2 = b.x + b.w/2;
    যদি(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    যদি (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    যদি (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    দশমিক t1 = a.y - a.h/2;
    দশমিক t2 = b.y - b.h/2;
    যদি (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    দশমিক b1 = a.y + a.h/2;
    দশমিক b2 = b.y + b.h/2;
    যদি(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    যদি (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    যদি (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    ফেরত d;
}

দশমিক overlap(দশমিক x1, দশমিক w1, দশমিক x2, দশমিক w2)
{
    দশমিক l1 = x1 - w1/2;
    দশমিক l2 = x2 - w2/2;
    দশমিক বাম = l1 > l2 ? l1 : l2;
    দশমিক r1 = x1 + w1/2;
    দশমিক r2 = x2 + w2/2;
    দশমিক ডান = r1 < r2 ? r1 : r2;
    ফেরত ডান - বাম;
}

দশমিক box_intersection(box a, box b)
{
    দশমিক w = overlap(a.x, a.w, b.x, b.w);
    দশমিক h = overlap(a.y, a.h, b.y, b.h);
    যদি(w < 0 || h < 0) ফেরত 0;
    দশমিক area = w*h;
    ফেরত area;
}

দশমিক box_union(box a, box b)
{
    দশমিক i = box_intersection(a, b);
    দশমিক u = a.w*a.h + b.w*b.h - i;
    ফেরত u;
}

দশমিক box_iou(box a, box b)
{
    ফেরত box_intersection(a, b)/box_union(a, b);
}

দশমিক box_rmse(box a, box b)
{
    ফেরত sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    দশমিক w = overlap(a.x, a.w, b.x, b.w);
    দশমিক h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    ফেরত di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    ফেরত du;
}


শূন্য test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    দশমিক inter =  box_union(a, b);
    দশমিক xinter = box_union(dxa, b);
    দশমিক yinter = box_union(dya, b);
    দশমিক winter = box_union(dwa, b);
    দশমিক hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
শূন্য test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    দশমিক inter =  box_intersection(a, b);
    দশমিক xinter = box_intersection(dxa, b);
    দশমিক yinter = box_intersection(dya, b);
    দশমিক winter = box_intersection(dwa, b);
    দশমিক hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

শূন্য test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    দশমিক iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    দশমিক xiou = box_iou(dxa, b);
    দশমিক yiou = box_iou(dya, b);
    দশমিক wiou = box_iou(dwa, b);
    দশমিক hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    দশমিক u = box_union(a,b);
    দশমিক i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    যদি(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        ফেরত dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    ফেরত dd;
}


শূন্য do_nms(box *boxes, দশমিক **probs, পূর্ণসংখ্যা যোগ_2, পূর্ণসংখ্যা classes, দশমিক thresh)
{
    পূর্ণসংখ্যা i, j, k;
    জন্য(i = 0; i < যোগ_2; ++i){
        পূর্ণসংখ্যা any = 0;
        জন্য(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        যদি(!any) {
            চালু;
        }
        জন্য(j = i+1; j < যোগ_2; ++j){
            যদি (box_iou(boxes[i], boxes[j]) > thresh){
                জন্য(k = 0; k < classes; ++k){
                    যদি (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    অন্যথা probs[j][k] = 0;
                }
            }
        }
    }
}

box encode_box(box b, box anchor)
{
    box encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = log2(b.w / anchor.w);
    encode.h = log2(b.h / anchor.h);
    ফেরত encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    ফেরত decode;
}
