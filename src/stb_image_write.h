/* stb_image_write - v1.09 - public domain - http://nothings.org/stb/stb_image_write.h
   writes out PNG/BMP/TGA/JPEG/HDR images to C stdio - Sean Barrett 2010-2015
                                     no warranty implied; use at your own risk

   Before #including,

       #define STB_IMAGE_WRITE_IMPLEMENTATION

   in the file that you want to have the implementation.

   Will probably not work correctly with strict-aliasing optimizations.

   If using a modern Microsoft Compiler, non-safe versions of CRT calls may cause 
   compilation warnings or even errors. To avoid this, also before #including,

       #define STBI_MSC_SECURE_CRT

ABOUT:

   This header file is a library for writing images to C stdio. It could be
   adapted to write to memory or a general streaming interface; let me know.

   The PNG output is not optimal; it is 20-50% larger than the file
   written by a decent optimizing implementation; though providing a custom
   zlib compress function (see STBIW_ZLIB_COMPRESS) can mitigate that.
   This library is designed for source code compactness and simplicity,
   not optimal image file size or run-time performance.

BUILDING:

   You can #define STBIW_ASSERT(x) before the #include to avoid using assert.h.
   You can #define STBIW_MALLOC(), STBIW_REALLOC(), and STBIW_FREE() to replace
   malloc,realloc,free.
   You can #define STBIW_MEMMOVE() to replace memmove()
   You can #define STBIW_ZLIB_COMPRESS to use a custom zlib-style compress function
   for PNG compression (instead of the builtin one), it must have the following signature:
   unsigned char * my_compress(unsigned char *data, int data_len, int *out_len, int quality);
   The returned data will be freed with STBIW_FREE() (free() by default),
   so it must be heap allocated with STBIW_MALLOC() (malloc() by default),

USAGE:

   There are five functions, one for each image file format:

     int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
     int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
     int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
     int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);
     int stbi_write_hdr(char const *filename, int w, int h, int comp, const float *data);

     void stbi_flip_vertically_on_write(int flag); // flag is non-zero to flip data vertically

   There are also five equivalent functions that use an arbitrary write function. You are
   expected to open/close your file-equivalent before and after calling these:

     int stbi_write_png_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data, int stride_in_bytes);
     int stbi_write_bmp_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
     int stbi_write_tga_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
     int stbi_write_hdr_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const float *data);
     int stbi_write_jpg_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data, int quality);

   where the callback is:
      void stbi_write_func(void *context, void *data, int size);

   You can configure it with these global variables:
      int stbi_write_tga_with_rle;             // defaults to true; set to 0 to disable RLE
      int stbi_write_png_compression_level;    // defaults to 8; set to higher for more compression
      int stbi_write_force_png_filter;         // defaults to -1; set to 0..5 to force a filter mode


   You can define STBI_WRITE_NO_STDIO to disable the file variant of these
   functions, so the library will not use stdio.h at all. However, this will
   also disable HDR writing, because it requires stdio for formatted output.

   Each function returns 0 on failure and non-0 on success.

   The functions create an image file defined by the parameters. The image
   is a rectangle of pixels stored from left-to-right, top-to-bottom.
   Each pixel contains 'comp' channels of data stored interleaved with 8-bits
   per channel, in the following order: 1=Y, 2=YA, 3=RGB, 4=RGBA. (Y is
   monochrome color.) The rectangle is 'w' pixels wide and 'h' pixels tall.
   The *data pointer points to the first byte of the top-left-most pixel.
   For PNG, "stride_in_bytes" is the distance in bytes from the first byte of
   a row of pixels to the first byte of the next row of pixels.

   PNG creates output files with the same number of components as the input.
   The BMP format expands Y to RGB in the file format and does not
   output alpha.

   PNG supports writing rectangles of data even when the bytes storing rows of
   data are not consecutive in memory (e.g. sub-rectangles of a larger image),
   by supplying the stride between the beginning of adjacent rows. The other
   formats do not. (Thus you cannot write a native-format BMP through the BMP
   writer, both because it is in BGR order and because it may have padding
   at the end of the line.)

   PNG allows you to set the deflate compression level by setting the global
   variable 'stbi_write_png_compression_level' (it defaults to 8).

   HDR expects linear float data. Since the format is always 32-bit rgb(e)
   data, alpha (if provided) is discarded, and for monochrome data it is
   replicated across all three channels.

   TGA supports RLE or non-RLE compressed data. To use non-RLE-compressed
   data, set the global variable 'stbi_write_tga_with_rle' to 0.
   
   JPEG does ignore alpha channels in input data; quality is between 1 and 100.
   Higher quality looks better but results in a bigger image.
   JPEG baseline (no JPEG progressive).

CREDITS:


   Sean Barrett           -    PNG/BMP/TGA 
   Baldur Karlsson        -    HDR
   Jean-Sebastien Guay    -    TGA monochrome
   Tim Kelsey             -    misc enhancements
   Alan Hickman           -    TGA RLE
   Emmanuel Julien        -    initial file IO callback implementation
   Jon Olick              -    original jo_jpeg.cpp code
   Daniel Gibson          -    integrate JPEG, allow external zlib
   Aarni Koskela          -    allow choosing PNG filter

   bugfixes:
      github:Chribba
      Guillaume Chereau
      github:jry2
      github:romigrou
      Sergio Gonzalez
      Jonas Karlsson
      Filip Wasil
      Thatcher Ulrich
      github:poppolopoppo
      Patrick Boettcher
      github:xeekworx
      Cap Petschulat
      Simon Rodriguez
      Ivan Tikhonov
      github:ignotion
      Adam Schackart

LICENSE

  See end of file for license information.

*/

#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define INCLUDE_STB_IMAGE_WRITE_H

// if STB_IMAGE_WRITE_STATIC causes problems, try defining STBIWDEF to 'inline' or 'static inline'
#ifndef STBIWDEF
#ifdef STB_IMAGE_WRITE_STATIC
#define STBIWDEF  സ്ഥിതം
#അല്ലെങ്കിൽ
#ifdef __cplusplus
#define STBIWDEF  ബാഹ്യ "C"
#അല്ലെങ്കിൽ
#define STBIWDEF  ബാഹ്യ
#endif
#endif
#endif

#ifndef STB_IMAGE_WRITE_STATIC  // C++ forbids static forward declarations
ബാഹ്യ പൂർണ്ണസംഖ്യ stbi_write_tga_with_rle;
ബാഹ്യ പൂർണ്ണസംഖ്യ stbi_write_png_compression_level;
ബാഹ്യ പൂർണ്ണസംഖ്യ stbi_write_force_png_filter;
#endif

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_png(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data, പൂർണ്ണസംഖ്യ stride_in_bytes);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_bmp(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_tga(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_hdr(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ദശാംശം *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_jpg(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data, പൂർണ്ണസംഖ്യ quality);
#endif

തരനിർവചനം ശൂന്യം stbi_write_func(ശൂന്യം *context, ശൂന്യം *data, പൂർണ്ണസംഖ്യ size);

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_png_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data, പൂർണ്ണസംഖ്യ stride_in_bytes);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_bmp_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_tga_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_hdr_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ദശാംശം *data);
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_jpg_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം  *data, പൂർണ്ണസംഖ്യ quality);

STBIWDEF ശൂന്യം stbi_flip_vertically_on_write(പൂർണ്ണസംഖ്യ flip_boolean);

#endif//INCLUDE_STB_IMAGE_WRITE_H

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef _WIN32
   #ifndef _CRT_SECURE_NO_WARNINGS
   #define _CRT_SECURE_NO_WARNINGS
   #endif
   #ifndef _CRT_NONSTDC_NO_DEPRECATE
   #define _CRT_NONSTDC_NO_DEPRECATE
   #endif
#endif

#ifndef STBI_WRITE_NO_STDIO
#അന്തര്ഭാവ <stdio.h>
#endif // STBI_WRITE_NO_STDIO

#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <math.h>

#എങ്കിൽ defined(STBIW_MALLOC) && defined(STBIW_FREE) && (defined(STBIW_REALLOC) || defined(STBIW_REALLOC_SIZED))
// ok
#അല്ലെങ്കിലെങ്കിൽ !defined(STBIW_MALLOC) && !defined(STBIW_FREE) && !defined(STBIW_REALLOC) && !defined(STBIW_REALLOC_SIZED)
// ok
#അല്ലെങ്കിൽ
#error "Must define all or none of STBIW_MALLOC, STBIW_FREE, and STBIW_REALLOC (or STBIW_REALLOC_SIZED)."
#endif

#ifndef STBIW_MALLOC
#define STBIW_MALLOC(sz)        malloc(sz)
#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_FREE(p)           സ്വതന്ത്രം(p)
#endif

#ifndef STBIW_REALLOC_SIZED
#define STBIW_REALLOC_SIZED(p,oldsz,newsz) STBIW_REALLOC(p,newsz)
#endif


#ifndef STBIW_MEMMOVE
#define STBIW_MEMMOVE(a,b,sz) memmove(a,b,sz)
#endif


#ifndef STBIW_ASSERT
#അന്തര്ഭാവ <assert.h>
#define STBIW_ASSERT(x) പ്രതിജ്ഞാ(x)
#endif

#define STBIW_UCHAR(x) (unsigned അക്ഷരം) ((x) & 0xff)

