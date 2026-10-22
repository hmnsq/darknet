#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

ਪੂਰਨ nms_comparator(ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    ਦਸ਼ਮਲਵ diff = 0;
    ਜੇ(b.ਛਾਂਟੋ_class >= 0){
        diff = a.prob[b.ਛਾਂਟੋ_class] - b.prob[b.ਛਾਂਟੋ_class];
    } ਨਹੀਂਤਾਂ {
        diff = a.objectness - b.objectness;
    }
    ਜੇ(diff < 0) ਵਾਪਸ 1;
    ਨਹੀਂਤਾਂ ਜੇ(diff > 0) ਵਾਪਸ -1;
    ਵਾਪਸ 0;
}

ਖਾਲੀ do_nms_obj(detection *dets, ਪੂਰਨ ਕੁੱਲ, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i, j, k;
    k = ਕੁੱਲ-1;
    ਲਈ(i = 0; i <= k; ++i){
        ਜੇ(dets[i].objectness == 0){
            detection ਅਦਲਾ_ਬਦਲੀ = dets[i];
            dets[i] = dets[k];
            dets[k] = ਅਦਲਾ_ਬਦਲੀ;
            --k;
            --i;
        }
    }
    ਕੁੱਲ = k+1;

    ਲਈ(i = 0; i < ਕੁੱਲ; ++i){
        dets[i].ਛਾਂਟੋ_class = -1;
    }

    qsort(dets, ਕੁੱਲ, ਆਕਾਰ(detection), nms_comparator);
    ਲਈ(i = 0; i < ਕੁੱਲ; ++i){
        ਜੇ(dets[i].objectness == 0) ਜਾਰੀ;
        box a = dets[i].bbox;
        ਲਈ(j = i+1; j < ਕੁੱਲ; ++j){
            ਜੇ(dets[j].objectness == 0) ਜਾਰੀ;
            box b = dets[j].bbox;
            ਜੇ (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                ਲਈ(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


ਖਾਲੀ do_nms_ਛਾਂਟੋ(detection *dets, ਪੂਰਨ ਕੁੱਲ, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i, j, k;
    k = ਕੁੱਲ-1;
    ਲਈ(i = 0; i <= k; ++i){
        ਜੇ(dets[i].objectness == 0){
            detection ਅਦਲਾ_ਬਦਲੀ = dets[i];
            dets[i] = dets[k];
            dets[k] = ਅਦਲਾ_ਬਦਲੀ;
            --k;
            --i;
        }
    }
    ਕੁੱਲ = k+1;

    ਲਈ(k = 0; k < classes; ++k){
        ਲਈ(i = 0; i < ਕੁੱਲ; ++i){
            dets[i].ਛਾਂਟੋ_class = k;
        }
        qsort(dets, ਕੁੱਲ, ਆਕਾਰ(detection), nms_comparator);
        ਲਈ(i = 0; i < ਕੁੱਲ; ++i){
            ਜੇ(dets[i].prob[k] == 0) ਜਾਰੀ;
            box a = dets[i].bbox;
            ਲਈ(j = i+1; j < ਕੁੱਲ; ++j){
                box b = dets[j].bbox;
                ਜੇ (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_ਮੰਜ਼ਿਲ_box(ਦਸ਼ਮਲਵ *f, ਪੂਰਨ stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    ਵਾਪਸ b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    ਦਸ਼ਮਲਵ l1 = a.x - a.w/2;
    ਦਸ਼ਮਲਵ l2 = b.x - b.w/2;
    ਜੇ (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    ਦਸ਼ਮਲਵ r1 = a.x + a.w/2;
    ਦਸ਼ਮਲਵ r2 = b.x + b.w/2;
    ਜੇ(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    ਜੇ (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    ਜੇ (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    ਦਸ਼ਮਲਵ t1 = a.y - a.h/2;
    ਦਸ਼ਮਲਵ t2 = b.y - b.h/2;
    ਜੇ (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    ਦਸ਼ਮਲਵ b1 = a.y + a.h/2;
    ਦਸ਼ਮਲਵ b2 = b.y + b.h/2;
    ਜੇ(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    ਜੇ (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    ਜੇ (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    ਵਾਪਸ d;
}

ਦਸ਼ਮਲਵ overlap(ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ w1, ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ w2)
{
    ਦਸ਼ਮਲਵ l1 = x1 - w1/2;
    ਦਸ਼ਮਲਵ l2 = x2 - w2/2;
    ਦਸ਼ਮਲਵ ਖੱਬਾ = l1 > l2 ? l1 : l2;
    ਦਸ਼ਮਲਵ r1 = x1 + w1/2;
    ਦਸ਼ਮਲਵ r2 = x2 + w2/2;
    ਦਸ਼ਮਲਵ ਸੱਜਾ = r1 < r2 ? r1 : r2;
    ਵਾਪਸ ਸੱਜਾ - ਖੱਬਾ;
}

ਦਸ਼ਮਲਵ box_intersection(box a, box b)
{
    ਦਸ਼ਮਲਵ w = overlap(a.x, a.w, b.x, b.w);
    ਦਸ਼ਮਲਵ h = overlap(a.y, a.h, b.y, b.h);
    ਜੇ(w < 0 || h < 0) ਵਾਪਸ 0;
    ਦਸ਼ਮਲਵ area = w*h;
    ਵਾਪਸ area;
}

ਦਸ਼ਮਲਵ box_union(box a, box b)
{
    ਦਸ਼ਮਲਵ i = box_intersection(a, b);
    ਦਸ਼ਮਲਵ u = a.w*a.h + b.w*b.h - i;
    ਵਾਪਸ u;
}

ਦਸ਼ਮਲਵ box_iou(box a, box b)
{
    ਵਾਪਸ box_intersection(a, b)/box_union(a, b);
}

ਦਸ਼ਮਲਵ box_rmse(box a, box b)
{
    ਵਾਪਸ ਵਰਗਮੂਲ(ਘਾਤ(a.x-b.x, 2) + 
                ਘਾਤ(a.y-b.y, 2) + 
                ਘਾਤ(a.w-b.w, 2) + 
                ਘਾਤ(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    ਦਸ਼ਮਲਵ w = overlap(a.x, a.w, b.x, b.w);
    ਦਸ਼ਮਲਵ h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    ਵਾਪਸ di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    ਵਾਪਸ du;
}


ਖਾਲੀ test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    ਲਿਖੋ("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ਦਸ਼ਮਲਵ inter =  box_union(a, b);
    ਦਸ਼ਮਲਵ xinter = box_union(dxa, b);
    ਦਸ਼ਮਲਵ yinter = box_union(dya, b);
    ਦਸ਼ਮਲਵ winter = box_union(dwa, b);
    ਦਸ਼ਮਲਵ hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    ਲਿਖੋ("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
ਖਾਲੀ test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    ਲਿਖੋ("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ਦਸ਼ਮਲਵ inter =  box_intersection(a, b);
    ਦਸ਼ਮਲਵ xinter = box_intersection(dxa, b);
    ਦਸ਼ਮਲਵ yinter = box_intersection(dya, b);
    ਦਸ਼ਮਲਵ winter = box_intersection(dwa, b);
    ਦਸ਼ਮਲਵ hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    ਲਿਖੋ("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

ਖਾਲੀ test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    ਦਸ਼ਮਲਵ iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    ਲਿਖੋ("%f\n", iou);
    dbox d = diou(a, b);
    ਲਿਖੋ("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    ਦਸ਼ਮਲਵ xiou = box_iou(dxa, b);
    ਦਸ਼ਮਲਵ yiou = box_iou(dya, b);
    ਦਸ਼ਮਲਵ wiou = box_iou(dwa, b);
    ਦਸ਼ਮਲਵ hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    ਲਿਖੋ("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    ਦਸ਼ਮਲਵ u = box_union(a,b);
    ਦਸ਼ਮਲਵ i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    ਜੇ(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        ਵਾਪਸ dd;
    }

    dd.dx = 2*ਘਾਤ((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*ਘਾਤ((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*ਘਾਤ((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*ਘਾਤ((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    ਵਾਪਸ dd;
}


ਖਾਲੀ do_nms(box *boxes, ਦਸ਼ਮਲਵ **probs, ਪੂਰਨ ਕੁੱਲ, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i, j, k;
    ਲਈ(i = 0; i < ਕੁੱਲ; ++i){
        ਪੂਰਨ any = 0;
        ਲਈ(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        ਜੇ(!any) {
            ਜਾਰੀ;
        }
        ਲਈ(j = i+1; j < ਕੁੱਲ; ++j){
            ਜੇ (box_iou(boxes[i], boxes[j]) > thresh){
                ਲਈ(k = 0; k < classes; ++k){
                    ਜੇ (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    ਨਹੀਂਤਾਂ probs[j][k] = 0;
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
    ਵਾਪਸ encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = ਘਾਤ(2., b.w) * anchor.w;
    decode.h = ਘਾਤ(2., b.h) * anchor.h;
    ਵਾਪਸ decode;
}
