//----------------------------------------------------------------------------
// File : NSOpenSavePanelWrappers.mm
// Project : MacMap
// Purpose : Objective C++ source file : NSOpenPanel/NSSavePanel C wrappers
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2015 Carte Blanche Conseil.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// See the LICENSE.txt file for more information.
//
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
// 20/06/2014 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bTrace.h>
#import "NSOpenSavePanelWrappers.h"

// ---------------------------------------------------------------------------
// Dummy class
// ------------
@interface OpenView : NSView <NSOpenSavePanelDelegate>{
	NSMutableArray*	_ftype;
    NSOpenPanel*    _panel;
}
@end

// ---------------------------------------------------------------------------
// Dummy class implementation
// ------------
@implementation OpenView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
	self=[super init];
	_ftype=[[NSMutableArray alloc] init];
    _panel=nil;
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
	[_ftype release];
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)addExt:(NSString*)ext{
	[_ftype addObject:ext];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setPanel:(NSOpenPanel*)panel{
    _panel=panel;
}

// ---------------------------------------------------------------------------
//
// ------------
//-(BOOL)panel:(id)sender shouldEnableURL:(NSURL *)url{
//_bTrace_("[OpenView panel:shouldEnableURL:",true);
//    if([_ftype count]==0){
//        return YES;
//    }
////    NSLog(url);
//   
//    if([url absoluteString]){
//        NSLog([url absoluteString]);
//    }
//    
//    if([url isFileURL]){
//        _tm_("file");
//    }
//    else{
//        _tm_("not file");
//    }
//    
//    [[NSWorkspace sharedWorkspace] isFilePackageAtPath:filename]
// 
//    isFilePackageAtPath
//    
//    return YES;
//}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)panel:(id)sender shouldShowFilename:(NSString *)filename{
BOOL isDirectory=NO;
BOOL isPackage=NO;
    
    (void)[[NSFileManager defaultManager] fileExistsAtPath:filename isDirectory:&isDirectory];
    if(isDirectory){
        isPackage=[[NSWorkspace sharedWorkspace] isFilePackageAtPath:filename];
    }

    if([_ftype count]==0){
        if(_panel&&[_panel canChooseDirectories]){
            if(isPackage){
                return NO;
            }
            return isDirectory;
        }
        return YES;
    }

    if(isDirectory&&!isPackage){
        return YES;
    }
    
NSString* ext=[filename pathExtension];
	if( [ext length]==0                      ||
        [ext compare:@"/"]==NSOrderedSame    ||
        ext==nil                             ||
        [ext length]<1)                      {
        return NO;
	}
    
NSEnumerator*	numer=[_ftype objectEnumerator];
NSString*		allowedExt;
	while((allowedExt=[numer nextObject])){
		if([ext caseInsensitiveCompare:allowedExt]==NSOrderedSame){
			return YES;
		}
	}
	return NO;
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// -----------
long NSGetFilePath(const char** fileTypes,
				   long nFileTypes,
				   unsigned int encoding,
				   bool canChooseDirectory,
                   void* accessoryView,
				   char* path){
//_bTrace_("NSOpenSavePanelWrappers.NSGetFilePath",true);
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
OpenView*			view=[[OpenView alloc] init];	
NSString*			nstr;
	if(fileTypes!=NULL){
		for(long i=0;i<nFileTypes;i++){
			nstr=[NSString stringWithCString:fileTypes[i] encoding:encoding];
			[view addExt:nstr];
		}
	}

NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setCanChooseDirectories:canChooseDirectory];
	[panel setCanChooseFiles:YES];
	[panel setAllowsMultipleSelection:NO];
	[panel setResolvesAliases:YES];
	[panel setDelegate:view];
    [panel setAccessoryView:(NSView*)accessoryView];
	
long	ret=NSCancelButton;
	if([panel runModal]==NSOKButton){
NSString*	filename=[[panel filenames] objectAtIndex:0];
		if([filename getCString:path maxLength:PATH_MAX encoding:encoding]==NO){
//_tm_("getCString failed for "+(int)encoding+" encoding\n");
		}
		ret=NSOKButton;
	}
	
	[view release];
    [localPool release];
	return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
long NSGetDirectoryPath(const char** fileTypes,
                        long nFileTypes,
                        unsigned int encoding,
                        bool canChooseDirectory,
                        void* accessoryView,
                        char* path){
//_bTrace_("NSOpenSavePanelWrappers.NSGetFilePath",true);
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
OpenView*			view=[[OpenView alloc] init];
NSString*			nstr;
    if(fileTypes!=NULL){
        for(long i=0;i<nFileTypes;i++){
            nstr=[NSString stringWithCString:fileTypes[i] encoding:encoding];
            [view addExt:nstr];
        }
    }
    
NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:canChooseDirectory];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:NO];
    [panel setResolvesAliases:YES];
    [panel setDelegate:view];
    [panel setAccessoryView:(NSView*)accessoryView];
    
    [panel setPrompt:NSLocalizedString(@"save",@"")];
    [panel setTitle:NSLocalizedString(@"save",@"")];
    [panel setCanCreateDirectories:YES];

    [view setPanel:panel];

