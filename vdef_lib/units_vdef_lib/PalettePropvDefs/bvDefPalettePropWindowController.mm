//----------------------------------------------------------------------------
// File : bvDefPalettePropWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base subclass for proportionnal vDef window controller
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
// 02/01/2013 creation.
//----------------------------------------------------------------------------

#import "bvDefPalettePropWindowController.h"
#import "bvDefPaletteProp.h"
#import "bvDefPalettePropStyle.h"
#import "vdef_utils.h"

#import <MacMapSuite/bTrace.h>
#import <MacMapStats/MMsDiscretization.h>

#import <mox_intf/bGenericMacMapApp.h>
#import <mox_intf/bEventLog.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/Color_Utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/NSUIUtils.h> 

#import <std_ext/bXMapStdIntf.h>

// ---------------------------------------------------------------------------
// 
// ------------
static double text2val(char* text){
double	r=1;
char*	c=strchr(text,'=');
	if(!c){
		return(r);
	}
	c++;
	c++;
char* d=strchr(c,'/');
	if(!d){
		r=matof(c);
		if(r<=0){
			r=1;
		}
		return(r);
	}
	d[0]=0;
double	a=matof(c);
	d++;
double	b=matof(d);
	r=a/b;
	if(r<=0){
		r=1;
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
static long val2index(double val, NSPopUpButton* c){
char	str[256];
long	best=0;
double	v,d,dmin=LONG_MAX;
	
	for(long i=1;i<=[c numberOfItems];i++){
		NSPopupButtonGetMenuItemValue(c,i,str,sizeof(str)-1);
		v=text2val(str);
		d=fabs(v-val);
		if(d==0){
			return(i);
		}
		if(d<dmin){
			dmin=d;
			best=i;
		}
	}
	return(best);
}

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bvDefPalettePropWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bvDefPalettePropWindowController init]",true);
	self=[super init];
	if(self){
		_last_click=-1;
		_hprev=YES;
		_qmin=0;
		_qmax=0;
		_nbclass=5;
		
		_uclss=NO;
		_area=YES;
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bvDefPalettePropWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[bvDefPalettePropWindowController awakeFromNib]",true);

// Nom du type et preview
char str[256];
	_ext->type_get()->name(str);
	[_tnm_fld setCharValue:str];
    [_tab selectTabViewItemAtIndex:0];

    [_prvw installController:self];

// Proportionnalité
	NSPopupButtonRemoveAllItems(_fld_pop);
	NSPopupButtonPopulateWithFields(_fld_pop,_ext->type_get(),kOBJ_SubType_,1);
	[_min_fld setDoubleValue:_qmin];
	[_max_fld setDoubleValue:_qmax];
		
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)_ext->getapp();
		
// Classes
	[_g_prvw installController:self];

    if(_g_fld_pop!=nil){
        NSPopupButtonRemoveAllItems(_g_fld_pop);
        NSPopupButtonPopulateWithFields(_g_fld_pop,_ext->type_get(),kOBJ_SubType_,1);
    }
    if(_g_clr_pop!=nil){
        [_g_ncl_fld setIntValue:_nbclass];
    }
    if(_g_clr_pop!=nil){
        [_g_min_fld setDoubleValue:_qmin];
    }
    if(_g_clr_pop!=nil){
        [_g_max_fld setDoubleValue:_qmax];
    }
            
    if(_g_clr_pop!=nil){
        NSPopupButtonRemoveAllItems(_g_clr_pop);
int     nc=countColors(gapp);
        for(long i=1;i<=nc;i++){
            getColorName(gapp,i,str);
            NSPopupButtonAddMenuItemValue(_g_clr_pop,str);
        }
    }
    
// Visibilité
	NSPopupButtonRemoveAllItems(_smn_pop);
	NSPopupButtonPopulateWithScales(_smn_pop,gapp,1);
	NSPopupButtonRemoveAllItems(_smx_pop);
	NSPopupButtonPopulateWithScales(_smx_pop,gapp,gapp->scaleMgr()->count());
	
// Options
	NSPopupButtonRemoveAllItems(_o_srf_pop);
	NSPopupButtonPopulateWithScales(_o_srf_pop,gapp,gapp->scaleMgr()->get_current());
	NSPopupButtonRemoveAllItems(_o_cnf_pop);
	NSPopupButtonPopulateWithFields(_o_cnf_pop,_ext->type_get(),kOBJ_Name_,1);
	NSPopupButtonRemoveAllItems(_o_xof_pop);
	NSPopupButtonPopulateWithFields(_o_xof_pop,_ext->type_get(),kOBJ_Dir_+1,1);
	NSPopupButtonRemoveAllItems(_o_yof_pop);
	NSPopupButtonPopulateWithFields(_o_yof_pop,_ext->type_get(),kOBJ_Dir_+1,2);
	
// Classes en trames désactivé pour le moment
	[_g_fil_pop setHidden:TRUE];
	
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		_uclss=NO;
	}
	else if(stl->_arr.count()<2){
		_uclss=NO;
	}
	else{
		_uclss=YES;
	}

	[self plot2Intf];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bvDefPalettePropWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Actions proportionnel ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseField:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseField]",true);
	_ext->fld_put([_fld_pop indexOfSelectedItem]+kOBJ_SubType_);
