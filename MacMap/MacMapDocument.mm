//----------------------------------------------------------------------------
// File : MacMapDocument.mm
// Project : MacMap
// Purpose : Objective C++ file : MacMap Cocoa Document
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2010-2019 Carte Blanche Conseil.
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
// 22/11/2010 creation.
//----------------------------------------------------------------------------
#import "MacMapDocument.h"
#import "AppDelegate.h"
#import "bMacMapApp.h"
#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// -----------
@implementation MacMapDocument

// ---------------------------------------------------------------------------
// 
// -----------
-(instancetype)init{
_bTrace_("[MacMapDocument init]",true);
    self=[super init];
_tm_((void*)self);
    if(self){
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)dealloc{
_bTrace_("[MacMapDocument dealloc]",true);
    [super dealloc];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)close{
_bTrace_("[MacMapDocument close]",true);
    [super close];
MacMapApplication* a=NSApp;
BOOL q=[a willQuit];
    if([a willQuit]){
        exit(0);
    }
}
// ---------------------------------------------------------------------------
//
// -----------
-(nullable instancetype)initWithType:(NSString *)typeName error:(NSError **)outError{
_bTrace_("[MacMapDocument initWithType]",true);
id          inst=[self init];
bMacMapApp*	gapp=(bMacMapApp*)bMacMapApp::instance();
    gapp->create_doc((void*)inst);
// Il faudrait gérer l'échec à la création
    if(outError!=NULL){
        *outError=[NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
    }
    return inst;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSString *)windowNibName{
    return @"MacMapDocument";
}

// ---------------------------------------------------------------------------
// Ouverture
// -----------
-(BOOL)readFromFileWrapper:(NSFileWrapper *)fileWrapper ofType:(NSString *)typeName error:(NSError **)outError{
_bTrace_("[MacMapDocument readFromFileWrapper]",true);
NSURL*		nsurl=[self fileURL];
char		path[PATH_MAX];
    
	if([[nsurl path] getCString:path maxLength:PATH_MAX encoding:NSUTF8StringEncoding/*NSMacOSRomanStringEncoding*/]==NO){
_te_("getCString failed");
		return NO;
	}
    
_tm_("path="+path);
bMacMapApp*	gapp=(bMacMapApp*)bMacMapApp::instance();
    if(gapp->open_doc(path,(void*)self)){
        return NO;
    }

// Il faudrait gérer l'échec à l'ouverture
    if(outError!=NULL){
		//*outError=[NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
	}
    return YES;
}

// ---------------------------------------------------------------------------
// Fermeture
// -----------
//-(void)close{
//_bTrace_("[MacMapDocument close]",true);
//_tm_((void*)self);
//bMacMapApp*	gapp=(bMacMapApp*)bMacMapApp::instance();
//	gapp->close_doc();
//	[super close];
//}

// ---------------------------------------------------------------------------
// Pas utilisé ?
// -----------
/*-(NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError{
 _bTrace_("[MacMapDocument dataOfType]",true);
 // Insert code here to write your document to data of the specified type. If the given outError != NULL, ensure that you set *outError when returning nil.
 // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
 // For applications targeted for Panther or earlier systems, you should use the deprecated API -dataRepresentationOfType:. In this case you can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.
 if(outError!=NULL){
 *outError=[NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
	}
	return nil;
}*/

// ---------------------------------------------------------------------------
// Pas utilisé ?
// -----------
/*-(BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError{
_bTrace_("[MacMapDocument readFromData]",true);
 // Insert code here to read your document from the given data of the specified type.  If the given outError != NULL, ensure that you set *outError when returning NO.
 // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead.
 // For applications targeted for Panther or earlier systems, you should use the deprecated API -loadDataRepresentation:ofType. In this case you can also choose to override -readFromFile:ofType: or -loadFileWrapperRepresentation:ofType: instead.
 if(outError!=NULL){
 *outError=[NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
	}
 return YES;
}*/

/*- (nullable NSFileWrapper *)fileWrapperOfType:(NSString *)typeName error:(NSError **)outError{
 _bTrace_("[MacMapDocument writeToURL]",true);
 return nil;
 }
 - (BOOL)writeToURL:(NSURL *)url ofType:(NSString *)typeName error:(NSError **)outError{
 _bTrace_("[MacMapDocument writeToURL]",true);
 return NO;
 }*/


// ---------------------------------------------------------------------------
//
// -----------
@end
