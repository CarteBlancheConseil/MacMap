//----------------------------------------------------------------------------
// File : bGraphicParams.cpp
// Project : MacMap
// Purpose : C++ source file : Graphic params storage class. Used for tool trace, selection and contrasts drawing.
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
// 20/03/2014 creation.
//----------------------------------------------------------------------------

#include "bGraphicParams.h"
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// 
// ------------

#ifdef __LITTLE_ENDIAN__
#define	kStdGraphicParamsConfigSign		'hprG'
#else
#define	kStdGraphicParamsConfigSign		'Grph'
#endif
#define	kStdGraphicParamsConfigName		"Grph"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGraphicParams::bGraphicParams(const char* name,
							   bGenericMacMapApp* gapp){
	_status=-1;
	_width=0;
	for(long i=0;i<4;i++){
		_stroke[i]=0;
		_fill[i]=0;
	}

bool	updt=false;
char	fname[256];
	if(strcmp(name,kStdGraphicParamsSelectionName)==0){
		strcpy(fname,kStdGraphicParamsSelectionName);
		_width=0.75;
		_stroke[3]=0.75;
	}
	else if(strcmp(name,kStdGraphicParamsContrastesName)==0){
		strcpy(fname,kStdGraphicParamsContrastesName);
		_width=1.0;
		_stroke[0]=0.90;
		_stroke[1]=0.60;
		_stroke[2]=0.60;
		_stroke[3]=0.80;
		_fill[0]=0.00;
		_fill[1]=0.00;
		_fill[2]=0.00;
		_fill[3]=0.00;
	}
	else{
		strcpy(fname,kStdGraphicParamsToolTraceName);
		_width=1.00;
		_stroke[0]=0.30;
		_stroke[1]=0.30;
		_stroke[2]=1.00;
		_stroke[3]=1.00;
		_fill[0]=0.75;
		_fill[1]=0.75;
		_fill[2]=1.00;
		_fill[3]=0.60;
	}
	
bGenericXMLBaseElement* root=NULL;
	gapp->document()->readTree(&root,kStdGraphicParamsConfigSign,name);
	if(root==NULL){
		root=GetXMLParamFromBundle(gapp,CFBundleGetMainBundle(),fname);
		updt=true;
	}
	
	if(root!=NULL){
		parse(root,gapp);
		gapp->classMgr()->ReleaseXMLInstance(root);
	}
	
	if(_status){
		updt=true;
	}
	
	if(updt&&update(fname,gapp)){
		_status=0;
	}	
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGraphicParams::bGraphicParams(bGenericXMLBaseElement* root,
							   bGenericMacMapApp* gapp){
	_status=-1;
	_width=0;
	for(long i=0;i<4;i++){
		_stroke[i]=0;
		_fill[i]=0;
	}	
	if(root!=NULL){
		parse(root,gapp);
	}
	if(_status){
		_width=1;
		_stroke[0]=1;
		_stroke[1]=0;
		_stroke[2]=0;
		_stroke[3]=1;
		_fill[0]=1;
		_fill[1]=0;
		_fill[2]=0;
		_fill[3]=0.75;
	}
}
				
// ---------------------------------------------------------------------------
// Destructeur
// ------------
bGraphicParams::~bGraphicParams(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bGraphicParams::update(bArray& arr, 
							long indent){
	add_cdesc(arr,indent,"param","");
	add_cdesc(arr,indent+1,"sign","graphic");
	add_ddesc(arr,indent+1,"float",_width,2);
	for(long i=0;i<4;i++){
		add_ddesc(arr,indent+1,"float",_stroke[i],2);
	}
	for(long i=0;i<4;i++){
		add_ddesc(arr,indent+1,"float",_fill[i],2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bGraphicParams::update(const char* name,
							bGenericMacMapApp* gapp){
bArray					arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

	update(arr,1);
	
bGenericXMLBaseElement* root=gapp->classMgr()->ParseXMLDescriptors(&arr);
	if(!root){
		free_descs(arr);
		return(false);
	}
	if(!gapp->document()->writeTree(root,kStdGraphicParamsConfigSign,name)){
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bGraphicParams::apply(CGContextRef ctx){
	if(_width>1.5){
		CGContextSetLineCap(ctx,kCGLineCapRound);
		CGContextSetLineJoin(ctx,kCGLineJoinRound);
	}	
	CGContextSetLineWidth(ctx,_width);
	CGContextSetRGBStrokeColor(ctx,_stroke[0],_stroke[1],_stroke[2],_stroke[3]);
	CGContextSetRGBFillColor(ctx,_fill[0],_fill[1],_fill[2],_fill[3]);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPathDrawingMode bGraphicParams::pathDrawingMode(){
	if(_width>0&&_stroke[3]>0&&_fill[3]>0){
		return(kCGPathFillStroke);
	}
	if(_width>0&&_stroke[3]>0&&_fill[3]<=0){
		return(kCGPathStroke);
	}
	if(_width<=0&&_stroke[3]<=0&&_fill[3]>0){
		return(kCGPathFill);
	}
	return(kCGPathStroke);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bGraphicParams::setWidth(float w){
	_width=w;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bGraphicParams::setStroke(const float* clr){
	for(long i=0;i<4;i++){
		_stroke[i]=clr[i];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bGraphicParams::setFill(const float* clr){
	for(long i=0;i<4;i++){
		_fill[i]=clr[i];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bGraphicParams::parse(bGenericXMLBaseElement* root, bGenericMacMapApp* gapp){
bGenericXMLBaseElement* elt=NULL;
char					val[_values_length_max_];
	elt=gapp->classMgr()->NthElement(root,1,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_width=matof(val);

	for(long i=0;i<4;i++){
		elt=gapp->classMgr()->NthElement(root,i+2,"float");
		if(!elt){
			return;
		}
		elt->getvalue(val);
		_stroke[i]=matof(val);
	}	

	for(long i=0;i<4;i++){		
		elt=gapp->classMgr()->NthElement(root,i+6,"float");
		if(!elt){
			return;
		}
		elt->getvalue(val);
		_fill[i]=matof(val);
	}	
	_status=0;
}
