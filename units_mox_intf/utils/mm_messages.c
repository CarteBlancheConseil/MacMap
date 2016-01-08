//----------------------------------------------------------------------------
// File : mm_messages.c
// Project : MacMap
// Purpose : C source file : Localization utils (messages, words, etc...)
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
// 03/02/2005 creation.
//----------------------------------------------------------------------------

#include <mox_intf/mm_messages.h>

// ---------------------------------------------------------------------------
// 
// -----------
void message_string(const char* msg_id, char* string, int maj){
	string[0]=0;
CFStringRef	cfid=CFStringCreateWithCString(kCFAllocatorDefault,msg_id,kCFStringEncodingMacRoman);
CFStringRef msg=CFCopyLocalizedString(cfid,CFSTR("MESSAGES"));
	CFRelease(cfid);
	if(!msg){
		return;
	}
	CFStringGetCString(msg,string,__MESSAGE_STRING_LENGTH_MAX__,kCFStringEncodingMacRoman);
	CFRelease(msg);
	if(maj){
		string[0]=toupper(string[0]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void b_message_string(const char* msg_id, char* string, CFBundleRef bndl, int maj){
	string[0]=0;
CFStringRef	cfid=CFStringCreateWithCString(kCFAllocatorDefault,msg_id,kCFStringEncodingMacRoman);
CFStringRef msg=CFBundleCopyLocalizedString(bndl,cfid,CFSTR("MESSAGES"),NULL);	
	CFRelease(cfid);
	if(!msg){
		return;
	}
	CFStringGetCString(msg,string,__MESSAGE_STRING_LENGTH_MAX__,kCFStringEncodingMacRoman);
	CFRelease(msg);
	if(maj){
		string[0]=toupper(string[0]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void message_cfstring(const char* msg_id, CFStringRef* string){
CFStringRef	cfid=CFStringCreateWithCString(kCFAllocatorDefault,msg_id,kCFStringEncodingMacRoman);
	(*string)=CFCopyLocalizedString(cfid,CFSTR("MESSAGES"));
	CFRelease(cfid);
}

// ---------------------------------------------------------------------------
// 
// -----------
void b_message_cfstring(const char* msg_id, CFStringRef* string, CFBundleRef bndl){
CFStringRef	cfid=CFStringCreateWithCString(kCFAllocatorDefault,msg_id,kCFStringEncodingMacRoman);
	(*string)=CFBundleCopyLocalizedString(bndl,cfid,CFSTR("MESSAGES"),NULL);	
	CFRelease(cfid);
}

// ---------------------------------------------------------------------------
// 
// -----------
void get_localized_name(char* string, CFBundleRef bndl){
	string[0]=0;
CFStringRef	name=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(bndl,CFSTR("CFBundleName"));
	if(!name){
fprintf(stderr,"NO CFBundleName\n");
		return;
	}
	CFStringGetCString(name,string,__MESSAGE_STRING_LENGTH_MAX__,kCFStringEncodingMacRoman);
}


