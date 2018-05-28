//----------------------------------------------------------------------------
// File : bvDefPaletteRangeWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base subclass for range vDef window controller
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
// 20/08/2011 creation.
//----------------------------------------------------------------------------

#import "bvDefPaletteRangeWindowController.h"
#import "bvDefPaletteRange.h"
#import "bvDefPaletteRangeStyle.h"
#import "vdef_utils.h"

#import <MacMapSuite/bTrace.h>
#import <MacMapStats/MMsDiscretization.h>

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
@implementation bvDefPaletteRangeWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bvDefPaletteRangeWindowController init]",true);
	self=[super init];
	if(self){
		_last_click=-1;
		_hprev=YES;
		_qmin=0;
		_qmax=0;
		_nbclass=5;
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bvDefPaletteRangeWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[bvDefPaletteRangeWindowController awakeFromNib]",true);

// Nom du type et preview
char str[256];
	_ext->type_get()->name(str);
	[_tnm_fld setCharValue:str];
    [_tab_viw selectTabViewItemAtIndex:0];

	[_prvw installController:self];

	((bvDefPaletteRange*)_ext)->limits().get(1,&_qmin);
	((bvDefPaletteRange*)_ext)->limits().get(((bvDefPaletteRange*)_ext)->limits().count(),&_qmax);
	_nbclass=_ext->runs().count();
	
	NSPopupButtonRemoveAllItems(_fld_pop);
	NSPopupButtonPopulateWithFields(_fld_pop,_ext->type_get(),kOBJ_SubType_,1);
	[_ncl_fld setIntValue:_nbclass];
	[_min_fld setFloatValue:_qmin];
	[_max_fld setFloatValue:_qmax];
	
	_tm_("_qmin :"+_qmin);
	_tm_("_qmax :"+_qmax);
	
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)_ext->getapp();
	
	NSPopupButtonRemoveAllItems(_clr_pop);
int	nc=countColors(gapp);
	for(int i=1;i<=nc;i++){
		getColorName(gapp,i,str);
		NSPopupButtonAddMenuItemValue(_clr_pop,str);
	}
	
// Visibilité
	NSPopupButtonRemoveAllItems(_smn_pop);
	NSPopupButtonPopulateWithScales(_smn_pop,gapp,1);
	NSPopupButtonRemoveAllItems(_smx_pop);
	NSPopupButtonPopulateWithScales(_smx_pop,gapp,gapp->scaleMgr()->count());
	
// Options
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
_bTrace_("[bvDefPaletteRangeWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Actions principal ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseField:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseField]",true);
	_ext->fld_put([_fld_pop indexOfSelectedItem]+kOBJ_SubType_);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseMethod:(id)sender{
	_bTrace_("[bvDefPaletteRangeWindowController doChooseMethod]",true);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutClassCount:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doPutClassCount]",true);
	_nbclass=[_ncl_fld intValue];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseMin:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseMin]",true);
int		k=[_min_pop indexOfSelectedItem];
	getMin((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),_ext->fld_get(),k,&_qmin);
	[_min_fld setFloatValue:_qmin];
_tm_(k+"->"+_qmin);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutMin:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doPutMin]",true);
	_qmin=[_min_fld floatValue];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseMax:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseMax]",true);
int		k=[_max_pop indexOfSelectedItem];
	getMax((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),_ext->fld_get(),k,&_qmax);
	[_max_fld setFloatValue:_qmax];
_tm_(k+"->"+_qmax);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutMax:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doPutMax]",true);
	_qmax=[_max_fld floatValue];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCompute:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doCompute]",true);	

	_tm_("field :"+_ext->fld_get());
	_tm_("_qmin :"+_qmin);
	_tm_("_qmax :"+_qmax);

MMsMatrix*	mx;
	getMatrix((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),_ext->fld_get(),_qmin,_qmax,&mx);

MMsMatrix*	clss;

	if(_nbclass<=0){
		_nbclass=d_estimate_class_count(mx);
	}
	
	if(_qmax<=_qmin){
		_qmax=LONG_MAX;
	}
	
	switch([_dsc_pop indexOfSelectedItem]+1){
		case 1:
			clss=d_equal_range(mx,1,_nbclass);
			break;
		case 2:
			clss=d_equal_manning(mx,1,_nbclass);
			break;
		case 3:
			clss=d_standardized(mx,1,_nbclass);
			break;
		case 4:
			clss=d_arithmetic_progression(mx,1,_nbclass);
			break;
		case 5:
			clss=d_geometric_progression(mx,1,_nbclass);
			break;
		case 6:
			clss=d_quantile(mx,1,_nbclass);
			break;
		case 7:
			clss=d_natural_breaks(mx,1,_nbclass);
			break;
		case 8:
			clss=d_bertin_average(mx,1,_nbclass);
			break;
		case 9:
			clss=d_bertin_median(mx,1,_nbclass);
			break;
	}
	