bvDefPalettePropStyle*	stl;	
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_fld=_ext->fld_get();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseProportionnality:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseProportionality]",true);
char					val[256];
double					d;
bvDefPalettePropStyle*	stl;	
	NSPopupButtonGetMenuItemValue(_prp_pop,[_prp_pop indexOfSelectedItem]+1,val,sizeof(val)-1);
	d=text2val(val);
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_exp=d;
	}
	[self updateSizeMax];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseOutOfRangeKind:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseProportionality]",true);
	((bvDefPaletteProp*)_ext)->oor_put([_oor_pop indexOfSelectedItem]+1);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseMin:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseMin]",true);
int		k=[_min_pop indexOfSelectedItem];
double	val;
	getMin((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),_ext->fld_get(),k,&val);
	[_min_fld setDoubleValue:val];
bvDefPalettePropStyle*	stl;	
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_qref=[_min_fld floatValue];
	}
	[self updateSizeMax];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutMin:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutMin]",true);
bvDefPalettePropStyle*	stl;	
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_qref=[_min_fld floatValue];
	}
	[self updateSizeMax];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseMax:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseMax]",true);
int		k=[_max_pop indexOfSelectedItem];
double	val;
	getMax((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),_ext->fld_get(),k,&val);
	[_max_fld setDoubleValue:val];
bvDefPalettePropStyle*	stl;	
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_qmax=[_max_fld floatValue];
	}
	[self updateSizeMax];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutMax:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutMax]",true);
bvDefPalettePropStyle*	stl;	
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_qmax=[_max_fld floatValue];
	}
	[self updateSizeMax];
}

// ---------------------------------------------------------------------------
// 
// -----------
#pragma mark ---- Actions classes ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassField:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassField]",true);
bvDefPalettePropStyle*	stl;
	((bvDefPaletteProp*)_ext)->subfld_put([_g_fld_pop indexOfSelectedItem]+kOBJ_SubType_);
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_subfld=((bvDefPaletteProp*)_ext)->subfld_get();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassMethod:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassMethod]",true);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutClassCount:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutClassCount]",true);
// Pas nécessaire
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassMin:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassMin]",true);
int		k=[_g_min_pop indexOfSelectedItem];
double	val;
	getMin((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),((bvDefPaletteProp*)_ext)->subfld_get(),k,&val);
	[_g_min_fld setDoubleValue:val];	
	((bvDefPaletteProp*)_ext)->cmin_put(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutClassMin:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutClassMin]",true);
	((bvDefPaletteProp*)_ext)->cmin_put([_g_min_pop doubleValue]);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassMax:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassMax]",true);
int		k=[_g_max_pop indexOfSelectedItem];
double	val;
	getMax((bGenericMacMapApp*)_ext->getapp(),_ext->type_get(),((bvDefPaletteProp*)_ext)->subfld_get(),k,&val);
	[_g_max_fld setDoubleValue:val];
	((bvDefPaletteProp*)_ext)->cmax_put(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutClassMax:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutClassMax]",true);
	((bvDefPaletteProp*)_ext)->cmax_put([_g_max_pop doubleValue]);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doComputeClass:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doComputeClass]",true);	
int			nbclass=[_g_ncl_fld intValue];
double		qmin=((bvDefPaletteProp*)_ext)->cmin_get();
double		qmax=((bvDefPaletteProp*)_ext)->cmax_get();
	if(qmax<=qmin){
		qmax=LONG_MAX;
		((bvDefPaletteProp*)_ext)->cmax_put(qmax);
	}
	
