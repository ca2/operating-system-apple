#include "framework.h"
#include "aura/graphics/_.h"
#include "aura/graphics/draw2d/context_image.h"
#include "aura/graphics/draw2d/icon.h"
#include <math.h>


namespace draw2d_quartz2d
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////



   image::image()
   {

      m_bMapped            = false;

   }
   

   image::~image()
   {

      destroy ();

   }


   ::draw2d::bitmap_pointer image::get_bitmap() const
   {
      
      return m_pbitmap;
      
   }


   ::draw2d::bitmap_pointer image::detach_bitmap()
   {
      
      return m_pbitmap.detach();
      
   }


   // bPreserve is currently disregarded
   ::e_status image::create(const ::size_i32 & size, ::enum_flag eflagCreate, int iGoodStride, bool bPreserve)
   {

      if(m_pbitmap.is_set()
            && m_pbitmap->get_os_data() != nullptr
            && m_pgraphics.is_set()
            && m_pgraphics->get_os_data() != nullptr
            && size == m_sizeRaw)
      {

         return true;

      }

      //destroy();

      if(size.is_empty())
      {

         return false;

      }
      
      ::draw2d::bitmap_pointer pbitmap;
      
      pbitmap.create(this);

      ::draw2d::graphics_pointer pgraphics;
      
      pgraphics.create(this);

      //m_pbitmap.defer_create(this);

      //m_pgraphics.defer_create(this);

      if(pbitmap.is_null() || pgraphics.is_null())
      {

         //destroy();

         return false;

      }
      
      //m_pcolorrefRaw = nullptr;
      
      //m_iScan = 0;
      
      color32_t * pcolorref = nullptr;
      
      i32 iScan = iGoodStride;

      if(!pbitmap->create_bitmap(nullptr, size, (void **) &pcolorref, &iScan) || pbitmap->get_os_data() == nullptr)
      {

         //destroy();

         return false;

      }

      pgraphics->set(pbitmap);

      if(pgraphics->get_os_data() == nullptr)
      {

         //destroy();

         return false;

      }
      
      if(bPreserve)
      {
         
         map();
         
         int cxMin = minimum(m_size.cx, size.cx);
         
         int cyMin = minimum(m_size.cy, size.cy);
         
         copy_colorref(pcolorref, cxMin, cyMin, iScan, m_pcolorrefRaw, m_iScan);
         
      }
      
      m_pbitmap = pbitmap;
      
      m_pgraphics = pgraphics;
      
      m_bMapped = false;
      
      init(size, pcolorref, iScan);

      m_pgraphics->m_pimage = this;
      
//      m_sizeRaw = size;
//
//      m_pcolorrefRaw = pcolorref;
//
//      m_iScan = iScan;
      
      //m_sizeAlloc = m_sizeRaw;
      
      set(eflagCreate);

      return true;

   }


   bool image::dc_select(bool bSelect)
   {
      /*      if(bSelect)
       {
       return m_spgraphics->SelectObject(m_pbitmap) != nullptr;
       }
       else
       {
       return m_spgraphics->SelectObject(m_hbitmapOriginal) != nullptr;
       }*/
      return true;
   }


   ::e_status image::create(::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap_pointer pbitmap = pgraphics->get_current_bitmap();

      if(pbitmap == nullptr)
      {

         return false;

      }

      ::size_i32 size = pbitmap->get_size();

      if(!create(size))
      {

         return false;

      }
      
      image_source imagesource(pgraphics);
      
      image_drawing_options imagedrawingoptions(size);
      
      image_drawing imagedrawing(imagedrawingoptions, imagesource);
      
      m_pgraphics->draw(imagedrawing);

      return true;

   }


   ::e_status image::destroy()
   {

      m_pbitmap.release();

      m_pgraphics.release();

      m_sizeRaw.cx            = 0;

      m_sizeRaw.cy            = 0;
      
      pixmap::clear();

      //m_pcolorrefRaw          = nullptr;
      
      //m_pcolorrefMap         = nullptr;

      m_iScan              = 0;

      return true;

   }


