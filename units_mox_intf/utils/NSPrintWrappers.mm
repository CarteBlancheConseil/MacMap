//----------------------------------------------------------------------------
// File : NSPrintWrappers.mm
// Project : MacMap
// Purpose : Objective C++ source file : C wrappers around Printing
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2016 Carte Blanche Conseil.
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
// 26/07/2016 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bTrace.h>
#import "NSPrintWrappers.h"

// ---------------------------------------------------------------------------
//
// -----------
long NSRunPageLayout(PMPageFormat pf){
//_bTrace_("NSPrintWrappers.NSRunPageLayout",true);
long                ret=0;
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
NSPageLayout*       nspl=[NSPageLayout pageLayout];
    
    PMCopyPageFormat(pf,(PMPageFormat)[[NSPrintInfo sharedPrintInfo] PMPageFormat]);
    [[NSPrintInfo sharedPrintInfo] updateFromPMPageFormat];
    ret=[nspl runModal];
    [localPool release];
    return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
void NSCopyPageFormat(PMPageFormat pf){
//_bTrace_("NSPrintWrappers.NSCopyPageFormat",true);
    if([NSPrintInfo sharedPrintInfo]){
        PMCopyPageFormat((PMPageFormat)[[NSPrintInfo sharedPrintInfo] PMPageFormat],pf);
    }
    else{
        pf=kPMNoPageFormat;
    }
}

// ---------------------------------------------------------------------------
//
// -----------
long NSRunPrintPanel(PMPrintSettings pt){
//_bTrace_("NSPrintWrappers.NSRunPrintPanel",true);
long                ret=0;
NSAutoreleasePool*	localPool=[[NSAutoreleasePool alloc] init];
NSPrintPanel*       nspp=[NSPrintPanel printPanel];
NSView*             view=[[NSView alloc] init];

    PMCopyPrintSettings(pt,(PMPrintSettings)[[NSPrintInfo sharedPrintInfo] PMPrintSettings]);
    [[NSPrintInfo sharedPrintInfo] updateFromPMPrintSettings];

NSPrintOperation *op=[NSPrintOperation currentOperation];
    if(op==nil){
        op=[NSPrintOperation printOperationWithView:view];
        [NSPrintOperation setCurrentOperation:op];
    }
    
    ret=[nspp runModalWithPrintInfo:[NSPrintInfo sharedPrintInfo]];
    
    [localPool release];
    
    return ret;
}

// ---------------------------------------------------------------------------
//
// -----------
void NSCopyPrintSettings(PMPrintSettings pt){
//_bTrace_("NSPrintWrappers.NSCopyPrintSettings",true);
    if([NSPrintInfo sharedPrintInfo]){
        PMCopyPrintSettings((PMPrintSettings)[[NSPrintInfo sharedPrintInfo] PMPrintSettings],pt);
    }
    else{
        pt=kPMNoPrintSettings;
    }
}

// ---------------------------------------------------------------------------
//
// -----------
PMPrintSession NSGetPrintSession(){
//_bTrace_("NSPrintWrappers.NSGetPrintSession",true);
    if([NSPrintInfo sharedPrintInfo]){
        return (PMPrintSession)[[NSPrintInfo sharedPrintInfo] PMPrintSession];
    }
    return NULL;
}