MMsMatrix*	mx;
	getMatrix(	(bGenericMacMapApp*)_ext->getapp(),
				_ext->type_get(),
				((bvDefPaletteProp*)_ext)->subfld_get(),
				qmin,
				qmax,
				&mx);

MMsMatrix*	clss=NULL;

	if(nbclass<=0){
		nbclass=d_estimate_class_count(mx);
	}
	
	switch([_g_dsc_pop indexOfSelectedItem]+1){
		case 1:
			clss=d_equal_range(mx,1,nbclass);
			break;
		case 2:
			clss=d_equal_manning(mx,1,nbclass);
			break;
		case 3:
			clss=d_standardized(mx,1,nbclass);
			break;
		case 4:
			clss=d_arithmetic_progression(mx,1,nbclass);
			break;
		case 5:
			clss=d_geometric_progression(mx,1,nbclass);
			break;
		case 6:
			clss=d_quantile(mx,1,nbclass);
			break;
		case 7:
			clss=d_natural_breaks(mx,1,nbclass);
			break;
		case 8:
			clss=d_bertin_average(mx,1,nbclass);
			break;
		case 9:
			clss=d_bertin_median(mx,1,nbclass);
			break;
	}
	
//_tm_("matrix");
//MMsDumpTTxt(mx,stderr);
//_tm_("classes");
//MMsDumpTTxt(clss,stderr);
	
color_range64			rng;
bvDefPalettePropStyle*	stl;
	
	rng.name[0]=0;
	rng.clr_spc=(_ext->cmyk_get())?5:4;
	rng.clrs[0]=0;
	rng.clrs[1]=0;
	rng.clrs[2]=0;
	rng.clrs[3]=1;
	rng.clrs[4]=1;
	rng.pat[0]=0;
	
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_arr.reset();
		for(long j=1;j<=clss->nl;j++){
			rng.bound=MMsGetDouble(clss,j,1);
			stl->_arr.add(&rng);
		}		
		((bvDefPaletteProp*)_ext)->cmin_put(MMsGetDouble(clss,1,1));
		((bvDefPaletteProp*)_ext)->cmax_put(MMsGetDouble(clss,clss->nl,1));
	}
	MMsMatrixFree(mx);
	
	[self plot2Intf];
	[self updateUI];
	[_g_run_tbl reloadData];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doAddClass:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doAddClass]",true);
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return;
	}
long	idx=_last_click+1;
	if(idx<=0){
		idx=stl->_arr.count();
	}
color_range64	rnga,rngb;
	stl->_arr.get(idx,&rnga);
	stl->_arr.get(idx+1,&rngb);
	rnga.bound=(rnga.bound+rngb.bound)/2.0;
	stl->_arr.insert(idx+1,&rnga);
	_last_click=idx;
	
	[self plot2Intf];
	[self updateUI];
	[_g_run_tbl reloadData];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doRemoveClass:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doRemoveClass]",true);
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return;
	}
long	idx=_last_click+1;
	if(idx==0){
		return;
	}
	if(stl->_arr.count()<3){
		return;
	}
	if(idx==1){
		stl->_arr.rmv(2);
	}
	else{
		stl->_arr.rmv(idx);
	}
	[_g_run_tbl deselectAll:(id)sender];
	[_g_run_tbl reloadData];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassColorimetry:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassColorimetry]",true);
double					cmin[5],cmax[5];
double					cx;
bvDefPalettePropStyle*	stl;	
color_range64			rng;
	
	getColor((bGenericMacMapApp*)_ext->getapp(),[_g_clr_pop indexOfSelectedItem]+1,cmin,cmax,_ext->cmyk_get());
	for(int i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		for(long j=1;j<=stl->_arr.count();j++){
			cx=(double)(j-1)/(double)(stl->_arr.count()-1);
			stl->_arr.get(j,&rng);
			rng.pat[0]=0;			
			rng.clrs[0]=cmin[0]+(cmax[0]-cmin[0])*cx;
			rng.clrs[1]=cmin[1]+(cmax[1]-cmin[1])*cx;
			rng.clrs[2]=cmin[2]+(cmax[2]-cmin[2])*cx;
			if(_ext->cmyk_get()){
				rng.clrs[3]=cmin[3]+(cmax[3]-cmin[3])*cx;
			}
			stl->_arr.put(j,&rng);
		}
	}
	[self plot2Intf];
	[self updateUI];	
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassFillKind:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassFillKind]",false);
	[_g_fil_tab selectTabViewItemAtIndex:[_g_fil_pop indexOfSelectedItem]];	
