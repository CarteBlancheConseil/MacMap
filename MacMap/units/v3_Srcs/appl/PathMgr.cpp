//----------------------------------------------------------------------------
// File : PathMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Paths to types managements (adding types, removing, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2002 Carte Blanche Conseil.
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
// 09/09/2002 creation.
//----------------------------------------------------------------------------

#include "PathMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/bXMLFile.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/xmldesc_utils.h>

//----------------------------------------------------------------------------

#define _fpath		"Contents/Resources/types.xml"
#define _datadir	"Contents/Datas/"

// ---------------------------------------------------------------------------
// 
// ------------
bool PATH_get(long n, int* dbtype, char* path){
_bTrace_("PATH_get",false);
bGenericXMLBaseElement*	root;	
	if(map_doc->readXMLParam(&root,"","types.xml")==false){
_te_("readXMLParam failed");
		return(false);
	}
	
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
	
	elt=clssmgr->NthElement(root,n,"int");
	if(!elt){
_te_("clssmgr->NthElement "+n+" failed for \"int\"");
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	elt->getvalue(val);
	
	(*dbtype)=atoi(val);
	
	elt=clssmgr->NthElement(root,n,"path");
	if(!elt){
_te_("clssmgr->NthElement "+n+" failed for \"path\"");
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	elt->getvalue(path);	
	
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
long PATH_count(void){
_bTrace_("PATH_count",false);	
bGenericXMLBaseElement*	root;	
	if(map_doc->readXMLParam(&root,"","types.xml")==false){
_te_("readXMLParam failed");
		return(false);
	}
long	n=clssmgr->CountElements(root,"int");
	clssmgr->ReleaseXMLInstance(root);	
	return(n);
}

// ---------------------------------------------------------------------------
// 
// ------------
char* PATH_getExt(long n, int *pthType, const char *ext){
_bTrace_("PATH_getExt",false);
char	path[1024];
size_t	sz;
char*	pth;
	
	pth=NULL;
	(*pthType)=0;
	if(!PATH_get(n,pthType,path)){
_te_("NULL for "+n);
		return(NULL);
	}
	sz=strlen(path);
	
	pth=(char*)malloc(sz+strlen(_datadir)+strlen(ext)+1);
	if(!pth){
_te_("malloc failed : sz="+(unsigned int)(sz+strlen(_datadir)+strlen(ext)+1));
		return(NULL);
	}
	strcpy(pth,path);
	memmove(&pth[sz],_datadir,strlen(_datadir)+1);
	sz=strlen(pth);
	memmove(&pth[sz],ext,strlen(ext)+1);	
	return(pth);
}

// ---------------------------------------------------------------------------
// 
// ------------
long PATH_add(int pthType, const char* path){
_bTrace_("PATH_add",false);	
bGenericXMLBaseElement*	root;	
	if(map_doc->readXMLParam(&root,"","types.xml")==false){
_te_("readXMLParam failed");
		return(false);
	}
	
long					k=clssmgr->CountElements(root,"int");
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
	
	sprintf(val,"%d",pthType);
	elt=clssmgr->CreateXMLInstance("int",val,root);
	if(!elt){
_te_("clssmgr->search == NULL for \"int\"");
		clssmgr->ReleaseXMLInstance(root);
		return(-1);
	}
	elt=clssmgr->CreateXMLInstance("path",(char*)path,root);
	if(!elt){
_te_("clssmgr->search == NULL for \"path\"");
		clssmgr->ReleaseXMLInstance(root);
		return(-2);
	}
	
	if(map_doc->writeXMLParam(root,"","types.xml")==false){
_te_("readXMLParam failed");
	}
	
	clssmgr->ReleaseXMLInstance(root);
	if(PATH_count()!=(k+1)){
_te_("PATH_count ("+PATH_count()+")!=(k+1)");
		return(-3);
	}
	return(k+1);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool PATH_change(long n, const char* path){
_bTrace_("PATH_change",true);
bGenericXMLBaseElement*	root;	
	if(map_doc->readXMLParam(&root,"","types.xml")==false){
_te_("readXMLParam failed");
		return(false);
	}
	
char					val[_names_length_max_];
bGenericXMLBaseElement*	elt;
long					k;
	
	k=0;
	for(int i=1;i<=root->countelements();i++){
		elt=root->getelement(i);
		elt->getclassname(val);
		if(!strcmp(val,"path")){
			k++;
		}
		if(k==n){
			elt->setvalue((char*)path);
_tm_("change path to "+path);
			break;
		}
	}
	if(map_doc->writeXMLParam(root,"","types.xml")==false){
_te_("readXMLParam failed");
	}	
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool PATH_rmv(long n){
_bTrace_("PATH_rmv",true);
bGenericXMLBaseElement*	root;	
	if(map_doc->readXMLParam(&root,"","types.xml")==false){
_te_("readXMLParam failed");
		return(false);
	}
	
char					val[_names_length_max_];
bGenericXMLBaseElement*	elt;
long					k;
	
	k=0;
	for(int i=1;i<=root->countelements();i++){
		elt=root->getelement(i);
		elt->getclassname(val);
		if(!strcmp(val,"int")){
			k++;
		}
		if(k==n){
			root->rmvelement(i);
			clssmgr->ReleaseXMLInstance(elt);
_tm_("remove int "+k+"->"+n);
			break;
		}
	}
	k=0;
	for(int i=1;i<=root->countelements();i++){
		elt=root->getelement(i);
		elt->getclassname(val);
		if(!strcmp(val,"path")){
			k++;
		}
		if(k==n){
			root->rmvelement(i);
			clssmgr->ReleaseXMLInstance(elt);
_tm_("remove path "+k+"->"+n);
			break;
		}
	}
	if(map_doc->writeXMLParam(root,"","types.xml")==false){
_te_("readXMLParam failed");
	}
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool PATH_make(){
_bTrace_("PATH_make",true);
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0");
bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);	
	if(map_doc->writeXMLParam(root,"","types.xml")==false){
_te_("readXMLParam failed");
	}	
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

