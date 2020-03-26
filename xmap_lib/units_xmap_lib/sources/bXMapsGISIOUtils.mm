//----------------------------------------------------------------------------
// File : bXMapsGISIOUtils.mm
// Project : MacMap
// Purpose : Objective C++ source file : C wrappers utilities for GIS import/export
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
// 07/07/2010 creation.
// 10/02/2015 NAV to NS.
//----------------------------------------------------------------------------

#import "bXMapsGISIOUtils.h"
#import <Cocoa/Cocoa.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/CGUtils.h>
#import <mox_intf/mm_messages.h>
#import <MacMapSuite/MacMapSuiteCPP.h>
#import <MacMapSuite/bTrace.h>

//----------------------------------------------------------------------------
@interface GISIOImportViewController : NSViewController{
    IBOutlet NSButton* _grd_btn;
    IBOutlet NSButton* _crt_chk;
}
//----------------------------------------------------------------------------
-(IBAction)doGrid:(id)sender;
-(IBAction)doCreate:(id)sender;
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
-(BOOL)createIfNeeded;
//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface GISIOExportViewController : NSViewController{
    IBOutlet NSButton* _fxp_btn;
}
//----------------------------------------------------------------------------
-(IBAction)doExportFormat:(id)sender;
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
//----------------------------------------------------------------------------
@end





// ---------------------------------------------------------------------------
//
// -----------
@implementation GISIOImportViewController
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doGrid:(id)sender{
    [[[self view] window] setIsVisible:NO];
    SendCommandToApplication('IGrd');
    [[[self view] window] setIsVisible:YES];
}
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doCreate:(id)sender{
}
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass{
_bTrace_("[GISIOImportViewController allocWithNibNamed owner class]",true);
    return [[loadClass alloc] initWithNibName:nibNamed
                                       bundle:[NSBundle bundleWithIdentifier:@"com.cbconseil.xmap-lib"]];
}
// ---------------------------------------------------------------------------
//
// ------------
-(BOOL)createIfNeeded{
    return [_crt_chk intValue];
}
// ---------------------------------------------------------------------------
//
// -----------
@end

// ---------------------------------------------------------------------------
//
// -----------
@implementation GISIOExportViewController
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doExportFormat:(id)sender{
    [[[self view] window] setIsVisible:NO];
    SendCommandToApplication('FExp');
    [[[self view] window] setIsVisible:YES];
}
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass{
_bTrace_("[GISIOExportViewController allocWithNibNamed owner class]",true);
    return [[loadClass alloc] initWithNibName:nibNamed
                                       bundle:[NSBundle bundleWithIdentifier:@"com.cbconseil.xmap-lib"]];
}
// ---------------------------------------------------------------------------
//
// -----------
@end


// ---------------------------------------------------------------------------
//
// ------------
void* GISIOImport_getAccessoryViewController(bool hideCreateIfNeeded){
_bTrace_("GISIOImport_getAccessoryViewController",true);
GISIOImportViewController   *ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    if(hideCreateIfNeeded){
        ctrlr=[GISIOImportViewController allocWithNibNamed:@"GISIOImportViewNoCTIN"
                                                    class:[GISIOImportViewController class]];
    }
    else{
        ctrlr=[GISIOImportViewController allocWithNibNamed:@"GISIOImportView"
                                                    class:[GISIOImportViewController class]];
    }
    [localPool release];
    return((void*)ctrlr);
}

// ---------------------------------------------------------------------------
//
// ------------
void* GISIOImport_getAccessoryViewFromController(void* ctrlr){
_bTrace_("GISIOImport_getAccessoryViewFromController",true);
GISIOImportViewController   *controller=(GISIOImportViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
_tm_("localPool");
    
NSView*   view=[controller view];
_tm_((void*)view);
_tm_("[controller view]");

    [localPool release];
    return((void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
bool GISIOImport_getCreateIfNeededFromController(void* ctrlr){
_bTrace_("GISIOImport_getCreateIfNeededFromController",true);
GISIOImportViewController   *controller=(GISIOImportViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
bool   value=[controller createIfNeeded];
    [localPool release];
    return(value);
}

// ---------------------------------------------------------------------------
//
// ------------
void GISIOImport_releaseController(void* ctrlr){
_bTrace_("GISIOImport_releaseController",true);
GISIOImportViewController   *controller=(GISIOImportViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    [localPool release];
}


// ---------------------------------------------------------------------------
//
// ------------
void* GISIOExport_getAccessoryViewController(){
_bTrace_("GISIOExport_getAccessoryViewController",true);
GISIOExportViewController   *ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    ctrlr=[GISIOExportViewController allocWithNibNamed:@"GISIOExportView"
                                                class:[GISIOExportViewController class]];
//NSView*   view=[ctrlr view];
    [localPool release];
    return((void*)ctrlr);
}

// ---------------------------------------------------------------------------
//
// ------------
void* GISIOExport_getAccessoryViewFromController(void* ctrlr){
_bTrace_("GISIOExport_getAccessoryViewFromController",true);
GISIOExportViewController   *controller=(GISIOExportViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
NSView*   view=[controller view];
    [localPool release];
    return((void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
void GISIOExport_releaseController(void* ctrlr){
_bTrace_("GISIOExport_releaseController",true);
GISIOExportViewController   *controller=(GISIOExportViewController*)ctrlr;
NSAutoreleasePool           *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    [localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
bGenericType* GISIOImport_getImportType(bGenericMacMapApp* gapp,
                                        int typeKind,
                                        import_prm prm){
_bTrace_("GISIOImport_getImportType",false);
bGenericType*   tp=NULL;
    switch(typeKind){
        case kBaseKindPoint:
            tp=gapp->typesMgr()->get(gapp->typesMgr()->index(prm.grid.t_point));
            break;
        case kBaseKindPolyline:
            tp=gapp->typesMgr()->get(gapp->typesMgr()->index(prm.grid.t_line));
            break;
        case kBaseKindText:
            tp=gapp->typesMgr()->get(gapp->typesMgr()->index(prm.grid.t_text));
            break;
        case kBaseKindPolygon:
            tp=gapp->typesMgr()->get(gapp->typesMgr()->index(prm.grid.t_poly));
            break;
        case kBaseKindRaster:
            tp=gapp->typesMgr()->get(gapp->typesMgr()->index(prm.grid.t_raster));
            break;
    }
    if((tp==NULL)&&(prm.ctin)){
char    name[FILENAME_MAX];
        strcpy(name,prm.name);
        UTF82MacRoman(name,FILENAME_MAX);
        tp=gapp->typesMgr()->get(gapp->typesMgr()->index(name));
        if(tp==NULL){
            if(gapp->typesMgr()->add(name,"",NULL,kBaseLocalDBID,typeKind)!=noErr){
_te_("_gapp->typesMgr()->add failed");
            }
            else{
                tp=gapp->typesMgr()->get(gapp->typesMgr()->count());
                gapp->layersMgr()->add(gapp->typesMgr()->count(),1);
            }
        }
    }
    return tp;
}
