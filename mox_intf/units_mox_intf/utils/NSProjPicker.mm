//----------------------------------------------------------------------------
// File : NSProjPicker.mm
// Project : MacMap
// Purpose : Objective C++ source file : Projection window
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2019 Carte Blanche Conseil.
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
// 09/07/2019 Creation.
//----------------------------------------------------------------------------

#import "NSProjPicker.h"

#import <mox_intf/NSUIUtils.h>

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/bStdProj.h>
#import <MacMapSuite/wtable.h>
#import <MacMapSuite/wproj.h>

#import <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------

@interface NSProjPicker : NSWindowController{
    IBOutlet NSTableView*	_prjtbl;
    IBOutlet NSTextField*	_epgtxt;
    
    IBOutlet NSButton*      _okbtn;
    IBOutlet NSButton*      _cancelbtn;
    
    long                    _code;
    int                     _srid;
    bStdTable*              _tbl;
}

//----------------------------------------------------------------------------
// Std
-(id)initWithSRID:(int)srid;
-(int)srid;
-(void)runAppModal:(long*)code;

//----------------------------------------------------------------------------
// Validation Dialogue
-(IBAction)validDialog:(id)sender;
-(IBAction)cancelDialog:(id)sender;

//----------------------------------------------------------------------------

@end




// ---------------------------------------------------------------------------
//
// ------------
@implementation NSProjPicker
// ---------------------------------------------------------------------------
//
// ------------
-(id)initWithSRID:(int)srid{
_bTrace_("[NSProjPicker initWithBool andSRID]",true);
    self=[self initWithWindowNibName:@"MMProjWindow"];
    if(self){
        _code=-1;
        _srid=srid;
        _tbl=wprj_gettable();
        if(_tbl==NULL){
_te_("GetProjTable failed");
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)dealloc{
_bTrace_("[NSProjPicker dealloc]",true);
_tm_((void*)self);
    if(_tbl){
        wtbl_free(_tbl);
        _tbl=NULL;
    }
    [super dealloc];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)awakeFromNib{
_bTrace_("[NSProjPicker awakeFromNib]",true);
    
    [_epgtxt setIntValue:_srid];
int tmp;
    for(long i=1;i<=_tbl->CountRecords();i++){
        _tbl->ReadVal(i,1,&tmp);
_tm_(_srid+" / "+tmp);
        if(_srid==tmp){
            [_prjtbl scrollRowToVisible:i-1];
            [_prjtbl selectRowIndexes:[NSIndexSet indexSetWithIndex:i-1] byExtendingSelection:NO];
            break;
        }
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)close{
_bTrace_("[NSProjPicker close]",true);
    [super close];
}

// ---------------------------------------------------------------------------
//
// -----------
-(int)srid{
    return _srid;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)runAppModal:(long*)code{
_bTrace_("[NSProjPicker runAppModal]",true);
    [[self window] makeKeyAndOrderFront:nil];
    [[self window] makeFirstResponder:nil];
    [NSApp runModalForWindow:[self window]];
    [[self window] setViewsNeedDisplay:NO];
    [[self window] orderOut:self];
    *code=_code;
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)validDialog:(id)sender{
    _code=1;
    [NSApp stopModal];
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)cancelDialog:(id)sender{
    _code=0;
    [NSApp stopModal];
}

#pragma mark ---- Gestion TableView ----
// -------------------------------------------------------------------------;--
//
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
    return _tbl->CountRecords();
}

// ---------------------------------------------------------------------------
//
// -----------
-(id)           tableView:(NSTableView*)aTableView
objectValueForTableColumn:(NSTableColumn*)aTableColumn
                      row:(NSInteger)rowIndex{
    if([[aTableColumn identifier] compare:@"epsgid"]==NSOrderedSame){
int     srid;
        _tbl->ReadVal(rowIndex+1,1,&srid);
        return [NSNumber numberWithInt:srid];
    }
    else if([[aTableColumn identifier] compare:@"projname"]==NSOrderedSame){
char    str[256];
        _tbl->ReadVal(rowIndex+1,2,str);
        return [NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
    }
    return nil;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)tableViewSelectionDidChange:(NSNotification*)notification{
int         srid;
    _tbl->ReadVal([_prjtbl selectedRow]+1,1,&srid);
bool		b;
bStdProj	p(srid,&b);
    if(b){
        /*if(p.is_latlong()){
            NSBeep();
            [_prjtbl deselectRow:[_prjtbl selectedRow]];
        }
        else */if(srid!=[_epgtxt intValue]){
            [_epgtxt setIntValue:srid];
            _srid=srid;
        }
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)controlTextDidChange:(NSNotification*)notification{
long    srid=[_epgtxt intValue];
    if(srid>0){
bool		b;
bStdProj	p(srid,&b);
        if(b){
            _srid=srid;
            /*if(p.is_latlong()){
                NSBeep();
            }
            else{*/
int             tmp;
                for(long i=1;i<=_tbl->CountRecords();i++){
                    _tbl->ReadVal(i,1,&tmp);
                    if(srid==tmp){
                        [_prjtbl scrollRowToVisible:i-1];
                        [_prjtbl selectRowIndexes:[NSIndexSet indexSetWithIndex:i-1] byExtendingSelection:NO];
                        break;
                    }
                }
            /*}*/
        }
    }
}

// ---------------------------------------------------------------------------
//
// ------------
@end

// ---------------------------------------------------------------------------
//
// ------------
bool runProjPicker (int* srid){
_bTrace_("NSProjPicker runProjPicker",true);

NSProjPicker        *controller;
NSAutoreleasePool   *localPool;
long                code;
    
    localPool=[[NSAutoreleasePool alloc] init];
    controller=[[NSProjPicker alloc] initWithSRID:(*srid)];

    [controller runAppModal:&code];
    
    (*srid)=[controller srid];
    
    [controller close];
    [controller release];
    [localPool release];
    
_tm_("srid="+(*srid));
_tm_("code="+(int)code);
    
    return code == 1;
}


