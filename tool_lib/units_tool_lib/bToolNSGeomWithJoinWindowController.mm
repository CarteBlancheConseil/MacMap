//----------------------------------------------------------------------------
// File : bToolNSGeomWithJoinWindowController
// Project : MacMap
// Purpose : Objective C++ source file : Tool Create window controller
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2017 Carte Blanche Conseil.
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
// 28/03/2017 creation.
//----------------------------------------------------------------------------

#import "bToolNSGeomWithJoinWindowController.h"
#import "bToolNSGeomWithJoin.h"

#import <mox_intf/NSUIUtils.h>

#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bToolNSGeomWithJoinWindowController
// ---------------------------------------------------------------------------
// 
// ------------
/*-(void)dealloc{
_bTrace_("[bToolNSGeomWithJoinWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}*/

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[bToolNSGeomWithJoinWindowController awakeFromNib]",true);
    [super awakeFromNib];
    
bToolNSGeomWithJoin*    ext=(bToolNSGeomWithJoin*)_ext;
char                    val[32];
    
    [_nea_chk setIntValue:ext->get_use_nea()];
    [_dir_chk setIntValue:ext->get_use_dir()];
    [_ang_chk setIntValue:ext->get_use_ang()];
    [_ext_chk setIntValue:ext->get_use_end()];
    
    val[1]=0;
    
    val[0]=ext->get_use_nea_key();
    [_nea_txt setCharValue:val];
    val[0]=ext->get_use_dir_key();
    [_dir_txt setCharValue:val];
    val[0]=ext->get_use_ang_key();
    [_ang_txt setCharValue:val];
    val[0]=ext->get_use_end_key();
    [_ext_txt setCharValue:val];

    if(!ext->get_nea()){
        [_nea_chk setEnabled:NO];
        [_nea_txt setEnabled:NO];
    }
    
    if(!ext->get_dir()){
        [_dir_chk setEnabled:NO];
        [_dir_txt setEnabled:NO];
    }
    
    if(!ext->get_ang()){
        [_ang_chk setEnabled:NO];
        [_ang_txt setEnabled:NO];
    }
    
    if(!ext->get_end()){
        [_ext_chk setEnabled:NO];
        [_ext_txt setEnabled:NO];
    }
    
}

#pragma mark ---- Gestion TableView ----
// ---------------------------------------------------------------------------
//
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
bGenericMacMapApp*  gapp=(bGenericMacMapApp*)_ext->getapp();
    return gapp->typesMgr()->count();
}

// ---------------------------------------------------------------------------
//
// -----------
-(id)           tableView:(NSTableView*)aTableView
objectValueForTableColumn:(NSTableColumn*)aTableColumn
                      row:(NSInteger)rowIndex{
NSString*           identifier=[aTableColumn identifier];
bGenericMacMapApp*  gapp=(bGenericMacMapApp*)_ext->getapp();
    
    if([identifier compare:@"check"]==NSOrderedSame){
bToolNSGeomWithJoin*    ext=(bToolNSGeomWithJoin*)_ext;
bGenericType*           tp=NULL;
        
        ext->get_types().get(rowIndex+1,&tp);
        return [NSNumber numberWithInt:(tp!=NULL)];
    }
    else if([identifier compare:@"name"]==NSOrderedSame){
char            value[256];
bGenericType*   tp=gapp->typesMgr()->get(rowIndex+1);
        tp->name(value);
        return [NSString stringWithCString:value encoding:NSMacOSRomanStringEncoding];
    }
    return nil;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)	tableView:(NSTableView*)aTableView
   setObjectValue:(id)object
   forTableColumn:(NSTableColumn*)aTableColumn
              row:(NSInteger)rowIndex{
NSString*           identifier=[aTableColumn identifier];
    
    if([identifier compare:@"check"]==NSOrderedSame){
NSNumber*               value=(NSNumber*)object;
bToolNSGeomWithJoin*    ext=(bToolNSGeomWithJoin*)_ext;
bGenericType*           tp=NULL;
        
        if([value boolValue]==YES){
bGenericMacMapApp*  gapp=(bGenericMacMapApp*)_ext->getapp();
            tp=gapp->typesMgr()->get(rowIndex+1);
        }
        ext->get_types().put(rowIndex+1,&tp);
    }
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)validDialog:(id)sender{
bToolNSGeomWithJoin*    ext=(bToolNSGeomWithJoin*)_ext;
char                    val[32];
    
    ext->set_use_nea([_nea_chk intValue]);
    ext->set_use_dir([_dir_chk intValue]);
    ext->set_use_ang([_ang_chk intValue]);
    ext->set_use_end([_ext_chk intValue]);

    val[0]=0;
    NSTextFieldGetValue(_nea_txt,val,31);
    ext->set_use_nea_key(val[0]);

    val[0]=0;
    NSTextFieldGetValue(_dir_txt,val,31);
    ext->set_use_dir_key(val[0]);

    val[0]=0;
    NSTextFieldGetValue(_ang_txt,val,31);
    ext->set_use_ang_key(val[0]);

    val[0]=0;
    NSTextFieldGetValue(_ext_txt,val,31);
    ext->set_use_end_key(val[0]);
    
    ext->putKeys();
    
    [super validDialog:sender];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
