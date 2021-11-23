#include "framework.h"


__FACTORY_EXPORT void draw2d_quartz2d_factory(::factory_map * pfactorymap)
{

   
   create_factory < ::draw2d_quartz2d::image        , ::image >();
   create_factory < ::draw2d_quartz2d::graphics     , ::draw2d::graphics > ();
   create_factory < ::draw2d_quartz2d::bitmap       , ::draw2d::bitmap > ();
   create_factory < ::draw2d_quartz2d::pen          , ::draw2d::pen > ();
   create_factory < ::draw2d_quartz2d::brush        , ::draw2d::brush > ();
   create_factory < ::draw2d_quartz2d::palette      , ::draw2d::palette > ();
   create_factory < ::draw2d_quartz2d::region       , ::draw2d::region > ();
   create_factory < ::draw2d_quartz2d::font         , ::write_text::font > ();
   create_factory < ::draw2d_quartz2d::path         , ::draw2d::path > ();


   create_factory < ::draw2d_quartz2d::draw2d        , ::draw2d::draw2d >();

   
}



