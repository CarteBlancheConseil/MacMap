//----------------------------------------------------------------------------
// File : bvDefPaletteQuickWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base subclass for quick (standard) vDef window controller
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------

#import "bvDefPaletteQuickWindowController.h"
#import "bvDefPaletteQuick.h"
#import "bvDefPaletteQuickStyle.h"

#import <MacMapSuite/bTrace.h>

#import <mox_intf/bGenericMacMapApp.h>
#import <mox_intf/Carb_Utils.h>
#import <mox_intf/bEventLog.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/Color_Utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/NSUIUtils.h> 

#import <std_ext/bXMapStdIntf.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bvDefPaletteQuickWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bvDefPaletteQuickWindowController init]",true);
	self=[super init];
	if(self){
		_last_click=-1;
		_hprev=YES;
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bvDefPaletteQuickWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[bvDefPaletteQuickWindowController awakeFromNib]",true);

// Nom du type et preview
char str[256];
	_ext->type_get()->name(str);
	[_tnm_fld setCharValue:str];
    [_tab_viw selectTabViewItemAtIndex:0];

	[_prvw installController:self];
	
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)_ext->getapp();
// Visibilité
	NSPopupButtonRemoveAllItems(_smn_pop);
	NSPopupButtonPopulateWithScales(_smn_pop,gapp,1);
	NSPopupButtonRemoveAllItems(_smx_pop);
	NSPopupButtonPopulateWithScales(_smx_pop,gapp,gapp->scaleMgr()->count());
	
// Options
    [_o_clf_pop setAutoenablesItems:NO];
	NSPopupButtonRemoveAllItems(_o_clf_pop);
	NSPopupButtonPopulateWithConstrainedFields(_o_clf_pop,_ext->type_get(),kOBJ_Name_,1);
	NSPopupButtonPopulateWithScales(_o_srf_pop,gapp,1);
	NSPopupButtonRemoveAllItems(_o_cnf_pop);
	NSPopupButtonPopulateWithFields(_o_cnf_pop,_ext->type_get(),kOBJ_Name_,1);
	NSPopupButtonRemoveAllItems(_o_xof_pop);
	NSPopupButtonPopulateWithFields(_o_xof_pop,_ext->type_get(),kOBJ_Dir_+1,1);
	NSPopupButtonRemoveAllItems(_o_yof_pop);
	NSPopupButtonPopulateWithFields(_o_yof_pop,_ext->type_get(),kOBJ_Dir_+1,2);
	
	[self plot2Intf];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bvDefPaletteQuickWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Actions principal ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleMin:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseScaleMin]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smn_pop indexOfSelectedItem]+1);
bvDefPaletteQuickStyle*	stl;

	for(long i=1;i<=_ext->runs().count();i++){
		if([_run_tbl isRowSelected:i-1]==YES){
			_ext->runs().get(i,&stl);
			stl->_smin=u->coef();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleMax:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseScaleMax]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smx_pop indexOfSelectedItem]+1);
bvDefPaletteQuickStyle*	stl;

	for(long i=1;i<=_ext->runs().count();i++){
		if([_run_tbl isRowSelected:i-1]==YES){
			_ext->runs().get(i,&stl);
			stl->_smax=u->coef()+1.0;
		}
	}
}


// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doClickPreview:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doClickPreview]",true);
}

#pragma mark ---- Actions options ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassField:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseClassField]",true);
	_ext->fld_put([_o_clf_pop indexOfSelectedItem]+kOBJ_Name_);
	[self plot2Intf];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleRef:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseScaleRef]",true);
	if([_o_srf_pop indexOfSelectedItem]==0){
		_ext->sref_put(0);
	}
	else{
bGenericUnit*	u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_o_srf_pop indexOfSelectedItem]-1);
		_ext->sref_put(u->coef());
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseUnit:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseUnit]",true);
	switch([_o_unt_pop indexOfSelectedItem]){
		case 0:
			_ext->coef_put(1);
			break;
		case 1:
			_ext->coef_put(_pm_coef_);
			break;	
		default:
			_ext->coef_put(1);
			break;
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseDrawOrder:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseDrawOrder]",true);
	_ext->sort_put([_o_ord_chk intValue]==0?2:-2);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseColorSpace:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseColorSpace]",true);
	_ext->cmyk_put([_o_cls_chk intValue]);
	[self convertColor];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckCondition:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doCheckCondition]",true);
	if([_o_dif_chk intValue]==1){
		[self doChooseCondField:sender];
		[self doChooseCondOperator:sender];
		[self doPutCondValue:sender];
	}
	else{
		_ext->cfld_put(0);
		_ext->cop_put(1);
		_ext->cval_put("");
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseCondField:(id)sender{
	_bTrace_("[bvDefPaletteQuickWindowController doChooseCondField]",true);
	_ext->cfld_put([_o_cnf_pop indexOfSelectedItem]+kOBJ_Name_);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseCondOperator:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseCondOperator]",true);
	_ext->cop_put([_o_cno_pop indexOfSelectedItem]+1);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutCondValue:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doPutCondValue]",true);