bvDefPalettePropStyle*	stl;	
color_range64			rng;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		for(long j=1;j<=stl->_arr.count();j++){
			stl->_arr.get(j,&rng);
			if([_g_fil_pop indexOfSelectedItem]==0){
				rng.pat[0]=0;
			}
			else if([_g_fil_pop indexOfSelectedItem]==1){
			}
			stl->_arr.put(j,&rng);
		}
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassFillColor:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassFillColor]",false);
bvDefPalettePropStyle*	stl;
NSColor*				clr=[_g_fil_clr color];
NSColor*				cclr;
color_range64			rng;
	
	if(_ext->cmyk_get()){
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceCMYKColorSpace"];
	}
	else{
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
	}
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		for(long j=1;j<stl->_arr.count();j++){
			if([_g_run_tbl isRowSelected:j-1]==YES){
				stl->_arr.get(j,&rng);
				if(_ext->cmyk_get()){
					rng.clr_spc=5;
					
					rng.clrs[0]=[cclr cyanComponent];
					rng.clrs[1]=[cclr magentaComponent];
					rng.clrs[2]=[cclr yellowComponent];
					rng.clrs[3]=[cclr blackComponent];
				}
				else{
					rng.clr_spc=4;

					rng.clrs[0]=[cclr redComponent];
					rng.clrs[1]=[cclr greenComponent];
					rng.clrs[2]=[cclr blueComponent];
				}
				rng.clrs[_ext->cmyk_get()?4:3]=1;
				stl->_arr.put(j,&rng);
			}
		}
	}	
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseClassFillPattern:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseClassFillPattern]",false);
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)_ext->getapp();
bGenericExt*		ext=gapp->xmapMgr()->find('PPck');
picker_prm			prm={_ext->type_get(),""};
color_range64		rng;
	
	if((ext->edit(&prm))&&(strlen(prm.name))){
_tm_(prm.name);
NSImage*	nsimg=GetNSImagePattern(gapp,
									_ext->type_get(),
									prm.name);
		if(!nsimg){
_te_("pas de nsimg");
			return;
		}
		[_g_fil_img setImage:nsimg];
bvDefPalettePropStyle*	stl;
		for(long i=1;i<=_ext->runs().count();i++){
			_ext->runs().get(i,&stl);
			for(long j=1;j<stl->_arr.count();j++){
				if([_g_run_tbl isRowSelected:j-1]==YES){
					stl->_arr.get(j,&rng);
					strcpy(rng.pat,prm.name);		
					stl->_arr.put(j,&rng);
				}
			}
		}		
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
#pragma mark ---- Actions divers ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutSize:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutSize]",true);
bvDefPalettePropStyle*	stl;
	
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_rref=[_szn_fld doubleValue];
	}
	[self updateSizeMax];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doChooseColor:(id)sender{
_bTrace_("[vDefPalettePropWindowController doChooseColor]",true);
bvDefPalettePropStyle*	stl;
NSColor*				clr=[_fil_clr color];
NSColor*				cclr;
	
	if(_ext->cmyk_get()){
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceCMYKColorSpace"];
	}
	else{
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
	}
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		if(_ext->cmyk_get()){
			stl->_color[0]=[cclr cyanComponent];
			stl->_color[1]=[cclr magentaComponent];
			stl->_color[2]=[cclr yellowComponent];
			stl->_color[3]=[cclr blackComponent];
		}
		else{
			stl->_color[0]=[cclr redComponent];
			stl->_color[1]=[cclr greenComponent];
			stl->_color[2]=[cclr blueComponent];
		}
	}
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleMin:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseScaleMin]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smn_pop indexOfSelectedItem]+1);
bvDefPalettePropStyle*	stl;

	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_smin=u->coef();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleMax:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseScaleMax]",true);
bGenericUnit*			u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_smx_pop indexOfSelectedItem]+1);
bvDefPalettePropStyle*	stl;

	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_smax=u->coef()+1.0;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doClickPreview:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doClickPreview]",true);
}

#pragma mark ---- Actions options ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseScaleRef:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseScaleRef]",true);
	/*if([_o_srf_pop indexOfSelectedItem]==0){
		_ext->sref_put(0);
	}
	else{*/
