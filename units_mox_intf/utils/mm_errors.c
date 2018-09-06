//----------------------------------------------------------------------------
// File : mm_errors.c
// Project : MacMap
// Purpose : C source file : Localization utils for errors
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

#include <mox_intf/mm_errors.h>

// ---------------------------------------------------------------------------
// 
// -----------
int error_num(int family_code, int error_code){
	return(-(family_code+error_code));
}

// ---------------------------------------------------------------------------
// 
// -----------
void error_string(int error_num, char* string){
char		num[__ERROR_STRING_LENGTH_MAX__];
	string[0]=0;
	sprintf(num,"%d",error_num);
CFStringRef	cfnum=CFStringCreateWithCString(kCFAllocatorDefault,num,kCFStringEncodingMacRoman);
CFStringRef err=CFCopyLocalizedString(cfnum,CFSTR("ERRORS"));
	CFRelease(cfnum);
	if(!err){
		return;
	}	
	CFStringGetCString(err,string,__ERROR_STRING_LENGTH_MAX__,kCFStringEncodingMacRoman);
	CFRelease(err);
}

// ---------------------------------------------------------------------------
// 
// -----------
void b_error_string(int error_num, char* string, CFBundleRef bndl){
char		num[__ERROR_STRING_LENGTH_MAX__];
	string[0]=0;
	sprintf(num,"%d",error_num);
CFStringRef	cfnum=CFStringCreateWithCString(kCFAllocatorDefault,num,kCFStringEncodingMacRoman);
CFStringRef err=CFBundleCopyLocalizedString(bndl,cfnum,CFSTR("ERRORS"),NULL);
	CFRelease(cfnum);
	if(!err){
		return;
	}
	CFStringGetCString(err,string,__ERROR_STRING_LENGTH_MAX__,kCFStringEncodingMacRoman);
	CFRelease(err);
}
