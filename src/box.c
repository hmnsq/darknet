#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

ಪೂರ್ಣಾಂಕ nms_comparator(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *pa, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    ದಶಮಾಂಶ diff = 0;
    ಆದರೆ(b.ವಿಂಗಡಿಸು_class >= 0){
        diff = a.prob[b.ವಿಂಗಡಿಸು_class] - b.prob[b.ವಿಂಗಡಿಸು_class];
    } ಇಲ್ಲದಿದ್ದರೆ {
        diff = a.objectness - b.objectness;
    }
    ಆದರೆ(diff < 0) ಹಿಂತಿರುಗಿ 1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(diff > 0) ಹಿಂತಿರುಗಿ -1;
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ do_nms_obj(detection *dets, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    k = ಒಟ್ಟು-1;
    ಗಾಗಿ(i = 0; i <= k; ++i){
        ಆದರೆ(dets[i].objectness == 0){
            detection ಅದಲುಬದಲು = dets[i];
            dets[i] = dets[k];
            dets[k] = ಅದಲುಬದಲು;
            --k;
            --i;
        }
    }
    ಒಟ್ಟು = k+1;

    ಗಾಗಿ(i = 0; i < ಒಟ್ಟು; ++i){
        dets[i].ವಿಂಗಡಿಸು_class = -1;
    }

    qsort(dets, ಒಟ್ಟು, ಗಾತ್ರ(detection), nms_comparator);
    ಗಾಗಿ(i = 0; i < ಒಟ್ಟು; ++i){
        ಆದರೆ(dets[i].objectness == 0) ಮುಂದುವರಿ;
        box a = dets[i].bbox;
        ಗಾಗಿ(j = i+1; j < ಒಟ್ಟು; ++j){
            ಆದರೆ(dets[j].objectness == 0) ಮುಂದುವರಿ;
            box b = dets[j].bbox;
            ಆದರೆ (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                ಗಾಗಿ(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


ಶೂನ್ಯ do_nms_ವಿಂಗಡಿಸು(detection *dets, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    k = ಒಟ್ಟು-1;
    ಗಾಗಿ(i = 0; i <= k; ++i){
        ಆದರೆ(dets[i].objectness == 0){
            detection ಅದಲುಬದಲು = dets[i];
            dets[i] = dets[k];
            dets[k] = ಅದಲುಬದಲು;
            --k;
            --i;
        }
    }
    ಒಟ್ಟು = k+1;

    ಗಾಗಿ(k = 0; k < classes; ++k){
        ಗಾಗಿ(i = 0; i < ಒಟ್ಟು; ++i){
            dets[i].ವಿಂಗಡಿಸು_class = k;
        }
        qsort(dets, ಒಟ್ಟು, ಗಾತ್ರ(detection), nms_comparator);
        ಗಾಗಿ(i = 0; i < ಒಟ್ಟು; ++i){
            ಆದರೆ(dets[i].prob[k] == 0) ಮುಂದುವರಿ;
            box a = dets[i].bbox;
            ಗಾಗಿ(j = i+1; j < ಒಟ್ಟು; ++j){
                box b = dets[j].bbox;
                ಆದರೆ (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_ಗಮ್ಯ_box(ದಶಮಾಂಶ *f, ಪೂರ್ಣಾಂಕ stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    ಹಿಂತಿರುಗಿ b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    ದಶಮಾಂಶ l1 = a.x - a.w/2;
    ದಶಮಾಂಶ l2 = b.x - b.w/2;
    ಆದರೆ (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    ದಶಮಾಂಶ r1 = a.x + a.w/2;
    ದಶಮಾಂಶ r2 = b.x + b.w/2;
    ಆದರೆ(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    ಆದರೆ (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    ಆದರೆ (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    ದಶಮಾಂಶ t1 = a.y - a.h/2;
    ದಶಮಾಂಶ t2 = b.y - b.h/2;
    ಆದರೆ (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    ದಶಮಾಂಶ b1 = a.y + a.h/2;
    ದಶಮಾಂಶ b2 = b.y + b.h/2;
    ಆದರೆ(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    ಆದರೆ (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    ಆದರೆ (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    ಹಿಂತಿರುಗಿ d;
}

ದಶಮಾಂಶ overlap(ದಶಮಾಂಶ x1, ದಶಮಾಂಶ w1, ದಶಮಾಂಶ x2, ದಶಮಾಂಶ w2)
{
    ದಶಮಾಂಶ l1 = x1 - w1/2;
    ದಶಮಾಂಶ l2 = x2 - w2/2;
    ದಶಮಾಂಶ ಎಡ = l1 > l2 ? l1 : l2;
    ದಶಮಾಂಶ r1 = x1 + w1/2;
    ದಶಮಾಂಶ r2 = x2 + w2/2;
    ದಶಮಾಂಶ ಬಲ = r1 < r2 ? r1 : r2;
    ಹಿಂತಿರುಗಿ ಬಲ - ಎಡ;
}

ದಶಮಾಂಶ box_intersection(box a, box b)
{
    ದಶಮಾಂಶ w = overlap(a.x, a.w, b.x, b.w);
    ದಶಮಾಂಶ h = overlap(a.y, a.h, b.y, b.h);
    ಆದರೆ(w < 0 || h < 0) ಹಿಂತಿರುಗಿ 0;
    ದಶಮಾಂಶ area = w*h;
    ಹಿಂತಿರುಗಿ area;
}

ದಶಮಾಂಶ box_union(box a, box b)
{
    ದಶಮಾಂಶ i = box_intersection(a, b);
    ದಶಮಾಂಶ u = a.w*a.h + b.w*b.h - i;
    ಹಿಂತಿರುಗಿ u;
}

ದಶಮಾಂಶ box_iou(box a, box b)
{
    ಹಿಂತಿರುಗಿ box_intersection(a, b)/box_union(a, b);
}

ದಶಮಾಂಶ box_rmse(box a, box b)
{
    ಹಿಂತಿರುಗಿ ವರ್ಗಮೂಲ(ಘಾತ(a.x-b.x, 2) + 
                ಘಾತ(a.y-b.y, 2) + 
                ಘಾತ(a.w-b.w, 2) + 
                ಘಾತ(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    ದಶಮಾಂಶ w = overlap(a.x, a.w, b.x, b.w);
    ದಶಮಾಂಶ h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    ಹಿಂತಿರುಗಿ di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    ಹಿಂತಿರುಗಿ du;
}


ಶೂನ್ಯ test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    ಬರೆ("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ದಶಮಾಂಶ inter =  box_union(a, b);
    ದಶಮಾಂಶ xinter = box_union(dxa, b);
    ದಶಮಾಂಶ yinter = box_union(dya, b);
    ದಶಮಾಂಶ winter = box_union(dwa, b);
    ದಶಮಾಂಶ hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    ಬರೆ("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
ಶೂನ್ಯ test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    ಬರೆ("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ದಶಮಾಂಶ inter =  box_intersection(a, b);
    ದಶಮಾಂಶ xinter = box_intersection(dxa, b);
    ದಶಮಾಂಶ yinter = box_intersection(dya, b);
    ದಶಮಾಂಶ winter = box_intersection(dwa, b);
    ದಶಮಾಂಶ hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    ಬರೆ("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

ಶೂನ್ಯ test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    ದಶಮಾಂಶ iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    ಬರೆ("%f\n", iou);
    dbox d = diou(a, b);
    ಬರೆ("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    ದಶಮಾಂಶ xiou = box_iou(dxa, b);
    ದಶಮಾಂಶ yiou = box_iou(dya, b);
    ದಶಮಾಂಶ wiou = box_iou(dwa, b);
    ದಶಮಾಂಶ hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    ಬರೆ("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    ದಶಮಾಂಶ u = box_union(a,b);
    ದಶಮಾಂಶ i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    ಆದರೆ(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        ಹಿಂತಿರುಗಿ dd;
    }

    dd.dx = 2*ಘಾತ((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*ಘಾತ((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*ಘಾತ((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*ಘಾತ((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    ಹಿಂತಿರುಗಿ dd;
}


ಶೂನ್ಯ do_nms(box *boxes, ದಶಮಾಂಶ **probs, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(i = 0; i < ಒಟ್ಟು; ++i){
        ಪೂರ್ಣಾಂಕ any = 0;
        ಗಾಗಿ(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        ಆದರೆ(!any) {
            ಮುಂದುವರಿ;
        }
        ಗಾಗಿ(j = i+1; j < ಒಟ್ಟು; ++j){
            ಆದರೆ (box_iou(boxes[i], boxes[j]) > thresh){
                ಗಾಗಿ(k = 0; k < classes; ++k){
                    ಆದರೆ (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    ಇಲ್ಲದಿದ್ದರೆ probs[j][k] = 0;
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
    ಹಿಂತಿರುಗಿ encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = ಘಾತ(2., b.w) * anchor.w;
    decode.h = ಘಾತ(2., b.h) * anchor.h;
    ಹಿಂತಿರುಗಿ decode;
}
