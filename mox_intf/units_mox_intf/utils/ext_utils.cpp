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
#include "bGenericCalc.h"
#include "bGenericGeog.h"
#include "CGUtils.h"
#include "NSProjPicker.h"
#include "NSValueNamePicker.h"
//#include "Carb_Utils.h"
#include "endian.h"
#include "mm_messages.h"

#include <std_ext/bStdUserExt.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/C_Utils.h>

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
/*void PopulatePopupControlWithScales(bGenericMacMapApp* gapp, ControlRef c, int current){
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
static void PopulatePopupControlWithConstraints(bGenericType* tp, int field, ControlRef c, int cur){
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
}*/

// ---------------------------------------------------------------------------
// 
// -----------
CGFloat* GetDash(bGenericMacMapApp* gapp,
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
	
bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(data,sz);
	free(data);
    if(!root){
		return(NULL);
	}
	
int	n=root->countelements();
	if(n==0){
		gapp->classMgr()->ReleaseXMLInstance(root);
		return(NULL);
	}
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
CGFloat*				dsh=new CGFloat[n];
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
OSType	t=0;
//Str255	s;

//    memcpy(s+1,name,strlen(name));
//	s[0]=strlen(name);
//	if(QTGetFileNameExtension(s,0,&t)){
//		t=0;
//	}
    
char    lname[FILENAME_MAX];
    strcpy(lname,name);
    strupper(lname);
    
    if(strstr(lname,".JPG")){
        t=kFileTypeJPEG;
    }
    else if(strstr(lname,".JPEG")){
        t=kFileTypeJPEG;
    }
    else if(strstr(lname,".TIF")){
        t=kFileTypeTIFF;
    }
    else if(strstr(lname,".TIFF")){
        t=kFileTypeTIFF;
    }
    else if(strstr(lname,".PNG")){
        t=kFileTypePNG;
    }
    else if(strstr(lname,".PDF")){
        t=kFileTypePDF;
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
//	if(t!=kQTFileTypePDF){
    if(t!=kFileTypePDF){
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
//	if(t!=kQTFileTypePDF){
    if(t!=kFileTypePDF){
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
_bTrace_("RecoverMacro",true);
//bStdUserMacro*	ext=NULL;
bGenericExt*    ext=NULL;
bGenericExtMgr*	mgr=gapp->macroMgr();
char			sgn[10];
user_ext_data	prm={NULL,0,NULL};

	for(int i=1;i<=mgr->count();i++){
//        ext=(bStdUserMacro*)(void*)mgr->get(i);
        ext=(bGenericExt*)(void*)mgr->get(i);
		ext->process(kExtProcessCallGetData,&prm);
		if(prm.sign!=0){
			sprintf(sgn,"%.4s",(char*)&prm.sign);
			if(strcmp(sgn,formula)==0){
				return(i);
			}
		}
		if(prm.name){
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
_bTrace_("RecoverScript",true);
//bStdUserScript*	ext=NULL;
bGenericExt*    ext=NULL;
bGenericExtMgr*	mgr=gapp->scriptMgr();
char			sgn[10];
user_ext_data	prm={NULL,0,NULL};

	for(int i=1;i<=mgr->count();i++){
//		ext=(bStdUserScript*)(void*)mgr->get(i);
        ext=(bGenericExt*)(void*)mgr->get(i);
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
_bTrace_("InitWithStyle",false);
_tm_("");
bGenericStyle*	styl=gapp->layersMgr()->get(style_index);
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
char	bf[_values_length_max_];
char*	fld=(char*)prm;
	elt->getclassname(bf);
	if(	(!strcmp(bf,"classfield"))		||
		(!strcmp(bf,"subclassfield"))	||
		(!strcmp(bf,"field"))			||
		(!strcmp(bf,"colorfield8"))		||
		(!strcmp(bf,"colorfield16m"))	||
		(!strcmp(bf,"dbfield"))			){
		elt->getvalue(bf);
		if(!strcmp(bf,fld)){
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
	if(field==kOBJ_Vertices_){
		return(1);
	}
bGenericStyle*			styl;
bGenericXMLBaseElement*	root;
char					f[256];

	tp->fields()->get_name(field,f);
    for(int i=1;i<=gapp->layersMgr()->count();i++){
        styl=gapp->layersMgr()->get(i);
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
	(*root)=gapp->classMgr()->ParseXMLData(buffer,bsz);
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
bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(buffer,sz);
    free(buffer);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void SetMarginToCurview(bGenericMacMapApp* gapp, 
						double d){
bGenericStyle*	stl;
long			margin=Measure_d2i(gapp,d);
	
    for(long i=1;i<=gapp->layersMgr()->count();i++){
        stl=gapp->layersMgr()->get(i);
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
    for(long i=1;i<=gapp->layersMgr()->count();i++){
        stl=gapp->layersMgr()->get(i);
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
    for(long i=1;i<=gapp->layersMgr()->count();i++){
		if(margins->get(i,&mrg)==false){
			return(-1);
		}
        stl=gapp->layersMgr()->get(i);
		stl->setmargin(mrg);
	}
	return(0);	
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
	gapp->document()->location(path);
	if(path[strlen(path)-1]=='/'){
		path[strlen(path)-1]=0;
	}
char*	c=strrchr(path,'/');
	if(c){
		c[0]=0;
	}
}

// ---------------------------------------------------------------------------
//
// -----------
void SendCommandToApplication(UInt32 sign){
NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                    location:NSZeroPoint
                               modifierFlags:0
                                   timestamp:0
                                windowNumber:0
                                     context:nil
                                     subtype:NSEventSubtypeMacMapCommand
                                       data1:sign
                                       data2:0];
    [NSApp postEvent:event atStart:NO];
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
// -----------
CGRect NSMainSreenBounds(){
    return(NSRectToCGRect([[NSScreen mainScreen] frame]));
}

// ---------------------------------------------------------------------------
//
// -----------
CGFloat NSMenuBarHeight(){
    return([[[NSApplication sharedApplication] mainMenu] menuBarHeight]);
}

#pragma mark -
#pragma mark =>GetAName/GetAValue
// ---------------------------------------------------------------------------
// 
// ------------
bool GetAName(char* name, const char* wtitle, bool cncl){
    return runNamePicker(name,wtitle,cncl);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool GetAValue(bGenericType* tp,
			   int field,
			   const char* title,
			   char* value){
    return runValuePicker(tp,field,title,value);
}

#pragma mark -
#pragma mark =>Proj Stuff
// ---------------------------------------------------------------------------
// 
// ------------
bool GetAProj(int* srid, bool uselonglat){
_bTrace_("ext_utils::GetAProj",true);
    return runProjPicker(srid);
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

#pragma mark -
#pragma mark =>NEW
// ---------------------------------------------------------------------------
//
// ------------
void AddToPalettesMenu(bGenericMacMapApp* gapp,
                       bGenericExt* ext){
char	name[FILENAME_MAX];
    GetName(ext,name);
    (void)gapp->menuMgr()->add_item(kMenuMgrMenuPalettesID,name,GetSignature(ext));
}

// ---------------------------------------------------------------------------
//
// ------------
char* CopyXMLTreeData(bGenericXMLBaseElement* root){
_bTrace_("ext_utils::CopyXMLTreeData",false);
    if(!root){
_te_("root == NULL");
        return NULL;
    }
bMemFile	f;
char*		bf=NULL;
int			sz;
    
    root->encode(0,&f);
    if(f.status()==0){
        f.mount(&bf,&sz);
        bf=(char*)realloc(bf,sz+1);
        bf[sz]=0;
        sz=sz+1;
    }
    return bf;
}

// ---------------------------------------------------------------------------
//
// ------------
void DumpXMLTreeData(bGenericXMLBaseElement* root,
                     FILE* f){
_bTrace_("ext_utils::DumpXMLTreeData",false);
char*   bf=CopyXMLTreeData(root);
    if(bf){
        fprintf(f,"%s\n",bf);
        free(bf);
    }
    else{
_te_("NULL data");
    }
}


