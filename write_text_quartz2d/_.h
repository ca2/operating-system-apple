// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/_.h"
//#include "aura/operating_system.h"
//#include "node_windows/process_data.h"


#ifdef _WRITE_TEXT_QUARTZ2D_STATIC
#define CLASS_DECL_WRITE_TEXT_QUARTZ2D
#elif defined(_WRITE_TEXT_QUARTZ2D_LIBRARY)
#define CLASS_DECL_WRITE_TEXT_QUARTZ2D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_WRITE_TEXT_QUARTZ2D  CLASS_DECL_IMPORT
#endif


//namespace write_text_win32
//{
//
//
//   class windowing;
//   class hwnd_array;
//
//
//   CLASS_DECL_WRITE_TEXT_WIN32 void wingdi_enum_fonts(::write_text::font_enum_item_array & itema, bool bRaster, bool bTrueType, bool bOther);
//   CLASS_DECL_WRITE_TEXT_WIN32 ::e_char_set wingdi_get_cs(int iCharSet);
//
//
//} // namespace write_text_win32


//#include "font_enum.h"


//#include "font_department.h"
//
//
//#include "write_text.h"



