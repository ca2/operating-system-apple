//
// Created by camilo on 19/01/2021. <33TBS!!
//

//
// Created by camilo on 19/01/2021. <3-<3ThomasBS!!
//
#pragma once


namespace acme
{


   namespace apple
   {


      class CLASS_DECL_ACME node :
         virtual public ::acme::posix::node
      {
      public:


         node();
         virtual ~node();


         //virtual ::color::color get_system_color(enum_system_color esystemcolor) override;

         virtual int node_init_check(int * pi, char *** ppz) override;

         //virtual ::e_status start_node();

         //override;

         void initialize(::object * pobject) override;

   //      virtual ::e_status get_system_time(system_time_t * psystemtime)
   //
   //      override;
   //
   //      virtual double get_time_zone();
   //
   //      virtual ::e_status system_time_to_time(time_t * ptime, const system_time_t * psystime, i32 nDST = -1)
   //
   //      override;
   //
   //      virtual ::e_status file_time_to_time(time_t * ptime, const filetime_t * pfiletime, i32 nDST = -1)
   //
   //      override;
   //
   //      virtual ::e_status time_to_system_time(system_time_t * psystemtime, const time_t * ptime)
   //
   //      override;
   //
   //      virtual ::e_status get_system_time_as_file_time(filetime_t * pfiletime)
   //
   //      override;
   //
   //      virtual ::e_status system_time_to_tm(tm * ptm, const system_time_t * psystemtime);
   //
   //      virtual ::e_status tm_to_system_time(system_time_t * psystemtime, const tm * ptm);
   //
   //      virtual ::e_status system_time_to_file_time(filetime_t * pfiletime, const system_time_t * psystemtime)
   //
   //      override;
   //
   //      virtual ::e_status file_time_to_system_time(system_time_t * psystemtime, const filetime_t * pfiletime)
   //
   //      override;
   //
   //      virtual ::e_status file_time_to_local_file_time(filetime_t * pfiletimeLocal, const filetime_t * pfiletime)
   //
   //      override;
   //
   //      virtual ::e_status is_valid_filetime(const filetime_t * pfiletime)
   //
   //      override;
   //
   //      virtual filetime get_filetime_now();


         void install_sigchld_handler() override;


         string audio_get_default_library_name() override;
      
         string multimedia_audio_mixer_get_default_library_name() override;
      
         string veriwell_multimedia_music_midi_get_default_library_name() override;
         
         
         void node_post(const ::routine & routine) override;
         

      };


   } // namespace apple


} // namespace acme



