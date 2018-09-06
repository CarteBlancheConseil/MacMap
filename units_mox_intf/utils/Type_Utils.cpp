//----------------------------------------------------------------------------
// File : Type_Utils.cpp
// Project : MacMap
// Purpose : C++ source file : C utils around MacMap types
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
// 28/11/2005 creation.
//----------------------------------------------------------------------------

#include <mox_intf/Type_Utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bGenericGeoElement.h>

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithType(bGenericMacMapApp* gapp, ControlRef c, int kind, int cur){
MenuRef				m=GetPopupControlMenu(c);
char				cname[256];
bGenericTypesMgr*	mgr=gapp->typesMgr();	
bGenericType*		tp;	
ControlKind			k;

	for(int i=1;i<=mgr->count();i++){
		tp=mgr->get(i);
		tp->name(cname);
		AddMenuItemValue(m,cname);
		if((kind!=kBaseNoKind)&&(tp->kind()!=kind)){
			DisableMenuItem(m,CountMenuItems(m));
		}
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
		SetControl32BitValue(c,cur);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int SelectionIsMonoType(bGenericMacMapApp* gapp){
bGenericType*	tp;
int				k=0;

	for(int i=1;i<=gapp->typesMgr()->count();i++){
		tp=gapp->typesMgr()->get(i);
		if(tp->nb_sel()==0){
			continue;
		}
		if(k==0){
			k=i;
		}
		else{
			return(0);
		}
	}
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
int ContrastesAreMonoType(bGenericMacMapApp* gapp){
bGenericType*	tp;
int				k=0;
	
	for(int i=1;i<=gapp->typesMgr()->count();i++){
		tp=gapp->typesMgr()->get(i);
		if(tp->nb_cont()==0){
			continue;
		}
		if(k==0){
			k=i;
		}
		else{
			return(0);
		}
	}
	return(k);
}


// ---------------------------------------------------------------------------
// 
// -----------
int SelectionIsMonoGender(bGenericMacMapApp* gapp){
int				i;
bGenericType*	tp;
int				k=kBaseNoKind;

	for(i=1;i<=gapp->typesMgr()->count();i++){
		tp=gapp->typesMgr()->get(i);
		if(tp->nb_sel()==0){
			continue;
		}
		if(k==kBaseNoKind){
			k=tp->kind();
		}
		else{
			if(k!=tp->kind()){
				return(kBaseNoKind);
			}
		}
	}
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
int FieldKindEqualToFieldConstraint(bGenericType* tp, int field){
int kf=0,kc=0;
	if(tp->fields()->count_constraints(field)==0){
		return(0);
	}
	if(!tp->fields()->get_kind(field,&kf)){
		return(0);
	}
	kc=tp->fields()->get_constraints_kind(field);
	return(kf==kc);
}

// ---------------------------------------------------------------------------
// 
// -----------
int SetXMLParamToType(	bGenericType* tp, 
						const char* sub, 
						const char* name,
						bGenericXMLBaseElement* root){
bMemFile	f;
char*		bf;
int			sz;

	if(root==NULL){
		return(tp->fields()->set_param(sub,name,NULL,0));
	}
	root->encode(0,&f);
	if(f.status()){
		return(0);
	}
	f.mount(&bf,&sz);
	if(f.status()){
		return(0);
	}
	if(!tp->fields()->set_param(sub,name,bf,sz)){
		free(bf);
		return(0);
	}
	free(bf);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* GetXMLParamFromType(bGenericMacMapApp* gapp,
											bGenericType* tp,
											const char* sub, 
											const char* name){
void*	bf=NULL;
int		sz=0;

	if(strstr(name,".xml")==NULL){
		return(NULL);
	}
	if(!tp->fields()->get_param(sub,name,&bf,&sz)){
		return(NULL);
	}
	if(!bf){
		return(NULL);
	}
//CFDataRef				xmlData=CFDataCreate(kCFAllocatorDefault,(UInt8*)bf,sz);
//	free(bf);
//	if(!xmlData){
//		return(NULL);
//	}
//bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(xmlData);
//	CFRelease(xmlData);

bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(bf,sz);
    free(bf);

    return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
typedef struct oid_r{
	int					oid;
	bGenericGeoElement*	geo;
}oid_r;

// ---------------------------------------------------------------------------
// 
// ------------
static int oid_r_comp(	const void* a,
						const void* b){
oid_r*	oa=(oid_r*)a;
oid_r*	ob=(oid_r*)b;
	if(oa->oid>ob->oid){
		return(1);
	}
	if(oa->oid<ob->oid){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int oid_hr_comp(	const void* a,
						const void* b){
	return(oid_r_comp(a,b));
}

// ---------------------------------------------------------------------------
// 
// ------------
static int oid_r_add(void *o, void *prm){
bArray*				arr=(bArray*)prm;
oid_r				r;
bGenericGeoElement* geo=(bGenericGeoElement*)o;

	r.geo=geo;
	r.oid=geo->getID();
	arr->add(&r);	
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool RecoverObjects(bGenericType* tp,
					bArray& oids,
					bArray& objects){
oid_r	oi;
int		k;
bool	r=true;
bArray	arr(sizeof(oid_r));

	tp->iterator()->iterate(&arr,oid_r_add);
	arr.sort(oid_r_comp);
//fprintf(stderr,"nb obj=%d\n",arr.count());
	for(int i=1;i<=oids.count();i++){
		oids.get(i,&oi.oid);
//fprintf(stderr,"%d ->%d\n",i,oi.oid);
		k=arr.search(&oi,oid_hr_comp);
		if(k==0){
fprintf(stderr,"%d not found\n",oi.oid);
			r=false;
			continue;
		}
//fprintf(stderr,"%d found\n",oi.oid);
		arr.get(k,&oi);
//fprintf(stderr,"adding %d\n",oi.oid);
//fflush(stderr);
		objects.add(&oi.geo);
//fprintf(stderr,"adding %d ok\n",oi.oid);
//fflush(stderr);
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
void add_fields(wbase bse, 
				field_desc* flds, 
				int n){
	for(int i=0;i<n;i++){
		flds[i].index=bse->get_index(flds[i].name);
		if(flds[i].index==0){
			bse->add(	flds[i].kind,
						flds[i].len,
						flds[i].decs,
						flds[i].name);
			flds[i].index=bse->get_index(flds[i].name);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int	TypeIsExportProtected(bGenericType* tp){
int	state;
	tp->fields()->h_read(1,kHDR_state_,&state);
	return((state&_kBaseExportLock_)==_kBaseExportLock_);
}