char	val[256];
	NSTextFieldGetValue(_o_val_fld,val,sizeof(val)-1);
	_ext->cval_put(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckOffset:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doCheckOffset]",true);
bvDefPaletteQuickStyle*	stl;
	if([_o_off_chk intValue]==1){
		[self doChooseOffsetXField:sender];
		[self doChooseOffsetYField:sender];
	}
	else{
		for(long i=1;i<=_ext->runs().count();i++){
			_ext->runs().get(i,&stl);
			stl->_offx=0;
			stl->_offy=0;
		}
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseOffsetXField:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseOffsetXField]",true);
bvDefPaletteQuickStyle*	stl;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_offx=[_o_xof_pop indexOfSelectedItem]+kOBJ_Dir_+1;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseOffsetYField:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doChooseOffsetYField]",true);
bvDefPaletteQuickStyle*	stl;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_offy=[_o_yof_pop indexOfSelectedItem]+kOBJ_Dir_+1;
	}		
}

#pragma mark ---- Actions appliquer ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doApply:(id)sender{
_bTrace_("[bvDefPaletteQuickWindowController doApply]",true);
	_ext->update();
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_prvw setNeedsDisplay:YES];
	[self checkIntfAvailability];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)checkIntfAvailability{
BOOL	state=NO;
	if([_run_tbl numberOfSelectedRows]>0){
		state=YES;
	}
	
	[_smn_pop setEnabled:state];
	[_smx_pop setEnabled:state];
	
// Options
	state=([_o_dif_chk intValue]==1);
	[_o_cnf_pop setEnabled:state];
	[_o_cno_pop setEnabled:state];
	[_o_val_fld setEnabled:state];
	
	if(_ext->type_get()->fields()->count()==kOBJ_Dir_){
		state=NO;
		[_o_off_chk setEnabled:state];
	}
	else{
		[_o_off_chk setEnabled:YES];
		state=([_o_off_chk intValue]==1);
	}
	[_o_xof_pop setEnabled:state];
	[_o_yof_pop setEnabled:state];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)plot2Intf{
_bTrace_("[bvDefPaletteQuickWindowController plot2Intf]",true);
long					idx,cur=[_run_tbl selectedRow]+1;
bvDefPaletteQuickStyle*	stl;
bGenericMacMapApp*		gapp=(bGenericMacMapApp*)_ext->getapp();
	
	if(cur){
		_ext->runs().get(cur,&stl);
		
		idx=GetBestScaleIndexForValue(gapp,stl->_smin);
		[_smn_pop selectItemAtIndex:(idx-1)];
		idx=GetBestScaleIndexForValue(gapp,stl->_smax);
		[_smx_pop selectItemAtIndex:(idx-1)];				
	}
	else{
		[_smn_pop selectItemAtIndex:0];
		[_smx_pop selectItemAtIndex:(gapp->scaleMgr()->count()-1)];		
	}
	
// Options + Visibilité
	[_o_clf_pop selectItemAtIndex:(_ext->fld_get()-kOBJ_Name_)];
	if(_ext->sref_get()==0){
		idx=1;
	}
	else{
		idx=GetBestScaleIndexForValue(gapp,_ext->sref_get())+2;
	}
	[_o_srf_pop selectItemAtIndex:(idx-1)];
	[_o_unt_pop selectItemAtIndex:(_ext->coef_get()==1)?0:1];
	[_o_ord_chk setIntValue:(_ext->sort_get()<0?1:0)];
	[_o_cls_chk setIntValue:_ext->cmyk_get()];
	
	if(_ext->runs().count()>0){
		_ext->runs().get(1,&stl);
		[_o_xof_pop selectItemAtIndex:((stl->_offx)?(stl->_offx-kOBJ_Dir_-1):0)];
		[_o_yof_pop selectItemAtIndex:((stl->_offy)?(stl->_offy-kOBJ_Dir_-1):1)];
		if((stl->_offx)&&(stl->_offy)){
			[_o_off_chk setIntValue:1];
		}
		else{
			[_o_off_chk setIntValue:0];
		}
	}
	else{
		[_o_xof_pop selectItemAtIndex:0];
		[_o_yof_pop selectItemAtIndex:1];
		[_o_off_chk setIntValue:0];
	}
	
	if(_ext->cfld_get()){
		[_o_dif_chk setIntValue:1];
	}
	else{
		[_o_dif_chk setIntValue:0];
	}
	[_o_cnf_pop selectItemAtIndex:((_ext->cfld_get())?(_ext->cfld_get()-kOBJ_Name_):1)];
	[_o_cno_pop selectItemAtIndex:((_ext->cop_get())?(_ext->cop_get()-1):0)];
char	val[256];
	_ext->cval_get(val);
	[_o_val_fld setCharValue:val];
}	

