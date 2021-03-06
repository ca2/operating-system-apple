//
//  visual_apple_image.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 21/10/17. Thomas Boregaard Soerensen <3
//
#include "framework.h"
#include "aura/graphics/image/save_image.h"
#include <CoreGraphics/CoreGraphics.h>


CFDataRef CopyImagePixels(CGImageRef inImage);

void * file_memory_to_image_data(int & width, int & height, int & iScan, const void * pdata, unsigned long size_i32);

void * cg_image_get_image_data(int & width, int & height, int & iScan, CGImageRef image);

void * nscursor_system(enum_cursor ecursor);

CGImageRef cgimageref_from_image(const ::image * pimage);

//void ns_set_cursor_cgimageref(CGImageRef image, int cx, int cy, int xHotSpot, int yHotSpot);

//void * nscursor_from_cgimageref(CGImageRef image, int cx, int cy, int xHotSpot, int yHotSpot);

void * get_png_image_data(unsigned long & size, CGImageRef image);
void * get_jpeg_image_data(unsigned long & size, CGImageRef image);


namespace coreimage_imaging
{


   void context_image::save_image(memory & memory, const ::image * pimage, const ::save_image * psaveimage)
   {

      if(pimage->is_empty())
      {

         throw exception(error_invalid_parameter);

      }

      unsigned long size;

      CGImageRef cgimage = cgimageref_from_image(pimage);

      ::acme::malloc < color32_t * > p;

      switch (psaveimage == nullptr ? ::draw2d::e_format_png : psaveimage->m_eformat)
      {
      case ::draw2d::e_format_jpeg:
      {

         p = get_jpeg_image_data(size, cgimage);

      }
      break;
      default:
      {

         p = get_png_image_data(size, cgimage);

      }

      }

      CGImageRelease(cgimage);

      memory.assign(p, size);

      //return true;

   }


   void context_image::_load_image( ::image * pimage, const ::payload & varFile, const ::image::load_options & options)
   {
      
      if(::is_null(pimage))
      {
         
         throw exception(error_invalid_parameter);
         
      }
      
      memory memory;
      
      auto pcontext = m_pcontext->m_pauracontext;
      
      bool bOk = pcontext->m_papexcontext->file().as_memory(varFile, memory);
      
      if(!bOk)
      {
         
         pimage->m_estatus = error_not_found;
         
         pimage->set_nok();
         
         return;
         
      }

      if(memory.is_empty())
      {
         
         pimage->m_estatus = error_invalid_empty_argument;
         
         pimage->set_nok();
         
         return;
         
      }

      auto pcontextimage = pcontext->context_image();

      auto pszData = memory.get_data();

      auto size = memory.get_size();

      char pszPngSignature []= {(char)137, 80, 78 ,71, 13 ,10, 26 ,10};

      bool bPng = size > sizeof(pszPngSignature)
      && strncmp((const char *) pszData, pszPngSignature, sizeof(pszPngSignature)) == 0;

      bool bJpegBegins = memory.begins("\x0FF\x0D8", 2);

      bool bJpegEnds = memory.ends("\x0FF\x0D9", 2);

      bool bGif87a = memory.begins("GIF87a", 6);

      bool bGif89a = memory.begins("GIF89a", 6);

      bool bJpeg =  bJpegBegins && bJpegEnds;

      bool bJfif = memory.begins("JFIF", 4);

      bool bExif = memory.begins("Exif", 4);

      bool bGif = bGif87a || bGif89a;

      bool bBinary = pszData && *pszData == '\0';

      if(!bPng
      && !bBinary
      && !bJpeg
      && !bJfif
      && !bExif
      && !bGif
      )
      {

         //estatus =
         
         pcontextimage->load_svg(pimage, memory);

         if (::is_ok(pimage))
         {

            return;

         }

      }
      else if (bGif)
      {

      //m_psystem->m_pacmefile->put_contents("/home/camilo/a.gif", memory);

         _load_multi_frame_image(pimage, memory);

         //if (!)
 /*        {
            pimage->set_nok();
            return pimage->m_estatus;
         }*/

         pimage->on_load_image();

         pimage->set_ok();

         pimage->m_estatus = ::success;

         return;

         // return pimage->m_estatus;

      }
      
      int w = 0;

      int h = 0;

      int iScan = 0;

      ::acme::malloc < color32_t * > pcolorref;

      pcolorref = file_memory_to_image_data(w, h, iScan, memory.get_data(), memory.get_size());
      
      if(pcolorref == nullptr)
      {
         
         pimage->set_nok();

         pimage->m_estatus = ::error_failed;

         return;

      }
      
      pimage->create({w, h});
      
      pimage->map();
      
      vertical_swap_copy_colorref(pimage->colorref(), w, h, pimage->scan_size(), pcolorref, iScan);
      
      pimage->set_ok();
      
      //return true;

   }


} // namespace coreimage_imaging


//void * cg_image_get_image_data(int & width, int & height, int & iScan, CGImageRef image)
//{
//
//   if(image == nullptr)
//   {
//
//      return nullptr;
//
//   }
//
//   //https://www.raywenderlich.com/69855/image-processing-in-ios-part-1-raw-bitmap-modification
//   // 1.
//   width = (int) CGImageGetWidth(image);
//   height = (int) CGImageGetHeight(image);
//
//   // 2.
//   long bytesPerPixel = 4;
//   long bytesPerRow = bytesPerPixel * width;
//   long bitsPerComponent = 8;
//
//   ::u32 * pixels;
//   pixels = (::u32 *) calloc(height * width, sizeof(::u32));
//   
//   iScan = width * sizeof(::u32);
//
//   // 3.
//   CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//   CGContextRef context = CGBitmapContextCreate(pixels, width, height, bitsPerComponent, bytesPerRow, colorSpace,
//                                                kCGImageAlphaPremultipliedLast);///kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
//
//   // 4.
//   CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
//
//   // 5. Cleanup
//   CGColorSpaceRelease(colorSpace);
//
//   CGContextRelease(context);
//
//   return pixels;
//
//}




