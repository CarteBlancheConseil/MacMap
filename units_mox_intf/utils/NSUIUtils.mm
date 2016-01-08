//----------------------------------------------------------------------------
// File : NSUIUtils.mm
// Project : MacMap
// Purpose : Objective C++ source file : Cocoa interface utils
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
// 31/03/2011 Creation.
//----------------------------------------------------------------------------

#import "NSUIUtils.h"

// ---------------------------------------------------------------------------
// 
// -----------
@implementation NSControl (NSControlAdditions)

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setCharValue:(const char*)aString{
	if(aString!=NULL){
NSString*	nsstr=[NSString stringWithCString:aString encoding:NSMacOSRomanStringEncoding];
		[self setStringValue:nsstr];
	}
	else{
		[self setStringValue:@""];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonRemoveAllItems(NSPopUpButton* c){
	if([c pullsDown]==YES){
		for(NSInteger i=[c numberOfItems]-1;i>0;i--){
			[c removeItemAtIndex:i];
		}
	}
	else{
		[c removeAllItems];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonRemoveItemsFrom(NSPopUpButton* c, NSInteger from){
	if([c pullsDown]==YES){
//fprintf(stderr,"pullsDown==YES\n");
		for(NSInteger i=[c numberOfItems]-1;i>=from;i--){
//fprintf(stderr,"%d\n",i);
			[c removeItemAtIndex:i];
		}
	}
	else{
//fprintf(stderr,"pullsDown==NO\n");
		for(NSInteger i=[c numberOfItems]-1;i>from;i--){
//fprintf(stderr,"%d\n",i);
			[c removeItemAtIndex:i];
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSPopupButtonGetMenuItemValue(NSPopUpButton* c, long idx, char* str, size_t max){
    if(c==nil){
        return 0;
    }
NSString*	nsstr;
	if([c pullsDown]==YES){	
		nsstr=[c itemTitleAtIndex:idx];
	}
	else{
		nsstr=[c itemTitleAtIndex:(idx-1)];
	}
	if([nsstr getCString:str maxLength:max encoding:NSMacOSRomanStringEncoding]==NO){
		str[0]=0;
		return 0;
	}
	return 1;
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonAddMenuItemValue(NSPopUpButton* c, const char* str){
    if(c==nil){
        return;
    }
NSString*	nsstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
char		str2[256];
long		n=2;

	while([c indexOfItemWithTitle:nsstr]>-1){
		sprintf(str2,"%s (%ld)",str,n);
		nsstr=[NSString stringWithCString:str2 encoding:NSMacOSRomanStringEncoding];
		n++;
	}
	[c addItemWithTitle:nsstr];
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonPopulateWithFields(NSPopUpButton* c, bGenericType* tp, long start, long current){	
char	str[256];
	for(long i=start;i<=tp->fields()->count();i++){
		tp->fields()->get_name(i,str);
		NSPopupButtonAddMenuItemValue(c,str);
	}
	if([c pullsDown]==YES){	
		[c selectItemAtIndex:(current)];
	}
	else{
		[c selectItemAtIndex:(current-1)];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonPopulateWithTypes(NSPopUpButton* c, bGenericMacMapApp* gapp, long kind, long current){
char			str[256];
bGenericType*	tp;
NSMenuItem*		item;
	
	for(long i=1;i<=gapp->typesMgr()->count();i++){
		tp=gapp->typesMgr()->get(i);
		tp->name(str);
		NSPopupButtonAddMenuItemValue(c,str);
		if((kind!=kBaseNoKind)&&(tp->kind()!=kind)){
			item=[c itemAtIndex:[c numberOfItems]-1];
			[item setEnabled:NO];
		}		
	}
	if([c pullsDown]==YES){	
		[c selectItemAtIndex:(current)];
	}
	else{
		[c selectItemAtIndex:(current-1)];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonPopulateWithScales(NSPopUpButton* c, bGenericMacMapApp* gapp, long current){	
char				str[256];
bGenericUnitMgr*	mgr=gapp->scaleMgr();
bGenericUnit*		u;	
	
	for(long i=1;i<=mgr->count();i++){
		u=mgr->get(i);
		u->long_name(str);
		if(strlen(str)==0){
			u->short_name(str);
		}
		if(strlen(str)==0){
			sprintf(str,"%.0f",u->coef());;
		}
		NSPopupButtonAddMenuItemValue(c,str);
	}
	if([c pullsDown]==YES){	
		[c selectItemAtIndex:(current)];
	}
	else{
		[c selectItemAtIndex:(current-1)];
	}
}

// ---------------------------------------------------------------------------
//
// ------------
void NSPopupButtonPopulateWithUnits(NSPopUpButton* c, bGenericMacMapApp* gapp, long current){
char				str[256];
bGenericUnitMgr*	mgr=gapp->distMgr();
bGenericUnit*		u;
    
    for(long i=1;i<=mgr->count();i++){
        u=mgr->get(i);
        u->long_name(str);
        if(strlen(str)==0){
            u->short_name(str);
        }
        if(strlen(str)==0){
            sprintf(str,"%.0f",u->coef());;
        }
        NSPopupButtonAddMenuItemValue(c,str);
    }
    if([c pullsDown]==YES){
        [c selectItemAtIndex:(current)];
    }
    else{
        [c selectItemAtIndex:(current-1)];
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonPopulateWithConstrainedFields(NSPopUpButton* c, bGenericType* tp, long start, long current){	
NSPopupButtonPopulateWithFields(c,tp,start,current);
	for(long i=start;i<=tp->fields()->count();i++){
		if(tp->fields()->count_constraints(i)==0){
			NSPopupButtonMenuItemDisable(c,i-start);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonPopulateWithDashes(NSPopUpButton* c, bGenericType* tp, long current){	
char	str[256];
	for(long i=1;i<=tp->fields()->count_param("dashes");i++){
		tp->fields()->get_param_name("dashes",i,str);
		NSPopupButtonAddMenuItemValue(c,str);
	}
	if([c pullsDown]==YES){	
		[c selectItemAtIndex:(current)];
	}
	else{
		[c selectItemAtIndex:(current-1)];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonMenuItemEnable(NSPopUpButton* c, long index){
	if((index<0)||(index>=[c numberOfItems])){
		fprintf(stderr,"PAS DE ITEM\n");
	}
	else{
NSMenuItem* item=[c itemAtIndex:index];
		[item setEnabled:YES];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonMenuItemDisable(NSPopUpButton* c, long index){
	if((index<0)||(index>=[c numberOfItems])){
		fprintf(stderr,"PAS DE ITEM\n");
	}
	else{
NSMenuItem* item=[c itemAtIndex:index];
		[item setEnabled:NO];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSPopupButtonMenuItemGetTag(NSPopUpButton* c, long index){
NSMenuItem* item=[c itemAtIndex:index];
	if(item){
		return([item tag]);
	}
	else{
		fprintf(stderr,"PAS DE ITEM\n");
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSPopupButtonMenuItemSetTitle(NSPopUpButton* c, long index, const char* title){
NSMenuItem* item=[c itemAtIndex:index];
	if(item){
NSString*	nstr=[NSString stringWithCString:title encoding:NSMacOSRomanStringEncoding];
		[item setTitle:nstr];
	}
	else{
		fprintf(stderr,"PAS DE ITEM\n");
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSTextFieldGetValue(NSTextField* c, char* str, size_t max){
NSString*	nsstr=[c stringValue];
	if([nsstr getCString:str maxLength:max encoding:NSMacOSRomanStringEncoding]==NO){
		str[0]=0;
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
void NSTextFieldSetValue(NSTextField* c, const char* str){
NSString*	nsstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
	[c setStringValue:nsstr];
//	[c setCharValue:str];
}

// ---------------------------------------------------------------------------
// 
// -----------
NSImage* GetNSImagePattern(bGenericMacMapApp* gapp,
						   bGenericType* tp,
						   const char* name){
	if(strlen(name)==0){
		return(NULL);
	}
void*	data;
int		sz;
	if(!tp->fields()->get_param("patterns",name,&data,&sz)){
		return(NULL);
	}
NSData*		nsdata=[NSData dataWithBytes:data length:sz];
NSImage*	nspdf=[[NSImage alloc] initWithData:nsdata];
	return(nspdf);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGRect NSMainSreenBounds(){
	return(NSRectToCGRect([[NSScreen mainScreen] frame]));
}

// ---------------------------------------------------------------------------
// 
// -----------
CGFloat NSMenuBarHeight(){
	return([NSMenuView menuBarHeight]);
}

