//----------------------------------------------------------------------------
// File : bToolJustification.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (justification by field) tool class
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
// 13/06/2007 creation.
//----------------------------------------------------------------------------
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolJustification.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/bCoreTextInfo.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolJustification	::bToolJustification(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdToolPres(elt,gapp,bndl){
	setclassname("justificationtool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolJustification::~bToolJustification(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolJustification::create(bGenericXMLBaseElement* elt){
	return(new bToolJustification(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::open(int* flags){
    _umode=kJustificationUserModeStd;
    _jmd=kJustificationMethodNone;

    bStdToolPres::open(flags);

	_cleft=load_curs("LeftCursor.png");
	_cright=load_curs("RightCursor.png");
	_cmiddle=load_curs("CenterCursor.png");
	_cspace=load_curs("JustCursor.png");
	
	set_use_track(false);
	set_use_drag(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::close(){
	bStdToolPres::close();
	if(_cleft){
		ReleaseCocoaCursor(_cleft);
	}
	if(_cright){
		ReleaseCocoaCursor(_cright);
	}
	if(_cmiddle){
		ReleaseCocoaCursor(_cmiddle);
	}
	if(_cspace){
		ReleaseCocoaCursor(_cspace);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::clic(CGPoint pt, int count){
_bTrace_("bToolJustification::clic",true);
	bStdToolPres::clic(pt,count);
	if(count>1){
		return;
	}
	
bArray*					ga;
bArray					runs(sizeof(bGenericXMLBaseElement*));
bGenericXMLBaseElement	*run,*gm,*elt;
bGenericGeoElement*		o=NULL;
int						i,n=_gapp->layersAccessCtx()->count();
CGPoint					p;
	
	set_obj(NULL);
	get_clic(&p);
	
	_fjust=0;
	_ijust=0;
	_fspc=0;
	_ispc=0;

	_styl=NULL;
	
	_curjust=0;
	_curspc=0;
	_pthlen=0;
	_txtwdt=0;
	_compjust=0;
	
	_xpts=NULL;
	_ypts=NULL;
	_offs=NULL;
	_npts=0;
	_noffs=0;
	
	for(i=n;i>0;i--){
		_styl=_gapp->layersAccessCtx()->get(i);
		if(!_styl){
			continue;
		}	
		ga=_styl->objsatpoint(&p,true);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(ga->count()>1){
			delete ga;
			break;
		}
		ga->get(1,&o);
		delete ga;
		break;
	}
	
	if(!o){
		_styl=NULL;
		return;
	}

bGenericUnit*	scl=_gapp->scaleMgr()->get();

	_styl->get_styleruns(runs,o,scl->coef(),1);

char			val[_values_length_max_];
bGenericType	*tp=_gapp->typesMgr()->get(o->getType());
	
	_jkd=kJustificationKindNone;

	for(int i=1;i<=runs.count();i++){
		runs.get(i,&run);
		_fjust=0;
		_fspc=0;
		gm=_gapp->classMgr()->NthElement(run,1,"polyjustification");
		if(!gm){
			gm=_gapp->classMgr()->NthElement(run,1,"pointjustification");
			if(!gm){
				continue;
			}
			_jkd=kJustificationKindOnPoint;
		}
		else{
			_jkd=kJustificationKindOnPath;
		}
		elt=_gapp->classMgr()->NthElement(gm,1,"field");
		if(!elt){
			continue;
		}
		elt->getvalue(val);
		_fjust=tp->fields()->get_index(val);
		
		if(_jmd==kJustificationMethodGlyphSpace){
			gm=_gapp->classMgr()->NthElement(run,1,"charspacing");
			elt=_gapp->classMgr()->NthElement(gm,1,"field");
			if(!elt){
				_fjust=0;
				continue;
			}
			elt->getvalue(val);
			_fspc=tp->fields()->get_index(val);
		}
		else if(_jmd==kJustificationKindOnWordSpace){
			gm=_gapp->classMgr()->NthElement(run,1,"wordspacing");		
			elt=_gapp->classMgr()->NthElement(gm,1,"field");
			if(!elt){
				_fspc=0;
				continue;
			}
			elt->getvalue(val);
			_fspc=tp->fields()->get_index(val);
		}
		break;
	}

	if(!_fjust){
		_styl=NULL;
_te_("no just field");
		return;
	}
	set_obj(o);

bGenericGraphicContext*	ctx=_styl->get_context_for_object(o,0,false);
bCoreTextInfo*			txt=ctx->getText();
float					*xpts,*ypts;
int						*offs;

	ctx->getGeometry(&xpts,&ypts,&_npts,&offs,&_noffs);
		
	if(	(_jkd==kJustificationKindOnPath)&&
		(_npts<2)						){
		ctx->setGeometry(NULL,NULL,0,NULL,0);
		_styl=NULL;
		_fjust=0;
		_fspc=0;
		_jmd=kJustificationKindNone;
		_umode=kJustificationUserModeStd;
		_jkd=kJustificationKindNone;
_te_("_npts<2");
		return;
	}
	if(	(_jkd==kJustificationKindOnPoint)&&
		(_npts==0)						){
		ctx->setGeometry(NULL,NULL,0,NULL,0);
		_styl=NULL;
		_fjust=0;
		_fspc=0;
		_jmd=kJustificationKindNone;
		_umode=kJustificationUserModeStd;
		_jkd=kJustificationKindNone;
_te_("_npts==0");
		return;
	}
	if(!xpts||!ypts){
		ctx->setGeometry(NULL,NULL,0,NULL,0);
		_styl=NULL;
		_fjust=0;
		_fspc=0;
		_jmd=kJustificationKindNone;
		_umode=kJustificationUserModeStd;
		_jkd=kJustificationKindNone;
_te_("(!_xpts||!_ypts)");
		return;
	}

	_xpts=new float[_npts];
	memmove(_xpts,xpts,_npts*sizeof(float));
	_ypts=new float[_npts];
	memmove(_ypts,ypts,_npts*sizeof(float));
	_offs=new int[_noffs];
	memmove(_offs,offs,_noffs*sizeof(int));

	ctx->setGeometry(NULL,NULL,0,NULL,0);

	_txtwdt=txt->textWidth();
	_pthlen=0;
	o->getValue(_fjust,&_ijust);
	if(_fspc){
		o->getValue(_fspc,&_ispc);
		_curspc=_ispc;
	}
	
CGPoint		cgp=pt;
	if(_jkd==kJustificationKindOnPath){		
float		r;
		for(int i=0;i<_npts-1;i++){
			_pthlen+=XYDist(_xpts[i],_ypts[i],_xpts[i+1],_ypts[i+1]);
		}
		XYProjOnPath(cgp.x,cgp.y,_xpts,_ypts,_npts,&cgp.x,&cgp.y,&_clkidx,&r);
		if((r>0)&&(r<1)){
			_xpts=(float*)realloc(_xpts,(_npts+1)*sizeof(float));
			_ypts=(float*)realloc(_ypts,(_npts+1)*sizeof(float));
			_npts++;
			memmove(&_xpts[_clkidx+1],&_xpts[_clkidx],_npts-_clkidx);
			memmove(&_ypts[_clkidx+1],&_ypts[_clkidx],_npts-_clkidx);
			_xpts[_clkidx]=cgp.x;
			_ypts[_clkidx]=cgp.y;
		}
		set_clic(&cgp);
		_curjust=get_justification_on_path(cgp);
		_compjust=_curjust-_ijust;
		_curjust=_ijust;
	}
	else if(_jkd==kJustificationKindOnPoint){
		_compjust=XYDist(_xpts[0],_ypts[0],cgp.x,cgp.y);
		_curjust=_ijust;
		_pthlen=rad2deg(ctx->getAngle());
	}
	
	set_use_drag(true);
	set_use_track(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::end_clic(){
_bTrace_("bToolJustification::end_clic",true);
	bStdToolPres::end_clic();
	for(;;){
		if(!_fjust||!_styl){
_te_("(!_fjust("+_fjust+")||!_styl("+_styl+"))");
			break;
		}

bGenericGeoElement*	o=get_obj();
		if(!o){
_te_("(!o)");
			break;
		}
		
		set_use_drag(false);
		set_use_track(false);

		if(_umode!=kJustificationUserModeSpace){
// Pour avoir la valeur précédente en annulation
			o->setValue(_fjust,&_ijust);
CGRect	bnds;
			_styl->rect(o,&bnds);
			_gapp->mapIntf()->inval(bnds);		
			
// Report de la nouvelle valeur
bEventLog	log(_gapp,
				getbundle(),
				kJustificationMessageID,
				GetSignature(this));
			switch(_umode){
				case kJustificationUserModeRight:
					put_value(_fjust,1);
					break;
				case kJustificationUserModeCenter:
					put_value(_fjust,0.5);
					break;
				case kJustificationUserModeLeft:
					put_value(_fjust,0);
					break;
				case kJustificationUserModeStd:
					put_value(_fjust,_curjust);
					break;
			}
			log.close();
			
			_styl->rect(o,&bnds);
			_gapp->mapIntf()->inval(bnds);		
		}
		else{
// Pour avoir la valeur précédente en annulation
			o->setValue(_fspc,&_ispc);
CGRect	bnds;
			_styl->rect(o,&bnds);
			_gapp->mapIntf()->inval(bnds);		
// Report de la nouvelle valeur
bEventLog	log(_gapp,
				getbundle(),
				kJustificationMessageID,
				GetSignature(this));
			put_value(_fspc,_curspc);
			log.close();
			
			_styl->rect(o,&bnds);
			_gapp->mapIntf()->inval(bnds);		
		}
		break;
	}

	set_use_drag(false);
	set_use_track(false);

	if(_xpts){
		delete _xpts;
	}
	if(_ypts){
		delete _ypts;
	}
	if(_offs){
		delete _offs;
	}
	_xpts=NULL;
	_ypts=NULL;
	_offs=NULL;
	_npts=0;
	_noffs=0;
	set_obj(NULL);
	
	/**/
	_fjust=0;
	_fspc=0;
	_styl=NULL;
	/**/
	
	_umode=kJustificationUserModeStd;
	_jkd=kJustificationKindNone;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
bGenericGeoElement*	o=get_obj();
	if(o){
		clearTempPathContext(true);
		_gapp->layersMgr()->SwitchContext(kCtxGraphicContext,getTempPathContext());
		_styl->draw(o);
		_gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
		validTempPathContext();
	}
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::set_modifiers(int k){	
	bStdToolPres::set_modifiers(k);
	if(!get_active()){
		return;
	}
	if(is_modifiers(cmdKey)){
		set_curs(_cright);
		_umode=kJustificationUserModeRight;
		set_use_track(false);
		set_use_drag(false);
	}
	else if(is_modifiers(optionKey)){
		set_curs(_cmiddle);
		_umode=kJustificationUserModeCenter;
		set_use_track(false);
		set_use_drag(false);
	}
	else if(is_modifiers(controlKey)){
		set_curs(_cleft);
		_umode=kJustificationUserModeLeft;
		set_use_track(false);
		set_use_drag(false);
	}
	else if(is_modifiers(shiftKey)){
		if(_fspc){
			set_curs(_cspace);
			_umode=kJustificationUserModeSpace;
			set_use_track(true);
			set_use_drag(true);
		}
		else{
			set_curs();
			_umode=kJustificationUserModeStd;
			set_use_track(true);
			set_use_drag(true);
		}
	}
	else{
		set_curs();
		_umode=kJustificationUserModeStd;
		if(get_on_drag()){
			set_use_track(true);
			set_use_drag(true);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::track_draw(){
	bStdToolPres::track_draw();
	if(!get_use_track()){
		return;
	}
	if(!get_use_drag()){
		return;
	}
	if(!_styl){
		return;
	}
CGPoint		cgp;
i2dvertex	vx;
	get_cur(&vx);
	if(vx.h==LONG_MIN){
		return;
	}
	Convert(&cgp,&vx);

double	val;

	if(_umode==kJustificationUserModeStd){
		if(_jkd==kJustificationKindOnPath){
			val=get_justification_on_path(cgp);
			if(val!=_curjust){
				_curjust=val;
				put_value(_fjust,val);
			}
		}
		else if(_jkd==kJustificationKindOnPoint){
			val=get_justification_on_point(cgp);
			if(val!=_curjust){
				_curjust=val;
				put_value(_fjust,val);
			}			
		}
	}
	else if(_umode==kJustificationUserModeSpace){
		if(_jkd==kJustificationKindOnPath){
			val=get_inter_on_path(cgp);
			if(val!=_curspc){
				_curspc=val;
				put_value(_fspc,val);
			}
		}
		else if(_jkd==kJustificationKindOnPoint){
			val=get_inter_on_point(cgp);
			if(val!=_curjust){
				_curspc=val;
				put_value(_fspc,val);
			}		
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolJustification::edit_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
ControlRef	c;
ControlID	cid={kJustificationEditSign,kJustificationButtonsID};		
	
	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				GetControlByID(wd,&cid,&c);
				_jmd=GetControl32BitValue(c);
				break;
			case kHICommandCancel:
				break;
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::edit_init(WindowRef wd){
ControlRef	c;
ControlID	cid={kJustificationEditSign,kJustificationButtonsID};
		
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_jmd);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolJustification::put_value(int fidx, double val){
bGenericGeoElement*	o=get_obj();
	if(o==NULL){
		return;
	}
	o->setValue(fidx,val);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolJustification::get_justification_on_path(CGPoint cgp){
int		idx;
float	r;
	XYProjOnPath(cgp.x,cgp.y,_xpts,_ypts,_npts,&cgp.x,&cgp.y,&idx,&r);
	if(idx==0){
		return(0);
	}
	if(idx>=_npts){
		return(1);
	}
	
	idx--;
double	d=0;
	for(int i=0;i<idx;i++){
		d+=XYDist(_xpts[i],_ypts[i],_xpts[i+1],_ypts[i+1]);
	}
	d+=XYDist(_xpts[idx],_ypts[idx],cgp.x,cgp.y);
	r=d/(_pthlen-_txtwdt);
	r-=_compjust;
	if(r<0){
		return(0);
	}
	if(r>1){
		return(1);
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolJustification::get_justification_on_point(CGPoint cgp){
_bTrace_("bToolJustification::get_justification_on_point",false);
i2dvertex	vxa,vxb;
	get_cur(&vxa);
CGPoint		best={_xpts[0],_ypts[0]};
	Convert(&vxb,&best);
double		a=rad2deg(vx_angle(&vxb,&vxa))-_pthlen;
	if(a<0){
		a+=360;
	}
_tm_("angle="+a);
	if((a>45)&&(a<315)){
		return(_curjust);
	}
double	d=XYDist(best.x,best.y,cgp.x,cgp.y)-_compjust;
double	r=1-(d/_txtwdt);
	if(r<0){
		return(0);
	}
	if(r>1){
		return(1);
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolJustification::get_inter_on_path(CGPoint cgp){
int		idx;
float	r;	

	XYProjOnPath(cgp.x,cgp.y,_xpts,_ypts,_npts,&cgp.x,&cgp.y,&idx,&r);
	r=0;	
	if(idx<_clkidx){
		for(int i=idx;i<_clkidx-1;i++){
			r+=XYDist(_xpts[i],_ypts[i],_xpts[i+1],_ypts[i+1]);
		}
		r+=XYDist(cgp.x,cgp.y,_xpts[idx],_ypts[idx]);
		r=-r;
		if(r<-2){
			r=-2;
		}
	}
	else{
		for(int i=_clkidx;i<idx-2;i++){
			r+=XYDist(_xpts[i],_ypts[i],_xpts[i+1],_ypts[i+1]);
		}
		r+=XYDist(cgp.x,cgp.y,_xpts[idx-1],_ypts[idx-1]);
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolJustification::get_inter_on_point(CGPoint cgp){
_bTrace_("bToolJustification::get_inter_on_point",false);
i2dvertex	vxa,vxb;
	get_clic(&vxb);
	Convert(&vxa,&cgp);
double		a=rad2deg(vx_angle(&vxb,&vxa))-_pthlen/*_compjust*/;
	if(a<0){
		a+=360;
	}
_tm_("angle="+a);
	if((a>45)&&(a<135)){
		return(_curspc);
	}
	if((a>225)&&(a<315)){
		return(_curspc);
	}
CGPoint		cgc;
	Convert(&cgc,&vxb);
float	r=XYDist(cgc.x,cgc.y,cgp.x,cgp.y);
	if(a>135){
		r=-r;
	}
	if(r<-2){
		r=-2;
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolJustification::load(){
bGenericXMLBaseElement* groot=bStdToolPres::load();
bGenericXMLBaseElement* elt=NULL;

	if(!groot){
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		return(NULL);
	}
	char					val[_values_length_max_];
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_jmd=atoi(val);
		if(	(_jmd<kJustificationMethodNone)		||
			(_jmd>kJustificationKindOnWordSpace)){
			_jmd=kJustificationMethodNone;
		}
		break;
	}
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolJustification::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	add_idesc(arr,2,"int",_jmd);
	return(bStdToolPres::make(arr));
}
