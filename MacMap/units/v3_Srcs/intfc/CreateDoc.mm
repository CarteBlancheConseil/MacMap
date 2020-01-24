//----------------------------------------------------------------------------
// File : CreateDoc.mm
// Project : MacMap
// Purpose : Objective C++ source file : Document creation utility
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 24/03/2005 creation.
// 11/02/2015 NAV to NS transformation.
//----------------------------------------------------------------------------

#import "CreateDoc.h"
#import "bMacMapApp.h"

#import <mox_intf/NSUIUtils.h>
#import <mox_intf/NSOpenSavePanelWrappers.h>
#import <MacMapSuite/bTrace.h>
#import <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------
@interface MMCreateDocViewController : NSViewController{
    IBOutlet NSPopUpButton* _unt_pop;
    IBOutlet NSTextField*   _res_txt;
    double                  _val;
}
//----------------------------------------------------------------------------
-(IBAction)setResolution:(id)sender;
//----------------------------------------------------------------------------
-(long)getUnit;
-(double)getResolution;
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
//----------------------------------------------------------------------------
@end


// ---------------------------------------------------------------------------
//
// -----------
@implementation MMCreateDocViewController
// ---------------------------------------------------------------------------
//
// ------------
-(void)awakeFromNib{
_bTrace_("[MMCreateDocViewController awakeFromNib]",true);
    [super awakeFromNib];
    NSPopupButtonRemoveAllItems(_unt_pop);
    _MMAPP_->distMgr()->app_load();
    NSPopupButtonPopulateWithUnits(_unt_pop,_MMAPP_,_MMAPP_->distMgr()->get_current());
    _MMAPP_->distMgr()->app_unload();
    _val=1;
    [_res_txt setDoubleValue:_val];
}
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)setResolution:(id)sender{
_bTrace_("[MMCreateDocViewController setResolution]",true);
    _val=[_res_txt doubleValue];
_tm_("reso :"+_val);
    [_res_txt setStringValue:[NSString stringWithFormat:@"%f",_val]];
}
// ---------------------------------------------------------------------------
//
// ------------
-(long)getUnit{
    return [_unt_pop indexOfSelectedItem]+1;
}
// ---------------------------------------------------------------------------
//
// ------------
-(double)getResolution{
_bTrace_("[MMCreateDocViewController getResolution]",true);
//NSLog([_res_txt stringValue]);
//    return [_res_txt doubleValue];
    return _val;
}
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                 class:(Class)loadClass{
_bTrace_("[MMCreateDocViewController allocWithNibNamed owner class]",true);
MMCreateDocViewController* ctrlr=[[loadClass alloc] initWithNibName:nibNamed
                                                             bundle:[NSBundle mainBundle]];
    return ctrlr;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)controlTextDidChange:(NSNotification*)notification{
//_bTrace_("[MMCreateDocViewController controlTextDidChange]",true);
//    _val=[_res_txt doubleValue];
//_tm_(_val);
//    if(_val<=0){
//        _val=1;
//    }
//_tm_(_val);
}

// ---------------------------------------------------------------------------
//
// -----------
@end

// ---------------------------------------------------------------------------
//
// ------------
void* MMCreateDoc_getAccessoryViewController(){
_bTrace_("MMCreateDoc_getAccessoryViewController",true);
MMCreateDocViewController   *ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    ctrlr=[MMCreateDocViewController allocWithNibNamed:@"MMCreateDocAccessoryView"
                                                class:[MMCreateDocViewController class]];
    [localPool release];
    return((__bridge void*)ctrlr);
}

// ---------------------------------------------------------------------------
//
// ------------
void* MMCreateDoc_getAccessoryViewFromController(void* ctrlr){
_bTrace_("MMCreateDoc_getAccessoryViewFromController",true);
MMCreateDocViewController   *controller=(__bridge MMCreateDocViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
NSView*   view=[controller view];
    [localPool release];
    return((__bridge void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
void MMCreateDoc_getParamsFromController(void* ctrlr, int* unit, double* reso){
_bTrace_("MMCreateDoc_getParamsFromController",true);
MMCreateDocViewController   *controller=(__bridge MMCreateDocViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    *unit=[controller getUnit];
    *reso=[controller getResolution];
_tm_("unit:"+(*unit)+" reso:"+(*reso));
    if(*reso<=0){
        NSBeep();
        *reso=1;
    }
    [localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
static void MMCreateDoc_releaseController(void* ctrlr){
_bTrace_("MMCreateDoc_releaseController",true);
MMCreateDocViewController   *controller=(MMCreateDocViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release]; // Crash : pourquoi ?
    [localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
OSStatus choose_path(char* path, int* unit, double* reso, int* srid){
_bTrace_("choose_path",true);
OSStatus    status=noErr;
void*       ctrlr=MMCreateDoc_getAccessoryViewController();
 
    if(NSPutFilePath("mmproject",
                     cNSUTF8StringEncoding,
                     MMCreateDoc_getAccessoryViewFromController(ctrlr),
                     path)==cNSCancelButton){
        MMCreateDoc_releaseController(ctrlr);
        return -1;
    }
    MMCreateDoc_getParamsFromController(ctrlr,unit,reso);
    MMCreateDoc_releaseController(ctrlr);
//	if(status==noErr){
_tm_("Path: "+path);
_tm_("Unit: "+(*unit));
_tm_("Resolution: "+(*reso));
//	}
	return status;
}


