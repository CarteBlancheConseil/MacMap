//----------------------------------------------------------------------------
// File : bStdAlert.cpp
// Project : MacMap
// Purpose : C++ source file : Alert windows utility classes
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
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
// 24/01/2005 creation.
// 02/01/2018 Cocoa porting.
//----------------------------------------------------------------------------

#include "bStdAlert.h"
#include <mox_intf/mm_messages.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Timer utils class
// ------------
@interface AlertTimer : NSObject{
    NSAlert* _alert;
}

// ---------------------------------------------------------------------------
-(void)setAlert:(NSAlert *)alert;
-(void)killWindow:(NSTimer *)timer;

// ---------------------------------------------------------------------------
@end

// ---------------------------------------------------------------------------
// Timer utils class implementation
// ------------
@implementation AlertTimer : NSObject

// ---------------------------------------------------------------------------
//
// ------------
-(void)setAlert:(NSAlert*)alert{
    _alert=alert;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)killWindow:(NSTimer*)timer{
    [[NSApplication sharedApplication] abortModal];
}

// ---------------------------------------------------------------------------
//
// ------------
@end

// ---------------------------------------------------------------------------
// Alert utils
// ------------
static long runAlert(NSAlertStyle style,
                    int def_btn,
                    bool use_cancel,
                    const char* error_string,
                    const char* explanation_string,
                    UInt32 delay,
                    const char* btn1,
                    const char* btn2,
                    const char* btn3,
                    void* window){
NSAlert *alert=[[NSAlert alloc] init];
    
NSString*    str1=(btn1?[NSString stringWithCString:btn1 encoding:NSMacOSRomanStringEncoding]:NSLocalizedString(@"ok", @""));
NSString*    str2=(btn2?[NSString stringWithCString:btn2 encoding:NSMacOSRomanStringEncoding]:NSLocalizedString(@"cancel", @""));
NSString*    str3=(btn3?[NSString stringWithCString:btn3 encoding:NSMacOSRomanStringEncoding]:nil);
    
    [alert addButtonWithTitle:str1];
    if(use_cancel){
        [alert addButtonWithTitle:str2];
        if(str3){
            [alert addButtonWithTitle:str3];
       }
    }
    
    [alert setMessageText:[NSString stringWithCString:error_string encoding:NSMacOSRomanStringEncoding]];
    [alert setInformativeText:[NSString stringWithCString:explanation_string encoding:NSMacOSRomanStringEncoding]];
    
    [alert setAlertStyle:style];

AlertTimer*  atm=nil;
    
long ret=0;
    if(window){
        [alert beginSheetModalForWindow:(NSWindow*)window completionHandler:^(NSModalResponse ret) {
            if (ret == NSAlertSecondButtonReturn) {
                NSLog(@"Delete was cancelled!");
                return;
            }
            
            NSLog(@"This project was deleted!");
        }];
    }
    else{
        if(delay>0){
            atm=[[AlertTimer alloc] init];
            [atm setAlert:alert];
            [atm performSelector:@selector(killWindow:) withObject:nil afterDelay:delay];
        }

        ret=[alert runModal];
    }
    
    if(atm!=nil){
        if(ret!=0){
            [NSObject cancelPreviousPerformRequestsWithTarget:atm];
        }
        //[atm release];
    }
    
    //[alert release];
    
    return ret;
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(	long kind, 
						int def_btn, 
						bool use_cancel, 
						const char* error_string, 
						const char* explanation_string){
    _hit=runAlert((NSAlertStyle)kind,def_btn,use_cancel,error_string,explanation_string,0,nil,nil,nil,nil);
	_res=(_hit==def_btn);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(	long kind, 
						int def_btn, 
						bool use_cancel, 
						const char* error_string, 
						const char* explanation_string,
						bool silent){
	_res=false;
	if(silent){
fprintf(stderr,"ALERT : %s\n",error_string);
fprintf(stderr,"%s\n",explanation_string);
		_res=true;
		return;
	}
    _hit=runAlert((NSAlertStyle)kind,def_btn,use_cancel,error_string,explanation_string,0,nil,nil,nil,nil);
	_res=(_hit==def_btn);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(long kind, 
					 int def_btn, 
					 bool use_cancel, 
					 const char* error_string, 
					 const char* explanation_string,
					 bool silent,
					 UInt32 delay){
	_res=false;
	if(silent){
fprintf(stderr,"ALERT : %s\n",error_string);
fprintf(stderr,"%s\n",explanation_string);
		_res=true;
		return;
	}
    _hit=runAlert((NSAlertStyle)kind,def_btn,use_cancel,error_string,explanation_string,delay,nil,nil,nil,nil);
	_res=(_hit==def_btn);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(long kind, 
					 int def_btn, 
					 bool use_cancel, 
					 const char* error_string, 
					 const char* explanation_string,
					 bool silent,
					 UInt32 delay,
					 const char* btn1, 
					 const char* btn2,
					 const char* btn3){
	_res=false;
	if(silent){
fprintf(stderr,"ALERT : %s\n",error_string);
fprintf(stderr,"%s\n",explanation_string);
		_res=true;
		return;
	}
    _hit=runAlert((NSAlertStyle)kind,def_btn,use_cancel,error_string,explanation_string,delay,btn1,btn2,btn3,nil);
	_res=(_hit==def_btn);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(long kind,
                     int def_btn,
                     bool use_cancel,
                     const char* error_string,
                     const char* explanation_string,
                     bool silent,
                     UInt32 delay,
                     const char* btn1,
                     const char* btn2,
                     const char* btn3,
                     void* window){
    _res=false;
    if(silent){
        fprintf(stderr,"ALERT : %s\n",error_string);
        fprintf(stderr,"%s\n",explanation_string);
        _res=true;
        return;
    }
    _hit=runAlert((NSAlertStyle)kind,def_btn,use_cancel,error_string,explanation_string,delay,btn1,btn2,btn3,window);
    _res=(_hit==def_btn);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdAlert::~bStdAlert(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bool bStdAlert::result(){
	return(_res);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningYes::bAlertWarningYes(	const char* error_string, 
									const char* explanation_string)
				:bStdAlert(	NSWarningAlertStyle,
							NSAlertFirstButtonReturn,
							true,
							error_string,
							explanation_string){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningYes::bAlertWarningYes(	const char* error_string,
									const char* explanation_string,
									bool silent)
                :bStdAlert(	NSWarningAlertStyle,
                            NSAlertFirstButtonReturn,
							true,
							error_string,
							explanation_string,
							silent){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningYes::bAlertWarningYes(	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                   UInt32 delay)
                :bStdAlert(	NSWarningAlertStyle,
                            NSAlertFirstButtonReturn,
                            true,
                            error_string,
                            explanation_string,
                            silent,
                            delay){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningYes::bAlertWarningYes(	const char* error_string, 
									const char* explanation_string,
									bool silent,
									UInt32 delay,
									const char* btn1, 
									const char* btn2,
									const char* btn3)
				:bStdAlert(	NSWarningAlertStyle,
                            NSAlertFirstButtonReturn,
							true,
							error_string,
							explanation_string,
							silent,
							delay,
							btn1,
							btn2,
							btn3){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningYes::bAlertWarningYes(	const char* error_string,
                                   const char* explanation_string,
                                   bool silent,
                                   UInt32 delay,
                                   const char* btn1,
                                   const char* btn2,
                                   const char* btn3,
                                   void* window)
                :bStdAlert(NSWarningAlertStyle,
                           NSAlertFirstButtonReturn,
                           true,
                           error_string,
                           explanation_string,
                           silent,
                           delay,
                           btn1,
                           btn2,
                           btn3,
                           window){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bAlertWarningYes::~bAlertWarningYes(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(	const char* error_string, 
									const char* explanation_string)
				:bStdAlert(	NSWarningAlertStyle,
							NSAlertSecondButtonReturn,
							true,
							error_string,
							explanation_string){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(	const char* error_string, 
									const char* explanation_string,
									bool silent)
				:bStdAlert(	NSWarningAlertStyle,
                            NSAlertSecondButtonReturn,
							true,
							error_string,
							explanation_string,
							silent){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay)
                :bStdAlert(	NSWarningAlertStyle,
                            NSAlertSecondButtonReturn,
                            true,
                            error_string,
                            explanation_string,
                            silent,
                            delay){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(   const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay,
                                    const char* btn1,
                                    const char* btn2,
                                    const char* btn3)
                :bStdAlert(	NSWarningAlertStyle,
                            NSAlertSecondButtonReturn,
                            true,
                            error_string,
                            explanation_string,
                            silent,
                            delay,
                            btn1,
                            btn2,
                            btn3){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(const char* error_string,
                                 const char* explanation_string,
                                 bool silent,
                                 UInt32 delay,
                                 const char* btn1,
                                 const char* btn2,
                                 const char* btn3,
                                 void* window)
                :bStdAlert(NSWarningAlertStyle,
                           NSAlertSecondButtonReturn,
                           true,
                           error_string,
                           explanation_string,
                           silent,
                           delay,
                           btn1,
                           btn2,
                           btn3,
                           window){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bAlertWarningNo::~bAlertWarningNo(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertStop	::bAlertStop(	const char* error_string, 
							const char* explanation_string)
			:bStdAlert(	NSCriticalAlertStyle,
                        NSAlertFirstButtonReturn,
						false,
						error_string,
						explanation_string){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertStop	::bAlertStop(	const char* error_string, 
							const char* explanation_string,
							bool silent)
			:bStdAlert(	NSCriticalAlertStyle,
                        NSAlertFirstButtonReturn,
						false,
						error_string,
						explanation_string,
						silent){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertStop	::bAlertStop(	const char* error_string, 
							const char* explanation_string,
							bool silent,
							UInt32 delay)
			:bStdAlert(	NSCriticalAlertStyle,
                        NSAlertFirstButtonReturn,
						false,
						error_string,
						explanation_string,
						silent,
						delay){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertStop::bAlertStop( const char* error_string,
                        const char* explanation_string,
                        bool silent,
                        UInt32 delay,
                        const char* btn1,
                        const char* btn2,
                        const char* btn3)
            :bStdAlert(	NSCriticalAlertStyle,
                        NSAlertFirstButtonReturn,
                        false,
                        error_string,
                        explanation_string,
                        silent,
                        delay,
                        btn1,
                        btn2,
                        btn3){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertStop::bAlertStop( const char* error_string,
                       const char* explanation_string,
                       bool silent,
                       UInt32 delay,
                       const char* btn1,
                       const char* btn2,
                       const char* btn3,
                       void* window)
            :bStdAlert(	NSCriticalAlertStyle,
                       NSAlertFirstButtonReturn,
                       false,
                       error_string,
                       explanation_string,
                       silent,
                       delay,
                       btn1,
                       btn2,
                       btn3,
                       window){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bAlertStop::~bAlertStop(){
}