bGenericUnit*	u=((bGenericMacMapApp*)_ext->getapp())->scaleMgr()->get([_o_srf_pop indexOfSelectedItem]+1/*-1*/);
		_ext->sref_put(u->coef());
	//}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseUnit:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseUnit]",true);
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
-(IBAction)doChooseColorSpace:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseColorSpace]",true);
	_ext->cmyk_put([_o_cls_chk intValue]);
	[self convertColor];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckCondition:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doCheckCondition]",true);
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
_bTrace_("[bvDefPalettePropWindowController doChooseCondField]",true);
	_ext->cfld_put([_o_cnf_pop indexOfSelectedItem]+kOBJ_Name_);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseCondOperator:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseCondOperator]",true);
	_ext->cop_put([_o_cno_pop indexOfSelectedItem]+1);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doPutCondValue:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doPutCondValue]",true);
char	val[256];
	NSTextFieldGetValue(_o_val_fld,val,sizeof(val)-1);
	_ext->cval_put(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doCheckOffset:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doCheckOffset]",true);
bvDefPalettePropStyle*	stl;
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
_bTrace_("[bvDefPalettePropWindowController doChooseOffsetXField]",true);
bvDefPalettePropStyle*	stl;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		stl->_offx=[_o_xof_pop indexOfSelectedItem]+kOBJ_Dir_+1;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)doChooseOffsetYField:(id)sender{
_bTrace_("[bvDefPalettePropWindowController doChooseOffsetYField]",true);
bvDefPalettePropStyle*	stl;
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
_bTrace_("[bvDefPalettePropWindowController doApply]",true);
	_ext->update();
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
_bTrace_("[bvDefPalettePropWindowController updateUI]",true);
	[_prvw setNeedsDisplay:YES];
	[_g_prvw setNeedsDisplay:YES];
	[self checkIntfAvailability];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateSizeMax{
	if(_area){
		[_szx_fld setDoubleValue:[self sizeMax]];
	}
	else{
		[_szx_fld setDoubleValue:[self widthMax]];
	}
	[_prvw setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)checkIntfAvailability{
BOOL	state=YES;
bvDefPalettePropStyle*	stl=NULL;
	if(!_ext->runs().get(1,&stl)){
		state=NO;
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
	
	
	if(_uclss){
		if(!_ext->runs().get(1,&stl)){
			state=NO;
		}
		else{
			state=YES;
		}
		
		[_g_run_tbl setEnabled:state];
		[_g_fld_pop setEnabled:state];
		[_g_dsc_pop setEnabled:state];
		[_g_ncl_fld setEnabled:state];
		[_g_min_pop setEnabled:state];
		[_g_min_fld setEnabled:state];
		[_g_max_pop setEnabled:state];
		[_g_max_fld setEnabled:state];
		[_g_dsc_btn setEnabled:state];
		
		[_g_clr_pop setEnabled:state];
		
		[_g_add_btn setEnabled:(state&&(stl->_arr.count()>1)&&[_g_run_tbl numberOfSelectedRows]==1)];

		if((state==YES)&&(stl->_arr.count()>2)){
			[_g_rmv_btn setEnabled:[_g_run_tbl numberOfSelectedRows]==1];
		}
		else{
			[_g_rmv_btn setEnabled:NO];
		}

		if((state==YES)&&(stl->_arr.count()>1)){
			[_g_fil_pop setEnabled:[_g_run_tbl numberOfSelectedRows]>0];
			[_g_fil_clr setEnabled:[_g_run_tbl numberOfSelectedRows]>0];
			[_g_fil_img setEnabled:[_g_run_tbl numberOfSelectedRows]>0];
		}
		else{
			[_g_fil_pop setEnabled:NO];
			[_g_fil_clr setEnabled:NO];
			[_g_fil_img setEnabled:NO];		
		}
	}
	else{
		[_g_run_tbl setEnabled:NO];
		[_g_fld_pop setEnabled:NO];
		[_g_dsc_pop setEnabled:NO];
		[_g_ncl_fld setEnabled:NO];
		[_g_min_pop setEnabled:NO];
		[_g_min_fld setEnabled:NO];
		[_g_max_pop setEnabled:NO];
		[_g_max_fld setEnabled:NO];
		[_g_dsc_btn setEnabled:NO];
		[_g_clr_pop setEnabled:NO];
		[_g_add_btn setEnabled:NO];
		[_g_rmv_btn setEnabled:NO];
		[_g_fil_pop setEnabled:NO];
		[_g_fil_clr setEnabled:NO];
		[_g_fil_img setEnabled:NO];		
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)plot2Intf{
_bTrace_("[bvDefPalettePropWindowController plot2Intf]",true);
long					idx;
bvDefPalettePropStyle*	stl;
bGenericMacMapApp*		gapp=(bGenericMacMapApp*)_ext->getapp();
	
	if(!_ext->runs().get(1,&stl)){
		[_smn_pop selectItemAtIndex:0];
		[_smx_pop selectItemAtIndex:(gapp->scaleMgr()->count()-1)];		
		[_o_xof_pop selectItemAtIndex:0];
		[_o_yof_pop selectItemAtIndex:1];
		[_o_off_chk setIntValue:0];
		[_szn_fld setDoubleValue:0];
		[_fil_clr setColor:[NSColor blackColor]];
		return;
	}
	
	idx=GetBestScaleIndexForValue(gapp,stl->_smin);
	[_smn_pop selectItemAtIndex:(idx-1)];
	idx=GetBestScaleIndexForValue(gapp,stl->_smax);
	[_smx_pop selectItemAtIndex:(idx-1)];				
		
// Options + Visibilité
	[_fld_pop selectItemAtIndex:(_ext->fld_get()-kOBJ_SubType_)];
	if(_ext->sref_get()==0){
		idx=1;
	}
	else{
		idx=GetBestScaleIndexForValue(gapp,_ext->sref_get())/*+2*/;
	}
	[_o_srf_pop selectItemAtIndex:(idx-1)];
	[_o_unt_pop selectItemAtIndex:(_ext->coef_get()==1)?0:1];
	[_o_cls_chk setIntValue:_ext->cmyk_get()];
	[_o_xof_pop selectItemAtIndex:((stl->_offx)?(stl->_offx-kOBJ_Dir_-1):0)];
	[_o_yof_pop selectItemAtIndex:((stl->_offy)?(stl->_offy-kOBJ_Dir_-1):1)];
	if((stl->_offx)&&(stl->_offy)){
		[_o_off_chk setIntValue:1];
	}
	else{
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
	
	
	
	[_szn_fld setDoubleValue:stl->_rref];
	[_prp_pop selectItemAtIndex:val2index(stl->_exp,_prp_pop)-1];
	[_min_fld setDoubleValue:stl->_qref];
	[_max_fld setDoubleValue:stl->_qmax];
	[_oor_pop selectItemAtIndex:((bvDefPaletteProp*)_ext)->oor_get()-1];
	
	//if(strlen(stl->_pat)<=0){
		if(_ext->cmyk_get()){
			[_fil_clr setColor:[NSColor colorWithDeviceCyan:stl->_color[0] 
													magenta:stl->_color[1] 
													 yellow:stl->_color[2] 
													  black:stl->_color[3] 
													  alpha:1]];
		}
		else{
			[_fil_clr setColor:[NSColor colorWithDeviceRed:stl->_color[0]
													 green:stl->_color[1] 
													  blue:stl->_color[2] 
													 alpha:1]];
		}
	/*}
	else{
		[_fil_img setImage:GetNSImagePattern(gapp,_ext->type_get(),rng->pat)];
	}*/

	if(((bvDefPaletteProp*)_ext)->subfld_get()>=kOBJ_SubType_){
		[_g_fld_pop selectItemAtIndex:(((bvDefPaletteProp*)_ext)->subfld_get()-kOBJ_SubType_)];
	}
	else{
_te_("bad subfield index :"+((bvDefPaletteProp*)_ext)->subfld_get());
	}
	[_g_ncl_fld setIntValue:stl->_arr.count()-1];
	[_g_min_fld setDoubleValue:((bvDefPaletteProp*)_ext)->cmin_get()];
	[_g_max_fld setDoubleValue:((bvDefPaletteProp*)_ext)->cmax_get()];
color_range64			rng;
	for(long j=1;j<stl->_arr.count();j++){
		if([_g_run_tbl isRowSelected:j-1]==YES){
			stl->_arr.get(j,&rng);
			if(strlen(rng.pat)<=0){
				if(_ext->cmyk_get()){
					[_g_fil_clr setColor:[NSColor colorWithDeviceCyan:rng.clrs[0] 
															  magenta:rng.clrs[1] 
															   yellow:rng.clrs[2] 
																black:rng.clrs[3] 
																alpha:1]];
				}
				else{
					[_g_fil_clr setColor:[NSColor colorWithDeviceRed:rng.clrs[0]
															   green:rng.clrs[1] 
																blue:rng.clrs[2] 
															   alpha:1]];
				}
				
				[_g_fil_pop selectItemAtIndex:0];
				[_g_fil_tab selectTabViewItemAtIndex:0];	
			}
			else{
				[_g_fil_img setImage:GetNSImagePattern(gapp,_ext->type_get(),rng.pat)];
				
				[_g_fil_pop selectItemAtIndex:1];
				[_g_fil_tab selectTabViewItemAtIndex:1];	
			}
		}
	}
}	

// ---------------------------------------------------------------------------
// 
// -----------
-(void)convertColor{
bvDefPalettePropStyle*	stl;
color_range64			rng;
	for(long i=1;i<=_ext->runs().count();i++){
		_ext->runs().get(i,&stl);
		if(stl->_arr.count()<2){
			if(_ext->cmyk_get()){
				compRGB2CMYK64(stl->_color);
				stl->_cc=5;
			}
			else{
				compCMYK2RGB64(stl->_color);
				stl->_cc=4;
			}			
		}
		else{
				for(long j=1;j<stl->_arr.count();j++){
				stl->_arr.get(j,&rng);
				if(_ext->cmyk_get()){
					compRGB2CMYK64(rng.clrs);
					rng.clr_spc=5;
				}
				else{
					compCMYK2RGB64(rng.clrs);
					rng.clr_spc=4;
				}
			}
		}
	}		
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)drawPreview:(CGRect)rect context:(CGContextRef)ctx{
_bTrace_("[bvDefPalettePropWindowController drawPreview]",true);
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return;
	}
		
//bGenericMacMapApp*		gapp=(bGenericMacMapApp*)_ext->getapp();	
HIRect					hir=(*((CGRect*)(&rect)));
//CGPDFDocumentRef		pdf=NULL;
color_range64			rng;
	
	hir=CGRectInset(hir,2,2);
	hir.size.width=hir.size.width/((double)stl->_arr.count()-1);
	
	for(long i=1;i<stl->_arr.count();i++){
		stl->_arr.get(i,&rng);
						
		if(strlen(rng.pat)==0){
			MMCGContextSetFillColor64(ctx,
									  rng.clrs,
									  _ext->cmyk_get());
			CGContextAddRect(ctx,hir);
			CGContextDrawPath(ctx,kCGPathFill);
		}
		else{
			/*pdf=GetPDFPattern(gapp,_ext->type_get(),stl->_pat);
			if(pdf){
				CGPDFPageRef			pg=CGPDFDocumentGetPage(pdf,1);
				CGRect					box=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
				float					color[4]={1,0,0,1 };
				CGPatternCallbacks		callbacks={0,&PDFPatternPlot,NULL};
				CGColorSpaceRef			bspc=CGColorSpaceCreateDeviceRGB();
				CGColorSpaceRef			pspc=CGColorSpaceCreatePattern(bspc);
				CGContextSetFillColorSpace(ctx,pspc);
				CGColorSpaceRelease(pspc);
				CGColorSpaceRelease(bspc);
				CGPatternRef			pat=CGPatternCreate(pdf, 
															box,
															CGAffineTransformIdentity,
															box.size.width,
															box.size.height,
															kCGPatternTilingConstantSpacing,
															false, 
															&callbacks);   
				CGContextSetFillPattern(ctx,pat,color);
				CGPatternRelease(pat);			
				CGContextAddRect(ctx,shape);
				CGContextDrawPath(ctx,mode);
#warning Plantage à identifier sur release
				//CGPDFDocumentRelease(pdf);					
			}*/
		}		
		
		if([_g_run_tbl isRowSelected:i-1]==YES){
			hir=CGRectInset(hir,1,1);
			CGContextSetLineWidth(ctx,1);
			CGContextSetRGBStrokeColor(ctx,0,0,0,1);
			CGContextStrokeRect(ctx,hir);
			hir=CGRectInset(hir,-1,-1);
		}
		hir.origin.x+=hir.size.width;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)clickPreview:(CGPoint)point modifiers:(NSUInteger)mod{
_bTrace_("[bvDefPalettePropWindowController clickPreview]",true);
	
bvDefPalettePropStyle*	stl;	
	if(!_ext->runs().get(1,&stl)){
		return;
	}
	if(stl->_arr.count()<2){
		return;
	}
NSTabViewItem	*tabitem=[_tab selectedTabViewItem];
	if(tabitem==NULL){
		return;
	}
NSString* str=[tabitem identifier];
	if([str compare:@"Grad"]!=NSOrderedSame){
		return;
	}
	
NSRect				nsr=[_g_prvw frame];
double				w=_hprev?nsr.size.width/((double)stl->_arr.count()-1):nsr.size.height/((double)stl->_arr.count()-1);
NSInteger			x=_hprev?floor(point.x/w):(((double)stl->_arr.count()-1)-1)-floor(point.y/w);
NSMutableIndexSet*	set;
	
	if((NSCommandKeyMask&mod)!=0){
		if([_g_run_tbl isRowSelected:x]==YES){
			[_g_run_tbl deselectRow:x];
		}
		else{
			[_g_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
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
			[_g_run_tbl selectRowIndexes:set byExtendingSelection:YES];
		}
		else{
			[_g_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
		}
	}
	else{
		[_g_run_tbl selectRowIndexes:[NSIndexSet indexSetWithIndex:x] byExtendingSelection:NO];
	}	
}

#pragma mark ---- Gestion TableView ----
// ---------------------------------------------------------------------------
// 
// -----------
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView{
//_bTrace_("[bvDefPalettePropWindowController numberOfRowsInTableView]",true);
bvDefPalettePropStyle*	stl;	
	if(!_ext->runs().get(1,&stl)){
		return(0);
	}
	return stl->_arr.count()-1;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(id)	tableView:(NSTableView*)aTableView 
		objectValueForTableColumn:(NSTableColumn*)aTableColumn 
		row:(NSInteger)rowIndex{
//_bTrace_("[bvDefPalettePropWindowController tableView objectValueForTableColumn]",true);
int						d;
char					val[_values_length_max_];
NSString*				identifier=[aTableColumn identifier];
color_range64			rng;
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
//_te_("pas de style");
		return NULL;
	}
	
	_ext->type_get()->fields()->get_decs(((bvDefPaletteProp*)_ext)->subfld_get(),&d);
	if(d==0){
		d=2;
	}	
	if([identifier compare:@"0"]==NSOrderedSame){
		stl->_arr.get(rowIndex+1,&rng);
	}
	else{
		stl->_arr.get(rowIndex+2,&rng);
	}	
	xToChar(_double,d,&rng.bound,val);
	return [NSString stringWithCString:val encoding:NSMacOSRomanStringEncoding];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)	tableView:(NSTableView*)aTableView 
		setObjectValue:(id)object
		forTableColumn:(NSTableColumn*)aTableColumn 
		row:(NSInteger)rowIndex{
double					v;
NSString*				identifier=[aTableColumn identifier];
NSString*				value=(NSString*)object;
color_range64			rng;
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return;
	}
	
	v=[value doubleValue];

	if([identifier compare:@"0"]==NSOrderedSame){
		stl->_arr.get(rowIndex+1,&rng);
		rng.bound=v;
		stl->_arr.put(rowIndex+1,&rng);
	}
	else{
		stl->_arr.get(rowIndex+2,&rng);
		rng.bound=v;
		stl->_arr.put(rowIndex+2,&rng);
	}
	
	[self plot2Intf];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)tableViewSelectionDidChange:(NSNotification*)notification{
//_bTrace_("[bvDefPalettePropWindowController tableViewSelectionDidChange]",true);
	[self plot2Intf];
	[self updateUI];
	_last_click=[_g_run_tbl selectedRow];
}

#pragma mark ---- Calcul des tailles max ----
// ---------------------------------------------------------------------------
// 
// -----------
-(double)sizeMax{
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return(0);
	}
	return(sqrt(pow(stl->_qmax,stl->_exp))*(stl->_rref/sqrt(pow(stl->_qref,stl->_exp))));
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)widthMax{
bvDefPalettePropStyle*	stl;
	if(!_ext->runs().get(1,&stl)){
		return(0);
	}
	return(pow(stl->_qmax,stl->_exp)*(stl->_rref/pow(stl->_qref,stl->_exp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
