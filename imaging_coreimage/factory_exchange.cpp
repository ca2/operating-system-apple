#include "framework.h"


//namespace coreimage_imaging
//{
//
//
//#define new ACME_NEW
//
//
//   factory_exchange::factory_exchange()
//   {
//
//      create_factory < imaging, ::imaging >();
//
//   }
//
//
//   factory_exchange::~factory_exchange()
//   {
//
//   }
//
//
//} // namespace coreimage_imaging


extern "C"
void imaging_factory_exchange(::factory_map * pfactorymap)
{

   create_factory < ::coreimage_imaging::context_image, ::context_image >();
   //coreimage_imaging::factory_exchange factoryexchange;

}