//_tm_("matrix");
//MMsDumpTTxt(mx,stderr);
_tm_("classes");
MMsDumpTTxt(clss,stderr);
	MMsMatrixFree(mx);
	
double	e;

	((bvDefPaletteRange*)_ext)->limits().reset();
	for(int i=1;i<=clss->nl;i++){
		e=MMsGetDouble(clss,i,1);
		((bvDefPaletteRange*)_ext)->limits().add(&e);
	}
	_nbclass=((bvDefPaletteRange*)_ext)->limits().count()-1;
	MMsMatrixFree(clss);
	
	((bvDefPaletteRange*)_ext)->limits().get(1,&_qmin);
	((bvDefPaletteRange*)_ext)->limits().get(((bvDefPaletteRange*)_ext)->limits().count(),&_qmax);
		
	((bvDefPaletteRange*)_ext)->rebuild_limits();
	
	[_run_tbl reloadData];
	[self plot2Intf];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doAddClass:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doAddClass]",true);
double	a,b;
int		idx=_last_click+1;
	
	if(idx<=0){
		idx=_ext->runs().count();
	}
	((bvDefPaletteRange*)_ext)->limits().get(idx,&a);
	((bvDefPaletteRange*)_ext)->limits().get(idx+1,&b);
	a=(a+b)/2.0;
	((bvDefPaletteRange*)_ext)->limits().insert(idx+1,&a);
	_last_click=idx;
	((bvDefPaletteRange*)_ext)->stl_alloc(idx);
	_nbclass++;

//	_last_click=idx;
	[_run_tbl reloadData];
	[_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:idx-1] byExtendingSelection:NO];
	
//	[self plot2Intf];
//	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doRemoveClass:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doRemoveClass]",true);
int		idx=_last_click+1;
	if(idx==0){
		return;
	}
	if(((bvDefPaletteRange*)_ext)->limits().count()<3){
		return;
	}
	if(idx==1){
		((bvDefPaletteRange*)_ext)->limits().rmv(2);
	}
	else{
		((bvDefPaletteRange*)_ext)->limits().rmv(idx);
	}
bvDefPaletteRangeStyle*	stl;
	_ext->runs().get(idx,&stl);
	delete stl;
	_ext->runs().rmv(idx);
	_nbclass--;	

	[_run_tbl deselectAll:(id)sender];
	[_run_tbl reloadData];

//	[self plot2Intf];
//	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseColorimetry:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseColorimetry]",true);
double					cmin[5],cmax[5];
double					cx;
bvDefPaletteRangeStyle*	stl;	
	
	getColor((bGenericMacMapApp*)_ext->getapp(),[_clr_pop indexOfSelectedItem]+1,cmin,cmax,_ext->cmyk_get());
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		cx=(double)(i-1)/(double)(_ext->runs().count()-1);
		stl->_fcolor[0]=cmin[0]+(cmax[0]-cmin[0])*cx;
		stl->_fcolor[1]=cmin[1]+(cmax[1]-cmin[1])*cx;
		stl->_fcolor[2]=cmin[2]+(cmax[2]-cmin[2])*cx;
		if(_ext->cmyk_get()){
			stl->_fcolor[3]=cmin[3]+(cmax[3]-cmin[3])*cx;
		}
		stl->_tcolor[0]=stl->_fcolor[0];
		stl->_tcolor[1]=stl->_fcolor[1];
		stl->_tcolor[2]=stl->_fcolor[2];
		stl->_tcolor[3]=stl->_fcolor[3];
		stl->_tcolor[4]=stl->_fcolor[4];
		stl->_fld[0]=0;
		stl->_visible=true;
	}
	//[self plot2Intf];
	[self updateUI];	
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseFillKind:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseFillKind]",false);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseFillColor:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseFillColor]",false);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseFillPattern:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseFillPattern]",false);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseFillFromColor:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseFillFromColor]",false);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseFillToColor:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseFillToColor]",false);
_tw_("NEED SUBCLASSER");
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleMin:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseScaleMin]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smn_pop indexOfSelectedItem]+1);
bvDefPaletteRangeStyle*	stl;

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
_bTrace_("[bvDefPaletteRangeWindowController doChooseScaleMax]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smx_pop indexOfSelectedItem]+1);
bvDefPaletteRangeStyle*	stl;

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
_bTrace_("[bvDefPaletteRangeWindowController doClickPreview]",true);
}

