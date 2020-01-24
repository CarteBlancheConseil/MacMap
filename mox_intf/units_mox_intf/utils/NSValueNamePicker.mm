//----------------------------------------------------------------------------
// File : NSValueNamePicker.mm
// Project : MacMap
// Purpose : Objective C++ source file : Name and value picker window
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
// 10/07/2019 Creation.
//----------------------------------------------------------------------------

#import "NSValueNamePicker.h"

#import <mox_intf/NSUIUtils.h>

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/valconv.h>

#import <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------

@interface NSNamePicker : NSWindowController{
    IBOutlet NSTextField*	_valtxt;
    
    IBOutlet NSButton*      _okbtn;
    IBOutlet NSButton*      _cancelbtn;

    long                    _code;
    
    char                    _name[256];
    char                    _title[256];
    bool                    _cncl;
}

//----------------------------------------------------------------------------
// Std
-(id)init;
-(void)setTitle:(const char*)text;
-(void)setCancel:(bool)cncl;
-(void)name:(char*)text;
-(void)setName:(char*)text;
-(void)runAppModal:(long*)code;

//----------------------------------------------------------------------------
// Validation Dialogue
-(IBAction)validDialog:(id)sender;
-(IBAction)cancelDialog:(id)sender;

//----------------------------------------------------------------------------

@end



//----------------------------------------------------------------------------

@interface NSValuePicker : NSWindowController{
    IBOutlet NSTextField*	_prttxt;
    IBOutlet NSTextField*	_valtxt;
    IBOutlet NSPopUpButton*	_valpop;
    
    IBOutlet NSButton*      _okbtn;
    IBOutlet NSButton*      _cancelbtn;
    
    long                    _code;
    
    char                    _value[256];
    char                    _title[256];
    bGenericType*           _tp;
    int                     _fld;
}

//----------------------------------------------------------------------------
// Std
-(id)initWithType:(bGenericType*)tp;
-(void)setTitle:(const char*)text;
-(void)setField:(int)fld;
-(void)value:(char*)text;
-(void)setValue:(char*)text;
-(void)runAppModal:(long*)code;

//----------------------------------------------------------------------------
// Validation Dialogue
-(IBAction)validDialog:(id)sender;
-(IBAction)cancelDialog:(id)sender;
-(IBAction)doChoose:(id)sender;

//----------------------------------------------------------------------------

@end




