//----------------------------------------------------------------------------
// File : NSContextualMenuWrappers.mm
// Project : MacMap
// Purpose : Objective C++ source file : NSMenu C wrappers
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
// 25/03/2014 Creation. Can handle 2 levels popup menus.
//----------------------------------------------------------------------------

#import "NSContextualMenuWrappers.h"
#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Dummy class
// ------------
@interface MenuContextPreview : NSView{
    NSMenu*     _mnu;   // Root menu
    NSMenu*     _sub;   // Current submenu checked
    NSMenuItem* _cur;   // Current subitem checked
	long        _choice;// Returned choice
}
@end

// ---------------------------------------------------------------------------
// Dummy class implementation
// ------------
@implementation MenuContextPreview : NSView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[MenuContextPreview init]",true);
	if((self=[super init])!=nil){
		_mnu=[[NSMenu alloc] initWithTitle:@"Contextual Menu"];
		[_mnu setAutoenablesItems:NO];
        _sub=nil;
        _cur=nil;
        _choice=0;
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
	[_mnu release];
    _cur=nil;
    _sub=nil;
    _choice=0;
	[super dealloc];
}

// ---------------------------------------------------------------------------
// add Items
// ------------
-(void)populateMenu:(menuitem_desc*)desc count:(long)n{
_bTrace_("[MenuContextPreview populateMenu]",true);
NSMenuItem*	item;
long        k=0;
	for(long i=0;i<n;i++){
        if(desc[i].index>0){
            continue;
        }
		if(strcmp("-",desc[i].name)==0){
			[_mnu insertItem:[NSMenuItem separatorItem] atIndex:k];
		}
		else{
			item=[_mnu insertItemWithTitle:[NSString stringWithCString:desc[i].name encoding:NSMacOSRomanStringEncoding] 
									action:@selector(action:) 
							 keyEquivalent:@"" 
								   atIndex:k];
            [item setState:(desc[i].checked?NSOnState:NSOffState)];
            if(desc[i].checked){
                _cur=item;
                _sub=_mnu;
            }
			if(desc[i].disabled){
				[item setEnabled:NO];
			}
		}
        k++;
	}

NSMenu*	submnu;
    for(long i=0;i<n;i++){
        if(desc[i].index==0){
            continue;
        }
        item=[_mnu itemAtIndex:desc[i].index-1];
        if(item){
            submnu=[item submenu];
            if(submnu==nil){
                //submnu=[[[NSMenu alloc] initWithTitle:@"Contextual SubMenu" ] autorelease];
                submnu=[[NSMenu alloc] initWithTitle:@"Contextual SubMenu" ];
                [submnu setAutoenablesItems:NO];
                [item setSubmenu:submnu];
            }
            if(strcmp("-",desc[i].name)==0){
                [submnu insertItem:[NSMenuItem separatorItem] atIndex:[submnu numberOfItems]];
            }
            else{
                item=[submnu insertItemWithTitle:[NSString stringWithCString:desc[i].name encoding:NSMacOSRomanStringEncoding]
                                        action:@selector(action:)
                                 keyEquivalent:@""
                                       atIndex:[submnu numberOfItems]];
                [item setState:(desc[i].checked?NSOnState:NSOffState)];
                if(desc[i].checked){
                    _cur=item;
                    _sub=submnu;
               }
                if(desc[i].disabled){
                    [item setEnabled:NO];
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Selector
// ------------
-(void)action:(NSMenuItem*)item{
_bTrace_("[MenuContextPreview action]",true);
	_choice=[_mnu indexOfItem:item];
    if(_choice==-1){
NSMenuItem*	subitem;
long        subchoice;
        for(long i=0;i<[_mnu numberOfItems];i++){
            subitem=[_mnu itemAtIndex:i];
            if([subitem hasSubmenu]){
                subchoice=[[subitem submenu] indexOfItem:item];
                if(subchoice>=0){
                    _choice=(i+1)*10000+subchoice;
                    break;
                }
            }
        }
    }
    else{
        _choice++;
    }
}

// ---------------------------------------------------------------------------
// getter
// ------------
-(long)popMenu:(NSEvent*)event{
_bTrace_("[MenuContextPreview popMenu]",true);
   if(_cur==nil){// Non checked menu
_tm_("Non checked menu");
        [NSMenu popUpContextMenu:_mnu withEvent:event forView:self];
    }
    else{// Checked menu
_tm_("Checked menu");
NSMenuItem* item=_cur;
        if(_sub!=_mnu){// Search submenu
NSMenuItem*	subitem;
            for(long i=0;i<[_mnu numberOfItems];i++){
                subitem=[_mnu itemAtIndex:i];
                if([subitem hasSubmenu]){
                    if([[subitem submenu] indexOfItem:_cur]>=0){
                        item=subitem;
                        break;
                    }
                }
            }
        }
        
NSRect  nsr=NSMakeRect([event locationInWindow].x,[event locationInWindow].y,0,0);

        nsr=[[event window] convertRectToScreen:nsr];
        
        if([_mnu popUpMenuPositioningItem:item
                            atLocation:nsr.origin
                                   inView:nil]){
            item=[_mnu highlightedItem];
            if(item){
                if([item hasSubmenu]){// If submenu, action will be send to it
                    _sub=[item submenu];
                    item=[_sub highlightedItem];
                    if(item){
                        [self action:[_sub highlightedItem]];
                    }
                }
                else{// else, send action to the item
                    [self action:[_mnu highlightedItem]];
                }
            }
        }
    }
_tm_(_choice);
	return(_choice);
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
//	C-based style wrapper routines around NSMenu
// ------------
long popUpContextMenuWithNSEvent(void* event,
								 menuitem_desc* desc,
								 long n){
_bTrace_("NSContextualMenuWrappers popUpContextMenuWithNSEvent",true);
MenuContextPreview	*view;
NSAutoreleasePool	*localPool;
	
    localPool=[[NSAutoreleasePool alloc] init];
    view=[[MenuContextPreview alloc] init];
	[view populateMenu:desc count:n];
	 
NSEvent*	evt=(__bridge NSEvent*)event;
long		choice=[view popMenu:evt];
	
	[view release];
	[localPool release];
	return(choice);
}

// ---------------------------------------------------------------------------
//	C-based style wrapper routines around NSMenu
// ------------
long popUpContextMenuWithCGPointAndNSWindow(CGPoint locationInWindow,
											void* window,
											menuitem_desc* desc,
											long n,
                                            CGFloat unused){
_bTrace_("NSContextualMenuWrappers popUpContextMenuWithCGPointAndNSWindow",true);
NSPoint		nsp=NSMakePoint(locationInWindow.x,locationInWindow.y);
NSWindow*	wd=(__bridge NSWindow*)window;
// Building an NSEvent with proper NSWindow and Location
NSEvent*	evt= [NSEvent otherEventWithType:NSEventTypeApplicationDefined
									location:nsp
							   modifierFlags:NSEventTypeApplicationDefined 
								   timestamp:(NSTimeInterval) 0
								windowNumber:[wd windowNumber]
                                     //context:[NSGraphicsContext currentContext]
                                     context:[wd graphicsContext]
									 subtype:0
									   data1:0
									   data2:0];
	return(popUpContextMenuWithNSEvent((__bridge void*)evt,desc,n));
}
