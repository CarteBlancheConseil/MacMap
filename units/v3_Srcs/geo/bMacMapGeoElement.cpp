//----------------------------------------------------------------------------
// File : bMacMapGeoElement.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap object class
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
// 16/05/2003 creation.
//----------------------------------------------------------------------------

#include "bMacMapGeoElement.h"
#include "bMacMapApp.h"
#include "bMacMapType.h"
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
enum{
	fMask		=0x00000001,
	fScreen		=0x00000002,
	fSelect		=0x00000004,
	fContrast	=0x00000008,
	fUpdate		=0x00000010,
	fNew		=0x00000020,
	fFlag1		=0x00000100,
	fFlag2		=0x00000200,
	fFlag3		=0x00000400,
	fFlag4		=0x00000800,
	fHandled	=0x80000000
};

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::_inval=true;
bool bMacMapGeoElement::_save=false;

// ---------------------------------------------------------------------------
// Constructeur : chargement -> on ne génère pas d'event ?
// ------------
bMacMapGeoElement	::bMacMapGeoElement(bGenericType* typ, int offset, int *status)
					:bStdGeoElement(){
_bTrace_("bMacMapGeoElement::bMacMapGeoElement(bGenericType*,int,int*)",false);

	(*status)=0;
	
	_vxs=NULL;
	_killed=false;
	
	if(typ->fields()->killed_record(offset)){
		_killed=true;
		if(typ->fields()->read(offset,kOBJ_ID_,&_id)){
			(*status)=-1;
			return;
		}
		(*status)=1;
		return;
	}
	
	_id=0;
	_typ=typ;
	_offset=offset;
	
	if(typ->fields()->read(offset,kOBJ_ID_,&_id)){
		(*status)=-2;
		return;
	}
	if(typ->fields()->read(offset,kOBJ_State_,&_state)){
		(*status)=-3;
		return;
	}
	
int	styp;
	if(typ->fields()->read(offset,kOBJ_SubType_,&styp)){
		(*status)=-4;
		return;
	}
	_styp=styp;
	
	if(typ->fields()->read(offset,kOBJ_Vertices_,&_vxs)){
_te_("bad vxs");
		(*status)=-5;
		_vxs=NULL;
// NO RETURN ! DEFAULT VXS build after !!!!
	}
    if((_vxs->no>_vxs->nv)||(_vxs->no<0)||(_vxs->nv<0)){
_te_("bad point/offset count : "+_vxs->no+"/"+_vxs->nv);
        (*status)=-11;
        _vxs=NULL;
    }
                        
// VERIFICATION VERTICES : ON CREE LE VXS SI PB
	if(!_vxs){
_te_("no _vxs for "+_offset);
ivx_rect	bnds;
i2dvertex	mid;

		typ->iterator()->bounds(&bnds);
		ivr_mid(&bnds,&mid);
		switch(typ->kind()){
			case kBaseKindPoint:
			case kBaseKindText:
				_vxs=ivs_new(_2D_VX,1,0);
				_vxs->vx.vx2[0]=mid;
				break;
			case kBaseKindPolyline:
				_vxs=ivs_new(_2D_VX,2,0);
				_vxs->vx.vx2[0]=mid;
				_vxs->vx.vx2[1].h=mid.h+10;
				_vxs->vx.vx2[1].v=mid.v+10;
				break;
			case kBaseKindPolygon:			
			case kBaseKindRaster:
				_vxs=ivs_new(_2D_VX,5,0);
				_vxs->vx.vx2[0].h=mid.h-10;
				_vxs->vx.vx2[0].v=mid.v+10;
				_vxs->vx.vx2[1].h=mid.h+10;
				_vxs->vx.vx2[1].v=mid.v+10;
				_vxs->vx.vx2[2].h=mid.h+10;
				_vxs->vx.vx2[2].v=mid.v-10;
				_vxs->vx.vx2[3].h=mid.h-10;
				_vxs->vx.vx2[3].v=mid.v-10;
				_vxs->vx.vx2[4]=_vxs->vx.vx2[0];
				break;
		}
	}
	switch(ivs_good(_vxs)){
		case 0:
			break;
		case -1:
_te_("bad _vxs for "+_offset+" / unknown vertices kind");
			(*status)=2;
			break;
		case -2:
_te_("bad _vxs for "+_offset+" / nb vertices="+_vxs->nv);
			(*status)=3;
			break;
		case -3:
_te_("bad _vxs for "+_offset+" / nb offsets="+_vxs->no);
			(*status)=4;
			break;
		case -4:
_te_("bad _vxs for "+_offset+" / nb offsets>nb vertices : "+_vxs->no+";"+_vxs->nv);
			(*status)=5;
			break;
		case -5:
_te_("bad _vxs for "+_offset+"(nb offsets="+_vxs->no+" for NULL offsets)");
			(*status)=5;
			break;
		default:
_te_("bad _vxs for "+_offset+" (unknown)");
			(*status)=6;
			break;
	};
    if((*status)==0){
        (*status)=ivs_integrity(typ->kind(),_vxs);
        if((*status)){
_tw_("ivs_integrity returns "+(*status));
        }
        (*status)=0;
    }
	if((*status)){
		ivs_free(_vxs);
		_vxs=NULL;
		return;
	}
                        
	ivs_bounds(_vxs,&_vxr);
    if(!ivr_in_ivr(&_vxr,((bMacMapType*)typ)->get_load_area())){
        (*status)=2;
        return;
    }

int	k;
	_color=1;
	if(typ->fields()->read(offset,kOBJ_Color_,&k)){
		(*status)=-7;
		return;
	}
	_color=k;
	_dir=0;
	if(typ->fields()->read(offset,kOBJ_Dir_,&k)){
		(*status)=-8;
		return;
	}
	_dir=k;
	if(!typ->iterator()->add(this)){
		(*status)=-10;
		return;
	}
	
	if(selected()){
		(void)selmgr->add(this);
	}
	
	if(contrasted()){
		(void)cntmgr->add(this);
	}
//_tm_(_id+": _vxs="+(void*)_vxs+" - "+_vxs->nv+" / "+_vxs->no);
}

