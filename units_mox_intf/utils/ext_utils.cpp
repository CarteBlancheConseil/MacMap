//----------------------------------------------------------------------------
// File : ext_utils.cpp
// Project : MacMap
// Purpose : C++ source file : C utils for externs
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
// 13/02/2006 creation.
//----------------------------------------------------------------------------

#include "ext_utils.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericExtLib.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bGenericGeog.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/mm_messages.h>
#include <std_ext/bStdUserMacro.h>
#include <std_ext/bStdUserScript.h>
#include <QuickTime/Movies.h>
#include <MacMapSuite/wtable.h>
#include <MacMapSuite/wproj.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 UniqueSignature(bGenericMacMapApp* gapp){
char				res[10];
int					k=0;
UInt32				sign;

	while(true){
		k++;
		sprintf(res,"u%03d",k);
		sign=(*(UInt32*)res);
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(UInt32));
#endif
		if(gapp->toolMgr()->find(sign)){
			continue;
		}
		if(gapp->xmapMgr()->find(sign)){
			continue;
		}
		if(gapp->vdefMgr()->find(sign)){
			continue;
		}
		if(gapp->geogMgr()->find(sign)){
			continue;
		}
		if(gapp->calcMgr()->find(sign)){
			continue;
		}
		if(gapp->varMgr()->find(sign)){
			continue;
		}
		if(gapp->macroMgr()->find(sign)){
			continue;
		}
		if(gapp->scriptMgr()->find(sign)){
			continue;
		}
		if(gapp->xboxMgr()->find(sign)){
			continue;
		}
		break;
	}
	return((*(UInt32*)res));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericType* NthTypeOfKind(bGenericMacMapApp* gapp, int idx, int kind){
bGenericType*	tp=NULL;
int				k=0;
	for(int i=1;i<=gapp->typesMgr()->count();i++){
		tp=gapp->typesMgr()->get(i);
		if(tp->kind()==kind){
			k++;
			if(k==idx){
				return(tp);
			}
		}
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithScales(bGenericMacMapApp* gapp, ControlRef c, int current){
MenuRef				m=GetPopupControlMenu(c);
char				cname[256];
bGenericUnitMgr*	mgr=gapp->scaleMgr();	
bGenericUnit*		u;	
ControlKind			k;

	for(int i=1;i<=mgr->count();i++){
		u=mgr->get(i);
		u->long_name(cname);
		if(strlen(cname)==0){
			u->short_name(cname);
		}
		if(strlen(cname)==0){
			sprintf(cname,"%.0f",u->coef());;
		}
		AddMenuItemValue(m,cname);
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
		SetControl32BitValue(c,current);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithDistUnits(bGenericMacMapApp* gapp, ControlRef c, int current){
MenuRef				m=GetPopupControlMenu(c);
char				cname[256];
bGenericUnitMgr*	mgr=gapp->distMgr();	
bGenericUnit*		u;	
ControlKind			k;

	for(int i=1;i<=mgr->count();i++){
		u=mgr->get(i);
		u->long_name(cname);
		if(strlen(cname)==0){
			u->short_name(cname);
		}
		if(strlen(cname)==0){
			sprintf(cname,"%.0f",u->coef());;
		}
		AddMenuItemValue(m,cname);
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
		SetControl32BitValue(c,current);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithFields(bGenericType* tp, ControlRef c, int start, int cur){
char		cname[256];
ControlKind	k;

	for(int i=start;i<=tp->fields()->count();i++){
		tp->fields()->get_name(i,cname);
		AddPopupControlMenuItemValue(c,cname);
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitValue(c,cur);	
	}
	else if(k.kind==kControlKindBevelButton){
		SetBevelButtonMenuValue(c,cur);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithTypedFields(bGenericType* tp, ControlRef c, int start, int cur, int* types, int n){
char		cname[256];
ControlKind	k;
int			i,j,fieldk;
	
	for(i=start;i<=tp->fields()->count();i++){
		tp->fields()->get_name(i,cname);
		AddPopupControlMenuItemValue(c,cname);
		tp->fields()->get_kind(i,&fieldk);
		for(j=0;j<n;j++){
			if(fieldk==types[j]){
				break;
			}
		}
		if(j==n){
			DisablePopupControlMenuItem(c,i);
		}
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitValue(c,cur);	
	}
	else if(k.kind==kControlKindBevelButton){
		SetBevelButtonMenuValue(c,cur);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void PopulatePopupControlWithConstraints(bGenericType* tp, int field, ControlRef c, int cur){
MenuRef		m=GetPopupControlMenu(c);
ControlKind	k;
int			n=tp->fields()->count_constraints(field);
	if(n<1){
		return;
	}
	
int		i,fk,ck,d;
char	cval[256];
int		ival;
double	dval;

	tp->fields()->get_kind(field,&fk);
	tp->fields()->get_decs(field,&d);
	ck=tp->fields()->get_constraints_kind(field);

	if(ck!=fk){
		fk=_char;
	}
	switch(fk){
		case _bool:
		case _int:
			for(i=1;i<=n;i++){
				tp->fields()->get_constraint(field,i,&ival);
				sprintf(cval," %d",ival);
				AddMenuItemValue(m,cval);
				if(i==cur){
					CheckMenuItem(m,CountMenuItems(m),true);
				}
			}
			break;
		case _double:
			for(i=1;i<=n;i++){
				tp->fields()->get_constraint(field,i,&dval);
				sprintf(cval," %.*f",d,dval);
				AddMenuItemValue(m,cval);
				if(i==cur){
					CheckMenuItem(m,CountMenuItems(m),true);
				}
			}
			break;
		case _char:
			for(i=1;i<=n;i++){
				tp->fields()->get_constraint(field,i,cval);
				AddMenuItemValue(m,cval);
				if(i==cur){
					CheckMenuItem(m,CountMenuItems(m),true);
				}
			}
			break;
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
void PopulatePopupControlWithOperators(bGenericMacMapApp* gapp, ControlRef c, int current){
MenuRef				m=GetPopupControlMenu(c);
char				cname[256];
bGenericExtLibMgr*	mgr=gapp->extMgr();	
bGenericExtLib*		e;
bGenericExt*		x;
bArray				arr(sizeof(bGenericExtLib*));
ControlKind			k;
	
	mgr->getlibs(arr,kXMLSubClassExt);	
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&e);
		x=e->inst();
		if(x->getsubclass()!=kXMLSubClassOperator){
			continue;
		}
		e->name(cname);
		AddMenuItemValue(m,cname);
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
		SetControl32BitValue(c,current);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
float* GetDash(	bGenericMacMapApp* gapp,
				bGenericType* tp,
				const char* name,
				int* count){
	*count=0;

	if(strlen(name)==0){
		return(NULL);
	}
	
void*	data;
int		sz;
	if(!tp->fields()->get_param("dashes",name,&data,&sz)){
		return(NULL);
	}

//CFDataRef	xmlData=CFDataCreate(kCFAllocatorDefault,(const UInt8*)data,sz);
//	free(data);
//	if(!xmlData){
//		return(NULL);
//	}
	
bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(data,sz);
	free(data);
    if(!root){
		return(NULL);
	}
//	CFRelease(xmlData);
	
int	n=root->countelements();
	if(n==0){
		gapp->classMgr()->ReleaseXMLInstance(root);
		return(NULL);
	}
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
float*					dsh=new float[n];
	for(int i=1;i<=n;i++){
		elt=root->getelement(i);
		elt->getvalue(val);
		dsh[i-1]=atof(val);
	}
	gapp->classMgr()->ReleaseXMLInstance(root);

	*count=n;
	return(dsh);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSType GetImageKind(const char* name){
OSType	t;
Str255	s;
//	c2pstrcpy(s,name);
	memcpy(s+1,name,strlen(name));
	s[0]=strlen(name);
	if(QTGetFileNameExtension(s,0,&t)){
		t=0;
	}
	return(t);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGPDFDocumentRef GetPDFPattern(	bGenericMacMapApp* gapp,
								bGenericType* tp,
								const char* name){
	if(strlen(name)==0){
		return(NULL);
	}
OSType	t=GetImageKind(name);
	if(t!=kQTFileTypePDF){
		return(NULL);
	}
void*	data;
int		sz;
	if(!tp->fields()->get_param("patterns",name,&data,&sz)){
		return(NULL);
	}
CGPDFDocumentRef	pdf=PDFCreate(data,sz);
//	free(data); // Pas de libération ?
// le free ne pose pas de problème sous 10.6, et plante sous 10.5
	return(pdf);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGPDFDocumentRef GetPDFIcon(bGenericMacMapApp* gapp,
							bGenericType* tp,
							const char* name){
	if(strlen(name)==0){
		return(NULL);
	}
OSType	t=GetImageKind(name);
	if(t!=kQTFileTypePDF){
		return(NULL);
	}
void*	data;
int		sz;
    
    if(tp){
        if(!tp->fields()->get_param("icons",name,&data,&sz)){
            return(NULL);
        }
    }
    else{
        gapp->document()->readParam(&data,&sz,"icons",name);
    }
    
CGPDFDocumentRef	pdf=PDFCreate(data,sz);
//	free(data); // Pas de libération ?
// le free ne pose pas de problème sous 10.6, et plante sous 10.5
	return(pdf);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGImageRef GetImageIcon(bGenericMacMapApp* gapp,
						bGenericType* tp,
						const char* name){
	if(strlen(name)==0){
		return(NULL);
	}
OSType	t=GetImageKind(name);
void*	data;
int		sz;
    if(tp){
        if(!tp->fields()->get_param("icons",name,&data,&sz)){
            return(NULL);
        }
    }
    else{
        gapp->document()->readParam(&data,&sz,"icons",name);
    }

CGImageRef	img=CGImageCreateFromData(data,sz,t);
	free(data);
	return(img);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGImageRef GetRaster(bGenericMacMapApp* gapp,
					 bGenericGeoElement* o){
char	name[256];
	o->getName(name);
	if(strlen(name)==0){
		return(NULL);
	}
bGenericType*	tp=gapp->typesMgr()->get(o->getType());
OSType			t=GetImageKind(name);
void*			data;
int				sz;
	if(!tp->fields()->get_param("rasters",name,&data,&sz)){
		return(NULL);
	}
CGImageRef		img=CGImageCreateFromData(data,sz,t);
	free(data);
	return(img);
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 GetSignatureFromBundle(CFBundleRef bndl){
CFStringRef	cfs=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(bndl,CFSTR("MacMapLibSign"));
	if(!cfs){
		return(_kNULLSign_);
	}
char	sign[256];
	CFStringGetCString(cfs,sign,256,kCFStringEncodingMacRoman);
	return(*((UInt32*)sign));
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 GetSignature(bGenericXMLBaseElement* elt){
	return(GetSignatureFromBundle(elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void GetSignatureString(bGenericXMLBaseElement* elt, char* sign){
	sign[0]=0;
CFStringRef	cfs=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(elt->getbundle(),CFSTR("MacMapLibSign"));
	if(!cfs){
		return;
	}
	CFStringGetCString(cfs,sign,256,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
void GetName(bGenericXMLBaseElement* elt, char* name){
	get_localized_name(name,elt->getbundle());
}

// ---------------------------------------------------------------------------
// 
// ------------
double GetPrecisionCoef(bGenericMacMapApp* gapp){
bGenericUnit*	ur=gapp->document()->dist_resolution_unit();
bGenericUnit*	up=gapp->distMgr()->get();
double			r=((ur->coef()*gapp->document()->dist_resolution())/up->coef());
	return(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
double GetRawPrecisionCoef(bGenericMacMapApp* gapp){
bGenericUnit*	ur=gapp->document()->dist_resolution_unit();
double			r=((ur->coef()*gapp->document()->dist_resolution()));
	return(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
double Measure_d2i(bGenericMacMapApp* gapp, double m){
	return(m/GetPrecisionCoef(gapp));
}

// ---------------------------------------------------------------------------
// 
// ------------
double Measure_i2d(bGenericMacMapApp* gapp, double m){
	return(m*GetPrecisionCoef(gapp));
}

// ---------------------------------------------------------------------------
// INT CHECK
// ------------
int ObjRefComp(	const void* a,
				const void* b){
bGenericGeoElement*	oa=*((bGenericGeoElement**)a);
bGenericGeoElement*	ob=*((bGenericGeoElement**)b);
	if(oa<ob){
		return(-1);
	}
	if(oa>ob){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// INT CHECK
// ------------
int ObjIDComp(	const void* a,
				const void* b){
bGenericGeoElement*	oa=*((bGenericGeoElement**)a);
bGenericGeoElement*	ob=*((bGenericGeoElement**)b);
	if(oa->getID()<ob->getID()){
		return(-1);
	}
	if(oa->getID()>ob->getID()){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void SetFlagWrapper(void* o, bool b){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	geo->sethandled(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool GetFlagWrapper(void* o){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	return(geo->handled());
}

// ---------------------------------------------------------------------------
// 
// ------------
void GetBoundsWrapper(void* o, ivx_rect* r){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	geo->getBounds(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverGeog(bGenericMacMapApp* gapp,
				const char* formula){
bGenericGeog*				ext=NULL;
bGenericExtMgr*				mgr=gapp->geogMgr();
bGenericXMLFormulaElement*	groot;
char						name[_names_length_max_];

	for(int i=1;i<=mgr->count();i++){
		ext=(bGenericGeog*)(void*)mgr->get(i);
		groot=ext->root();
		groot->getclassname(name);
		if(strcmp(name,formula)==0){
			return(i);
		}
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverCalc(bGenericMacMapApp* gapp,
				const char* formula){
//_bTrace_("RecoverCalc",false);
	if(strcmp(formula,"fcalcuser")==0){
		return(0);
	}
bGenericCalc*	ext=NULL;
bGenericExtMgr*	mgr=gapp->calcMgr();
char			sgn[10];
char			name[_names_length_max_];


	for(int i=1;i<=mgr->count();i++){
		ext=(bGenericCalc*)(void*)mgr->get(i);
		ext->get_params(name,sgn);
//_tm_(formula+"->"+name+":"+sgn);		
		if(strlen(sgn)>0){
			if(strcmp(sgn,formula)==0){
				return(i);
			}
		}
		else{
			if(strcmp(name,formula)==0){
				return(i);
			}
		}
		/*//else{
		if(strlen(name)>0){
			if(strcmp(name,formula)==0){
				return(i);
			}
		}*/
		
		
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverVar(	bGenericMacMapApp* gapp,
				const char* formula){
	if(strcmp(formula,"fvaruser")==0){
		return(0);
	}
bGenericVar*	ext=NULL;
bGenericExtMgr*	mgr=gapp->varMgr();
char			sgn[10];
char			name[_names_length_max_];

	for(int i=1;i<=mgr->count();i++){
		ext=(bGenericVar*)(void*)mgr->get(i);
		ext->get_params(name,sgn);
		if(strlen(sgn)>0){
			if(strcmp(sgn,formula)==0){
				return(i);
			}
		}
		else{
			if(strcmp(name,formula)==0){
				return(i);
			}
		}
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverMacro(	bGenericMacMapApp* gapp,
					const char* formula){
bStdUserMacro*	ext=NULL;
bGenericExtMgr*	mgr=gapp->macroMgr();
char			sgn[10];
user_ext_data	prm={NULL,0,NULL};

	for(int i=1;i<=mgr->count();i++){
		ext=(bStdUserMacro*)(void*)mgr->get(i);
		ext->process(kExtProcessCallGetData,&prm);
		if(prm.sign!=0){
			sprintf(sgn,"%.4s",(char*)&prm.sign);
			if(strcmp(sgn,formula)==0){
				return(i);
			}
		}
		/*else */if(prm.name){
			if(strcmp(prm.name,formula)==0){
				return(i);
			}
		}
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverScript(	bGenericMacMapApp* gapp,
					const char* formula){
bStdUserScript*	ext=NULL;
bGenericExtMgr*	mgr=gapp->scriptMgr();
char			sgn[10];
user_ext_data	prm={NULL,0,NULL};

	for(int i=1;i<=mgr->count();i++){
		ext=(bStdUserScript*)(void*)mgr->get(i);
		ext->process(kExtProcessCallGetData,&prm);
		if(prm.sign!=0){
			sprintf(sgn,"%.4s",(char*)&prm.sign);
			if(strcmp(sgn,formula)==0){
				return(i);
			}
		}
		else if(prm.name){
			if(strcmp(prm.name,formula)==0){
				return(i);
			}
		}
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int RecoverScale(	bGenericMacMapApp* gapp,
					bGenericUnit* u){
bGenericUnitMgr*	mgr=gapp->scaleMgr();
bGenericUnit*		lu;

	for(int i=1;i<=mgr->count();i++){
		lu=mgr->get(i);
		if(lu==u){
			return(i);
		}
	} 
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int InitWithStyle(	bGenericMacMapApp* gapp,
					int style_index,
					bGenericGeoElement*	o){
bGenericStyle*	styl=gapp->layersAccessCtx()->get(style_index);
	if(!styl){
		return(0);
	}
char*	fld=styl->getclassfield();
	if(fld==NULL){
		return(0);
	}
double	d=styl->getclassbound(styl->get_curclass());
	return(o->setValue(fld,d));
}

// ---------------------------------------------------------------------------
// 
// ------------
static bool dta_search(bGenericXMLBaseElement* elt, void* prm, int indent){
//_bTrace_("dta_search",false);
char	bf[_values_length_max_];
char*	fld=(char*)prm;
	elt->getclassname(bf);
//_tm_(bf);
	if(	(!strcmp(bf,"classfield"))		||
		(!strcmp(bf,"subclassfield"))	||
		(!strcmp(bf,"field"))			||
		(!strcmp(bf,"colorfield8"))		||
		(!strcmp(bf,"colorfield16m"))	||
		(!strcmp(bf,"dbfield"))			){
		elt->getvalue(bf);
//_tm_(bf+"->"+fld);
		if(!strcmp(bf,fld)){
//_tm_("YEAHAAAAAAAA!!!!");
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void IdleApp(	){
	YieldToAnyThread();
}

// ---------------------------------------------------------------------------
// 
// ------------
int FieldInView(	bGenericMacMapApp* gapp,
					bGenericType* tp,
					int field){
//_bTrace_("FieldInView",true);

	if(field==kOBJ_Vertices_){
		return(1);
	}
bGenericStyle*			styl;
bGenericXMLBaseElement*	root;
char					f[256];

	tp->fields()->get_name(field,f);
	for(int i=1;i<=gapp->layersAccessCtx()->count();i++){
		styl=gapp->layersAccessCtx()->get(i);
		if(styl->gettype()!=tp){
			continue;
		}
		root=styl->root();
		if(!root){
			continue;			
		}
		if(!root->dotoall(f,0,dta_search)){
			return(1);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int GetBufferFromXMLParam(	bGenericXMLBaseElement* root,
							void** buffer,
							int* bsz){
bMemFile	f;
char*		bf;
int			sz;

	(*buffer)=NULL;
	(*bsz)=0;

	root->encode(0,&f);
	if(f.status()){
		return(0);
	}
	f.mount(&bf,&sz);
	if(f.status()){
		return(0);
	}
	(*buffer)=(void*)bf;
	(*bsz)=sz;
	return(1);
}

// ---------------------------------------------------------------------------
// 
// -----------
int GetXMLParamFromBuffer(	bGenericMacMapApp* gapp,
							bGenericXMLBaseElement** root,
							void* buffer,
							int bsz){
//CFDataRef	xmlData=CFDataCreate(kCFAllocatorDefault,(UInt8*)buffer,bsz);
//	if(!xmlData){
//		return(0);
//	}
	(*root)=gapp->classMgr()->ParseXMLData(buffer,bsz);
//	CFRelease(xmlData);
	return((*root)!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* GetXMLParamFromBundle(bGenericMacMapApp* gapp,
											  CFBundleRef bndl,
											  const char* name){
char		path[PATH_MAX];
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
CFURLRef	url=CFBundleCopyResourceURL(bndl,cfs,CFSTR("xml"),NULL);
	CFRelease(cfs);
	if(!url){
		return(NULL);
	}
	cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
	CFRelease(url);
	CFStringGetCString(cfs,path,PATH_MAX,kCFStringEncodingMacRoman);
	CFRelease(cfs);
	
void*		buffer;
int			sz;
bStdFile	stl(path,"r");
	if(stl.status()){
		return(NULL);
	}
	stl.mount((char**)&buffer,&sz);
//CFDataRef	xmlData=CFDataCreate(kCFAllocatorDefault,(UInt8*)buffer,sz);
//	free(buffer);
//	if(!xmlData){
//		return(NULL);
//	}
bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(buffer,sz);
    free(buffer);
//	CFRelease(xmlData);
	return(root);	
}

// ---------------------------------------------------------------------------
// 
// ------------
void SetMarginToCurview(bGenericMacMapApp* gapp, 
						double d){
bGenericStyle*	stl;
long			margin=Measure_d2i(gapp,d);
	
	for(long i=1;i<=gapp->layersAccessCtx()->count();i++){
		stl=gapp->layersAccessCtx()->get(i);
		stl->setmargin(margin);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void SaveCurrentMargins(bGenericMacMapApp* gapp, 
						bArray** margins){
bGenericStyle*	stl;
long			mrg;
	(*margins)=new bArray(sizeof(long));
	for(long i=1;i<=gapp->layersAccessCtx()->count();i++){
		stl=gapp->layersAccessCtx()->get(i);
		mrg=stl->getmargin();
		(*margins)->add(&mrg);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int RestoreCurrentMargins(bGenericMacMapApp* gapp, 
						  bArray* margins){
bGenericStyle*	stl;
long			mrg;
	for(long i=1;i<=gapp->layersAccessCtx()->count();i++){
		if(margins->get(i,&mrg)==false){
			return(-1);
		}
		stl=gapp->layersAccessCtx()->get(i);
		stl->setmargin(mrg);
	}
	return(0);	
}

// ---------------------------------------------------------------------------
// 
// ------------
Boolean db_file_filter(	AEDesc* item, 
						void* xinfo, 
						void* cba, 
						NavFilterModes mode){
NavFileOrFolderInfo*	info=(NavFileOrFolderInfo*)xinfo;
OSErr					err;
FSRef					ref;
CFURLRef				url;
CFStringRef				cfs;
char					ext[FILENAME_MAX];

	if(item->descriptorType==typeFSRef) {
		if(!info->isFolder){
			if((err=AEGetDescData(item,&ref,sizeof(FSRef)))){
				return(false);
			}
			url=CFURLCreateFromFSRef(kCFAllocatorDefault,&ref);
			cfs=CFURLCopyPathExtension(url);
			CFRelease(url);
			if(!cfs){
				return(false);
			}
			CFStringGetCString(cfs,ext,FILENAME_MAX,kCFStringEncodingUTF8);
			CFRelease(cfs);
			return(ext2sign(ext)!=0);
		}
		else{
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
long GetBestScaleIndexForValue(bGenericMacMapApp* gapp,
							   double val){
long				bidx=1;
double				d,dmin=LONG_MAX;
bGenericUnitMgr*	mgr=gapp->scaleMgr();
bGenericUnit*		u;	

	for(long i=1;i<=mgr->count();i++){
		u=mgr->get(i);
		d=fabs(u->coef()-val);
		if(d<dmin){
			dmin=d;
			bidx=i;
		}
	}
	return(bidx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void GetBasePath(bGenericMacMapApp* gapp, 
				 char* path){
//_bTrace_("GetBasePath",true);
	gapp->document()->location(path);
//_tm_("base path :"+path);
	if(path[strlen(path)-1]=='/'){
		path[strlen(path)-1]=0;
	}
char*	c=strrchr(path,'/');
	if(c){
		c[0]=0;
	}
//_tm_("base path :"+path);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGRect ivr2cgr(bGenericMacMapApp* gapp, ivx_rect ivr){
i2dvertex	vxa,vxb;
	vxa.h=ivr.left;
	vxa.v=ivr.bottom;
	vxb.h=ivr.right;
	vxb.v=ivr.top;
CGPoint	cga,cgb;
	gapp->locConverter()->convert(&cga,&vxa);
	gapp->locConverter()->convert(&cgb,&vxb);
	return(CGRectMake(cga.x,cga.y,cgb.x-cga.x,cgb.y-cga.y));
}

// ---------------------------------------------------------------------------
// 
// -----------
ivx_rect cgr2ivr(bGenericMacMapApp* gapp, CGRect cgr){
CGPoint	cga,cgb;
	cga.x=CGRectGetMinX(cgr);
	cga.y=CGRectGetMaxY(cgr);
	cgb.x=CGRectGetMaxX(cgr);
	cgb.y=CGRectGetMinY(cgr);
i2dvertex	vxa,vxb;
	gapp->locConverter()->convert(&vxa,&cga);
	gapp->locConverter()->convert(&vxb,&cgb);
ivx_rect ivr;
	ivrset(&ivr,vxa.h,vxa.v,vxb.h,vxb.v);
	return(ivr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool GetAName(char* name, const char* wtitle, bool cncl){
OSStatus			status;
IBNibRef			nib=NULL;
const EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
ynrec				wr={NULL,false};
CFBundleRef			bndl=CFBundleGetMainBundle();

	for(;;){
		status=CreateNibReferenceWithCFBundle(bndl,CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("GetAName"),&wr.wd);
		if(status){
			break;
		}
		if(wtitle){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,wtitle,kCFStringEncodingMacRoman);
			SetWindowTitleWithCFString(wr.wd,cfs);
			CFRelease(cfs);
		}
EventHandlerUPP hdlr=yn_evt_hdlr;
		InstallWindowEventHandler(wr.wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)&wr,NULL);

ControlID		cid={'GANm',1};
ControlRef		c;
char			val[256];

		GetControlByID(wr.wd,&cid,&c);
		SetTextControlValue(c,name);
		SetKeyboardFocus(wr.wd,c,kControlFocusNextPart);
		
		if(!cncl){
			cid.id=2;
			GetControlByID(wr.wd,&cid,&c);
			DisableControl(c);
			HideControl(c);
		}

		ShowWindow(wr.wd);
		status=RunAppModalLoopForWindow(wr.wd);
		if(status){
			break;
		}
		
		if(wr.ok){
			cid.id=1;
			GetControlByID(wr.wd,&cid,&c);
			GetTextControlValue(c,val,sizeof(val)-1);
			strcpy(name,val);
		}

		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(wr.wd){
		DisposeWindow(wr.wd);
	}
	return(wr.ok);
}

// ---------------------------------------------------------------------------
// 
// -----------
typedef struct ynrecaskforval{
	WindowRef		wd;
	bool			ok;
	bGenericType*	tp;
	int				field;
}ynrecaskforval;

// ---------------------------------------------------------------------------
// 
// -----------
static OSStatus ynaskforval_evt_hdlr(EventHandlerCallRef hdlr, 
									 EventRef evt, 
									 void *up){
OSStatus		result=eventNotHandledErr;
HICommand		cmd;
ynrecaskforval*	x=(ynrecaskforval*)up;
ControlRef		c;
ControlID		cid={'GAVl',2};
int				idx,ck,d,fk;
char			str[1024],buffer[1024];
	
	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				QuitAppModalLoopForWindow(x->wd);
				x->ok=true;
				result=noErr;
				break;
			case kHICommandCancel:
				QuitAppModalLoopForWindow(x->wd);
				x->ok=false;
				result=noErr;
				break;
			case 'PopV':
				GetControlByID(x->wd,&cid,&c);
				idx=GetControl32BitValue(c);
				if(idx){
					ck=x->tp->fields()->get_constraints_kind(x->field);
					x->tp->fields()->get_decs(x->field,&d);
					x->tp->fields()->get_kind(x->field,&fk);
					if(fk!=ck){
						sprintf(str,"%d",idx);
					}
					else{
						x->tp->fields()->get_constraint(x->field,idx,buffer);
						xToChar(ck,d,buffer,str);
					}
					cid.id=1;
					GetControlByID(x->wd,&cid,&c);
					SetTextControlValue(c,str);
				}
				break;
		}
	}
	return(result);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool GetAValue(bGenericType* tp,
			   int field,
			   const char* prompt,
			   char* value){
OSStatus			status;
IBNibRef			nib=NULL;
const EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
ynrecaskforval		wr={NULL,false,tp,field};
CFBundleRef			bndl=CFBundleGetMainBundle();
	
	value[0]=0;
	for(;;){
		status=CreateNibReferenceWithCFBundle(bndl,CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("GetAValue"),&wr.wd);
		if(status){
			break;
		}
		
		EventHandlerUPP hdlr=ynaskforval_evt_hdlr;
		InstallWindowEventHandler(wr.wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)&wr,NULL);
		
		ControlID		cid={'GAVl',1};
		ControlRef		c;
		char			val[256];
		
		GetControlByID(wr.wd,&cid,&c);
		SetKeyboardFocus(wr.wd,c,kControlFocusNextPart);
		cid.id=2;
		GetControlByID(wr.wd,&cid,&c);
		ClearPopupControl(c,1);
		if((tp==NULL)||(field<=0)){
			DisableControl(c);
		}
		else if(tp->fields()->count_constraints(field)<=0){
			DisableControl(c);
		}
		else{
			PopulatePopupControlWithConstraints(tp,field,c,1);
		}
		cid.id=3;
		GetControlByID(wr.wd,&cid,&c);
		SetTextControlValue(c,prompt);
		
		ShowWindow(wr.wd);
		status=RunAppModalLoopForWindow(wr.wd);
		if(status){
			break;
		}
		
		if(wr.ok){			
			cid.id=1;
			GetControlByID(wr.wd,&cid,&c);
			GetTextControlValue(c,val,sizeof(val)-1);
			strcpy(value,val);
		}
		
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(wr.wd){
		DisposeWindow(wr.wd);
	}
	return(wr.ok);
}

#pragma mark -
#pragma mark =>Proj Stuff
// ---------------------------------------------------------------------------
// 
// -----------
typedef struct ynprojrec{
	WindowRef	wd;
	bool		ok;
	wtable		tbl;
	int			srid;
	bool		usell;
	bool		rpt;
}ynprojrec;

// ---------------------------------------------------------------------------
// 
// -----------
static OSStatus proj_ctrl_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
ynprojrec*			prm=(ynprojrec*)up;
char				str[64];
ControlID			cid={'Proj',3};
ControlRef			c;

	GetControlByID(prm->wd,&cid,&c);
	GetTextControlValue(c,str,sizeof(str)-1);
int					srid=atoi(str);
	if(prm->srid==srid){
		return(eventNotHandledErr);
	}
	prm->srid=srid;
bool				b;
DataBrowserItemID	item;
bStdProj			p(srid,&b);

	cid.id=1;
	GetControlByID(prm->wd,&cid,&c);
	prm->rpt=false;
	if(!b){
		item=DataBrowserSelectedItem(c);
		SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);		
		prm->rpt=true;
		return(eventNotHandledErr);
	}
	if((!prm->usell)&&(p.is_latlong())){
		item=DataBrowserSelectedItem(c);
		SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);		
		prm->rpt=true;
		return(eventNotHandledErr);
	}
	for(item=1;item<=prm->tbl->CountRecords();item++){
		prm->tbl->ReadVal(item,1,&srid);
		if(prm->srid==srid){
			SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsAssign);
			RevealDataBrowserItem(c,item,'Epsg',kDataBrowserRevealOnly);
			Draw1Control(c);
			prm->rpt=true;			
			return(eventNotHandledErr);
		}
	}
	item=DataBrowserSelectedItem(c);
	SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);
	prm->rpt=true;
	return(eventNotHandledErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
static void proj_populate(WindowRef wd, wtable tbl, int csrid){
ControlID			cid={'Proj',1};
ControlRef			c;
int					srid;
DataBrowserItemID	cur=0;

	GetControlByID(wd,&cid,&c);
	for(DataBrowserItemID itemId=1;itemId<=(DataBrowserItemID)tbl->CountRecords();itemId++){
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,'Epsg');
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,'Proj');
		tbl->ReadVal(itemId,1,&srid);
		if(srid==csrid){
			cur=itemId;
		}
	}
	if(cur!=0){
		SetDataBrowserSelectedItems(c,1,&cur,kDataBrowserItemsAssign);
		RevealDataBrowserItem(c,cur,'Epsg',kDataBrowserRevealOnly);
		Draw1Control(c);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
static void proj_action(DataBrowserItemID item, ynprojrec* prm){
	if(!prm->rpt){
		return;
	}
	
char				str[256]="";
ControlID			cid={'Proj',3};
ControlRef			c;

	if(GetControlByID(prm->wd,&cid,&c)){
		return;
	}
	if(item==0){
		SetTextControlValue(c,str);
		return;
	}
int			srid;
bool		b;
	prm->tbl->ReadVal(item,1,&srid);
bStdProj	p(srid,&b);
	if(!b){
		SetTextControlValue(c,str);
		return;
	}
	if((prm->usell==false)&&(p.is_latlong())){
		SetTextControlValue(c,str);
		return;
	}
	sprintf(str,"%d",srid);
	SetTextControlValue(c,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
static OSStatus proj_handler(	ControlRef browser, 
								DataBrowserItemID itemID, 
								DataBrowserPropertyID property, 
								DataBrowserItemDataRef itemData, 
								Boolean changeValue){
CFStringRef	name;
int			srid;
char		cname[1024];
ynprojrec*	prm=(ynprojrec*)GetControlReference(browser);
	
	switch(property){
		case 'Epsg':
			if(!changeValue){
				prm->tbl->ReadVal(itemID,1,&srid);
				sprintf(cname,"%d",srid);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case 'Proj':
			if(!changeValue){
				prm->tbl->ReadVal(itemID,2,cname);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		case kDataBrowserItemIsContainerProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,false)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void proj_notifier(	ControlRef browser, 
					DataBrowserItemID item, 
					DataBrowserItemNotification msg){
ynprojrec*	prm=(ynprojrec*)GetControlReference(browser);
    switch(msg){  
        case kDataBrowserItemAdded:
			break;
        case kDataBrowserItemRemoved:
            break;
        case kDataBrowserContainerOpened:
            break;
        case kDataBrowserContainerClosing:
            break;
        case kDataBrowserContainerClosed:
			break;
        case kDataBrowserItemSelected:
			proj_action(item,prm);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				proj_action(0,prm);
			}
            break;
    }
}


// ---------------------------------------------------------------------------
// 
// ------------
bool GetAProj(int* srid, bool uselonglat){
_bTrace_("ext_utils::GetAProj",true);
OSStatus			status;
IBNibRef			nib=NULL;
const EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
const EventTypeSpec	kControlEvents[]={{kEventClassControl,kEventControlDraw}};
ynprojrec			wr={NULL,false,NULL,(*srid),uselonglat,true};
CFBundleRef			bndl=CFBundleGetMainBundle();

	for(;;){
		wr.tbl=wprj_gettable();
		if(wr.tbl==NULL){
			break;
		}		
		status=CreateNibReferenceWithCFBundle(bndl,CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("GetAProj"),&wr.wd);
		if(status){
			break;
		}

EventHandlerUPP hdlr=yn_evt_hdlr;
		InstallWindowEventHandler(wr.wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)&wr,NULL);

ControlID		cid={'Proj',1};
ControlRef		c;
char			val[256];

		GetControlByID(wr.wd,&cid,&c);
		DataBrowserInit(c,proj_handler,proj_notifier,(long)&wr);
		proj_populate(wr.wd,wr.tbl,wr.srid);	

		cid.id=3;
		GetControlByID(wr.wd,&cid,&c);
		sprintf(val,"%d",wr.srid);
		SetTextControlFilter(c,int_filter);
		InstallControlEventHandler(c,proj_ctrl_evt_hdlr,GetEventTypeCount(kControlEvents),kControlEvents,(void*)&wr,NULL);
		SetKeyboardFocus(wr.wd,c,kControlFocusNextPart);
		SetTextControlValue(c,val);

		ShowWindow(wr.wd);
		status=RunAppModalLoopForWindow(wr.wd);
		if(status){
			break;
		}
	
		wr.rpt=false;
		
		cid.id=1;
		GetControlByID(wr.wd,&cid,&c);
		DataBrowserClose(c);
		
		if(wr.ok){			
			cid.id=3;
			GetControlByID(wr.wd,&cid,&c);
			GetTextControlValue(c,val,sizeof(val)-1);
			(*srid)=atoi(val);
			if((*srid)==0){
				(*srid)=-1;
			}
		}
		
		
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(wr.wd){
		DisposeWindow(wr.wd);
	}
	if(wr.tbl){
		wtbl_free(wr.tbl);
	}
	return(wr.ok);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool GetProjName(int srid, char* name){
_bTrace_("ext_utils::GetProjName",true);
_tm_("get name for : "+srid);
char	lname[1024]="";
wtable	tbl=wprj_gettable();
	if(tbl==NULL){
		strcpy(name,lname);
		return(false);
	}
int		bf;
	for(int i=1;i<=tbl->CountRecords();i++){
		tbl->ReadVal(i,1,&bf);
		if(bf==srid){
			tbl->ReadVal(i,2,lname);
_tm_(lname);
			strcpy(name,lname);
			wtbl_free(tbl);
			return(true);
		}
	}
	name[0]=0;
	wtbl_free(tbl);
	return(false);
}