// ---------------------------------------------------------------------------
//
// ------------
@implementation NSNamePicker
// ---------------------------------------------------------------------------
//
// ------------
-(id)init{
_bTrace_("[NSNamePicker init]",true);
    self=[self initWithWindowNibName:@"MMNameWindow"];
    if(self){
        _code=-1;
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)dealloc{
_bTrace_("[NSNamePicker dealloc]",true);
_tm_((void*)self);

    [super dealloc];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)awakeFromNib{
_bTrace_("[NSNamePicker awakeFromNib]",true);
    [[self window] setTitle:[NSString stringWithCString:_title encoding:NSMacOSRomanStringEncoding]];
    NSTextFieldSetValue(_valtxt,_name);
    if(!_cncl){
        [_cancelbtn setHidden:YES];
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)close{
_bTrace_("[NSNamePicker close]",true);
    [super close];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setTitle:(const char*)text{
    if(text){
        strcpy(_title,text);
    }
    else{
        strcpy(_title,"");
    }
    [[self window] setTitle:[NSString stringWithCString:_title encoding:NSMacOSRomanStringEncoding]];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setCancel:(bool)cncl{
    if(!cncl){
        [_cancelbtn setHidden:YES];
    }
    _cncl=cncl;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)name:(char*)text{
    NSTextFieldGetValue(_valtxt,text,255);
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setName:(char*)text{
_bTrace_("[NSNamePicker setName]",true);
    if(text){
        strcpy(_name,text);
    }
    else{
        strcpy(_name,"");
    }
    NSTextFieldSetValue(_valtxt,_name);
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)runAppModal:(long*)code{
_bTrace_("[NSNamePicker runAppModal]",true);
    [[self window] makeKeyAndOrderFront:nil];
    [[self window] makeFirstResponder:_valtxt];
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

// ---------------------------------------------------------------------------
//
// ------------
@end

// ---------------------------------------------------------------------------
//
// ------------
@implementation NSValuePicker
// ---------------------------------------------------------------------------
//
// ------------
-(id)initWithType:(bGenericType*)tp{
_bTrace_("[NSValuePicker initWithType]",true);
    self=[self initWithWindowNibName:@"MMValueWindow"];
    if(self){
        _code=-1;
        _tp=tp;
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)dealloc{
_bTrace_("[NSValuePicker dealloc]",true);
_tm_((void*)self);
    [super dealloc];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)awakeFromNib{
_bTrace_("[NSValuePicker awakeFromNib]",true);
    NSTextFieldSetValue(_prttxt,_title);
    NSTextFieldSetValue(_valtxt,_value);

    NSPopupButtonRemoveAllItems(_valpop);
    if((_tp==NULL)||(_fld<=0)){
        [_valpop setEnabled:NO];
    }
    else if(_tp->fields()->count_constraints(_fld)<=0){
        [_valpop setEnabled:NO];
    }
    else{
        NSPopupButtonPopulateWithConstraints(_valpop,_tp,_fld,1);
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)close{
    _bTrace_("[NSValuePicker close]",true);
    [super close];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setTitle:(const char*)text{
    if(text){
        strcpy(_title,text);
    }
    else{
        strcpy(_title,"");
    }
    NSTextFieldSetValue(_prttxt,_title);
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setField:(int)fld{
    _fld=fld;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)value:(char*)text{
    NSTextFieldGetValue(_valtxt,text,255);
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setValue:(char*)text{
_bTrace_("[NSValuePicker setValue]",true);
    if(text){
        strcpy(_value,text);
    }
    else{
        strcpy(_value,"");
    }
    NSTextFieldSetValue(_valtxt,_value);
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)runAppModal:(long*)code{
_bTrace_("[NSValuePicker runAppModal]",true);
    [[self window] makeKeyAndOrderFront:nil];
    [[self window] makeFirstResponder:_valtxt];
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

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)doChoose:(id)sender{
    if(_fld<0){
        return;
    }
    if(_tp==NULL){
        return;
    }
int	idx=[_valpop indexOfSelectedItem]+1;
    if(idx){
char	str[1024];
int		ck=_tp->fields()->get_constraints_kind(_fld);
int		d,fk;
        _tp->fields()->get_decs(_fld,&d);
        _tp->fields()->get_kind(_fld,&fk);
        if(fk!=ck){
            sprintf(str,"%d",idx);
        }
        else{
char	buffer[1024];
            _tp->fields()->get_constraint(_fld,idx,buffer);
            xToChar(ck,d,buffer,str);
        }
        NSTextFieldSetValue(_valtxt,str);
    }
}

// ---------------------------------------------------------------------------
//
// ------------
@end



// ---------------------------------------------------------------------------
//
// ------------
bool runNamePicker  (char* name,
                     const char* wtitle,
                     bool cncl){
_bTrace_("NSValuePicker runNamePicker",true);
NSNamePicker        *controller;
NSAutoreleasePool   *localPool;
long                code;
    
    localPool=[[NSAutoreleasePool alloc] init];
    controller=[[NSNamePicker alloc] init];
    
    [controller setTitle:wtitle];
    [controller setCancel:cncl];
    [controller setName:name];
    
    [controller runAppModal:&code];
    
    [controller name:name];

    [controller close];
    [controller release];
    [localPool release];
    
    return code == 1;
}

// ---------------------------------------------------------------------------
//
// ------------
bool runValuePicker (bGenericType* tp,
                     int field,
                     const char* title,
                     char* value){
_bTrace_("NSValuePicker runValuePicker",true);
NSValuePicker       *controller;
NSAutoreleasePool   *localPool;
long                code;
    
    localPool=[[NSAutoreleasePool alloc] init];
    controller=[[NSValuePicker alloc] initWithType:tp];
    
    [controller setTitle:title];
    [controller setField:field];
    [controller setValue:value];
    
    [controller runAppModal:&code];
    
    [controller value:value];
    
    [controller close];
    [controller release];
    [localPool release];
    
    return code == 1;
}