//   bool image::to(::draw2d::graphics * pgraphics, const ::point_i32 & point, const ::size_i32 & size, const ::point_i32 & pointSrc)
//   {
//
//      return pgraphics->BitBlt(point.x, point.y, size.cx, size.cy, get_graphics(), pointSrc.x, pointSrc.y);
//
//   }
//

bool image::_draw_raw(const ::rectangle_i32 & rectDst, ::image * pimageSrc, const ::point_i32 & pointSrc)
   {
   
      rectangle_f64 rectangleSource(pointSrc, rectDst.size());
   
      image_source imagesource(pimageSrc, rectangleSource);
      
      image_drawing_options imagedrawingoptions(rectDst);
      
      image_drawing imagedrawing(imagedrawingoptions, imagesource);
      
      return m_pgraphics->draw(imagedrawing);
      
      
   }


   ::e_status image::SetIconMask(::draw2d::icon * picon, i32 cx, i32 cy)
   {
      
      if(cx <= 0 || cy <= 0)
      {
         
         return false;
         
      }
      
      if(!create({cx, cy}))
      {
         
         return false;
         
      }


      // White blend image
      auto pimage1 = m_pcontext->context_image()->create_image({cx,  cy});
      
      pimage1->set_rgb(255, 255, 255);
      
      {
      
         image_source imagesource(picon);
         
         auto rectangle = ::rectangle_f64_dimension(0, 0, cx, cy);
         
         image_drawing_options imagedrawingoptions(rectangle);
         
         image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->g()->draw(imagedrawing);
         
      }

      // Black blend image
      auto pimage2 = m_pcontext->context_image()->create_image({cx,  cy});
      pimage2->fill(0, 0, 0, 0);

  
      {
      
         image_source imagesource(picon);
         
         auto rectangle = ::rectangle_f64_dimension(0, 0, cx, cy);
         
         image_drawing_options imagedrawingoptions(rectangle);
         
         image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage2->g()->draw(imagedrawing);
         
      }

      // Mask image
      auto pimageM= m_pcontext->context_image()->create_image({cx,  cy});

      
      
      {
      
         image_source imagesource(picon);
         
         auto rectangle = ::rectangle_f64_dimension(0, 0, cx, cy);
         
         image_drawing_options imagedrawingoptions(rectangle);
         
         image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageM->g()->draw(imagedrawing);
         
      }
      //pimageM->g()->draw(
      //rectangle_i32_dimension(0, 0, cx, cy),
      //picon);

      byte * r1=(byte*)pimage1->colorref();
      byte * r2=(byte*)pimage2->colorref();
      byte * srcM=(byte*)pimageM->colorref();
      byte * dest=(byte*)colorref();
      i32 iSize = cx*cy;

      byte b;
      byte bMax;
      while ( iSize-- > 0)
      {
         if(srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b =(byte)(r1[0]  - r2[0]);
            bMax = maximum(b, bMax);
            b =(byte)(r1[1]  - r2[1]);
            bMax = maximum(b, bMax);
            b =(byte)(r1[2]  - r2[2]);
            bMax = maximum(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0]  =  bMax;
         dest[1]  =  bMax;
         dest[2]  =  bMax;
         dest     += 4;
         srcM     += 4;
         r1       += 4;
         r2       += 4;
         
      }
      
      return true;

   }
   
   
   bool image::set_mapped()
   {
      
      m_bMapped = true;
      
      return true;
      
   }


   bool image::stretch_image(::image * pimage)
   {

      return ::image::stretch_image(pimage);

   }


   ::draw2d::graphics * image::_get_graphics() const
   {

      unmap();

      return m_pgraphics;

   }


   bool image::map(bool bApplyTransform) const
   {

      ((image *)this)->m_bMapped = true;
      
      return true;

   }
   

//   bool image::detach(::image * pimage)
//   {
//
//      ::image_pointer pimage = pimpl;
//
//      m_spgraphics = pimpl->m_spgraphics;
//      m_pbitmap = pimpl->m_pbitmap;
//
//      image::detach(pimpl);
//
//      pimpl->m_pbitmap.release();
//      pimpl->m_spgraphics.release();
//
//      return true;
//
//   }


   bool image::_unmap()
   {

      m_bMapped = false;
      
      return true;

   }


   bool image::blend(const ::point_i32 & pointDstParam, ::image * pimplSrc, const ::point_i32 & pointSrcParam, const ::size_i32 & sizeParam, byte bA)
   {
      
      ::point_i32 pointDst(pointDstParam);
      
      ::point_i32 pointSrc(pointSrcParam);
      
      ::size_i32 size(sizeParam);

      ::image * pimplDst = this;

      pimplDst->map();

      pimplSrc->map();

      pointDst += m_point;

      if (pointSrc.x < 0)
      {
         pointDst.x -= pointSrc.x;
         pointSrc.x = 0;
      }

      if (pointSrc.y < 0)
      {
         pointDst.y -= pointSrc.y;
         pointSrc.y = 0;
      }

      if (pointDst.x < 0)
      {
         size.cx += pointDst.x;
         pointDst.x = 0;
      }

      if (size.cx < 0)
         return true;

      if (pointDst.y < 0)
      {
         size.cy += pointDst.y;
         pointDst.y = 0;
      }

      if (size.cy < 0)
         return true;

      int xEnd = minimum(size.cx, minimum(pimplSrc->width() - pointSrc.x, pimplDst->width() - pointDst.x));

      int yEnd = minimum(size.cy, minimum(pimplSrc->height() - pointSrc.y, pimplDst->height() - pointDst.y));

      if (xEnd < 0)
         return false;

      if (yEnd < 0)
         return false;

      i32 scanDst = pimplDst->scan_size();

      i32 scanSrc = pimplSrc->scan_size();

      byte * pdst2;

      byte * psrc2;

#ifdef __APPLE__
      byte * pdst = &((byte *)pimplDst->colorref())[scanDst * (pimplDst->height() - pointDst.y - yEnd) + pointDst.x * sizeof(color32_t)];

      byte * psrc = &((byte *)pimplSrc->colorref())[scanSrc * (pimplSrc->height() - pointSrc.y - yEnd) + pointSrc.x * sizeof(color32_t)];

#else

      byte * pdst = &((byte *)imageDst.m_pcolorref)[scanDst * pointDst.y + pointDst.x * sizeof(color32_t)];

      byte * psrc = &((byte *)imageSrc.m_pcolorref)[scanSrc *  pointSrc.y + pointSrc.x * sizeof(color32_t)];

#endif

//      bool bFontListBlend = true;
//
//      if (bFontListBlend)
//      {
//
//         if (bA == 0)
//         {
//
//         }
//         else if (bA == 255)
//         {
//
//            for (int y = 0; y < yEnd; y++)
//            {
//
//               pdst2 = &pdst[scanDst * y];
//
//               psrc2 = &psrc[scanSrc * y];
//
//               //::memcpy_dup(pdst2, psrc2, xEnd * 4);
//               for (int x = 0; x < xEnd; x++)
//               {
//
//                  //*pdst2 = *psrc2;
//
//                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
//                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
//                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
//                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
//                  byte acomplement = ~psrc2[3];
//                  pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
//                  pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
//                  pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
//                  pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
//
//
//
//                  pdst2 += 4;
//
//                  psrc2 += 4;
//
//               }
//               //pdst2 += xEnd;
//               //psrc2 += xEnd;
//
//            }
//         }
//         else
//         {
//            for (int y = 0; y < yEnd; y++)
//            {
//
//               pdst2 = &pdst[scanDst * y];
//
//               psrc2 = &psrc[scanSrc * y];
//
//               //::memcpy_dup(pdst2, psrc2, xEnd * 4);
//               for (int x = 0; x < xEnd; x++)
//               {
//
//                  //*pdst2 = *psrc2;
//
//                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
//                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
//                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
//                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
//                  //byte acomplement = (~psrc2[3] * bA) >> 8;
//                  //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
//                  //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
//                  //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
//                  //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
//                  byte acomplement = (~psrc2[3] * bA) >> 8;
//                  pdst2[0] = clip_byte(((psrc2[0] * bA) + (pdst2[0] * acomplement)) >> 8);
//                  pdst2[1] = clip_byte(((psrc2[1] * bA) + (pdst2[1] * acomplement)) >> 8);
//                  pdst2[2] = clip_byte(((psrc2[2] * bA) + (pdst2[2] * acomplement)) >> 8);
//                  pdst2[3] = clip_byte(((psrc2[3] * bA) + (pdst2[3] * acomplement)) >> 8);
//
//
//
//                  pdst2 += 4;
//
//                  psrc2 += 4;
//
//               }
//               //pdst2 += xEnd;
//               //psrc2 += xEnd;
//
//            }
//
//         }
//
//         // bFontListData
//
//      }
//      else
      {

         // !bFontListData
         // bBouncingBall...

         if (bA == 0)
         {

         }
         else if (bA == 255)
         {

            for (int y = 0; y < yEnd; y++)
            {

               pdst2 = &pdst[scanDst * y];

               psrc2 = &psrc[scanSrc * y];

               //::memcpy_dup(pdst2, psrc2, xEnd * 4);
               for (int x = 0; x < xEnd; x++)
               {

                  //*pdst2 = *psrc2;

                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
                  byte a = pdst2[3];
                  byte alpha = psrc2[3];
                  if (a == 0)
                  {

                  }
                  else if(alpha == 0)
                  {

                     *((color32_t *)pdst2) = 0;

                  }
                  else
                  {

                     //int d0 = pdst2[0] * 255 / a;
                     //int d1 = pdst2[1] * 255 / a;
                     //int d2 = pdst2[2] * 255 / a;

                     //int s0 = psrc2[0] * 255 / alpha;
                     //int s1 = psrc2[1] * 255 / alpha;
                     //int s2 = psrc2[2] * 255 / alpha;

                     //d0 = ((s0 * a) + (d0 * alpha)) >> 8;
                     //d1 = ((s1 * a) + (d1 * alpha)) >> 8;
                     //d2 = ((s2 * a) + (d2 * alpha)) >> 8;
                     //pdst2[3] = ((psrc2[3] * a) + (pdst2[3] * alpha)) >> 8;

                     //pdst[0] = (d0 * pdst2[3]) >> 8;
                     //pdst[1] = (d1 * pdst2[3]) >> 8;
                     //pdst[2] = (d2 * pdst2[3]) >> 8;

                     //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
                     //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
                     //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
                     //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);

                     pdst2[0] = (pdst2[0] * alpha) >> 8;
                     pdst2[1] = (pdst2[1] * alpha) >> 8;
                     pdst2[2] = (pdst2[2] * alpha) >> 8;
                     pdst2[3] = (pdst2[3] * alpha) >> 8;
                  }



                  pdst2 += 4;

                  psrc2 += 4;

               }
               //pdst2 += xEnd;
               //psrc2 += xEnd;

            }
         }
         else
         {
            for (int y = 0; y < yEnd; y++)
            {

               pdst2 = &pdst[scanDst * y];

               psrc2 = &psrc[scanSrc * y];

               //::memcpy_dup(pdst2, psrc2, xEnd * 4);
               for (int x = 0; x < xEnd; x++)
               {

                  //*pdst2 = *psrc2;

                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
                  //byte acomplement = (~psrc2[3] * bA) >> 8;
                  //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
                  //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
                  //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
                  //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
                  byte acomplement = (~psrc2[3] * bA) >> 8;
                  pdst2[0] = byte_clip(((psrc2[0] * bA) + (pdst2[0] * acomplement)) >> 8);
                  pdst2[1] = byte_clip(((psrc2[1] * bA) + (pdst2[1] * acomplement)) >> 8);
                  pdst2[2] = byte_clip(((psrc2[2] * bA) + (pdst2[2] * acomplement)) >> 8);
                  pdst2[3] = byte_clip(((psrc2[3] * bA) + (pdst2[3] * acomplement)) >> 8);



                  pdst2 += 4;

                  psrc2 += 4;

               }
               //pdst2 += xEnd;
               //psrc2 += xEnd;

            }

         }

      }

      return true;

   }



} // namespace draw2d_quartz2d





