//
//  apple.m
//  acme
//
//  Created by Camilo Sasuke Tsumanuma on 04/01/18. with tbs in <3
//
#include "framework.h"
#import "NSString+SymlinksAndAliases.h"
#import "acme/primitive/primitive/runnable.h"




char * mm_ca2_command_line()
{
   
   return ns_string([[NSBundle mainBundle] objectForInfoDictionaryKey:@"ca2_command_line"]);
   
}


char * ns_get_bundle_identifier()
{
   
   return ns_string([[NSBundle mainBundle] bundleIdentifier]);
   
}


char * ns_get_executable_path()
{
   
   return ns_string([[NSBundle mainBundle] executablePath]);
   
}


char * mm_error_string(OSStatus status)
{
   
   NSError * error = [NSError errorWithDomain:NSOSStatusErrorDomain code: status userInfo: nil];
   
   NSString * strError = [error localizedDescription];
   
   return ns_string(strError);
   
}


char * mm_error_description(OSStatus status)
{
   
   NSError * error = [NSError errorWithDomain:NSOSStatusErrorDomain code: status userInfo: nil];
   
   NSString * strError = [error description];
   
   return ns_string(strError);
   
}







//char * ns_realpath(const char * pszPath)
//{
//   
//   NSString * str = [[NSString alloc] initWithUTF8String:pszPath];
//   
//   NSString * strFullPath = [str stringByResolvingSymlinksAndAliases];
//   
//   if(strFullPath == nil)
//   {
//    
//      return strdup(pszPath);
//      
//   }
//   
//   return ns_string(strFullPath);
//   
//}


void ns_main_async(dispatch_block_t block)
{
   
   //   dispatch_block_t block = ^{
   //      // Code for the method goes here
   //   };
   //
   
   if ([NSThread isMainThread])
   {
      
      block();
      
   }
   else
   {
      
      dispatch_async(dispatch_get_main_queue(), block);
      
   }
   
}



class matter;


CLASS_DECL_ACME ::e_status __call(::matter * prunnable);


void main_async_runnable(::matter * prunnable)
{
   
   ns_main_async(^
                 {

                    __call(prunnable);
                    
                 });
   
   //[[mmos get] runRunnableOnMainThread: prunnable];
   
}


void _main_sync_runnable(::matter * prunnable, DWORD dwMillis)
{
   
   ns_main_sync(^
                {
                   
                    __call(prunnable);
                   
                });
   
   //[[mmos get] runRunnableOnMainThread: prunnable];
   
}


NSString * __ns_get_text(const char * psz);


NSString * __ns_get_text(NSString * str)
{

   const char * psz = [str UTF8String];
   
   return __ns_get_text(psz);

}


NSString * __ns_get_text(const char * psz)
{
   
   int iLen = __c_get_text_length(psz);
   
   char * p = (char *) malloc(iLen+1);
   
   memset(p, 0, iLen+1);
   
   __c_get_text(p, iLen + 1, psz);
   
   NSString * strText = [[NSString alloc] initWithUTF8String: p];
   
   free(p);
   
   return strText;
   
}

//
//
//bool set_task_name(const char * pszTaskName)
//{
//
//   NSString * strTaskName = [[NSString alloc] initWithUTF8String:pszTaskName];
//
//   [[NSThread currentThread] setName : strTaskName];
//
//   return true;
//
//}
//
//bool set_task_name(void * p, const char * pszTaskName)
//{
//
//   NSString * strTaskName = [[NSString alloc] initWithUTF8String:pszTaskName];
//
//   [[NSThread currentThread] setName : strTaskName];
//
//   return true;
//                     
//}
//
//
//char * ns_get_task_name()
//{
//
//   return ns_string([[NSThread currentThread] name]);
//
//}




char * ns_get_default_browser_path()
{
   
    /*
   CFURLRef appURL = LSCopyDefaultApplicationURLForURL((__bridge CFURLRef)[NSURL URLWithString: @"http:"], kLSRolesAll, NULL);
   
   CFStringRef str = CFURLGetString(appURL);
   r
   char * psz = strdup([(__bridge NSString *)str UTF8String]);
   
   CFRelease(appURL);r
   
   //CFRelease(str);
   
     */
    
    char * psz = strdup("");
     
   return psz;
   
}


