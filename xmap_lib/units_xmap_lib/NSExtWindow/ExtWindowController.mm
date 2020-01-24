//----------------------------------------------------------------------------
// File : ExtWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base window controller class for externs panels (Macro, Calcs, etc...)
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
// 31/12/2014 creation.
//----------------------------------------------------------------------------

#import "ExtWindowController.h"

#import <MacMapSuite/bTrace.h>

#import <mox_intf/NSUIUtils.h>
#import <mox_intf/bEventLog.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/endian.h>
#import <mox_intf/mm_messages.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation ExtPanel

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation ExtWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[ExtWindowController init]",true);
	self=[super init];
	if(self){
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[ExtWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
    [super awakeFromNib];
    _mgr=NULL;
    _xsign=_kNULLSign_;
    
    [_mgr_viw setTarget:self];
    [_mgr_viw setDoubleAction:@selector(doubleClick:)];
    
    [self updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[ExtWindowController close]",true);
_tm_((void*)self);
	[super close];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)checkMacMapEvent{
bGenericMacMapApp*
                gapp=(bGenericMacMapApp*)_ext->getapp();
bArray*			arr=gapp->eventMgr()->events();
bGenericEvent*	evt;
bGenericExt*	ext;
long			i,j;
    
    for(i=1;i<=arr->count();i++){
        arr->get(i,&evt);
        if(evt->kind()!=kEventKindUserExt){
            continue;
        }
        for(j=1;j<=evt->elements()->count();j++){
            evt->elements()->get(j,&ext);
            if(ext->getsubclass()==_xsign){
                [_mgr_viw reloadData];
            }
        }
    }
    [super checkMacMapEvent];
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doAdd:(id)sender{
_bTrace_("[ExtWindowController doAdd]",true);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doRemove:(id)sender{
_bTrace_("[ExtWindowController doRemove]",true);
NSInteger rowNumber=[_mgr_viw selectedRow];
_tm_(rowNumber);
    if(rowNumber==-1){
        return;
    }
    
bGenericMacMapApp*
            gapp=(bGenericMacMapApp*)_ext->getapp();
bEventLog	log(gapp,
                _ext->getbundle(),
                kXMapExtWdDeleteMsgID,
                GetSignature(_ext));
    
    if(!_mgr->remove(rowNumber+1)){
        NSBeep();
        return;
    }
    log.close();
    [_mgr_viw reloadData];
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doEdit:(id)sender{
_bTrace_("[ExtWindowController doEdit]",true);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doDuplicate:(id)sender{
_bTrace_("[ExtWindowController doDuplicate]",true);
NSInteger rowNumber=[_mgr_viw selectedRow];
_tm_(rowNumber);
    if(rowNumber==-1){
        return;
    }
bGenericMacMapApp*
    gapp=(bGenericMacMapApp*)_ext->getapp();
// Get source
bGenericExt* ext=_mgr->get(rowNumber+1);
    if(ext==NULL){
_te_("get failed for "+(rowNumber+1));
        NSBeep();
        return;
    }

// Mem dump
bMemFile	f;
char*		bf;
int			sz;
    ext->encode(0,&f);
    if(f.status()){
        _te_("file status ="+f.status());
        NSBeep();
        return;
    }
    f.mount(&bf,&sz);
    bf=(char*)realloc(bf,sz+1);
    bf[sz]=0;
    
// Making new root
bGenericXMLBaseElement	*root,*elt;
    if(!GetXMLParamFromBuffer(gapp,&root,bf,sz)){
_te_("GetXMLParamFromBuffer failed");
        free(bf);
        NSBeep();
        return;
    }
    free(bf);
    
// Rename
char	str1[_values_length_max_];
char	str2[_values_length_max_];
char	str3[_values_length_max_];
    
    elt=gapp->classMgr()->NthElement(root,1,"name");
    if(elt){
        elt->getvalue(str2);
    }
    else{
_te_("no name");
        return;
    }
    message_string(kMsgCopy,str3,true);
    sprintf(str1,"%s %s",str2,str3);
    
int k=0;
    do{
        k++;
        sprintf(str1,"%s %s-%d",str2,str3,k);
        if(!_mgr->find(str1)){
            break;
        }
    }while(true);
    elt->setvalue(str1);
_tm_("new name="+str1);
    
// New signature
UInt32		fsgn=UniqueSignature(gapp);
    sprintf(str2,"%.4s",&fsgn);
    elt=gapp->classMgr()->NthElement(root,1,"sign");
    if(elt){
        elt->setvalue(str2);
    }
    else{
_te_("no sign");
        NSBeep();
        return;
    }
    
// Pour calcs et vars
#ifdef __LITTLE_ENDIAN__
    elt=gapp->classMgr()->NthElement(root,2,"sign");
    if(elt){
        elt->getvalue(str2);
        fsgn=(*((UInt32*)str2));
        LBSwapWord(&fsgn,sizeof(UInt32));
        sprintf(str2,"%.4s",&fsgn);	
        elt->setvalue(str2);
    }
#endif
    
// Add
bEventLog	log(gapp,
                _ext->getbundle(),
                kXMapExtWdCloneMsgID,
                GetSignature(_ext));
    if(!_mgr->add(root)){
_te_("_mgr->add failed");
        NSBeep();
        return;
    }
    log.close();
    
    [_mgr_viw reloadData];
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
NSInteger rowNumber=[_mgr_viw selectedRow];
    [_rmv_btn setEnabled:(rowNumber>-1)];
    [_edt_btn setEnabled:(rowNumber>-1)];
    [_dup_btn setEnabled:(rowNumber>-1)];
}

#pragma mark ---- Gestion TableView ----
// ---------------------------------------------------------------------------
//
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
    return _mgr->count();
}

// ---------------------------------------------------------------------------
//
// -----------
-(id)           tableView:(NSTableView*)aTableView
objectValueForTableColumn:(NSTableColumn*)aTableColumn
                      row:(NSInteger)rowIndex{
char	name[256]="????";
    _mgr->ext_name(rowIndex+1,name);
    return [NSString stringWithCString:name encoding:NSMacOSRomanStringEncoding];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)tableViewSelectionDidChange:(NSNotification*)notification{
    [self updateUI];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)doubleClick:(id)object {
_bTrace_("[ExtWindowController doubleClick]",true);
NSInteger rowNumber=[_mgr_viw clickedRow];
UInt32    sign=_mgr->ext_signature(rowNumber+1);
#ifdef __LITTLE_ENDIAN__
    LBSwapWord(&sign,sizeof(UInt32));
#endif
    SendCommandToApplication(sign);
}

#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)prepareForClose{	
	[super prepareForClose];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