long	ret=NSCancelButton;
    if([panel runModal]==NSOKButton){
NSString*	filename=[[panel filenames] objectAtIndex:0];
        if([filename getCString:path maxLength:PATH_MAX encoding:encoding]==NO){
//_tm_("getCString failed for "+(int)encoding+" encoding\n");
        }
        ret=NSOKButton;
    }
    
    [view release];
    [localPool release];
    return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
long NSGetFilePaths(const char** fileTypes,
                    long nFileTypes,
                    unsigned int encoding,
                    bool canChooseDirectory,
                    bool multipleAllowed,
                    void* accessoryView,
                    bArray& paths){
//_bTrace_("NSOpenSavePanelWrappers.NSGetFilePaths",true);
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
OpenView*			view=[[OpenView alloc] init];
NSString*			nstr;
    if(fileTypes!=NULL){
        for(long i=0;i<nFileTypes;i++){
            nstr=[NSString stringWithCString:fileTypes[i] encoding:encoding];
            [view addExt:nstr];
        }
    }
    
NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:canChooseDirectory];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:multipleAllowed];
    [panel setResolvesAliases:YES];
    [panel setDelegate:view];
    [panel setAccessoryView:(NSView*)accessoryView];

long	ret=NSCancelButton;
char    path[PATH_MAX];
    if([panel runModal]==NSOKButton){
        for(long i=0;i<[[panel filenames] count];i++){
NSString*	filename=[[panel filenames] objectAtIndex:i];
            if([filename getCString:path maxLength:PATH_MAX encoding:encoding]==NO){
//_tm_("getCString failed for "+(int)encoding+" encoding\n");
            }
            else{
               paths.add(&path);
            }
        }
        ret=NSOKButton;
    }
    
    [view release];
    [localPool release];
    return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
long NSPutFilePath(const char* setAllowedFileType,
                   unsigned int encoding,
                   void* accessoryView,
                   char* path){
//_bTrace_("NSOpenSavePanelWrappers.NSPutFilePath",true);
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
NSSavePanel*        panel = [NSSavePanel savePanel];

    [panel setAccessoryView:(NSView*)accessoryView];
    [panel setAllowedFileTypes:[NSArray arrayWithObjects:[NSString stringWithCString:setAllowedFileType
                                                                            encoding:encoding],nil]];
long	ret=NSCancelButton;
    if([panel runModal]==NSOKButton){
NSString*	filename=[[panel URL] path];
        if([filename getCString:path maxLength:PATH_MAX encoding:encoding]==NO){
//_tm_("getCString failed for "+(int)encoding+" encoding\n");
        }
        ret=NSOKButton;
    }
    
    [localPool release];
    return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
long NSGetUserCachePath(unsigned int encoding,
                        char* path){
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
long        ret=0;
NSArray*    paths=NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask,YES);
    if(paths){
NSString*   nsstr=[paths objectAtIndex:0];
        if(nsstr){
            ret=[nsstr getCString:path maxLength:PATH_MAX encoding:encoding];
        }
    }
    [localPool release];
    return ret;
}
