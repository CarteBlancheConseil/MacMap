//----------------------------------------------------------------------------
// File : bStdAlert.cpp
// Project : MacMap
// Purpose : C++ source file : Alert windows utility classes
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
// 24/01/2005 creation.
//----------------------------------------------------------------------------

#include "bStdAlert.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/mm_messages.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(	int kind, 
						int def_btn, 
						bool has_cancel, 
						const char* error_string, 
						const char* explanation_string){
	_res=false;
	
AlertStdCFStringAlertParamRec	prm;

    (void)GetStandardAlertDefaultParams(&prm,kStdCFStringAlertVersionOne);

	prm.defaultButton=def_btn;
	if(def_btn==kAlertStdAlertOKButton){
		prm.cancelButton=kAlertStdAlertCancelButton;
	}
	else if(def_btn==kAlertStdAlertCancelButton){
		prm.cancelButton=kAlertStdAlertOKButton;
	}	
	
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	message_string(kMsgOk,msg,1);
	prm.defaultText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	if(has_cancel){
		message_string(kMsgCancel,msg,1);
		prm.cancelText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	}
	
CFStringRef	err=CFStringCreateWithCString(kCFAllocatorDefault,error_string,kCFStringEncodingMacRoman);
CFStringRef	exp=CFStringCreateWithCString(kCFAllocatorDefault,explanation_string,kCFStringEncodingMacRoman);
	
DialogRef	alrt;
	
    (void)CreateStandardAlert(kind,err,exp,&prm,&alrt);
		
    (void)RunStandardAlert(alrt,NULL,&_hit);
  
	_res=(_hit==def_btn);
	CFRelease(err);
	CFRelease(exp);		
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(	int kind, 
						int def_btn, 
						bool has_cancel, 
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
	
AlertStdCFStringAlertParamRec	prm;

    (void)GetStandardAlertDefaultParams(&prm,kStdCFStringAlertVersionOne);

	prm.defaultButton=def_btn;
	if(def_btn==kAlertStdAlertOKButton){
		prm.cancelButton=kAlertStdAlertCancelButton;
	}
	else if(def_btn==kAlertStdAlertCancelButton){
		prm.cancelButton=kAlertStdAlertOKButton;
	}

char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	message_string(kMsgOk,msg,1);
	prm.defaultText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	if(has_cancel){
		message_string(kMsgCancel,msg,1);
		prm.cancelText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	}
	
CFStringRef	err=CFStringCreateWithCString(kCFAllocatorDefault,error_string,kCFStringEncodingMacRoman);
CFStringRef	exp=CFStringCreateWithCString(kCFAllocatorDefault,explanation_string,kCFStringEncodingMacRoman);
	
DialogRef	alrt;
	
    (void)CreateStandardAlert(kind,err,exp,&prm,&alrt);
		
    (void)RunStandardAlert(alrt,NULL,&_hit);
  
	_res=(_hit==def_btn);
	CFRelease(err);
	CFRelease(exp);		
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(int kind, 
					 int def_btn, 
					 bool has_cancel, 
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
	
AlertStdCFStringAlertParamRec	prm;
	
    (void)GetStandardAlertDefaultParams(&prm,kStdCFStringAlertVersionOne);

	prm.defaultButton=def_btn;
	if(def_btn==kAlertStdAlertOKButton){
		prm.cancelButton=kAlertStdAlertCancelButton;
	}
	else if(def_btn==kAlertStdAlertCancelButton){
		prm.cancelButton=kAlertStdAlertOKButton;
	}
	
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	message_string(kMsgOk,msg,1);
	prm.defaultText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	if(has_cancel){
		message_string(kMsgCancel,msg,1);
		prm.cancelText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	}
	
CFStringRef	err=CFStringCreateWithCString(kCFAllocatorDefault,error_string,kCFStringEncodingMacRoman);
CFStringRef	exp=CFStringCreateWithCString(kCFAllocatorDefault,explanation_string,kCFStringEncodingMacRoman);

DialogRef	alrt;
	
    (void)CreateStandardAlert(kind,err,exp,&prm,&alrt);
		
	SetDialogTimeout(alrt,def_btn,delay);
	
    (void)RunStandardAlert(alrt,NULL,&_hit);
	
	_res=(_hit==def_btn);
	CFRelease(err);
	CFRelease(exp);		
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdAlert::bStdAlert(int kind, 
					 int def_btn, 
					 bool has_cancel, 
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
	
AlertStdCFStringAlertParamRec	prm;
	
    (void)GetStandardAlertDefaultParams(&prm,kStdCFStringAlertVersionOne);

	prm.defaultButton=def_btn;
	if(def_btn==kAlertStdAlertOKButton){
		if(btn3){
			//prm.cancelButton=kAlertStdAlertOtherButton;
			prm.cancelButton=0;
		}
		else{
			prm.cancelButton=kAlertStdAlertCancelButton;
		}
	}
	else if(def_btn==kAlertStdAlertCancelButton){
		prm.cancelButton=kAlertStdAlertOKButton;
	}
	
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	if(btn1){
		strcpy(msg,btn1);
	}
	else{
		message_string(kMsgOk,msg,1);
	}
	prm.defaultText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	
	if(has_cancel){
		if(btn2){
			strcpy(msg,btn2);
		}
		else{
			message_string(kMsgCancel,msg,1);
		}
		prm.cancelText=CFStringCreateWithCString(kCFAllocatorDefault,msg,kCFStringEncodingMacRoman);
	}
	
	if(btn3){
		prm.otherText=CFStringCreateWithCString(kCFAllocatorDefault,btn3,kCFStringEncodingMacRoman);
	}

	
CFStringRef	err=CFStringCreateWithCString(kCFAllocatorDefault,error_string,kCFStringEncodingMacRoman);
CFStringRef	exp=CFStringCreateWithCString(kCFAllocatorDefault,explanation_string,kCFStringEncodingMacRoman);
	
DialogRef	alrt;
	
    (void)CreateStandardAlert(kind,err,exp,&prm,&alrt);
		
	SetDialogTimeout(alrt,def_btn,delay);
	
    (void)RunStandardAlert(alrt,NULL,&_hit);
	
	_res=(_hit==def_btn);
	CFRelease(err);
	CFRelease(exp);		
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
				:bStdAlert(	kAlertCautionAlert,
							kAlertStdAlertOKButton,
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
				:bStdAlert(	kAlertCautionAlert,
							kAlertStdAlertOKButton,
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
									UInt32 delay,
									const char* btn1, 
									const char* btn2,
									const char* btn3)
				:bStdAlert(	kAlertCautionAlert,
							kAlertStdAlertOKButton,
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
// Destructeur
// -----------
bAlertWarningYes::~bAlertWarningYes(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bAlertWarningNo::bAlertWarningNo(	const char* error_string, 
									const char* explanation_string)
				:bStdAlert(	kAlertCautionAlert,
							kAlertStdAlertCancelButton,
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
				:bStdAlert(	kAlertCautionAlert,
							kAlertStdAlertCancelButton,
							true,
							error_string,
							explanation_string,
							silent){
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
			:bStdAlert(	kAlertStopAlert,
						kAlertStdAlertOKButton,
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
			:bStdAlert(	kAlertStopAlert,
						kAlertStdAlertOKButton,
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
			:bStdAlert(	kAlertStopAlert,
						kAlertStdAlertOKButton,
						false,
						error_string,
						explanation_string,
						silent,
						delay){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bAlertStop::~bAlertStop(){
}