#ifdef STB_IMAGE_WRITE_STATIC
സ്ഥിതം പൂർണ്ണസംഖ്യ stbi__flip_vertically_on_write=0;
സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_png_compression_level = 8;
സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_tga_with_rle = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_force_png_filter = -1;
#അല്ലെങ്കിൽ
പൂർണ്ണസംഖ്യ stbi_write_png_compression_level = 8;
പൂർണ്ണസംഖ്യ stbi__flip_vertically_on_write=0;
പൂർണ്ണസംഖ്യ stbi_write_tga_with_rle = 1;
പൂർണ്ണസംഖ്യ stbi_write_force_png_filter = -1;
#endif

STBIWDEF ശൂന്യം stbi_flip_vertically_on_write(പൂർണ്ണസംഖ്യ flag)
{
   stbi__flip_vertically_on_write = flag;
}

തരനിർവചനം ഘടന
{
   stbi_write_func *func;
   ശൂന്യം *context;
} stbi__write_context;

// initialize a callback-based context
സ്ഥിതം ശൂന്യം stbi__start_write_callbacks(stbi__write_context *s, stbi_write_func *c, ശൂന്യം *context)
{
   s->func    = c;
   s->context = context;
}

#ifndef STBI_WRITE_NO_STDIO

സ്ഥിതം ശൂന്യം stbi__stdio_write(ശൂന്യം *context, ശൂന്യം *data, പൂർണ്ണസംഖ്യ size)
{
   fwrite(data,1,size,(FILE*) context);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbi__start_write_file(stbi__write_context *s, സ്ഥിരം അക്ഷരം *filename)
{
   FILE *f;
#ifdef STBI_MSC_SECURE_CRT
   എങ്കിൽ (fopen_s(&f, filename, "wb"))
      f = ശൂന്യ;
#അല്ലെങ്കിൽ
   f = fopen(filename, "wb");
#endif
   stbi__start_write_callbacks(s, stbi__stdio_write, (ശൂന്യം *) f);
   തിരികെ f != ശൂന്യ;
}

സ്ഥിതം ശൂന്യം stbi__end_write_file(stbi__write_context *s)
{
   fclose((FILE *)s->context);
}

#endif // !STBI_WRITE_NO_STDIO

തരനിർവചനം unsigned പൂർണ്ണസംഖ്യ stbiw_uint32;
തരനിർവചനം പൂർണ്ണസംഖ്യ stb_image_write_test[വലുപ്പം(stbiw_uint32)==4 ? 1 : -1];

സ്ഥിതം ശൂന്യം stbiw__writefv(stbi__write_context *s, സ്ഥിരം അക്ഷരം *fmt, va_list v)
{
   ആകുമ്പോൾ (*fmt) {
      തിരഞ്ഞെടുപ്പ് (*fmt++) {
         അവസ്ഥ ' ': നിർത്തുക;
         അവസ്ഥ '1': { unsigned അക്ഷരം x = STBIW_UCHAR(va_arg(v, പൂർണ്ണസംഖ്യ));
                     s->func(s->context,&x,1);
                     നിർത്തുക; }
         അവസ്ഥ '2': { പൂർണ്ണസംഖ്യ x = va_arg(v,പൂർണ്ണസംഖ്യ);
                     unsigned അക്ഷരം b[2];
                     b[0] = STBIW_UCHAR(x);
                     b[1] = STBIW_UCHAR(x>>8);
                     s->func(s->context,b,2);
                     നിർത്തുക; }
         അവസ്ഥ '4': { stbiw_uint32 x = va_arg(v,പൂർണ്ണസംഖ്യ);
                     unsigned അക്ഷരം b[4];
                     b[0]=STBIW_UCHAR(x);
                     b[1]=STBIW_UCHAR(x>>8);
                     b[2]=STBIW_UCHAR(x>>16);
                     b[3]=STBIW_UCHAR(x>>24);
                     s->func(s->context,b,4);
                     നിർത്തുക; }
         സ്ഥിരസ്ഥിതി:
            STBIW_ASSERT(0);
            തിരികെ;
      }
   }
}

സ്ഥിതം ശൂന്യം stbiw__writef(stbi__write_context *s, സ്ഥിരം അക്ഷരം *fmt, ...)
{
   va_list v;
   va_start(v, fmt);
   stbiw__writefv(s, fmt, v);
   va_end(v);
}

സ്ഥിതം ശൂന്യം stbiw__putc(stbi__write_context *s, unsigned അക്ഷരം c)
{
   s->func(s->context, &c, 1);
}

സ്ഥിതം ശൂന്യം stbiw__write3(stbi__write_context *s, unsigned അക്ഷരം a, unsigned അക്ഷരം b, unsigned അക്ഷരം c)
{
   unsigned അക്ഷരം arr[3];
   arr[0] = a, arr[1] = b, arr[2] = c;
   s->func(s->context, arr, 3);
}

സ്ഥിതം ശൂന്യം stbiw__write_pixel(stbi__write_context *s, പൂർണ്ണസംഖ്യ rgb_dir, പൂർണ്ണസംഖ്യ comp, പൂർണ്ണസംഖ്യ write_alpha, പൂർണ്ണസംഖ്യ expand_mono, unsigned അക്ഷരം *d)
{
   unsigned അക്ഷരം bg[3] = { 255, 0, 255}, px[3];
   പൂർണ്ണസംഖ്യ k;

   എങ്കിൽ (write_alpha < 0)
      s->func(s->context, &d[comp - 1], 1);

   തിരഞ്ഞെടുപ്പ് (comp) {
      അവസ്ഥ 2: // 2 pixels = mono + alpha, alpha is written separately, so same as 1-channel case
      അവസ്ഥ 1:
         എങ്കിൽ (expand_mono)
            stbiw__write3(s, d[0], d[0], d[0]); // monochrome bmp
         അല്ലെങ്കിൽ
            s->func(s->context, d, 1);  // monochrome TGA
         നിർത്തുക;
      അവസ്ഥ 4:
         എങ്കിൽ (!write_alpha) {
            // composite against pink background
            വേണ്ടി (k = 0; k < 3; ++k)
               px[k] = bg[k] + ((d[k] - bg[k]) * d[3]) / 255;
            stbiw__write3(s, px[1 - rgb_dir], px[1], px[1 + rgb_dir]);
            നിർത്തുക;
         }
         /* FALLTHROUGH */
      അവസ്ഥ 3:
         stbiw__write3(s, d[1 - rgb_dir], d[1], d[1 + rgb_dir]);
         നിർത്തുക;
   }
   എങ്കിൽ (write_alpha > 0)
      s->func(s->context, &d[comp - 1], 1);
}

സ്ഥിതം ശൂന്യം stbiw__write_pixels(stbi__write_context *s, പൂർണ്ണസംഖ്യ rgb_dir, പൂർണ്ണസംഖ്യ vdir, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, ശൂന്യം *data, പൂർണ്ണസംഖ്യ write_alpha, പൂർണ്ണസംഖ്യ scanline_pad, പൂർണ്ണസംഖ്യ expand_mono)
{
   stbiw_uint32 zero = 0;
   പൂർണ്ണസംഖ്യ i,j, j_end;

   എങ്കിൽ (y <= 0)
      തിരികെ;

   എങ്കിൽ (stbi__flip_vertically_on_write)
      vdir *= -1;

   എങ്കിൽ (vdir < 0)
      j_end = -1, j = y-1;
   അല്ലെങ്കിൽ
      j_end =  y, j = 0;

   വേണ്ടി (; j != j_end; j += vdir) {
      വേണ്ടി (i=0; i < x; ++i) {
         unsigned അക്ഷരം *d = (unsigned അക്ഷരം *) data + (j*x+i)*comp;
         stbiw__write_pixel(s, rgb_dir, comp, write_alpha, expand_mono, d);
      }
      s->func(s->context, &zero, scanline_pad);
   }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbiw__outfile(stbi__write_context *s, പൂർണ്ണസംഖ്യ rgb_dir, പൂർണ്ണസംഖ്യ vdir, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, പൂർണ്ണസംഖ്യ expand_mono, ശൂന്യം *data, പൂർണ്ണസംഖ്യ alpha, പൂർണ്ണസംഖ്യ pad, സ്ഥിരം അക്ഷരം *fmt, ...)
{
   എങ്കിൽ (y < 0 || x < 0) {
      തിരികെ 0;
   } അല്ലെങ്കിൽ {
      va_list v;
      va_start(v, fmt);
      stbiw__writefv(s, fmt, v);
      va_end(v);
      stbiw__write_pixels(s,rgb_dir,vdir,x,y,comp,data,alpha,pad, expand_mono);
      തിരികെ 1;
   }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_bmp_core(stbi__write_context *s, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data)
{
   പൂർണ്ണസംഖ്യ pad = (-x*3) & 3;
   തിരികെ stbiw__outfile(s,-1,-1,x,y,comp,1,(ശൂന്യം *) data,0,pad,
           "11 4 22 4" "4 44 22 444444",
           'B', 'M', 14+40+(x*3+pad)*y, 0,0, 14+40,  // file header
            40, x,y, 1,24, 0,0,0,0,0,0);             // bitmap header
}

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_bmp_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data)
{
   stbi__write_context s;
   stbi__start_write_callbacks(&s, func, context);
   തിരികെ stbi_write_bmp_core(&s, x, y, comp, data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_bmp(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data)
{
   stbi__write_context s;
   എങ്കിൽ (stbi__start_write_file(&s,filename)) {
      പൂർണ്ണസംഖ്യ r = stbi_write_bmp_core(&s, x, y, comp, data);
      stbi__end_write_file(&s);
      തിരികെ r;
   } അല്ലെങ്കിൽ
      തിരികെ 0;
}
#endif //!STBI_WRITE_NO_STDIO

സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_tga_core(stbi__write_context *s, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, ശൂന്യം *data)
{
   പൂർണ്ണസംഖ്യ has_alpha = (comp == 2 || comp == 4);
   പൂർണ്ണസംഖ്യ colorbytes = has_alpha ? comp-1 : comp;
   പൂർണ്ണസംഖ്യ format = colorbytes < 2 ? 3 : 2; // 3 color channels (RGB/RGBA) = 2, 1 color channel (Y/YA) = 3

   എങ്കിൽ (y < 0 || x < 0)
      തിരികെ 0;

   എങ്കിൽ (!stbi_write_tga_with_rle) {
      തിരികെ stbiw__outfile(s, -1, -1, x, y, comp, 0, (ശൂന്യം *) data, has_alpha, 0,
         "111 221 2222 11", 0, 0, format, 0, 0, 0, 0, 0, x, y, (colorbytes + has_alpha) * 8, has_alpha * 8);
   } അല്ലെങ്കിൽ {
      പൂർണ്ണസംഖ്യ i,j,k;
      പൂർണ്ണസംഖ്യ jend, jdir;

      stbiw__writef(s, "111 221 2222 11", 0,0,format+8, 0,0,0, 0,0,x,y, (colorbytes + has_alpha) * 8, has_alpha * 8);

      എങ്കിൽ (stbi__flip_vertically_on_write) {
         j = 0;
         jend = y;
         jdir = 1;
      } അല്ലെങ്കിൽ {
         j = y-1;
         jend = -1;
         jdir = -1;
      }
      വേണ്ടി (; j != jend; j += jdir) {
         unsigned അക്ഷരം *row = (unsigned അക്ഷരം *) data + j * x * comp;
         പൂർണ്ണസംഖ്യ len;

         വേണ്ടി (i = 0; i < x; i += len) {
            unsigned അക്ഷരം *begin = row + i * comp;
            പൂർണ്ണസംഖ്യ diff = 1;
            len = 1;

            എങ്കിൽ (i < x - 1) {
               ++len;
               diff = memcmp(begin, row + (i + 1) * comp, comp);
               എങ്കിൽ (diff) {
                  സ്ഥിരം unsigned അക്ഷരം *prev = begin;
                  വേണ്ടി (k = i + 2; k < x && len < 128; ++k) {
                     എങ്കിൽ (memcmp(prev, row + k * comp, comp)) {
                        prev += comp;
                        ++len;
                     } അല്ലെങ്കിൽ {
                        --len;
                        നിർത്തുക;
                     }
                  }
               } അല്ലെങ്കിൽ {
                  വേണ്ടി (k = i + 2; k < x && len < 128; ++k) {
                     എങ്കിൽ (!memcmp(begin, row + k * comp, comp)) {
                        ++len;
                     } അല്ലെങ്കിൽ {
                        നിർത്തുക;
                     }
                  }
               }
            }

            എങ്കിൽ (diff) {
               unsigned അക്ഷരം header = STBIW_UCHAR(len - 1);
               s->func(s->context, &header, 1);
               വേണ്ടി (k = 0; k < len; ++k) {
                  stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin + k * comp);
               }
            } അല്ലെങ്കിൽ {
               unsigned അക്ഷരം header = STBIW_UCHAR(len - 129);
               s->func(s->context, &header, 1);
               stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin);
            }
         }
      }
   }
   തിരികെ 1;
}

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_tga_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data)
{
   stbi__write_context s;
   stbi__start_write_callbacks(&s, func, context);
   തിരികെ stbi_write_tga_core(&s, x, y, comp, (ശൂന്യം *) data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_tga(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data)
{
   stbi__write_context s;
   എങ്കിൽ (stbi__start_write_file(&s,filename)) {
      പൂർണ്ണസംഖ്യ r = stbi_write_tga_core(&s, x, y, comp, (ശൂന്യം *) data);
      stbi__end_write_file(&s);
      തിരികെ r;
   } അല്ലെങ്കിൽ
      തിരികെ 0;
}
#endif

// *************************************************************************************************
// Radiance RGBE HDR writer
// by Baldur Karlsson

#define stbiw__max(a, b)  ((a) > (b) ? (a) : (b))

ശൂന്യം stbiw__linear_to_rgbe(unsigned അക്ഷരം *rgbe, ദശാംശം *linear)
{
   പൂർണ്ണസംഖ്യ exponent;
   ദശാംശം maxcomp = stbiw__max(linear[0], stbiw__max(linear[1], linear[2]));

   എങ്കിൽ (maxcomp < 1e-32f) {
      rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
   } അല്ലെങ്കിൽ {
      ദശാംശം normalize = (ദശാംശം) frexp(maxcomp, &exponent) * 256.0f/maxcomp;

      rgbe[0] = (unsigned അക്ഷരം)(linear[0] * normalize);
      rgbe[1] = (unsigned അക്ഷരം)(linear[1] * normalize);
      rgbe[2] = (unsigned അക്ഷരം)(linear[2] * normalize);
      rgbe[3] = (unsigned അക്ഷരം)(exponent + 128);
   }
}

ശൂന്യം stbiw__write_run_data(stbi__write_context *s, പൂർണ്ണസംഖ്യ length, unsigned അക്ഷരം databyte)
{
   unsigned അക്ഷരം lengthbyte = STBIW_UCHAR(length+128);
   STBIW_ASSERT(length+128 <= 255);
   s->func(s->context, &lengthbyte, 1);
   s->func(s->context, &databyte, 1);
}

ശൂന്യം stbiw__write_dump_data(stbi__write_context *s, പൂർണ്ണസംഖ്യ length, unsigned അക്ഷരം *data)
{
   unsigned അക്ഷരം lengthbyte = STBIW_UCHAR(length);
   STBIW_ASSERT(length <= 128); // inconsistent with spec but consistent with official code
   s->func(s->context, &lengthbyte, 1);
   s->func(s->context, data, length);
}

ശൂന്യം stbiw__write_hdr_scanline(stbi__write_context *s, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ ncomp, unsigned അക്ഷരം *scratch, ദശാംശം *scanline)
{
   unsigned അക്ഷരം scanlineheader[4] = { 2, 2, 0, 0 };
   unsigned അക്ഷരം rgbe[4];
   ദശാംശം linear[3];
   പൂർണ്ണസംഖ്യ x;

   scanlineheader[2] = (width&0xff00)>>8;
   scanlineheader[3] = (width&0x00ff);

   /* skip RLE for images too small or large */
   എങ്കിൽ (width < 8 || width >= 32768) {
      വേണ്ടി (x=0; x < width; x++) {
         തിരഞ്ഞെടുപ്പ് (ncomp) {
            അവസ്ഥ 4: /* fallthrough */
            അവസ്ഥ 3: linear[2] = scanline[x*ncomp + 2];
                    linear[1] = scanline[x*ncomp + 1];
                    linear[0] = scanline[x*ncomp + 0];
                    നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                    linear[0] = linear[1] = linear[2] = scanline[x*ncomp + 0];
                    നിർത്തുക;
         }
         stbiw__linear_to_rgbe(rgbe, linear);
         s->func(s->context, rgbe, 4);
      }
   } അല്ലെങ്കിൽ {
      പൂർണ്ണസംഖ്യ c,r;
      /* encode into scratch buffer */
      വേണ്ടി (x=0; x < width; x++) {
         തിരഞ്ഞെടുപ്പ്(ncomp) {
            അവസ്ഥ 4: /* fallthrough */
            അവസ്ഥ 3: linear[2] = scanline[x*ncomp + 2];
                    linear[1] = scanline[x*ncomp + 1];
                    linear[0] = scanline[x*ncomp + 0];
                    നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                    linear[0] = linear[1] = linear[2] = scanline[x*ncomp + 0];
                    നിർത്തുക;
         }
         stbiw__linear_to_rgbe(rgbe, linear);
         scratch[x + width*0] = rgbe[0];
         scratch[x + width*1] = rgbe[1];
         scratch[x + width*2] = rgbe[2];
         scratch[x + width*3] = rgbe[3];
      }

      s->func(s->context, scanlineheader, 4);

      /* RLE each component separately */
      വേണ്ടി (c=0; c < 4; c++) {
         unsigned അക്ഷരം *comp = &scratch[width*c];

         x = 0;
         ആകുമ്പോൾ (x < width) {
            // find first run
            r = x;
            ആകുമ്പോൾ (r+2 < width) {
               എങ്കിൽ (comp[r] == comp[r+1] && comp[r] == comp[r+2])
                  നിർത്തുക;
               ++r;
            }
            എങ്കിൽ (r+2 >= width)
               r = width;
            // dump up to first run
            ആകുമ്പോൾ (x < r) {
               പൂർണ്ണസംഖ്യ len = r-x;
               എങ്കിൽ (len > 128) len = 128;
               stbiw__write_dump_data(s, len, &comp[x]);
               x += len;
            }
            // if there's a run, output it
            എങ്കിൽ (r+2 < width) { // same test as what we break out of in search loop, so only true if we break'd
               // find next byte after run
               ആകുമ്പോൾ (r < width && comp[r] == comp[x])
                  ++r;
               // output run up to r
               ആകുമ്പോൾ (x < r) {
                  പൂർണ്ണസംഖ്യ len = r-x;
                  എങ്കിൽ (len > 127) len = 127;
                  stbiw__write_run_data(s, len, comp[x]);
                  x += len;
               }
            }
         }
      }
   }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_hdr_core(stbi__write_context *s, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, ദശാംശം *data)
{
   എങ്കിൽ (y <= 0 || x <= 0 || data == ശൂന്യ)
      തിരികെ 0;
   അല്ലെങ്കിൽ {
      // Each component is stored separately. Allocate scratch space for full output scanline.
      unsigned അക്ഷരം *scratch = (unsigned അക്ഷരം *) STBIW_MALLOC(x*4);
      പൂർണ്ണസംഖ്യ i, len;
      അക്ഷരം buffer[128];
      അക്ഷരം header[] = "#?RADIANCE\n# Written by stb_image_write.h\nFORMAT=32-bit_rle_rgbe\n";
      s->func(s->context, header, വലുപ്പം(header)-1);

#ifdef STBI_MSC_SECURE_CRT
      len = sprintf_s(buffer, "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#അല്ലെങ്കിൽ
      len = sprintf(buffer, "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#endif
      s->func(s->context, buffer, len);

      വേണ്ടി(i=0; i < y; i++)
         stbiw__write_hdr_scanline(s, x, comp, scratch, data + comp*x*(stbi__flip_vertically_on_write ? y-1-i : i)*x);
      STBIW_FREE(scratch);
      തിരികെ 1;
   }
}

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_hdr_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ദശാംശം *data)
{
   stbi__write_context s;
   stbi__start_write_callbacks(&s, func, context);
   തിരികെ stbi_write_hdr_core(&s, x, y, comp, (ദശാംശം *) data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_hdr(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ദശാംശം *data)
{
   stbi__write_context s;
   എങ്കിൽ (stbi__start_write_file(&s,filename)) {
      പൂർണ്ണസംഖ്യ r = stbi_write_hdr_core(&s, x, y, comp, (ദശാംശം *) data);
      stbi__end_write_file(&s);
      തിരികെ r;
   } അല്ലെങ്കിൽ
      തിരികെ 0;
}
#endif // STBI_WRITE_NO_STDIO


//////////////////////////////////////////////////////////////////////////////
//
// PNG writer
//

#ifndef STBIW_ZLIB_COMPRESS
// stretchy buffer; stbiw__sbpush() == vector<>::push_back() -- stbiw__sbcount() == vector<>::size()
#define stbiw__sbraw(a) ((പൂർണ്ണസംഖ്യ *) (a) - 2)
#define stbiw__sbm(a)   stbiw__sbraw(a)[0]
#define stbiw__sbn(a)   stbiw__sbraw(a)[1]

#define stbiw__sbneedgrow(a,n)  ((a)==0 || stbiw__sbn(a)+n >= stbiw__sbm(a))
#define stbiw__sbmaybegrow(a,n) (stbiw__sbneedgrow(a,(n)) ? stbiw__sbgrow(a,n) : 0)
#define stbiw__sbgrow(a,n)  stbiw__sbgrowf((ശൂന്യം **) &(a), (n), വലുപ്പം(*(a)))

#define stbiw__sbpush(a, v)      (stbiw__sbmaybegrow(a,1), (a)[stbiw__sbn(a)++] = (v))
#define stbiw__sbcount(a)        ((a) ? stbiw__sbn(a) : 0)
#define stbiw__sbfree(a)         ((a) ? STBIW_FREE(stbiw__sbraw(a)),0 : 0)

സ്ഥിതം ശൂന്യം *stbiw__sbgrowf(ശൂന്യം **arr, പൂർണ്ണസംഖ്യ increment, പൂർണ്ണസംഖ്യ itemsize)
{
   പൂർണ്ണസംഖ്യ m = *arr ? 2*stbiw__sbm(*arr)+increment : increment+1;
   ശൂന്യം *p = STBIW_REALLOC_SIZED(*arr ? stbiw__sbraw(*arr) : 0, *arr ? (stbiw__sbm(*arr)*itemsize + വലുപ്പം(പൂർണ്ണസംഖ്യ)*2) : 0, itemsize * m + വലുപ്പം(പൂർണ്ണസംഖ്യ)*2);
   STBIW_ASSERT(p);
   എങ്കിൽ (p) {
      എങ്കിൽ (!*arr) ((പൂർണ്ണസംഖ്യ *) p)[1] = 0;
      *arr = (ശൂന്യം *) ((പൂർണ്ണസംഖ്യ *) p + 2);
      stbiw__sbm(*arr) = m;
   }
   തിരികെ *arr;
}

സ്ഥിതം unsigned അക്ഷരം *stbiw__zlib_flushf(unsigned അക്ഷരം *data, unsigned പൂർണ്ണസംഖ്യ *bitbuffer, പൂർണ്ണസംഖ്യ *bitcount)
{
   ആകുമ്പോൾ (*bitcount >= 8) {
      stbiw__sbpush(data, STBIW_UCHAR(*bitbuffer));
      *bitbuffer >>= 8;
      *bitcount -= 8;
   }
   തിരികെ data;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbiw__zlib_bitrev(പൂർണ്ണസംഖ്യ code, പൂർണ്ണസംഖ്യ codebits)
{
   പൂർണ്ണസംഖ്യ res=0;
   ആകുമ്പോൾ (codebits--) {
      res = (res << 1) | (code & 1);
      code >>= 1;
   }
   തിരികെ res;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ stbiw__zlib_countm(unsigned അക്ഷരം *a, unsigned അക്ഷരം *b, പൂർണ്ണസംഖ്യ limit)
{
   പൂർണ്ണസംഖ്യ i;
   വേണ്ടി (i=0; i < limit && i < 258; ++i)
      എങ്കിൽ (a[i] != b[i]) നിർത്തുക;
   തിരികെ i;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ stbiw__zhash(unsigned അക്ഷരം *data)
{
   stbiw_uint32 hash = data[0] + (data[1] << 8) + (data[2] << 16);
   hash ^= hash << 3;
   hash += hash >> 5;
   hash ^= hash << 4;
   hash += hash >> 17;
   hash ^= hash << 25;
   hash += hash >> 6;
   തിരികെ hash;
}

#define stbiw__zlib_flush() (out = stbiw__zlib_flushf(out, &bitbuf, &bitcount))
#define stbiw__zlib_add(code,codebits) \
      (bitbuf |= (code) << bitcount, bitcount += (codebits), stbiw__zlib_flush())
#define stbiw__zlib_huffa(b,c)  stbiw__zlib_add(stbiw__zlib_bitrev(b,c),c)
// default huffman tables
#define stbiw__zlib_huff1(n)  stbiw__zlib_huffa(0x30 + (n), 8)
#define stbiw__zlib_huff2(n)  stbiw__zlib_huffa(0x190 + (n)-144, 9)
#define stbiw__zlib_huff3(n)  stbiw__zlib_huffa(0 + (n)-256,7)
#define stbiw__zlib_huff4(n)  stbiw__zlib_huffa(0xc0 + (n)-280,8)
#define stbiw__zlib_huff(n)  ((n) <= 143 ? stbiw__zlib_huff1(n) : (n) <= 255 ? stbiw__zlib_huff2(n) : (n) <= 279 ? stbiw__zlib_huff3(n) : stbiw__zlib_huff4(n))
#define stbiw__zlib_huffb(n) ((n) <= 143 ? stbiw__zlib_huff1(n) : stbiw__zlib_huff2(n))

#define stbiw__ZHASH   16384

#endif // STBIW_ZLIB_COMPRESS

unsigned അക്ഷരം * stbi_zlib_compress(unsigned അക്ഷരം *data, പൂർണ്ണസംഖ്യ data_len, പൂർണ്ണസംഖ്യ *out_len, പൂർണ്ണസംഖ്യ quality)
{
#ifdef STBIW_ZLIB_COMPRESS
   // user provided a zlib compress implementation, use that
   തിരികെ STBIW_ZLIB_COMPRESS(data, data_len, out_len, quality);
#അല്ലെങ്കിൽ // use builtin
   സ്ഥിതം unsigned short lengthc[] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258, 259 };
   സ്ഥിതം unsigned അക്ഷരം  lengtheb[]= { 0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0 };
   സ്ഥിതം unsigned short distc[]   = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577, 32768 };
   സ്ഥിതം unsigned അക്ഷരം  disteb[]  = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
   unsigned പൂർണ്ണസംഖ്യ bitbuf=0;
   പൂർണ്ണസംഖ്യ i,j, bitcount=0;
   unsigned അക്ഷരം *out = ശൂന്യ;
   unsigned അക്ഷരം ***hash_table = (unsigned അക്ഷരം***) STBIW_MALLOC(stbiw__ZHASH * വലുപ്പം(അക്ഷരം**));
   എങ്കിൽ (hash_table == ശൂന്യ)
      തിരികെ ശൂന്യ;
   എങ്കിൽ (quality < 5) quality = 5;

   stbiw__sbpush(out, 0x78);   // DEFLATE 32K window
   stbiw__sbpush(out, 0x5e);   // FLEVEL = 1
   stbiw__zlib_add(1,1);  // BFINAL = 1
   stbiw__zlib_add(1,2);  // BTYPE = 1 -- fixed huffman

   വേണ്ടി (i=0; i < stbiw__ZHASH; ++i)
      hash_table[i] = ശൂന്യ;

   i=0;
   ആകുമ്പോൾ (i < data_len-3) {
      // hash next 3 bytes of data to be compressed
      പൂർണ്ണസംഖ്യ h = stbiw__zhash(data+i)&(stbiw__ZHASH-1), best=3;
      unsigned അക്ഷരം *bestloc = 0;
      unsigned അക്ഷരം **hlist = hash_table[h];
      പൂർണ്ണസംഖ്യ n = stbiw__sbcount(hlist);
      വേണ്ടി (j=0; j < n; ++j) {
         എങ്കിൽ (hlist[j]-data > i-32768) { // if entry lies within window
            പൂർണ്ണസംഖ്യ d = stbiw__zlib_countm(hlist[j], data+i, data_len-i);
            എങ്കിൽ (d >= best) best=d,bestloc=hlist[j];
         }
      }
      // when hash table entry is too long, delete half the entries
      എങ്കിൽ (hash_table[h] && stbiw__sbn(hash_table[h]) == 2*quality) {
         STBIW_MEMMOVE(hash_table[h], hash_table[h]+quality, വലുപ്പം(hash_table[h][0])*quality);
         stbiw__sbn(hash_table[h]) = quality;
      }
      stbiw__sbpush(hash_table[h],data+i);

      എങ്കിൽ (bestloc) {
         // "lazy matching" - check match at *next* byte, and if it's better, do cur byte as literal
         h = stbiw__zhash(data+i+1)&(stbiw__ZHASH-1);
         hlist = hash_table[h];
         n = stbiw__sbcount(hlist);
         വേണ്ടി (j=0; j < n; ++j) {
            എങ്കിൽ (hlist[j]-data > i-32767) {
               പൂർണ്ണസംഖ്യ e = stbiw__zlib_countm(hlist[j], data+i+1, data_len-i-1);
               എങ്കിൽ (e > best) { // if next match is better, bail on current match
                  bestloc = ശൂന്യ;
                  നിർത്തുക;
               }
            }
         }
      }

      എങ്കിൽ (bestloc) {
         പൂർണ്ണസംഖ്യ d = (പൂർണ്ണസംഖ്യ) (data+i - bestloc); // distance back
         STBIW_ASSERT(d <= 32767 && best <= 258);
         വേണ്ടി (j=0; best > lengthc[j+1]-1; ++j);
         stbiw__zlib_huff(j+257);
         എങ്കിൽ (lengtheb[j]) stbiw__zlib_add(best - lengthc[j], lengtheb[j]);
         വേണ്ടി (j=0; d > distc[j+1]-1; ++j);
         stbiw__zlib_add(stbiw__zlib_bitrev(j,5),5);
         എങ്കിൽ (disteb[j]) stbiw__zlib_add(d - distc[j], disteb[j]);
         i += best;
      } അല്ലെങ്കിൽ {
         stbiw__zlib_huffb(data[i]);
         ++i;
      }
   }
   // write out final bytes
   വേണ്ടി (;i < data_len; ++i)
      stbiw__zlib_huffb(data[i]);
   stbiw__zlib_huff(256); // end of block
   // pad with 0 bits to byte boundary
   ആകുമ്പോൾ (bitcount)
      stbiw__zlib_add(0,1);

   വേണ്ടി (i=0; i < stbiw__ZHASH; ++i)
      (ശൂന്യം) stbiw__sbfree(hash_table[i]);
   STBIW_FREE(hash_table);

   {
      // compute adler32 on input
      unsigned പൂർണ്ണസംഖ്യ s1=1, s2=0;
      പൂർണ്ണസംഖ്യ blocklen = (പൂർണ്ണസംഖ്യ) (data_len % 5552);
      j=0;
      ആകുമ്പോൾ (j < data_len) {
         വേണ്ടി (i=0; i < blocklen; ++i) s1 += data[j+i], s2 += s1;
         s1 %= 65521, s2 %= 65521;
         j += blocklen;
         blocklen = 5552;
      }
      stbiw__sbpush(out, STBIW_UCHAR(s2 >> 8));
      stbiw__sbpush(out, STBIW_UCHAR(s2));
      stbiw__sbpush(out, STBIW_UCHAR(s1 >> 8));
      stbiw__sbpush(out, STBIW_UCHAR(s1));
   }
   *out_len = stbiw__sbn(out);
   // make returned pointer freeable
   STBIW_MEMMOVE(stbiw__sbraw(out), out, *out_len);
   തിരികെ (unsigned അക്ഷരം *) stbiw__sbraw(out);
#endif // STBIW_ZLIB_COMPRESS
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ stbiw__crc32(unsigned അക്ഷരം *buffer, പൂർണ്ണസംഖ്യ len)
{
   സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ crc_table[256] =
   {
      0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
      0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
      0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
      0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
      0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
      0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
      0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
      0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
      0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
      0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
      0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
      0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
      0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
      0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
      0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
      0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
      0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
      0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
      0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
      0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
      0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
      0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
      0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
      0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
      0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
      0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
      0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
      0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
      0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
      0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
      0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
      0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
   };

   unsigned പൂർണ്ണസംഖ്യ crc = ~0u;
   പൂർണ്ണസംഖ്യ i;
   വേണ്ടി (i=0; i < len; ++i)
      crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
   തിരികെ ~crc;
}

#define stbiw__wpng4(o,a,b,c,d) ((o)[0]=STBIW_UCHAR(a),(o)[1]=STBIW_UCHAR(b),(o)[2]=STBIW_UCHAR(c),(o)[3]=STBIW_UCHAR(d),(o)+=4)
#define stbiw__wp32(data,v) stbiw__wpng4(data, (v)>>24,(v)>>16,(v)>>8,(v));
#define stbiw__wptag(data,s) stbiw__wpng4(data, s[0],s[1],s[2],s[3])

സ്ഥിതം ശൂന്യം stbiw__wpcrc(unsigned അക്ഷരം **data, പൂർണ്ണസംഖ്യ len)
{
   unsigned പൂർണ്ണസംഖ്യ crc = stbiw__crc32(*data - len - 4, len+4);
   stbiw__wp32(*data, crc);
}

സ്ഥിതം unsigned അക്ഷരം stbiw__paeth(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b, പൂർണ്ണസംഖ്യ c)
{
   പൂർണ്ണസംഖ്യ p = a + b - c, pa = abs(p-a), pb = abs(p-b), pc = abs(p-c);
   എങ്കിൽ (pa <= pb && pa <= pc) തിരികെ STBIW_UCHAR(a);
   എങ്കിൽ (pb <= pc) തിരികെ STBIW_UCHAR(b);
   തിരികെ STBIW_UCHAR(c);
}

// @OPTIMIZE: provide an option that always forces left-predict or paeth predict
സ്ഥിതം ശൂന്യം stbiw_encode_png_വരി(unsigned അക്ഷരം *pixels, പൂർണ്ണസംഖ്യ stride_bytes, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ filter_type, signed അക്ഷരം *വരി_buffer)
{
   സ്ഥിതം പൂർണ്ണസംഖ്യ mapping[] = { 0,1,2,3,4 };
   സ്ഥിതം പൂർണ്ണസംഖ്യ firstmap[] = { 0,1,0,5,6 };
   പൂർണ്ണസംഖ്യ *mymap = (y != 0) ? mapping : firstmap;
   പൂർണ്ണസംഖ്യ i;
   പൂർണ്ണസംഖ്യ type = mymap[filter_type];
   unsigned അക്ഷരം *z = pixels + stride_bytes * (stbi__flip_vertically_on_write ? height-1-y : y);
   പൂർണ്ണസംഖ്യ signed_stride = stbi__flip_vertically_on_write ? -stride_bytes : stride_bytes;
   വേണ്ടി (i = 0; i < n; ++i) {
      തിരഞ്ഞെടുപ്പ് (type) {
         അവസ്ഥ 0: വരി_buffer[i] = z[i]; നിർത്തുക;
         അവസ്ഥ 1: വരി_buffer[i] = z[i]; നിർത്തുക;
         അവസ്ഥ 2: വരി_buffer[i] = z[i] - z[i-signed_stride]; നിർത്തുക;
         അവസ്ഥ 3: വരി_buffer[i] = z[i] - (z[i-signed_stride]>>1); നിർത്തുക;
         അവസ്ഥ 4: വരി_buffer[i] = (signed അക്ഷരം) (z[i] - stbiw__paeth(0,z[i-signed_stride],0)); നിർത്തുക;
         അവസ്ഥ 5: വരി_buffer[i] = z[i]; നിർത്തുക;
         അവസ്ഥ 6: വരി_buffer[i] = z[i]; നിർത്തുക;
      }
   }
   വേണ്ടി (i=n; i < width*n; ++i) {
      തിരഞ്ഞെടുപ്പ് (type) {
         അവസ്ഥ 0: വരി_buffer[i] = z[i]; നിർത്തുക;
         അവസ്ഥ 1: വരി_buffer[i] = z[i] - z[i-n]; നിർത്തുക;
         അവസ്ഥ 2: വരി_buffer[i] = z[i] - z[i-signed_stride]; നിർത്തുക;
         അവസ്ഥ 3: വരി_buffer[i] = z[i] - ((z[i-n] + z[i-signed_stride])>>1); നിർത്തുക;
         അവസ്ഥ 4: വരി_buffer[i] = z[i] - stbiw__paeth(z[i-n], z[i-signed_stride], z[i-signed_stride-n]); നിർത്തുക;
         അവസ്ഥ 5: വരി_buffer[i] = z[i] - (z[i-n]>>1); നിർത്തുക;
         അവസ്ഥ 6: വരി_buffer[i] = z[i] - stbiw__paeth(z[i-n], 0,0); നിർത്തുക;
      }
   }
}

unsigned അക്ഷരം *stbi_write_png_to_mem(unsigned അക്ഷരം *pixels, പൂർണ്ണസംഖ്യ stride_bytes, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ *out_len)
{
   പൂർണ്ണസംഖ്യ force_filter = stbi_write_force_png_filter;
   പൂർണ്ണസംഖ്യ ctype[5] = { -1, 0, 4, 2, 6 };
   unsigned അക്ഷരം sig[8] = { 137,80,78,71,13,10,26,10 };
   unsigned അക്ഷരം *out,*o, *filt, *zlib;
   signed അക്ഷരം *വരി_buffer;
   പൂർണ്ണസംഖ്യ j,zlen;

   എങ്കിൽ (stride_bytes == 0)
      stride_bytes = x * n;

   എങ്കിൽ (force_filter >= 5) {
      force_filter = -1;
   }

   filt = (unsigned അക്ഷരം *) STBIW_MALLOC((x*n+1) * y); എങ്കിൽ (!filt) തിരികെ 0;
   വരി_buffer = (signed അക്ഷരം *) STBIW_MALLOC(x * n); എങ്കിൽ (!വരി_buffer) { STBIW_FREE(filt); തിരികെ 0; }
   വേണ്ടി (j=0; j < y; ++j) {
      പൂർണ്ണസംഖ്യ filter_type;
      എങ്കിൽ (force_filter > -1) {
         filter_type = force_filter;
         stbiw_encode_png_വരി(pixels, stride_bytes, x, y, j, n, force_filter, വരി_buffer);
      } അല്ലെങ്കിൽ { // Estimate the best filter by running through all of them:
         പൂർണ്ണസംഖ്യ best_filter = 0, best_filter_val = 0x7fffffff, est, i;
         വേണ്ടി (filter_type = 0; filter_type < 5; filter_type++) {
            stbiw_encode_png_വരി(pixels, stride_bytes, x, y, j, n, filter_type, വരി_buffer);

            // Estimate the entropy of the line using this filter; the less, the better.
            est = 0;
            വേണ്ടി (i = 0; i < x*n; ++i) {
               est += abs((signed അക്ഷരം) വരി_buffer[i]);
            }
            എങ്കിൽ (est < best_filter_val) {
               best_filter_val = est;
               best_filter = filter_type;
            }
         }
         എങ്കിൽ (filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
            stbiw_encode_png_വരി(pixels, stride_bytes, x, y, j, n, best_filter, വരി_buffer);
            filter_type = best_filter;
         }
      }
      // when we get here, filter_type contains the filter type, and line_buffer contains the data
      filt[j*(x*n+1)] = (unsigned അക്ഷരം) filter_type;
      STBIW_MEMMOVE(filt+j*(x*n+1)+1, വരി_buffer, x*n);
   }
   STBIW_FREE(വരി_buffer);
   zlib = stbi_zlib_compress(filt, y*( x*n+1), &zlen, stbi_write_png_compression_level);
   STBIW_FREE(filt);
   എങ്കിൽ (!zlib) തിരികെ 0;

   // each tag requires 12 bytes of overhead
   out = (unsigned അക്ഷരം *) STBIW_MALLOC(8 + 12+13 + 12+zlen + 12);
   എങ്കിൽ (!out) തിരികെ 0;
   *out_len = 8 + 12+13 + 12+zlen + 12;

   o=out;
   STBIW_MEMMOVE(o,sig,8); o+= 8;
   stbiw__wp32(o, 13); // header length
   stbiw__wptag(o, "IHDR");
   stbiw__wp32(o, x);
   stbiw__wp32(o, y);
   *o++ = 8;
   *o++ = STBIW_UCHAR(ctype[n]);
   *o++ = 0;
   *o++ = 0;
   *o++ = 0;
   stbiw__wpcrc(&o,13);

   stbiw__wp32(o, zlen);
   stbiw__wptag(o, "IDAT");
   STBIW_MEMMOVE(o, zlib, zlen);
   o += zlen;
   STBIW_FREE(zlib);
   stbiw__wpcrc(&o, zlen);

   stbiw__wp32(o,0);
   stbiw__wptag(o, "IEND");
   stbiw__wpcrc(&o,0);

   STBIW_ASSERT(o == out + *out_len);

   തിരികെ out;
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_png(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ stride_bytes)
{
   FILE *f;
   പൂർണ്ണസംഖ്യ len;
   unsigned അക്ഷരം *png = stbi_write_png_to_mem((unsigned അക്ഷരം *) data, stride_bytes, x, y, comp, &len);
   എങ്കിൽ (png == ശൂന്യ) തിരികെ 0;
#ifdef STBI_MSC_SECURE_CRT
   എങ്കിൽ (fopen_s(&f, filename, "wb"))
      f = ശൂന്യ;
#അല്ലെങ്കിൽ
   f = fopen(filename, "wb");
#endif
   എങ്കിൽ (!f) { STBIW_FREE(png); തിരികെ 0; }
   fwrite(png, 1, len, f);
   fclose(f);
   STBIW_FREE(png);
   തിരികെ 1;
}
#endif

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_png_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ stride_bytes)
{
   പൂർണ്ണസംഖ്യ len;
   unsigned അക്ഷരം *png = stbi_write_png_to_mem((unsigned അക്ഷരം *) data, stride_bytes, x, y, comp, &len);
   എങ്കിൽ (png == ശൂന്യ) തിരികെ 0;
   func(context, png, len);
   STBIW_FREE(png);
   തിരികെ 1;
}


/* ***************************************************************************
 *
 * JPEG writer
 *
 * This is based on Jon Olick's jo_jpeg.cpp:
 * public domain Simple, Minimalistic JPEG writer - http://www.jonolick.com/code.html
 */

സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം stbiw__jpg_ZigZag[] = { 0,1,5,6,14,15,27,28,2,4,7,13,16,26,29,42,3,8,12,17,25,30,41,43,9,11,18,
      24,31,40,44,53,10,19,23,32,39,45,52,54,20,22,33,38,46,51,55,60,21,34,37,47,50,56,59,61,35,36,48,49,57,58,62,63 };

സ്ഥിതം ശൂന്യം stbiw__jpg_writeBits(stbi__write_context *s, പൂർണ്ണസംഖ്യ *bitBufP, പൂർണ്ണസംഖ്യ *bitCntP, സ്ഥിരം unsigned short *bs) {
   പൂർണ്ണസംഖ്യ bitBuf = *bitBufP, bitCnt = *bitCntP;
   bitCnt += bs[1];
   bitBuf |= bs[0] << (24 - bitCnt);
   ആകുമ്പോൾ(bitCnt >= 8) {
      unsigned അക്ഷരം c = (bitBuf >> 16) & 255;
      stbiw__putc(s, c);
      എങ്കിൽ(c == 255) {
         stbiw__putc(s, 0);
      }
      bitBuf <<= 8;
      bitCnt -= 8;
   }
   *bitBufP = bitBuf;
   *bitCntP = bitCnt;
}

സ്ഥിതം ശൂന്യം stbiw__jpg_DCT(ദശാംശം *d0p, ദശാംശം *d1p, ദശാംശം *d2p, ദശാംശം *d3p, ദശാംശം *d4p, ദശാംശം *d5p, ദശാംശം *d6p, ദശാംശം *d7p) {
   ദശാംശം d0 = *d0p, d1 = *d1p, d2 = *d2p, d3 = *d3p, d4 = *d4p, d5 = *d5p, d6 = *d6p, d7 = *d7p;
   ദശാംശം z1, z2, z3, z4, z5, z11, z13;

   ദശാംശം tmp0 = d0 + d7;
   ദശാംശം tmp7 = d0 - d7;
   ദശാംശം tmp1 = d1 + d6;
   ദശാംശം tmp6 = d1 - d6;
   ദശാംശം tmp2 = d2 + d5;
   ദശാംശം tmp5 = d2 - d5;
   ദശാംശം tmp3 = d3 + d4;
   ദശാംശം tmp4 = d3 - d4;

   // Even part
   ദശാംശം tmp10 = tmp0 + tmp3;   // phase 2
   ദശാംശം tmp13 = tmp0 - tmp3;
   ദശാംശം tmp11 = tmp1 + tmp2;
   ദശാംശം tmp12 = tmp1 - tmp2;

   d0 = tmp10 + tmp11;       // phase 3
   d4 = tmp10 - tmp11;

   z1 = (tmp12 + tmp13) * 0.707106781f; // c4
   d2 = tmp13 + z1;       // phase 5
   d6 = tmp13 - z1;

   // Odd part
   tmp10 = tmp4 + tmp5;       // phase 2
   tmp11 = tmp5 + tmp6;
   tmp12 = tmp6 + tmp7;

   // The rotator is modified from fig 4-8 to avoid extra negations.
   z5 = (tmp10 - tmp12) * 0.382683433f; // c6
   z2 = tmp10 * 0.541196100f + z5; // c2-c6
   z4 = tmp12 * 1.306562965f + z5; // c2+c6
   z3 = tmp11 * 0.707106781f; // c4

   z11 = tmp7 + z3;      // phase 5
   z13 = tmp7 - z3;

   *d5p = z13 + z2;         // phase 6
   *d3p = z13 - z2;
   *d1p = z11 + z4;
   *d7p = z11 - z4;

   *d0p = d0;  *d2p = d2;  *d4p = d4;  *d6p = d6;
}

സ്ഥിതം ശൂന്യം stbiw__jpg_calcBits(പൂർണ്ണസംഖ്യ val, unsigned short bits[2]) {
   പൂർണ്ണസംഖ്യ tmp1 = val < 0 ? -val : val;
   val = val < 0 ? val-1 : val;
   bits[1] = 1;
   ആകുമ്പോൾ(tmp1 >>= 1) {
      ++bits[1];
   }
   bits[0] = val & ((1<<bits[1])-1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbiw__jpg_processDU(stbi__write_context *s, പൂർണ്ണസംഖ്യ *bitBuf, പൂർണ്ണസംഖ്യ *bitCnt, ദശാംശം *CDU, ദശാംശം *fdtbl, പൂർണ്ണസംഖ്യ DC, സ്ഥിരം unsigned short HTDC[256][2], സ്ഥിരം unsigned short HTAC[256][2]) {
   സ്ഥിരം unsigned short EOB[2] = { HTAC[0x00][0], HTAC[0x00][1] };
   സ്ഥിരം unsigned short M16zeroes[2] = { HTAC[0xF0][0], HTAC[0xF0][1] };
   പൂർണ്ണസംഖ്യ dataOff, i, diff, end0pos;
   പൂർണ്ണസംഖ്യ DU[64];

   // DCT rows
   വേണ്ടി(dataOff=0; dataOff<64; dataOff+=8) {
      stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff+1], &CDU[dataOff+2], &CDU[dataOff+3], &CDU[dataOff+4], &CDU[dataOff+5], &CDU[dataOff+6], &CDU[dataOff+7]);
   }
   // DCT columns
   വേണ്ടി(dataOff=0; dataOff<8; ++dataOff) {
      stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff+8], &CDU[dataOff+16], &CDU[dataOff+24], &CDU[dataOff+32], &CDU[dataOff+40], &CDU[dataOff+48], &CDU[dataOff+56]);
   }
   // Quantize/descale/zigzag the coefficients
   വേണ്ടി(i=0; i<64; ++i) {
      ദശാംശം v = CDU[i]*fdtbl[i];
      // DU[stbiw__jpg_ZigZag[i]] = (int)(v < 0 ? ceilf(v - 0.5f) : floorf(v + 0.5f));
      // ceilf() and floorf() are C99, not C89, but I /think/ they're not needed here anyway?
      DU[stbiw__jpg_ZigZag[i]] = (പൂർണ്ണസംഖ്യ)(v < 0 ? v - 0.5f : v + 0.5f);
   }

   // Encode DC
   diff = DU[0] - DC;
   എങ്കിൽ (diff == 0) {
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[0]);
   } അല്ലെങ്കിൽ {
      unsigned short bits[2];
      stbiw__jpg_calcBits(diff, bits);
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[bits[1]]);
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
   }
   // Encode ACs
   end0pos = 63;
   വേണ്ടി(; (end0pos>0)&&(DU[end0pos]==0); --end0pos) {
   }
   // end0pos = first element in reverse order !=0
   എങ്കിൽ(end0pos == 0) {
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
      തിരികെ DU[0];
   }
   വേണ്ടി(i = 1; i <= end0pos; ++i) {
      പൂർണ്ണസംഖ്യ startpos = i;
      പൂർണ്ണസംഖ്യ nrzeroes;
      unsigned short bits[2];
      വേണ്ടി (; DU[i]==0 && i<=end0pos; ++i) {
      }
      nrzeroes = i-startpos;
      എങ്കിൽ ( nrzeroes >= 16 ) {
         പൂർണ്ണസംഖ്യ lng = nrzeroes>>4;
         പൂർണ്ണസംഖ്യ nrmarker;
         വേണ്ടി (nrmarker=1; nrmarker <= lng; ++nrmarker)
            stbiw__jpg_writeBits(s, bitBuf, bitCnt, M16zeroes);
         nrzeroes &= 15;
      }
      stbiw__jpg_calcBits(DU[i], bits);
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTAC[(nrzeroes<<4)+bits[1]]);
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
   }
   എങ്കിൽ(end0pos != 63) {
      stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
   }
   തിരികെ DU[0];
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stbi_write_jpg_core(stbi__write_context *s, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം* data, പൂർണ്ണസംഖ്യ quality) {
   // Constants that don't pollute global namespace
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_dc_luminance_nrcodes[] = {0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_dc_luminance_മൂല്യങ്ങൾ[] = {0,1,2,3,4,5,6,7,8,9,10,11};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_ac_luminance_nrcodes[] = {0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_ac_luminance_മൂല്യങ്ങൾ[] = {
      0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
      0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
      0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
      0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
      0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
      0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
      0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
   };
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_dc_chrominance_nrcodes[] = {0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_dc_chrominance_മൂല്യങ്ങൾ[] = {0,1,2,3,4,5,6,7,8,9,10,11};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_ac_chrominance_nrcodes[] = {0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
   സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം std_ac_chrominance_മൂല്യങ്ങൾ[] = {
      0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
      0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
      0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
      0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
      0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
      0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
      0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
   };
   // Huffman tables
   സ്ഥിതം സ്ഥിരം unsigned short YDC_HT[256][2] = { {0,2},{2,3},{3,3},{4,3},{5,3},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9}};
   സ്ഥിതം സ്ഥിരം unsigned short UVDC_HT[256][2] = { {0,2},{1,2},{2,2},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9},{1022,10},{2046,11}};
   സ്ഥിതം സ്ഥിരം unsigned short YAC_HT[256][2] = {
      {10,4},{0,2},{1,2},{4,3},{11,4},{26,5},{120,7},{248,8},{1014,10},{65410,16},{65411,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {12,4},{27,5},{121,7},{502,9},{2038,11},{65412,16},{65413,16},{65414,16},{65415,16},{65416,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {28,5},{249,8},{1015,10},{4084,12},{65417,16},{65418,16},{65419,16},{65420,16},{65421,16},{65422,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {58,6},{503,9},{4085,12},{65423,16},{65424,16},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {59,6},{1016,10},{65430,16},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {122,7},{2039,11},{65438,16},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {123,7},{4086,12},{65446,16},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {250,8},{4087,12},{65454,16},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {504,9},{32704,15},{65462,16},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {505,9},{65470,16},{65471,16},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {506,9},{65479,16},{65480,16},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {1017,10},{65488,16},{65489,16},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {1018,10},{65497,16},{65498,16},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {2040,11},{65506,16},{65507,16},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {65515,16},{65516,16},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{0,0},{0,0},{0,0},{0,0},{0,0},
      {2041,11},{65525,16},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
   };
   സ്ഥിതം സ്ഥിരം unsigned short UVAC_HT[256][2] = {
      {0,2},{1,2},{4,3},{10,4},{24,5},{25,5},{56,6},{120,7},{500,9},{1014,10},{4084,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {11,4},{57,6},{246,8},{501,9},{2038,11},{4085,12},{65416,16},{65417,16},{65418,16},{65419,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {26,5},{247,8},{1015,10},{4086,12},{32706,15},{65420,16},{65421,16},{65422,16},{65423,16},{65424,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {27,5},{248,8},{1016,10},{4087,12},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{65430,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {58,6},{502,9},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{65438,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {59,6},{1017,10},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{65446,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {121,7},{2039,11},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{65454,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {122,7},{2040,11},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{65462,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {249,8},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{65470,16},{65471,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {503,9},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{65479,16},{65480,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {504,9},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{65488,16},{65489,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {505,9},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{65497,16},{65498,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {506,9},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{65506,16},{65507,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {2041,11},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{65515,16},{65516,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
      {16352,14},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{65525,16},{0,0},{0,0},{0,0},{0,0},{0,0},
      {1018,10},{32707,15},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
   };
   സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ YQT[] = {16,11,10,16,24,40,51,61,12,12,14,19,26,58,60,55,14,13,16,24,40,57,69,56,14,17,22,29,51,87,80,62,18,22,
                             37,56,68,109,103,77,24,35,55,64,81,104,113,92,49,64,78,87,103,121,120,101,72,92,95,98,112,100,103,99};
   സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ UVQT[] = {17,18,24,47,99,99,99,99,18,21,26,66,99,99,99,99,24,26,56,99,99,99,99,99,47,66,99,99,99,99,99,99,
                              99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99};
   സ്ഥിതം സ്ഥിരം ദശാംശം aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f, 
                                 1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f };

   പൂർണ്ണസംഖ്യ row, col, i, k;
   ദശാംശം fdtbl_Y[64], fdtbl_UV[64];
   unsigned അക്ഷരം YTable[64], UVTable[64];

   എങ്കിൽ(!data || !width || !height || comp > 4 || comp < 1) {
      തിരികെ 0;
   }

   quality = quality ? quality : 90;
   quality = quality < 1 ? 1 : quality > 100 ? 100 : quality;
   quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

   വേണ്ടി(i = 0; i < 64; ++i) {
      പൂർണ്ണസംഖ്യ uvti, yti = (YQT[i]*quality+50)/100;
      YTable[stbiw__jpg_ZigZag[i]] = (unsigned അക്ഷരം) (yti < 1 ? 1 : yti > 255 ? 255 : yti);
      uvti = (UVQT[i]*quality+50)/100;
      UVTable[stbiw__jpg_ZigZag[i]] = (unsigned അക്ഷരം) (uvti < 1 ? 1 : uvti > 255 ? 255 : uvti);
   }

   വേണ്ടി(row = 0, k = 0; row < 8; ++row) {
      വേണ്ടി(col = 0; col < 8; ++col, ++k) {
         fdtbl_Y[k]  = 1 / (YTable [stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
         fdtbl_UV[k] = 1 / (UVTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
      }
   }

   // Write Headers
   {
      സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം head0[] = { 0xFF,0xD8,0xFF,0xE0,0,0x10,'J','F','I','F',0,1,1,0,0,1,0,1,0,0,0xFF,0xDB,0,0x84,0 };
      സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം head2[] = { 0xFF,0xDA,0,0xC,3,1,0,2,0x11,3,0x11,0,0x3F,0 };
      സ്ഥിരം unsigned അക്ഷരം head1[] = { 0xFF,0xC0,0,0x11,8,(unsigned അക്ഷരം)(height>>8),STBIW_UCHAR(height),(unsigned അക്ഷരം)(width>>8),STBIW_UCHAR(width),
                                      3,1,0x11,0,2,0x11,1,3,0x11,1,0xFF,0xC4,0x01,0xA2,0 };
      s->func(s->context, (ശൂന്യം*)head0, വലുപ്പം(head0));
      s->func(s->context, (ശൂന്യം*)YTable, വലുപ്പം(YTable));
      stbiw__putc(s, 1);
      s->func(s->context, UVTable, വലുപ്പം(UVTable));
      s->func(s->context, (ശൂന്യം*)head1, വലുപ്പം(head1));
      s->func(s->context, (ശൂന്യം*)(std_dc_luminance_nrcodes+1), വലുപ്പം(std_dc_luminance_nrcodes)-1);
      s->func(s->context, (ശൂന്യം*)std_dc_luminance_മൂല്യങ്ങൾ, വലുപ്പം(std_dc_luminance_മൂല്യങ്ങൾ));
      stbiw__putc(s, 0x10); // HTYACinfo
      s->func(s->context, (ശൂന്യം*)(std_ac_luminance_nrcodes+1), വലുപ്പം(std_ac_luminance_nrcodes)-1);
      s->func(s->context, (ശൂന്യം*)std_ac_luminance_മൂല്യങ്ങൾ, വലുപ്പം(std_ac_luminance_മൂല്യങ്ങൾ));
      stbiw__putc(s, 1); // HTUDCinfo
      s->func(s->context, (ശൂന്യം*)(std_dc_chrominance_nrcodes+1), വലുപ്പം(std_dc_chrominance_nrcodes)-1);
      s->func(s->context, (ശൂന്യം*)std_dc_chrominance_മൂല്യങ്ങൾ, വലുപ്പം(std_dc_chrominance_മൂല്യങ്ങൾ));
      stbiw__putc(s, 0x11); // HTUACinfo
      s->func(s->context, (ശൂന്യം*)(std_ac_chrominance_nrcodes+1), വലുപ്പം(std_ac_chrominance_nrcodes)-1);
      s->func(s->context, (ശൂന്യം*)std_ac_chrominance_മൂല്യങ്ങൾ, വലുപ്പം(std_ac_chrominance_മൂല്യങ്ങൾ));
      s->func(s->context, (ശൂന്യം*)head2, വലുപ്പം(head2));
   }

   // Encode 8x8 macroblocks
   {
      സ്ഥിതം സ്ഥിരം unsigned short fillBits[] = {0x7F, 7};
      സ്ഥിരം unsigned അക്ഷരം *imageData = (സ്ഥിരം unsigned അക്ഷരം *)data;
      പൂർണ്ണസംഖ്യ DCY=0, DCU=0, DCV=0;
      പൂർണ്ണസംഖ്യ bitBuf=0, bitCnt=0;
      // comp == 2 is grey+alpha (alpha is ignored)
      പൂർണ്ണസംഖ്യ ofsG = comp > 2 ? 1 : 0, ofsB = comp > 2 ? 2 : 0;
      പൂർണ്ണസംഖ്യ x, y, pos;
      വേണ്ടി(y = 0; y < height; y += 8) {
         വേണ്ടി(x = 0; x < width; x += 8) {
            ദശാംശം YDU[64], UDU[64], VDU[64];
            വേണ്ടി(row = y, pos = 0; row < y+8; ++row) {
               വേണ്ടി(col = x; col < x+8; ++col, ++pos) {
                  പൂർണ്ണസംഖ്യ p = (stbi__flip_vertically_on_write ? height-1-row : row)*width*comp + col*comp;
                  ദശാംശം r, g, b;
                  എങ്കിൽ(row >= height) {
                     p -= width*comp*(row+1 - height);
                  }
                  എങ്കിൽ(col >= width) {
                     p -= comp*(col+1 - width);
                  }

                  r = imageData[p+0];
                  g = imageData[p+ofsG];
                  b = imageData[p+ofsB];
                  YDU[pos]=+0.29900f*r+0.58700f*g+0.11400f*b-128;
                  UDU[pos]=-0.16874f*r-0.33126f*g+0.50000f*b;
                  VDU[pos]=+0.50000f*r-0.41869f*g-0.08131f*b;
               }
            }

            DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, YDU, fdtbl_Y, DCY, YDC_HT, YAC_HT);
            DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, UDU, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
            DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, VDU, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
         }
      }

      // Do the bit alignment of the EOI marker
      stbiw__jpg_writeBits(s, &bitBuf, &bitCnt, fillBits);
   }

   // EOI
   stbiw__putc(s, 0xFF);
   stbiw__putc(s, 0xD9);

   തിരികെ 1;
}

STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_jpg_to_func(stbi_write_func *func, ശൂന്യം *context, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ quality)
{
   stbi__write_context s;
   stbi__start_write_callbacks(&s, func, context);
   തിരികെ stbi_write_jpg_core(&s, x, y, comp, (ശൂന്യം *) data, quality);
}


#ifndef STBI_WRITE_NO_STDIO
STBIWDEF പൂർണ്ണസംഖ്യ stbi_write_jpg(അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ comp, സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ quality)
{
   stbi__write_context s;
   എങ്കിൽ (stbi__start_write_file(&s,filename)) {
      പൂർണ്ണസംഖ്യ r = stbi_write_jpg_core(&s, x, y, comp, data, quality);
      stbi__end_write_file(&s);
      തിരികെ r;
   } അല്ലെങ്കിൽ
      തിരികെ 0;
}
#endif

#endif // STB_IMAGE_WRITE_IMPLEMENTATION

/* Revision history
      1.09  (2018-02-11)
             fix typo in zlib quality API, improve STB_I_W_STATIC in C++
      1.08  (2018-01-29)
             add stbi__flip_vertically_on_write, external zlib, zlib quality, choose PNG filter
      1.07  (2017-07-24)
             doc fix
      1.06 (2017-07-23)
             writing JPEG (using Jon Olick's code)
      1.05   ???
      1.04 (2017-03-03)
             monochrome BMP expansion
      1.03   ???
      1.02 (2016-04-02)
             avoid allocating large structures on the stack
      1.01 (2016-01-16)
             STBIW_REALLOC_SIZED: support allocators with no realloc support
             avoid race-condition in crc initialization
             minor compile issues
      1.00 (2015-09-14)
             installable file IO function
      0.99 (2015-09-13)
             warning fixes; TGA rle support
      0.98 (2015-04-08)
             added STBIW_MALLOC, STBIW_ASSERT etc
      0.97 (2015-01-18)
             fixed HDR asserts, rewrote HDR rle logic
      0.96 (2015-01-17)
             add HDR output
             fix monochrome BMP
      0.95 (2014-08-17)
		       add monochrome TGA output
      0.94 (2014-05-31)
             rename private functions to avoid conflicts with stb_image.h
      0.93 (2014-05-27)
             warning fixes
      0.92 (2010-08-01)
             casts to unsigned char to fix warnings
      0.91 (2010-07-17)
             first public release
      0.90   first internal release
*/

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this 
software, either in source code form or as a compiled binary, for any purpose, 
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this 
software dedicate any and all copyright interest in the software to the public 
domain. We make this dedication for the benefit of the public at large and to 
the detriment of our heirs and successors. We intend this dedication to be an 
overt act of relinquishment in perpetuity of all present and future rights to 
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
