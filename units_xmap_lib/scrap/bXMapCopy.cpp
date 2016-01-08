//----------------------------------------------------------------------------
// File : bXMapCopy.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, copy MacMap objects
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
// 21/11/2006 Creation.
// 29/04/2015 Elimination ScrapManager.
//----------------------------------------------------------------------------

#include "bXMapCopy.h"
#include "MMPasteBoardData.h"
#include <mox_intf/bStdWait.h>
#include <mox_intf/mm_messages.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
#define _FREE_AND_RETURN_ if(bf){free(bf);}if(vbf){free(vbf);}return true;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCopy	::bXMapCopy(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("copy");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCopy::~bXMapCopy(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapCopy::create(bGenericXMLBaseElement* elt){
	return(new bXMapCopy(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCopy::process(int msg, void* prm){
_bTrace_("bXMapCopy::process",true);
bArray	arr(*_gapp->selMgr()->elements());
	if(arr.count()==0){
		return(true);
	}
bGenericGeoElement*	o;
bGenericType*		tp;
UInt32				bsz=0,vbsz=256,vtsz;
void				*bf=NULL,*vbf=malloc(vbsz),*tmp;
tag					tg;
char				name[256];
int					k,fsz;
UInt32				etsz=(sizeof(tag)-sizeof(void*));
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	get_localized_name(mess,_bndl);
bBarberWait			wt(mess,NULL,true,true);
	
	for(long i=1;i<=arr.count();i++){
//_tm_(i);
        if(!wt.set_progress(0)){
            _FREE_AND_RETURN_
		}
		arr.get(i,&o);
		
		tp=_gapp->typesMgr()->get(o->getType());
		tp->name(name);
		tg.sign=kScrapMacMapTagType;
		tg.snsz=nmsz(name);
		tg.cnsz=0;
		vtsz=(etsz+tg.snsz+tg.cnsz);
		if(!bf){
			bf=malloc(vtsz);
		}
		else{
			tmp=realloc(bf,bsz+vtsz);
			if(!tmp){
_te_("realloc bf(1) failed for "+(unsigned int)vtsz);
                _FREE_AND_RETURN_
			}
            bf=tmp;
		}
		memcpy((void*)((UInt32)bf+bsz),&tg,etsz);
		bsz+=etsz;
		memcpy((void*)((UInt32)bf+bsz),name,tg.snsz);
		bsz+=tg.snsz;
		
		for(long j=kOBJ_Vertices_;j<=tp->fields()->count();j++){
//_tm_(i+" + "+j);
			tp->fields()->get_name(j,name);
			tp->fields()->get_kind(j,&k);
			tp->fields()->get_size(j,&fsz);
			tg.sign=kScrapMacMapTagField;
			tg.snsz=nmsz(name);
			
			switch(k){
				case _ivxs2:{
					ivertices*	vxs;
					tp->fields()->read(o->getOffset(),j,&vxs);
					if(j==kOBJ_Vertices_){
//#warning SI kOBJ_Vertices_ LE VXS EST L'ORIGINAL
						ivertices*	buf=vxs;
						ivs2ivs(buf,&vxs);
					}
                    if(vxs==NULL){
_te_("NULL vxs (1)");
                        _FREE_AND_RETURN_
                    }
					tg.cnsz=sizeofivs2(vxs->nv,vxs->no);
					vxs=ivs_pack(vxs);
                    if(vxs==NULL){
_te_("NULL vxs (2)");
                        _FREE_AND_RETURN_
                    }
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
                        if(!tmp){
_te_("realloc vbf(1) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
						}
                        vbf=tmp;
					}
					memcpy(vbf,vxs,tg.cnsz);
					free(vxs);
					}break;
				case _ivxs3:{
					ivertices*	vxs;
					tp->fields()->read(o->getOffset(),j,&vxs);
                    if(vxs==NULL){
_te_("NULL vxs (3)");
                        _FREE_AND_RETURN_
                    }
					vxs=ivs_pack(vxs);
                    if(vxs==NULL){
_te_("NULL vxs (4)");
                        _FREE_AND_RETURN_
                    }
					tg.cnsz=sizeofivs3(vxs->nv,vxs->no);
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
                        if(!tmp){
_te_("realloc vbf(2) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
						}
                        vbf=tmp;
					}
					memcpy(vbf,vxs,tg.cnsz);
					free(vxs);
					}break;
				case _dvxs2:{
					dvertices*	vxs;
					tp->fields()->read(o->getOffset(),j,&vxs);
                    if(vxs==NULL){
_te_("NULL vxs (5)");
                        _FREE_AND_RETURN_
                    }
					vxs=dvs_pack(vxs);
                    if(vxs==NULL){
_te_("NULL vxs (6)");
                        _FREE_AND_RETURN_
                    }
					tg.cnsz=sizeofdvs2(vxs->nv,vxs->no);
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
                        if(!tmp){
_te_("realloc vbf(3) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
						}
                        vbf=tmp;
					}
					memcpy(vbf,vxs,tg.cnsz);
					free(vxs);
					}break;
				case _dvxs3:{
					dvertices*	vxs;
					tp->fields()->read(o->getOffset(),j,&vxs);
                    if(vxs==NULL){
_te_("NULL vxs (7)");
                        _FREE_AND_RETURN_
                    }
					vxs=dvs_pack(vxs);
                    if(vxs==NULL){
_te_("NULL vxs (8)");
                        _FREE_AND_RETURN_
                    }
					tg.cnsz=sizeofdvs3(vxs->nv,vxs->no);
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
						if(!tmp){
_te_("realloc vbf(4) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
						}
                        vbf=tmp;
					}
					memcpy(vbf,vxs,tg.cnsz);
					free(vxs);
					}break;
				case _char:
					tg.cnsz=fsz;
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
                        if(!tmp){
_te_("realloc vbf(5) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
                        }
                        vbf=tmp;
					}
					tp->fields()->read(o->getOffset(),j,vbf);
					tg.cnsz=nmsz((char*)vbf);
					break;
				default:
					tg.cnsz=fsz;
					if(tg.cnsz>vbsz){
						vbsz=tg.cnsz;
						tmp=(UInt32*)realloc(vbf,vbsz);
                        if(!tmp){
_te_("realloc vbf(6) failed for "+(unsigned int)vbsz+" at ("+i+";"+j+")");
                            _FREE_AND_RETURN_
                        }
                        vbf=tmp;
					}
					tp->fields()->read(o->getOffset(),j,vbf);
					break;
			}
			
			vtsz=(etsz+tg.snsz+tg.cnsz);
			tmp=realloc(bf,bsz+vtsz);
			if(!tmp){
_te_("realloc bf(2) failed for "+(unsigned int)vtsz);
                _FREE_AND_RETURN_
			}
            bf=tmp;
			memcpy((void*)((UInt32)bf+bsz),&tg,etsz);
			bsz+=etsz;
			memcpy((void*)((UInt32)bf+bsz),name,tg.snsz);
			bsz+=tg.snsz;
			memcpy((void*)((UInt32)bf+bsz),vbf,tg.cnsz);
			bsz+=tg.cnsz;
		}
	}
    
    putPasteBoardData(bf,bsz);
    
	free(vbf);
	free(bf);
    
    killObjects(arr);

	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCopy::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCopy::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapCopy::killObjects(bArray& arr){
}

