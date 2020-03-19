//----------------------------------------------------------------------------
// File : vdef_utils.cpp
// Project : MacMap
// Purpose : C++ source file : vDefs utilities
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
// 28/03/2007 creation.
//----------------------------------------------------------------------------

#include "vdef_utils.h"
#include <mox_intf/Color_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct read_p{
	bArray* arr;
	double	val;
	int		fld;
}read_p;


// ---------------------------------------------------------------------------
// 
// ------------
static int read_all(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
read_p*				rp=(read_p*)prm;

	geo->getValue(rp->fld,&rp->val);
	rp->arr->add(&rp->val);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int read_selected(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
read_p*				rp=(read_p*)prm;

	if(geo->selected()){
		geo->getValue(rp->fld,&rp->val);
		rp->arr->add(&rp->val);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int read_contrasted(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
read_p*				rp=(read_p*)prm;

	if(geo->contrasted()){
		geo->getValue(rp->fld,&rp->val);
		rp->arr->add(&rp->val);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int comp_double(	const void* a,
						const void* b){
double*	oa=(double*)a;
double*	ob=(double*)b;
	if((*oa)>(*ob)){
		return(1);
	}
	if((*oa)<(*ob)){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
static bGenericXMLBaseElement* get_colors(bGenericMacMapApp* gapp){
CFBundleRef	bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.vdef-lib"));
CFURLRef	url=CFBundleCopyResourceURL(bndl,CFSTR("colors"),CFSTR("xml"),NULL);
	if(!url){
		return(NULL);
	}
char		path[PATH_MAX];
CFStringRef	cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
	CFRelease(url);
	CFStringGetCString(cfs,path,PATH_MAX,kCFStringEncodingMacRoman);
	CFRelease(cfs);
bStdFile	f(path,"r");
	if(f.status()){
		return(NULL);
	}
char*	buff;
int		sz;
	f.mount(&buff,&sz);
	if(sz<=0){
		return(NULL);
	}
bGenericXMLBaseElement*	root=gapp->classMgr()->ParseXMLData(buff,sz);
    free(buff);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
void getMin(bGenericMacMapApp* gapp,
			bGenericType* tp,
			int field,
			int nth,
			double* min){
bArray	arr(sizeof(double));
read_p	rp={&arr,0,field};
bArray	cnt(*gapp->cntMgr()->elements());
bArray	sel(*gapp->selMgr()->elements());
bool	cntok=false,selok=false;
	
	if(cnt.count()>1){
int		idx=ContrastesAreMonoType(gapp);
		cntok=idx==tp->index();
	}
	
	if(sel.count()>1){
int		idx=SelectionIsMonoType(gapp);
		selok=idx==tp->index();
	}
	
	if(cntok){
		tp->iterator()->iterate(&rp,read_contrasted);
	}
	else if(selok){
		tp->iterator()->iterate(&rp,read_selected);
	}
	else{
		tp->iterator()->iterate(&rp,read_all);
	}
	arr.sort(comp_double);
	
int		n=1;
double	a,b;
	if(arr.get(1,&a)){
		for(int i=2;(i<=arr.count())&&(n<nth);i++){
			arr.get(i,&b);
			if(b!=a){
				n++;
				a=b;
			}
		}
		*min=a;				
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void getMax(bGenericMacMapApp* gapp,
			bGenericType* tp,
			int field,
			int nth,
			double* max){
bArray	arr(sizeof(double));
read_p	rp={&arr,0,field};
bArray	cnt(*gapp->cntMgr()->elements());
bArray	sel(*gapp->selMgr()->elements());
bool	cntok=false,selok=false;
	
	if(cnt.count()>1){
int		idx=ContrastesAreMonoType(gapp);
		cntok=idx==tp->index();
	}
	
	if(sel.count()>1){
int		idx=SelectionIsMonoType(gapp);
		selok=idx==tp->index();
	}
	
	if(cntok){
		tp->iterator()->iterate(&rp,read_contrasted);
	}
	else if(selok){
		tp->iterator()->iterate(&rp,read_selected);
	}
	else{
		tp->iterator()->iterate(&rp,read_all);
	}
	arr.sort(comp_double);
	
int		n=1;
double	a,b;
	if(arr.get(arr.count(),&a)){
		for(long i=arr.count()-1;(i>0)&&(n<nth);i--){
			arr.get(i,&b);
			if(b!=a){
				n++;
				a=b;
			}
		}
		*max=a;				
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void getBornes(bGenericMacMapApp* gapp,
			   bGenericType* tp,
			   int field,
			   double* min,
			   double* max){
bArray	arr(sizeof(double));
read_p	rp={&arr,0,field};
bArray	cnt(*gapp->cntMgr()->elements());
bArray	sel(*gapp->selMgr()->elements());
bool	cntok=false,selok=false;
	
	if(cnt.count()>1){
int		idx=ContrastesAreMonoType(gapp);
		cntok=idx==tp->index();
	}
	
	if(sel.count()>1){
int		idx=SelectionIsMonoType(gapp);
		selok=idx==tp->index();
	}
	
	if(cntok){
		tp->iterator()->iterate(&rp,read_contrasted);
	}
	else if(selok){
		tp->iterator()->iterate(&rp,read_selected);
	}
	else{
		tp->iterator()->iterate(&rp,read_all);
	}
	arr.sort(comp_double);
	
	arr.get(1,min);
	arr.get(arr.count(),max);
}

// ---------------------------------------------------------------------------
// 
// -----------
void getMatrix(bGenericMacMapApp* gapp,
			   bGenericType* tp,
			   int field,
			   double min,
			   double max,
			   MMsMatrix** mx){
//_bTrace_("vdef_utils::getMatrix",true);
bArray	arr(sizeof(double));
read_p	rp={&arr,0,field};
bArray	cnt(*gapp->cntMgr()->elements());
bArray	sel(*gapp->selMgr()->elements());
bool	cntok=false,selok=false;
	
	if(cnt.count()>1){
int		idx=ContrastesAreMonoType(gapp);
		cntok=idx==tp->index();
	}
	
	if(sel.count()>1){
int		idx=SelectionIsMonoType(gapp);
		selok=idx==tp->index();
	}
	
	if(cntok){
		tp->iterator()->iterate(&rp,read_contrasted);
	}
	else if(selok){
		tp->iterator()->iterate(&rp,read_selected);
	}
	else{
		tp->iterator()->iterate(&rp,read_all);
	}
	
long	n=0;
double	a;
	
	for(long i=1;i<=arr.count();i++){
		arr.get(i,&a);
		if((a>=min)&&(a<=max)){
			n++;
		}
	}
	*mx=MMsMatrixAlloc(n,1);
	
	n=0;
	for(long i=1;i<=arr.count();i++){
		arr.get(i,&a);
//_tm_(min+"<"+a+"<"+max);
		if((a>=min)&&(a<=max)){
			n++;
			MMsSetDouble(*mx,n,1,a);
		}
//		else{
//_tw_(a+" out");
//		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int countColors(bGenericMacMapApp* gapp){
bGenericXMLBaseElement* root=get_colors(gapp);
	if(!root){
		return(0);
	}
bGenericXMLBaseElement* chld;
int						i=0;
	do{
		chld=gapp->classMgr()->NthElement(root,i+1,"param");
		if(!chld){
			break;
		}
		i++;
	}
	while(true);
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(i);
}

// ---------------------------------------------------------------------------
// 
// -----------
void getColorName(bGenericMacMapApp* gapp,
				  int index,
				  char* name){
	name[0]=0;
bGenericXMLBaseElement* root=get_colors(gapp);
	if(!root){
		return;
	}
bGenericXMLBaseElement* chld;
bGenericXMLBaseElement* elt;
	
	chld=gapp->classMgr()->NthElement(root,index,"param");
	if(chld){
		elt=gapp->classMgr()->NthElement(chld,1,"name");
		if(elt){
			elt->getvalue(name);
		}
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
void getColor(bGenericMacMapApp* gapp,
			  int index,
			  double* cmin,
			  double* cmax,
			  int cmyk){
	cmin[0]=1;
	cmin[1]=1;
	cmin[2]=1;
	cmin[3]=1;
	cmin[4]=1;
	cmax[0]=0;
	cmax[1]=0;
	cmax[2]=0;
	cmax[3]=0;
	cmax[4]=1;
	
bGenericXMLBaseElement* root=get_colors(gapp);
	if(!root){
		return;
	}
bGenericXMLBaseElement* chld;
bGenericXMLBaseElement* elt;
char					val[_values_length_max_];
int						n=0;
	
	for(;;){
		chld=gapp->classMgr()->NthElement(root,index,"param");
		if(!chld){
			break;
		}
		
		n=gapp->classMgr()->CountElements(chld,"float")/2;
		
		for(int i=1;i<=n;i++){
			elt=gapp->classMgr()->NthElement(chld,i,"float");
			if(!elt){
				break;
			}
			elt->getvalue(val);
			cmin[i-1]=matof(val);
			
			elt=gapp->classMgr()->NthElement(chld,i+n,"float");
			if(!elt){
				break;
			}
			elt->getvalue(val);
			cmax[i-1]=matof(val);
		}
		
		break;
	}
	
	if(cmyk){
	}
	else{
	}
	
	if(cmyk&&(n!=4)){
		RGB2CMYK64(cmin[0], 
				   cmin[1], 
				   cmin[2], 
				   &cmin[0], 
				   &cmin[1], 
				   &cmin[2],
				   &cmin[3]);
		RGB2CMYK64(cmax[0], 
				   cmax[1], 
				   cmax[2], 
				   &cmax[0], 
				   &cmax[1], 
				   &cmax[2],
				   &cmax[3]);
	}
	else if(!cmyk&&(n!=3)){
		CMYK2RGB64(cmin[0], 
				   cmin[1], 
				   cmin[2],
				   cmin[3],
				   &cmin[0], 
				   &cmin[1], 
				   &cmin[2]);
		CMYK2RGB64(cmax[0], 
				   cmax[1], 
				   cmax[2],
				   cmax[3],
				   &cmax[0], 
				   &cmax[1], 
				   &cmax[2]);
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
int getDashIndex(bGenericType* tp, char* dsh){
char	val[256];
	
	for(long i=1;i<=tp->fields()->count_param("dashes");i++){
		tp->fields()->get_param_name("dashes",i,val);
		if(strcmp(val,dsh)==0){
			return(i);
		}
	}
	return(0);
}
