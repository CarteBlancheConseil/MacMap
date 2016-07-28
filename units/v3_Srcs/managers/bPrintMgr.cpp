//----------------------------------------------------------------------------
// File : bPrintMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Print utility class
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
// 22/03/2006 creation.
//----------------------------------------------------------------------------

#include "bPrintMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/endian.h>
#include <MacMapSuite/vx_utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bStdDirectory.h>

// ---------------------------------------------------------------------------
// 
// ------------
#ifdef __LITTLE_ENDIAN__
#define kPrintSignature	'tnrp'
#else
#define kPrintSignature	kHICommandPrint
#endif

#define kPrintConfigName		"print_config"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bPrintMgr::bPrintMgr(){
	ivrset(&_area,0,0,0,0);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bPrintMgr::~bPrintMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bPrintMgr::load(){
	if(!read()){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bPrintMgr::unload(){
	if(!write()){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
ivx_rect* bPrintMgr::get_print_area(){
	return(&_area);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bPrintMgr::set_print_area(ivx_rect* area){
_bTrace_("bPrintMgr::set_print_area",false);
	if(!ivr_ok(area)){
_te_("bad area :"+area->left+":"+area->top+":"+area->right+":"+area->bottom);
		return;
	}
	_area=*area;
_tm_("print area set to :"+_area.left+":"+_area.top+":"+_area.right+":"+_area.bottom);
	if(!write()){
_te_("write failed");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bPrintMgr::get_page_format(PMPageFormat* pf, const char* name){
	*pf=kPMNoPageFormat;
OSStatus		status;
char			path[PATH_MAX];
	map_doc->location(path);
bStdDirectory	root(path);
	if(root.status()){
		return(root.status());
	}
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
		return(rsrc.status());
	}
char			nm[256];
int				sgn=kPrintSignature;
	sprintf(nm,"%.4s/",(char*)&sgn);
bStdDirectory	prn(nm);
	if(prn.status()){
		return(prn.status());
	}
bStdFile		f(name,"r");
	if(f.status()){
		return(f.status());
	}
void*			buf;
int				sz;
	f.mount((char**)&buf,&sz);
	if(f.status()){
		return(f.status());
	}
CFDataRef		dt=CFDataCreate(kCFAllocatorDefault,(UInt8*)buf,sz);
	free(buf);
	if(!dt){
		return(-1);
	}
    
    status=PMPageFormatCreateWithDataRepresentation(dt,pf);
	CFRelease(dt);
	if(status){
		*pf=kPMNoPageFormat;
		return(status);
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bPrintMgr::set_page_format(PMPageFormat pf, const char* name){
OSStatus		status;
char			path[PATH_MAX];
	map_doc->location(path);
bStdDirectory	root(path);
	if(root.status()){
		return(root.status());
	}
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
		return(rsrc.status());
	}
char			nm[256];
int				sgn=kPrintSignature;
	sprintf(nm,"%.4s/",(char*)&sgn);
bStdDirectory	prn(nm);
	if(prn.status()){
		return(prn.status());
	}
bStdFile		f(name,"w");
	if(f.status()){
		return(f.status());
	}
    
CFDataRef	dt;
    status=PMPageFormatCreateDataRepresentation(pf,&dt,kPMDataFormatXMLDefault);
	if(status){
		return(status);
	}
	f.write((void*)CFDataGetBytePtr(dt),CFDataGetLength(dt));
	CFRelease(dt);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bPrintMgr::set_print_context(CGContextRef ctx){
	_ctx=ctx;
}

// ---------------------------------------------------------------------------
// 
// -----------
CGContextRef bPrintMgr::get_print_context(){
	return(_ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bPrintMgr::read(){
_bTrace_("bPrintMgr::read",false);
bGenericXMLBaseElement*	root;
	if(!map_doc->readTree(&root,kPrintSignature,kPrintConfigName)){
		if(!write()){
_te_("write failed");
			return(false);
		}
_tm_("print area set to :0:0:0:0");
		return(true);
	}
	
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
	elt=clssmgr->NthElement(root,1,"int");
	if(elt){
		elt->getvalue(val);
		_area.left=atoi(val);
	}
	elt=clssmgr->NthElement(root,2,"int");
	if(elt){
		elt->getvalue(val);
		_area.top=atoi(val);
	}
	elt=clssmgr->NthElement(root,3,"int");
	if(elt){
		elt->getvalue(val);
		_area.right=atoi(val);
	}
	elt=clssmgr->NthElement(root,4,"int");
	if(elt){
		elt->getvalue(val);
		_area.bottom=atoi(val);
	}
	if(!ivr_ok(&_area)){
		ivrset(&_area,-100,-100,100,100);
	}
	clssmgr->ReleaseXMLInstance(root);
_tm_("print area set to :"+_area.left+":"+_area.top+":"+_area.right+":"+_area.bottom);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bPrintMgr::write(){
_bTrace_("bPrintMgr::write",false);
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0");
	add_idesc(arr,1,"int",_area.left);
	add_idesc(arr,1,"int",_area.top);
	add_idesc(arr,1,"int",_area.right);
	add_idesc(arr,1,"int",_area.bottom);
bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return(false);
	}
	if(!map_doc->writeTree(root,kPrintSignature,kPrintConfigName)){
_te_("writeTree failed");
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