// ---------------------------------------------------------------------------
// 
// -----------
-(void)convertColor{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)drawPreview:(CGRect)rect context:(CGContextRef)ctx{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)clickPreview:(CGPoint)point modifiers:(NSUInteger)mod{
_bTrace_("[bvDefPaletteQuickWindowController clickPreview]",true);
NSRect				nsr=[_prvw frame];
double				w=_hprev?nsr.size.width/(double)_ext->runs().count():nsr.size.height/(double)_ext->runs().count();
NSUInteger			x=_hprev?floor(point.x/w):(_ext->runs().count()-1)-floor(point.y/w);
NSMutableIndexSet*	set;
	
	if((NSCommandKeyMask&mod)!=0){
		if([_run_tbl isRowSelected:x]==YES){
			[_run_tbl deselectRow:x];
		}
		else{
			[_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
		}
	}
	else if((NSShiftKeyMask&mod)!=0){
		set=[NSMutableIndexSet indexSet];
		if(_last_click>-1){
			if(_last_click<x){
				for(long i=_last_click+1;i<=x;i++){
					[set addIndex:i];
				}
			}
			else{
				for(long i=x;i<_last_click;i++){
					[set addIndex:i];
				}
			}
			[_run_tbl selectRowIndexes:set byExtendingSelection:YES];
		}
		else{
			[_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
		}
	}
	else{
		[_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSInteger)getPopupValueForField:(NSPopUpButton*)popup{
NSInteger   r=[popup indexOfSelectedItem];
	if(r==0){
		return(0);
	}
	return(r-2+kOBJ_Name_);
}

#pragma mark ---- Gestion TableView ----
// ---------------------------------------------------------------------------
// 
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
//_bTrace_("[bvDefPaletteQuickWindowController numberOfRowsInTableView]",true);
	return _ext->runs().count();
}

// ---------------------------------------------------------------------------
// 
// -----------
-(id)	tableView:(NSTableView*)aTableView 
		objectValueForTableColumn:(NSTableColumn*)aTableColumn 
		row:(NSInteger)rowIndex{
//_bTrace_("[bvDefPaletteQuickWindowController tableView objectValueForTableColumn]",true);
int		k=_ext->type_get()->fields()->get_constraints_kind(_ext->fld_get());
int		d;
char	val[_values_length_max_];
char	pc[_values_length_max_];
double	pf;
int		pi;
	
	_ext->type_get()->fields()->get_decs(_ext->fld_get(),&d);
	switch(k){
		case _int:
		case _bool:
			_ext->type_get()->fields()->get_constraint(_ext->fld_get(),rowIndex+1,&pi);
			sprintf(val,"%d",pi);
			break;
		case _double:
		case _date:
		case _time:
			_ext->type_get()->fields()->get_constraint(_ext->fld_get(),rowIndex+1,&pf);
			sprintf(val,"%f",pf);
			break;
		default:
			_ext->type_get()->fields()->get_constraint(_ext->fld_get(),rowIndex+1,pc);
			xToChar(k,d,pc,val);
			break;
	}
	return [NSString stringWithCString:val encoding:NSMacOSRomanStringEncoding];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)tableViewSelectionDidChange:(NSNotification*)notification{
//_bTrace_("[bvDefPaletteQuickWindowController tableViewSelectionDidChange]",true);
	[self plot2Intf];
	[self updateUI];
	_last_click=[_run_tbl selectedRow];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