bool ns_open_url(const char * psz)
{
   
   NSString * str = [NSString stringWithUTF8String:psz];
   
   if(str == NULL)
   {
      
      return false;
      
   }
   
   NSURL * url = [[NSURL alloc] initWithString: str];
   
   if(url == NULL)
   {
      
      return false;
      
   }
   
   if(![[NSWorkspace sharedWorkspace] openURL: url])
   {
      
      return false;
      
   }
   
   return true;
   
}


bool ns_open_file(const char * psz)
{
   
   NSString * path = [NSString stringWithUTF8String:psz];
   
   if(path == NULL)
   {
      
      return false;
      
   }
   
   if(![[NSWorkspace sharedWorkspace] openFile: path])
   {
      
      return false;
      
   }
   
   return true;
   
}

void ns_launch_app(const char * psz, const char ** argv, int iFlags);


void node_launch_app(const char * psz, const char ** argv, int iFlags)
{
   
   return ns_launch_app(psz, argv, iFlags);
   
}


void ns_launch_app_at_url(NSURL * url, const char ** argv, int iFlags)
;

void ns_launch_bundle(const char * pszBundle, const char ** argv)
{

   NSString * bundle = [[NSString alloc] initWithUTF8String:pszBundle];
   
   NSURL * url = [[NSWorkspace sharedWorkspace] URLForApplicationWithBundleIdentifier:bundle];

   ns_launch_app_at_url(url, argv, 0x00010000 | 0x00080000);
                        
}







void os_post_quit()
{
   
   ns_main_sync(^()
   {
   
      [NSApp terminate:nil];
      
   });
   
}


void ns_Sleep(unsigned int uiMillis)
{

   [NSThread sleepForTimeInterval: ((double) uiMillis / 1000.0) ];

}



//https://stackoverflow.com/questions/14699604/replacements-for-getmacosstatuserrorstring-getmacosstatuscommentstring

//11
//down vote
//accepted
//You can use [NSError errorWithDomain:NSOSStatusErrorDomain code:errorCode userInfo:nil] to get an NSError representing the OSStatus of errorCode. NSError provides the ability to get a localized error message from the error code (-localizedDescription), or a regular unlocalized one for debugging purposes (-description).

// GetMacOSStatusErrorString and GetMacOSStatusCommentString







void library_launch(const char *psz)
{
   NSString *path =  [NSString stringWithUTF8String:psz];
   NSWorkspace *ws=[NSWorkspace sharedWorkspace];
   NSURL* url = [NSURL fileURLWithPath:path isDirectory:YES];
   
   NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
   [ws launchApplicationAtURL:url
                      options:NSWorkspaceLaunchDefault
                configuration:dict
                        error:nil];
   
   
}



char * macos_get_version()
{

   NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
 
   return strdup([appVersion UTF8String]);
   
}







void ns_launch_app_at_url(NSURL * url, const char ** argv, int iFlags)
{

   NSWorkspace * workspace = [NSWorkspace sharedWorkspace];

   NSMutableDictionary * dict = [[NSMutableDictionary alloc] init];

   if(argv != NULL)
   {

      NSMutableArray * array = [[NSMutableArray alloc] init];

      while(*argv != NULL)
      {

         [array addObject: [[NSString alloc] initWithUTF8String: *argv]];

         argv++;

      }

      [dict setObject:array forKey:NSWorkspaceLaunchConfigurationArguments];

   }

   //NSWorkspaceLaunchWithoutActivation
   //NSWorkspaceLaunchNewInstance
   //NSWorkspaceLaunchDefault

   [workspace launchApplicationAtURL:url options: iFlags configuration:dict error:nil];


}


void ns_launch_app(const char * psz, const char ** argv, int iFlags)
{

   NSString * path = [[NSString alloc] initWithUTF8String:psz];

   NSURL * url = [NSURL fileURLWithPath:path isDirectory:YES];

   ns_launch_app_at_url(url, argv, iFlags);

}







