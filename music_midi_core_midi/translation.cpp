#include "framework.h"


namespace music
{
   
   
   namespace midi
   {
      
      
      namespace core_midi
      {
         
CLASS_DECL_MUSIC_MIDI_CORE_MIDI ::e_status     translate_os_status(OSStatus status)
      {


         switch(status)
         {

         case noErr:

            return ::success;

         default:

            break;

         }

         return ::error_midi;

      }
         
         
      } // namespace core_midi
      

   } // namespace midi


} // namespace music



