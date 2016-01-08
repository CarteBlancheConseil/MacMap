//----------------------------------------------------------------------------
// File : bXMapStatScatterPlotParam.cpp
// Project : MacMap
// Purpose : C++ source file : Params class for scatter plot graphs
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
// 22/04/2008 creation.
//----------------------------------------------------------------------------

/*#include "bXMapStatScatterPlotParam.h"

#include <mox_intf/CGUtils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>

#include <MacMapStats/MMsIO.h>


// ---------------------------------------------------------------------------
// 
// ------------
typedef struct r_prm{
	bXMapStatScatterPlotParam*	x;
	int							n;
}r_prm;

// ---------------------------------------------------------------------------
// 
// ------------
static itrun defc[7]={	{1,0,0,0,1,0,0,0,1,4,4,0},
						
						{2,1,0,0,1,0,0,0,1,2,2,0},
						{2,0,1,0,1,0,0,0,1,2,2,0},
						{2,0,0,1,1,0,0,0,1,2,2,0},
						
						{2,1,1,0,1,0,0,0,1,2,2,0},
						{2,0,1,1,1,0,0,0,1,2,2,0},
						{2,1,0,1,1,0,0,0,1,2,2,0}	};
							
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapStatScatterPlotParam	::bXMapStatScatterPlotParam(bArray& elts,
														int grp, 
														bool mapupdt)
				:_objects(sizeof(StatObject*))
				,_colors(sizeof(itrun)){
	_grp=grp;
//	_mapupdt=mapupdt;

	for(int i=1;i<=6;i++){
		_colors.add(&defc[i]);
	}
	_vv=defc[0];
	
StatObject*			so;
bGenericGeoElement*	go;
	for(int i=1;i<=elts.count();i++){
		elts.get(i,&go);
		so=new StatObject;
		if(_grp){
			if(!go->getValue(_grp,&so->clss)){
				so->clss=1;
			}
		}
		else{
			so->clss=1;
		}
		so->geo=go;
		_objects.add(&so);
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapStatScatterPlotParam::~bXMapStatScatterPlotParam(){
StatObject*			so;
	for(int i=1;i<=_objects.count();i++){
		_objects.get(i,&so);
		delete so;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStatScatterPlotParam::SetGroupField(int f){
	_grp=f;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStatScatterPlotParam::SaveGroups(	bGenericMacMapApp* gapp,
											const char* path,
											const char* name){

bArray					arr(sizeof(xmlelt));
void*					bf;
int						sz;
bGenericXMLBaseElement*	root;
StatObject*				so;

	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.2.0");
	for(int i=1;i<=_objects.count();i++){
		_objects.get(i,&so);
		add_idesc(arr,1,"int",so->clss);
	}
	root=gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	arr.reset();
	if(!root){
//_te_("ParseXMLDescriptors");
		return;
	}
	if(!GetBufferFromXMLParam(root,&bf,&sz)){
		gapp->classMgr()->ReleaseXMLInstance(root);
//_te_("GetBufferFromXMLParam");
		return;
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
	(void)MMsIOWriteParam(path,name,bf,sz,"groups.xml");
	free(bf);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStatScatterPlotParam::RecoverGroups(	bGenericMacMapApp* gapp,
												const char* path,
												const char* name){
_bTrace_("bXMapStatScatterPlotParam::RecoverGroups",true);
bArray					arr(sizeof(xmlelt));
void*					bf;
int						status,sz;
bGenericXMLBaseElement*	root;
r_prm					p={this,0};

	status=MMsIOReadParam(path,name,&bf,&sz,"groups.xml");
	if(status){
_te_("MMsIOReadParam err");
		return;
	}
	status=GetXMLParamFromBuffer(gapp,&root,bf,sz)?0:-1;
	free(bf);
	if(status){
_te_("GetXMLParamFromBuffer err");
		return;
	}
	root->dotoall(&p,0,put_grp);
	gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStatScatterPlotParam::SizeGroups(long n){
_bTrace_("bXMapStatScatterPlotParam::RecoverGroups",true);
	if(n<_colors.count()){
		for(long i=_colors.count();i>n;i--){
			_tm_("remove "+i);
			_colors.rmv(i);
		}
	}
	else if(n>_colors.count()){
		for(long i=_colors.count();i<=n;i++){
			_tm_("add ");
			_colors.add(&defc[0]);
		}		
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapStatScatterPlotParam::put_grp(bGenericXMLBaseElement *elt, 
										void *prm, 
										int indent){
r_prm*		p=(r_prm*)prm;	
char		clssname[_values_length_max_];
StatObject*	so;

	elt->getclassname(clssname);
	if(!strcmp("int",clssname)){
		elt->getvalue(clssname);
		p->n++;
		if(p->x->_objects.get(p->n,&so)){
			so->clss=atoi(clssname);
		}
	}
	return(true);
}
*/
