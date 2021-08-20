//
//  visual_apple_image.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 21/10/17. Thomas Boregaard Soerensen <3
//
#include "framework.h"

#include <Carbon/Carbon.h>


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


   ::e_status context_image::save_image(memory & memory, const ::image * pimage, const ::save_image * psaveimage)
   {

      if(pimage->is_empty())
      {

         return error_failed;

      }

      unsigned long size;

      CGImageRef cgimage = cgimageref_from_image(pimage);

      ::acme::malloc < color32_t * > p;

      switch (psaveimage == nullptr ? ::draw2d::format_png : psaveimage->m_eformat)
      {
      case ::draw2d::format_jpeg:
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

      return true;

   }



   ::e_status context_image::_load_image( ::image * pimage, const ::payload & varFile, bool bSync, bool bCreateHelperMaps)
   {
      
      if(::is_null(pimage))
      {
         
         return ::error_failed;
         
      }
      
      memory memory;
      
      auto pcontext = m_pcontext;
      
      bool bOk = pcontext->m_papexcontext->file().as_memory(varFile, memory);
      
      if(!bOk)
      {
         
         return ::error_failed;
         
      }
      
      auto pcontextimage = pcontext->context_image();
      
      auto estatus = pcontextimage->load_svg(pimage, memory);
      
      if(::succeeded(estatus))
      {

         return estatus;
         
      }
      
      const char * psz = (const char *) memory.get_data();
      
      if (memory.get_size() > 3 && strnicmp(psz, "gif", 3) == 0)
      {

         if (!_load_multi_frame_image(pimage, memory))
         {

            pimage->set_nok();

            return pimage->m_estatus;

         }

         pimage->on_load_image();

         pimage->set_ok();

         //pimage->notify(OK);

         pimage->m_estatus = ::success;

         return pimage->m_estatus;

      }
      
      int w = 0;

      int h = 0;

      int iScan = 0;

      ::acme::malloc < color32_t * > pcolorref;

      pcolorref = file_memory_to_image_data(w, h, iScan, memory.get_data(), memory.get_size());
      
      if(pcolorref == nullptr)
      {
         
         return false;
         
      }
      
      if(!pimage->create({w, h}))
      {
         
         return false;
         
      }
      
      pimage->map();
      
      vertical_swap_copy_colorref(pimage->colorref(), w, h, pimage->scan_size(), pcolorref, iScan);
      
      pimage->set_ok();
      
      return true;

   }


} // namespace coreimage_imaging


void * cg_image_get_image_data(int & width, int & height, int & iScan, CGImageRef image)
{

   if(image == nullptr)
   {

      return nullptr;

   }

   //https://www.raywenderlich.com/69855/image-processing-in-ios-part-1-raw-bitmap-modification
   // 1.
   width = (int) CGImageGetWidth(image);
   height = (int) CGImageGetHeight(image);

   // 2.
   long bytesPerPixel = 4;
   long bytesPerRow = bytesPerPixel * width;
   long bitsPerComponent = 8;

   ::u32 * pixels;
   pixels = (::u32 *) calloc(height * width, sizeof(::u32));
   
   iScan = width * sizeof(::u32);

   // 3.
   CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
   CGContextRef context = CGBitmapContextCreate(pixels, width, height, bitsPerComponent, bytesPerRow, colorSpace,
                                                kCGImageAlphaPremultipliedLast);///kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);

   // 4.
   CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);

   // 5. Cleanup
   CGColorSpaceRelease(colorSpace);

   CGContextRelease(context);

   return pixels;

}




