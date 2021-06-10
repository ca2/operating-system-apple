//
//  dls_synth_message_out.cpp
//  music_midi_core_midi
//
//  Created by Camilo Sasuke on 2021-06-09 03:56 BRT <3ThomasBorregaardSørensen__!!
//  Copyright © 2021 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//
#include "framework.h"


namespace music
{
   
   
   namespace midi
   {
      
      
      namespace core_midi
      {

         
         dls_synth_message_out::dls_synth_message_out()
         {
            
            open(0);
            
         }
         
         
         dls_synth_message_out::~dls_synth_message_out()
         {
            
            
         }
         
         
         ::e_status dls_synth_message_out::open(int iPort)
         {
            
            OSStatus result = noErr;
            
            result = NewAUGraph(&m_audiograph);
            
            if (noErr != result)
            {
                               
               return ::error_failed;
                               
            }

            AudioComponentDescription descriptionSynth ={};
            descriptionSynth.componentType           = kAudioUnitType_MusicDevice;
            descriptionSynth.componentSubType        = kAudioUnitSubType_DLSSynth;
            descriptionSynth.componentManufacturer   = kAudioUnitManufacturer_Apple;
            descriptionSynth.componentFlags          = 0;
            descriptionSynth.componentFlagsMask      = 0;
            
            result = AUGraphAddNode (m_audiograph, &descriptionSynth, &m_nodeSynth);

            if (noErr != result)
            {
                               
               return ::error_failed;
                               
            }
            
            AudioComponentDescription descriptionOutput={};
            descriptionOutput.componentType           = kAudioUnitType_Output;
            descriptionOutput.componentSubType        = kAudioUnitSubType_DefaultOutput;
            descriptionOutput.componentManufacturer   = kAudioUnitManufacturer_Apple;
            descriptionOutput.componentFlags          = 0;
            descriptionOutput.componentFlagsMask      = 0;

            result = AUGraphAddNode(m_audiograph, &descriptionOutput, &m_nodeOutput);
            
            if (noErr != result)
            {
                       
               return ::error_failed;
                       
            }
            
            // 6.connect input->eq->output node
            result = AUGraphConnectNodeInput(m_audiograph, m_nodeSynth, 0,m_nodeOutput, 0);
            if (noErr != result)
            {
                       
               return ::error_failed;
                       
            }
            
            result = AUGraphUpdate (m_audiograph, NULL);
            if (noErr != result)
                   {
                              
                      return ::error_failed;
                              
                   }

            // 3.open augraphic
            result = AUGraphOpen(m_audiograph);
            if (noErr != result)
            {
               
               return ::error_failed;
               
            }

            // 4.get audio unit instance from nodes
            result = AUGraphNodeInfo(m_audiograph, m_nodeOutput, NULL, &m_unitOutput);
            
            if (noErr != result)
            {
                       
               return ::error_failed;
                       
            }
            
            // 7. initialize graphic
              result = AUGraphInitialize(m_audiograph);
              if (noErr != result)
              {
                         
                 return ::error_failed;
                         
              }
              

            result = AUGraphNodeInfo(m_audiograph, m_nodeSynth, NULL, &m_unitSynth);
            
            if (noErr != result)
            {
                       
               return ::error_failed;
                       
            }




             CAShow(m_audiograph);
            
             
            // Start the graph
            result = AUGraphStart (m_audiograph);
             
            if (noErr != result)
              {
                         
                 return ::error_failed;
                         
              }
            return ::success;
         }
         
         
         ::e_status dls_synth_message_out::note_on(int iChannel, unsigned char uchNote, unsigned char uchVelocity)
         {
            
            int noteOnCommand = ::u32(0x90 | iChannel);
            OSStatus result=MusicDeviceMIDIEvent (m_unitSynth, noteOnCommand, uchNote, uchVelocity, 0);
            
            if (noErr != result)
                  {
                             
                     return ::error_failed;
                             
                  }
            return ::success;
         }
         
         
         ::e_status dls_synth_message_out::note_off(int iChannel, unsigned char uchNote, unsigned char uchVelocity)
         {
            int noteOffCommand = ::u32(0x80 | iChannel);
                OSStatus result=    MusicDeviceMIDIEvent (m_unitSynth, noteOffCommand, uchNote, uchVelocity, 0);
                    
                    if (noErr != result)
                          {
                                     
                             return ::error_failed;
                                     
                          }
                    return ::success;
         }
         
         
         ::e_status dls_synth_message_out::program_change(int iChannel, unsigned char uchProgram)
         {
            
            
            int programChangeCommand = ::u32(0xC0 | iChannel);
                OSStatus result=    MusicDeviceMIDIEvent (m_unitSynth, programChangeCommand, uchProgram, 0, 0);
                    
                    if (noErr != result)
                          {
                                     
                             return ::error_failed;
                                     
                          }
                    return ::success;

         }
         
         ::e_status dls_synth_message_out::step()
         {
            
            
         }

      } // namespace core_midi


   } // namespace midi


} // namespace core