// ---------------------------------------------------------------------------
// Constructeur : creation
// ------------
bMacMapGeoElement	::bMacMapGeoElement(bGenericType* typ, int *status)
					:bStdGeoElement(){
_bTrace_("bMacMapGeoElement::bMacMapGeoElement(bGenericType*,int*)",false);
	_id=0;
	_typ=typ;
	_styp=1;
	_offset=0;
	_state=0;
	_color=1;
	_dir=0;
	_killed=false;
	_vxs=NULL;
	(*status)=0;
	
	_offset=typ->append();
	if(_offset<0){
		(*status)=-1;
		return;
	}
	
ivx_rect	bnds;
i2dvertex	mid;
	typ->iterator()->bounds(&bnds);
	ivr_mid(&bnds,&mid);
	
	mid.h++;
	mid.v++;
	
	switch(typ->kind()){
		case kBaseKindPoint:
		case kBaseKindText:
			_vxs=ivs_new(_2D_VX,1,0);
			_vxs->vx.vx2[0]=mid;
			break;
		case kBaseKindPolyline:
			_vxs=ivs_new(_2D_VX,2,0);
			_vxs->vx.vx2[0]=mid;
			_vxs->vx.vx2[1].h=mid.h+10;
			_vxs->vx.vx2[1].v=mid.v+10;
			break;
		case kBaseKindPolygon:			
		case kBaseKindRaster:
			_vxs=ivs_new(_2D_VX,5,0);
			_vxs->vx.vx2[0].h=mid.h-10;
			_vxs->vx.vx2[0].v=mid.v+10;
			_vxs->vx.vx2[1].h=mid.h+10;
			_vxs->vx.vx2[1].v=mid.v+10;
			_vxs->vx.vx2[2].h=mid.h+10;
			_vxs->vx.vx2[2].v=mid.v-10;
			_vxs->vx.vx2[3].h=mid.h-10;
			_vxs->vx.vx2[3].v=mid.v-10;
			_vxs->vx.vx2[4]=_vxs->vx.vx2[0];
			break;
	}
	if(!_vxs){
		(*status)=-2;
		return;
	}
	ivs_bounds(_vxs,&_vxr);
	if(typ->fields()->write(_offset,kOBJ_Vertices_,&_vxs)){// Ecrit également le bound
		(*status)=-6;
		_vxs=NULL;
		return;
	}

	if(typ->fields()->read(_offset,kOBJ_ID_,&_id)){
_te_("ReadVal for "+_offset);
		(*status)=-3;
		return;
	}
	if(typ->fields()->read(_offset,kOBJ_State_,&_state)){
		(*status)=-4;
		return;
	}
	
int	x;
	if(typ->fields()->read(_offset,kOBJ_SubType_,&x)){
		(*status)=-5;
		return;
	}
	_styp=x;
	if(typ->fields()->read(_offset,kOBJ_Color_,&x)){
		(*status)=-8;
		return;
	}
	_color=x;
	if(typ->fields()->read(_offset,kOBJ_Dir_,&x)){
		(*status)=-9;
		return;
	}
	_dir=x;

	if(!typ->iterator()->add(this)){
_te_("typ->iterator()->add failed");
		(*status)=-10;
		return;
	}
	
	eventmgr->create(this);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapGeoElement	::bMacMapGeoElement(bGenericGeoElement* ref, int *status)
					:bStdGeoElement(){
	(*status)=0;
	
	_id=0;
	
	_offset=0;
	_state=0;
	_killed=false;
	
	_typ=_MMAPP_->typesMgr()->get(ref->getType());
	_styp=ref->getSubType();
	_color=ref->getColor();
	_dir=ref->getDirection();
char	name[256];
	ref->getName(name);
		
	_offset=_typ->append();
	if(_offset<0){
		(*status)=-1;
		return;
	}
	
ivertices*	buf;
	ref->getVertices(&buf);
	
	ivs2ivs(buf,&_vxs);
	if(!_vxs){
		(*status)=-2;
		return;
	}
	
	ivs_bounds(_vxs,&_vxr);
	
	if(_typ->fields()->read(_offset,kOBJ_ID_,&_id)){
		(*status)=-3;
		return;
	}
	if(_typ->fields()->write(_offset,kOBJ_State_,&_state)){
		(*status)=-4;
		return;
	}
	if(_typ->fields()->write(_offset,kOBJ_Name_,name)){
		(*status)=-5;
		return;
	}
int	x=_styp;
	if(_typ->fields()->write(_offset,kOBJ_SubType_,&x)){
		(*status)=-6;
		return;
	}
	if(_typ->fields()->write(_offset,kOBJ_Vertices_,&_vxs)){
		(*status)=-7;
		_vxs=NULL;
		return;
	}

	x=_color;
	if(_typ->fields()->write(_offset,kOBJ_Color_,&x)){
		(*status)=-8;
		return;
	}
	x=_dir;
	if(_typ->fields()->write(_offset,kOBJ_Dir_,&x)){
		(*status)=-9;
		return;
	}
	
	if(!_typ->iterator()->add(this)){
		(*status)=-10;
		return;
	}
	
// Duplication des champs
int		max=0,sz;
	
	for(int i=kOBJ_Dir_+1;i<=_typ->fields()->count();i++){
		_typ->fields()->get_size(i,&sz);
		if(sz>max){
			max=sz;
		}
	}

void*	buffer=malloc(max);
	
	for(int i=kOBJ_Dir_+1;i<=_typ->fields()->count();i++){
		_typ->fields()->read(((bMacMapGeoElement*)ref)->_offset,i,buffer);
		_typ->fields()->write(_offset,i,buffer);
	}
	
	free(buffer);
	
	eventmgr->create(this);
	inval(kOBJ_Vertices_);

}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapGeoElement::~bMacMapGeoElement(){
//_bTrace_("bMacMapGeoElement::~bMacMapGeoElement()",false);
//_tm_(_id+": _vxs="+(void*)_vxs);
    if(_vxs){
		ivs_free(_vxs);
	}
// Dont write when user stops loading objects
    if((_save)&&(!_killed)){
int state=0;
		if(selected()){
			state|=fSelect;
		}
		if(contrasted()){
			state|=fContrast;
		}
		if(masked()){
			state|=fMask;
		}
		if(_typ->fields()->write(_offset,kOBJ_State_,&state)){
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::kill(){
_bTrace_("bMacMapGeoElement::kill",false);
	if(selected()){
		setselect(false,true);
	}
	if(contrasted()){
		setcontrast(false,true);
	}
	if(!_typ->iterator()->rmv(this/*,false*/)){
_te_("iterator()->rmv failed");
		return;
	}
	if(_typ->fields()->kill_record(_offset)){
_te_("_typ->fields()->kill_record failed");
		return;
	}	
	_killed=true;
	inval(kOBJ_Vertices_);
	eventmgr->kill(this);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::unkill(){
_bTrace_("bMacMapGeoElement::unkill",false);
	if(_typ->fields()->unkill_record(_offset)){
_te_("_typ->fields()->unkill_record(_offset) failed");
		return;
	}
	if(!_typ->iterator()->add(this)){
_te_("_typ->iterator()->add(this) failed");
		return;
	}
	_killed=false;
	inval(kOBJ_Vertices_);
	eventmgr->create(this);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::killed(){
	return(_killed);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::selected(){
	return((_state&fSelect)==fSelect);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setselect(bool b){
	if(b){
		if((!selected())&&(!masked())){
			_state|=fSelect;
			_typ->inc_sel(+1);
		}
	}
	else{
		if(selected()){
			_state^=fSelect;
			_typ->inc_sel(-1);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setselect(bool b, bool report){
	if(b){
		if((!selected())&&(!masked())){
			setselect(true);
			selmgr->add(this);
		}
	}
	else{
		if(selected()){
			setselect(false);
			selmgr->rmv(this);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::contrasted(){
	return((_state&fContrast)==fContrast);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setcontrast(bool b){
	if(b){
		if((!contrasted())&&(!masked())){
			_state|=fContrast;
			_typ->inc_cont(+1);
		}
	}
	else{
		if(contrasted()){
			_state^=fContrast;
			_typ->inc_cont(-1);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setcontrast(bool b, bool unused){
	if(b){
		if((!contrasted())&&(!masked())){
			setcontrast(true);
			cntmgr->add(this);
		}
	}
	else{
		if(contrasted()){
			setcontrast(false);
			cntmgr->rmv(this);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::masked(){
	return((_state&fMask)==fMask);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setmasked(bool b, bool report){
	if(b){
		if(!masked()){
			if(report){
				layersmgr->InvalElement(this);
			}
			_state|=fMask;
			if(selected()){
				setselect(false,report);
			}
			if(contrasted()){
				cntmgr->rmv(this);
			}
		}
	}
	else{
		if(masked()){
			_state^=fMask;
			if(contrasted()){
				cntmgr->add(this);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::atscreen(){
	return((_state&fScreen)==fScreen);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setatscreen(bool b){
	if(b){
		if((!atscreen())&&(!masked())){
			_state|=fScreen;
		}
	}
	else{
		if(atscreen()){
			_state^=fScreen;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::needupdate(){
	return((_state&fUpdate)==fUpdate);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::setneedupdate(bool b){
	if(b){
		if((!needupdate())&&(!masked())){
			_state|=fUpdate;
		}
	}
	else{
		if(needupdate()){
			_state^=fUpdate;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::handled(){
	return((_state&fHandled)==fHandled);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::sethandled(bool b){
	if(b){
		if(!handled()){
			_state|=fHandled;
		}
	}
	else{
		if(handled()){
			_state^=fHandled;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGeoElement::getID(){
	return(_id);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGeoElement::getType(){
	return(_typ->index());
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::getBounds(ivx_rect* vxr){
	(*vxr)=_vxr;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::getVertices(ivertices** vxs){
	(*vxs)=_vxs;
}

// ---------------------------------------------------------------------------
// Normalement, pas de pb si vxs == _vxs
// ------------
bool bMacMapGeoElement::setVertices(ivertices* vxs){
_bTrace_("bMacMapGeoElement::setVertices",false);
//if(_id==210298){
//_tm_(_id+": _vxs="+(void*)_vxs);
//}
    
	if(check_lock(kOBJ_Vertices_)){
_te_("kOBJ_Vertices_ is lock");
		return(false);
	}
	if(!vxs){
_te_("vxs==NULL");
		return(false);
	}
int status;
	if((status=ivs_good(vxs))){
_te_("ivs_good returns "+status);
		return(false);
	}
	if((_typ->kind()==kBaseKindPolyline)&&(vxs->nv<2)){
_te_("nb vertices="+vxs->nv+" for kBaseKindPolyline kind");
		return(false);
	}
	if((_typ->kind()==kBaseKindPolygon)&&(vxs->nv<4)){
_te_("nb vertices="+vxs->nv+" for kBaseKindPolygon kind");
		return(false);
	}
	if((_typ->kind()==kBaseKindRaster)&&(vxs->nv<4)){
_te_("nb vertices="+vxs->nv+" for kBaseKindRaster kind");
		return(false);
	}
	if(!ivs_goodgeo(vxs)){
_te_("ivs_goodgeo=false");
		return(false);
	}

ivertices*	buf;
ivx_rect	vr;
	ivs_bounds(vxs,&vr);
	if(_typ->kind()==kBaseKindRaster){
		if(ivr2ivs(&vr,&buf)){
_te_("buf==NULL");
			return(false);
		}
	}
	else{
		if(ivs2ivs(vxs,&buf)){
_te_("buf==NULL");
			return(false);
		}
        if((_typ->kind()==kBaseKindPoint)&&(buf->nv>1)){
            ivs_build_point_offsets(buf);
        }
	}
	
	eventmgr->modify(this,kOBJ_Vertices_);

	if(_typ->fields()->write(_offset,kOBJ_Vertices_,&buf)){
		ivs_free(buf);
_te_("write vxs");
		return(false);
	}
    
	inval(kOBJ_Vertices_);
	
	if(!_typ->iterator()->rmv(this)){
_te_("iterator->rmv");
		return(false);
	}
	
	if(_vxs){
		ivs_free(_vxs);
	}
	_vxs=buf;
	_vxr=vr;

	if(!_typ->iterator()->add(this)){
_te_("iterator->add");
		return(false);
	}
	
	inval(kOBJ_Vertices_);
//if(_id==210298){
//_tm_(_id+": _vxs="+(void*)_vxs);
//}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGeoElement::getSubType(){
	return(_styp);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setSubType(int subtype){
	if(check_lock(kOBJ_SubType_)){
		return(false);
	}
	eventmgr->modify(this,kOBJ_SubType_);
	if(_typ->fields()->write(_offset,kOBJ_SubType_,&subtype)==noErr){
		_styp=subtype;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGeoElement::getColor(){
	return(_color);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setColor(int color){
	if(check_lock(kOBJ_Color_)){
		return(false);
	}
	eventmgr->modify(this,kOBJ_Color_);
	if(_typ->fields()->write(_offset,kOBJ_Color_,&color)==noErr){
		_color=color;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGeoElement::getDirection(){
	return(_dir);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setDirection(int direction){
	if(check_lock(kOBJ_Dir_)){
		return(false);
	}
	eventmgr->modify(this,kOBJ_Dir_);
	if(_typ->fields()->write(_offset,kOBJ_Dir_,&direction)==noErr){
		_dir=direction;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::getName(char* name){
	if(_typ->fields()->read(_offset,kOBJ_Name_,name)){
		name[0]=0;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setName(char* name){
	if(check_lock(kOBJ_Name_)){
		return(false);
	}
	eventmgr->modify(this,kOBJ_Name_);
	if(_typ->fields()->write(_offset,kOBJ_Name_,name)){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(int fieldIdx, void* value){
	if((fieldIdx>kOBJ_Dir_)||(fieldIdx<kOBJ_Rect_)){
		if(_typ->fields()->read(_offset,fieldIdx,value)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:	
				if(_typ->fields()->read(_offset,fieldIdx,value)){
					return(false);
				}
				break;
			case kOBJ_SubType_:
				(*((int*)value))=getSubType();
				break;
			case kOBJ_Color_:
				(*((int*)value))=getColor();
				break;
			case kOBJ_Dir_:
				(*((int*)value))=getDirection();
				break;
			case kOBJ_Vertices_:
				getVertices((ivertices**)value);
				break;
			case kOBJ_Rect_:
				getBounds((ivx_rect*)value);
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(char* fieldName, void* value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(getValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(int fieldIdx, char* value){
int		x;
char	buff[1024];
int		k,d,ck;
	
	_typ->fields()->get_kind(fieldIdx,&k);
	_typ->fields()->get_decs(fieldIdx,&d);
	
	value[0]=0;
	if((fieldIdx>kOBJ_Dir_)||(fieldIdx<kOBJ_Vertices_)){
		switch(k){
			case _char:
			case _bool:
			case _int:
			case _double:
			case _date:
			case _time:
				if(_typ->fields()->read(_offset,fieldIdx,buff)){
					return(false);
				}
				break;
			default:
				return(false);
				break;
		}
		if(_typ->fields()->count_constraints(fieldIdx)>0){
			ck=_typ->fields()->get_constraints_kind(fieldIdx);
			if((ck!=k)&&(ck!=_bit)){
				if(!xToInt(k,d,buff,&x)){
					return(false);
				}
				if(_typ->fields()->get_constraint(fieldIdx,x,value)==false){
					value[0]=0;
					return(false);
				}
			}
			else{
				if(!xToChar(k,d,buff,value)){
					return(false);
				}			
			}
		}
		else{
			if(!xToChar(k,d,buff,value)){
				return(false);
			}
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:
				if(_typ->fields()->read(_offset,kOBJ_Name_,value)){
					value[0]=0;
					return(false);
				}
				break;
			case kOBJ_SubType_:
				if(_typ->fields()->count_constraints(kOBJ_SubType_)>0){
					if(_typ->fields()->get_constraint(kOBJ_SubType_,getSubType(),value)==false){
						value[0]=0;
						return(false);
					}
				}
				else{// Possible que certaines base n'aient pas de soustype à contrainte : Grid ASCII
					sprintf(value,"%d",getSubType());
				}
				break;
			case kOBJ_Color_:
				if(_typ->fields()->count_constraints(kOBJ_Color_)>0){
					if(_typ->fields()->get_constraint(kOBJ_Color_,getColor(),value)==false){
						value[0]=0;
						return(false);
					}
				}
				else{// Possible que certaines base n'aient pas de couleur à contrainte : Grid ASCII
					sprintf(value,"%d",getColor());
				}
				break;
			case kOBJ_Dir_:
				sprintf(value,"%d",getDirection());
				break;
			case kOBJ_Vertices_:{// Pour avoir un truc à afficher
ivertices*		vxs;
d2dvertex		vx;
				getVertices(&vxs);
				_MMAPP_->locConverter()->convert(&vx,&_vxs->vx.vx2[0]);
				sprintf(value,"%.*f;%.*f",	_MMAPP_->document()->dist_pref_digits(),vx.x,
											_MMAPP_->document()->dist_pref_digits(),vx.y);
				}break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(char* fieldName, char* value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(getValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(int fieldIdx, int* value){
	if((fieldIdx>kOBJ_Dir_)||(fieldIdx<kOBJ_Vertices_)){
char	buff[1024];
int		k,d;
		_typ->fields()->get_kind(fieldIdx,&k);
		_typ->fields()->get_decs(fieldIdx,&d);
		switch(k){
			case _char:
			case _bool:
			case _int:
			case _double:
			case _date:
			case _time:
				if(_typ->fields()->read(_offset,fieldIdx,buff)){
					return(false);
				}
				break;
			default:
				return(false);
				break;
		}
		if(!xToInt(k,d,buff,value)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:{
char	val[256];
				if(_typ->fields()->read(_offset,fieldIdx,val)){
					(*value)=LONG_MIN;
					return(false);
				}
				(*value)=atoi(val);
				}
				break;
			case kOBJ_SubType_:
				(*value)=getSubType();
				break;
			case kOBJ_Color_:
				(*value)=getColor();
				break;
			case kOBJ_Dir_:
				(*value)=getDirection();
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(char* fieldName, int* value){
long	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(getValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(int fieldIdx, double* value){
	if((fieldIdx>kOBJ_Dir_)||(fieldIdx<kOBJ_Vertices_)){
char	buff[1024];
int		k,d;
		_typ->fields()->get_kind(fieldIdx,&k);
		_typ->fields()->get_decs(fieldIdx,&d);
		switch(k){
			case _char:
			case _bool:
			case _int:
			case _double:
			case _date:
			case _time:
				if(_typ->fields()->read(_offset,fieldIdx,buff)){
					return(false);
				}
				break;
			default:
				return(false);
				break;
		}
		if(!xToDouble(k,d,buff,value)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:{
char	val[256];
				if(_typ->fields()->read(_offset,fieldIdx,val)){
					(*value)=NAN;
					return(false);
				}
				(*value)=matof(val);
				}
				break;
			case kOBJ_SubType_:
				(*value)=getSubType();
				break;
			case kOBJ_Color_:
				(*value)=getColor();
				break;
			case kOBJ_Dir_:
				(*value)=getDirection();
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::getValue(char *fieldName, double *value){
long	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(getValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(int fieldIdx, void* value){
	if(check_lock(fieldIdx)){
		return(false);
	}
	if(fieldIdx>kOBJ_Dir_){
		eventmgr->modify(this,fieldIdx);
		if(_typ->fields()->write(_offset,fieldIdx,value)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:
				return(setName((char*)value));
				break;
			case kOBJ_SubType_:
				return(setSubType(*(int*)value));
				break;
			case kOBJ_Color_:
				return(setColor(*(int*)value));
				break;
			case kOBJ_Dir_:
				return(setDirection(*(int*)value));
				break;
			case kOBJ_Vertices_:
				return(setVertices((ivertices*)value));
				break;
			default:
				return(false);
				break;
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(char* fieldName, void* value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(setValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(int fieldIdx, char* value){
	if(check_lock(fieldIdx)){
		return(false);
	}

	if(fieldIdx>kOBJ_Dir_){
char	buff[1024];
int		fk,ck,d,x;
		_typ->fields()->get_kind(fieldIdx,&fk);
		ck=_typ->fields()->get_constraints_kind(fieldIdx);
		_typ->fields()->get_decs(fieldIdx,&d);
		if((ck!=0)&&(ck!=_bit)&&(ck!=fk)){// Classe
			x=_typ->fields()->get_constraint_index(fieldIdx,value);
			if(!x){
				return(false);
			}
			if(!intToX(fk,d,x,buff)){
				return(false);
			}
		}
		else{
			if(!charToX(fk,d,value,buff)){
				return(false);
			}
		}
		eventmgr->modify(this,fieldIdx);
		if(_typ->fields()->write(_offset,fieldIdx,buff)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:
				return(setName(value));
				break;
			case kOBJ_SubType_:
				return(setSubType(_typ->fields()->get_constraint_index(fieldIdx,value)));
				break;
			case kOBJ_Color_:
				return(setColor(_typ->fields()->get_constraint_index(fieldIdx,value)));
				break;
			case kOBJ_Dir_:
				return(setDirection(atoi(value)));
				break;
			default:
				return(false);
				break;
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(char* fieldName, char* value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(setValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(int fieldIdx, int value){
	if(check_lock(fieldIdx)){
		return(false);
	}
	if(fieldIdx>kOBJ_Dir_){
char	buff[1024];
int		k,d;
		_typ->fields()->get_kind(fieldIdx,&k);
		_typ->fields()->get_decs(fieldIdx,&d);
		if(!intToX(k,d,value,buff)){
			return(false);
		}
		eventmgr->modify(this,fieldIdx);
		if(_typ->fields()->write(_offset,fieldIdx,buff)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:
				{
char val[256];
				sprintf(val,"%d",value);
				return(setName(val));
				}
				break;
			case kOBJ_SubType_:
				return(setSubType(value));
				break;
			case kOBJ_Color_:	
				return(setColor(value));
				break;
			case kOBJ_Dir_:
				return(setDirection(value));
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(char* fieldName, int value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(setValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(int fieldIdx, double value){
	if(check_lock(fieldIdx)){
		return(false);
	}
	eventmgr->modify(this,fieldIdx);
	if(fieldIdx>kOBJ_Dir_){
char	buff[1024];
int		k,d;
		_typ->fields()->get_kind(fieldIdx,&k);
		_typ->fields()->get_decs(fieldIdx,&d);
		if(!doubleToX(k,d,value,buff)){
			return(false);
		}
		if(_typ->fields()->write(_offset,fieldIdx,buff)){
			return(false);
		}
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:
				{
char val[256];
				sprintf(val,"%f",value);
				return(setName(val));
				}				
				break;
			case kOBJ_SubType_:
				return(setSubType(round(value)));
				break;
			case kOBJ_Color_:
				return(setColor(round(value)));
				break;
			case kOBJ_Dir_:
				return(setDirection(round(value)));
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::setValue(char* fieldName, double value){
int	idx=_typ->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(setValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::flag1(){
	return((_state&fFlag1)==fFlag1);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_flag1(bool b){
	if(b){
		if(!flag1()){
			_state|=fFlag1;
		}
	}
	else{
		if(flag1()){
			_state^=fFlag1;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::flag2(){
	return((_state&fFlag2)==fFlag2);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_flag2(bool b){
	if(b){
		if(!flag2()){
			_state|=fFlag2;
		}
	}
	else{
		if(flag2()){
			_state^=fFlag2;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::flag3(){
	return((_state&fFlag3)==fFlag3);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_flag3(bool b){
	if(b){
		if(!flag3()){
			_state|=fFlag3;
		}
	}
	else{
		if(flag3()){
			_state^=fFlag3;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::flag4(){
	return((_state&fFlag4)==fFlag4);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_flag4(bool b){
	if(b){
		if(!flag4()){
			_state|=fFlag4;
		}
	}
	else{
		if(flag4()){
			_state^=fFlag4;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::is_lock(){
	//return((_state&fMask)==fMask);
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_lock(bool b){
	/*if(b){
		if(!masked()){
			if(report){
				layersmgr->invalElement(this);
			}
			_state|=fMask;
			if(selected()){
				setselect(false,report);
			}
			if(contrasted()){
				setcontrast(false,report);
			}
		}
	}
	else{
		if(masked()){
			_state^=fMask;
			if(report){
				layersmgr->invalElement(this);
			}
		}
	}*/
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::inval(int idx){
	if(!_inval){
		return;
	}
bGenericStyle*	stl;
CGRect			cgr;
	
	for(long i=1;i<=_MMAPP_->layersAccessCtx()->count();i++){
		stl=_MMAPP_->layersAccessCtx()->get(i);
		if(stl->gettype()!=_typ){
			continue;
		}
		stl->rect(this,&cgr);
		_MMAPP_->mapIntf()->inval(CGRectInset(cgr,-3,-3));
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_inval(bool flg){
	_inval=flg;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::get_inval(){
	return(_inval);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGeoElement::set_save_objstate(bool flg){
	_save=flg;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::get_save_objstate(){
	return(_save);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bMacMapGeoElement::get_ref(){
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapGeoElement::check_lock(int idx){
// TYPE
// CHAMP
// OBJET
//    return((is_lock())&&(_typ->is_lock())&&(_typ->fields()->is_writeprotected(idx)));
    return((is_lock())||(_typ->is_lock())||(_typ->fields()->is_writeprotected(idx)));
}