#pragma mark ---- Actions options ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleRef:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseScaleRef]",true);
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
_bTrace_("[bvDefPaletteRangeWindowController doChooseUnit]",true);
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
_bTrace_("[bvDefPaletteRangeWindowController doChooseDrawOrder]",true);
	_ext->sort_put([_o_ord_chk intValue]==0?2:-2);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseColorSpace:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseColorSpace]",true);
	_ext->cmyk_put([_o_cls_chk intValue]);
	[self convertColor];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckCondition:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doCheckCondition]",true);
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
	_bTrace_("[bvDefPaletteRangeWindowController doChooseCondField]",true);
	_ext->cfld_put([_o_cnf_pop indexOfSelectedItem]+kOBJ_Name_);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseCondOperator:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseCondOperator]",true);
	_ext->cop_put([_o_cno_pop indexOfSelectedItem]+1);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutCondValue:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doPutCondValue]",true);
char	val[256];
	NSTextFieldGetValue(_o_val_fld,val,sizeof(val)-1);
	_ext->cval_put(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckOffset:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doCheckOffset]",true);
bvDefPaletteRangeStyle*	stl;
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
_bTrace_("[bvDefPaletteRangeWindowController doChooseOffsetXField]",true);
bvDefPaletteRangeStyle*	stl;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_offx=[_o_xof_pop indexOfSelectedItem]+kOBJ_Dir_+1;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseOffsetYField:(id)sender{
_bTrace_("[bvDefPaletteRangeWindowController doChooseOffsetYField]",true);
bvDefPaletteRangeStyle*	stl;
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
_bTrace_("[bvDefPaletteRangeWindowController doApply]",true);
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
	[_rmv_btn setEnabled:[_run_tbl numberOfSelectedRows]==1];

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
_bTrace_("[bvDefPaletteRangeWindowController plot2Intf]",true);
long					idx,cur=[_run_tbl selectedRow]+1;
bvDefPaletteRangeStyle*	stl;
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
	[_fld_pop selectItemAtIndex:(_ext->fld_get()-kOBJ_SubType_)];
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
		[_ncl_fld setIntValue:_ext->runs().count()];
		
double	x;
		((bvDefPaletteRange*)_ext)->limits().get(1,&x);
		[_min_fld setDoubleValue:x];
		((bvDefPaletteRange*)_ext)->limits().get(((bvDefPaletteRange*)_ext)->limits().count(),&x);
		[_max_fld setDoubleValue:x];
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
_bTrace_("[bvDefPaletteRangeWindowController clickPreview]",true);
NSRect				nsr=[_prvw frame];
double				w=_hprev?nsr.size.width/(double)_ext->runs().count():nsr.size.height/(double)_ext->runs().count();
NSInteger			x=_hprev?floor(point.x/w):(_ext->runs().count()-1)-floor(point.y/w);
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

#pragma mark ---- Gestion TableView ----
// ---------------------------------------------------------------------------
// 
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
//_bTrace_("[bvDefPaletteRangeWindowController numberOfRowsInTableView]",true);
	return _ext->runs().count();
}

// ---------------------------------------------------------------------------
// 
// -----------
-(id)	tableView:(NSTableView*)aTableView 
		objectValueForTableColumn:(NSTableColumn*)aTableColumn 
		row:(NSInteger)rowIndex{
//_bTrace_("[bvDefPaletteRangeWindowController tableView objectValueForTableColumn]",true);
int			d;
char		val[_values_length_max_];
double		v;
NSString*	identifier=[aTableColumn identifier];
	
	_ext->type_get()->fields()->get_decs(_ext->fld_get(),&d);
	if(d==0){
		d=2;
	}
	if([identifier compare:@"0"]==NSOrderedSame){
		((bvDefPaletteRange*)_ext)->limits().get(rowIndex+1,&v);
	}
	else{
		((bvDefPaletteRange*)_ext)->limits().get(rowIndex+2,&v);
	}
	xToChar(_double,d,&v,val);
	return [NSString stringWithCString:val encoding:NSMacOSRomanStringEncoding];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)	tableView:(NSTableView*)aTableView
   setObjectValue:(id)object
   forTableColumn:(NSTableColumn*)aTableColumn
              row:(NSInteger)rowIndex{
bvDefPaletteRangeStyle*	stl;
NSString*               identifier=[aTableColumn identifier];
NSString*               value=(NSString*)object;
double                  v=[value doubleValue];
    
    if([identifier compare:@"0"]==NSOrderedSame){
        ((bvDefPaletteRange*)_ext)->limits().put(rowIndex+1,&v);
        if(_ext->runs().get(rowIndex+1,&stl)){
            stl->_vmin=v;
        }
        if(_ext->runs().get(rowIndex,&stl)){
            stl->_vmax=v;
        }
    }
    else{
        ((bvDefPaletteRange*)_ext)->limits().put(rowIndex+2,&v);
        if(_ext->runs().get(rowIndex+1,&stl)){
            stl->_vmax=v;
        }
        if(_ext->runs().get(rowIndex+2,&stl)){
            stl->_vmin=v;
        }
    }
    
    [self plot2Intf];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)tableViewSelectionDidChange:(NSNotification*)notification{
	[self plot2Intf];
	[self updateUI];
	_last_click=[_run_tbl selectedRow];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
