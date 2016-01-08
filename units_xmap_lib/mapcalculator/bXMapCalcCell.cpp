//----------------------------------------------------------------------------
// File : bXMapCalcCell.cpp
// Project : MacMap
// Purpose : C++ source file : MapCalculator editor cells classes
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapCalcCell.h"
#include <mox_intf/CGUtils.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bGenericCalc.h>
#include <std_ext/bStdUserExt.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define kCellFont			"Geneva"
#define kCellFontSize		9
#define kCellHeight			16
#define kCellMargin			2
#define kCellInterMargin	15
#define kCellDeadEndSize	5

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCell
// ---------------------------------------------------------------------------
// 
// ------------
CGPoint				bXMapCalcCell::_go={0,0};
bGenericMacMapApp*	bXMapCalcCell::_gapp=NULL;
bXMapCalcCell*		bXMapCalcCell::_gsel=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCell::bXMapCalcCell(bGenericMacMapApp* gapp, const char* name){
	_lft=NULL;
	_rgt=NULL;
	_par=NULL;
	_r=1;
	_g=1;
	_b=1;
	_cnt=1;
	_knd=kXMapCalcEditorKindNone;
	_lref=0;
	sprintf(_name,"%s",name);
	if(gapp){
		_gapp=gapp;
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCell::~bXMapCalcCell(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::set_gorigin(CGPoint cgp){
	_go=cgp;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCell::parse(bGenericXMLBaseElement* root, void* prm, int indent){
UInt32			knd=root->getsubclass();
bool			b=true;
parse_prm*		p=(parse_prm*)prm;
bXMapCalcCell*	cell=NULL;

//fprintf(stderr,"%d->%.4s(%d) --- ",indent,&knd,p->stack->count());
//fprintf(stderr,"%d --- ",p->cur);

	if(indent==0){
//fprintf(stderr,"zap\n");
		return(true);
	}
	
	if(p->stack->count()>0){
		if(indent<=p->cur){
			for(int i=1;i<=(p->cur-indent)+1;i++){
				p->stack->rmv(p->stack->count());
			}
//fprintf(stderr,"remove (cur(%d)->indent(%d)) --- ",p->cur,indent);
			
		}
		p->stack->get(p->stack->count(),&cell);
if(cell){
//fprintf(stderr,"root=%s --- ",cell->_name);
}
else{
//fprintf(stderr,"root=NULL --- ");
}
	}


	if(_gapp==NULL){
		_gapp=(bGenericMacMapApp*)root->getapp();
	}
	switch(knd){
		case kXMLSubClassOperator:
			b=bXMapCalcCellOperator::parse(root,&cell);
			break;
		case kXMLSubClassIterator:
			b=bXMapCalcCellIterator::parse(root,&cell);
			break;
		case kXMLSubClassFunction:
			b=bXMapCalcCellFunction::parse(root,&cell);
			break;
		case kXMLSubClassConstant:
			b=bXMapCalcCellConstant::parse(root,&cell);
			break;
		case kXMLSubClassField:
			b=bXMapCalcCellField::parse(root,&cell);
			break;
		case kXMLSubClassCalc:
			b=bXMapCalcCellCalc::parse(root,&cell);
			break;
		case kXMLSubClassMacro:
			b=bXMapCalcCellMacro::parse(root,&cell);
			break;
	}
	
	if(p->root==NULL){
	//	p->root=cell;
	}
	if(b){
		p->stack->add(&cell);
//fprintf(stderr,"top=%s --- ",cell->_name);
	}
	else{
//fprintf(stderr,"top=NULL --- ");
	}
	
	p->cur=indent;
//fprintf(stderr,"%d\n",p->stack->count());
	
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::select(bool flg){
	if(flg){
		_gsel=this;
	}
	else{
		_gsel=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCell::click(CGPoint cgp){
	_bTrace_("bXMapCalcCell::click",false);
CGRect	cgr;
	rect(&cgr);
_tm_(cgp.x+":"+cgp.y+"=>"+cgr.origin.x+":"+cgr.origin.y+":"+cgr.size.width+":"+cgr.size.height);
	
	if(CGRectContainsPoint(cgr,cgp)){
		select(true);
		return(true);
	}
	if(_lft){
		if(_lft->click(cgp)){
			return(true);
		}
	}
	if(_rgt){
		if(_rgt->click(cgp)){
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::draw(CGContextRef ctx, CGPoint go){
	
//_bTrace_("bXMapCalcCell::draw",false);
	_o=go;
	_o.x-=_go.x;
	_o.y-=_go.y;
CGRect	cgr;
	rect(&cgr);
		
	CGContextSetLineWidth(ctx,1);
	
	CGContextSetRGBFillColor(ctx,_r,_g,_b,1);
	CGContextFillRect(ctx,cgr);
	CGContextSetRGBStrokeColor(ctx,0.1,0.1,0.1,1);
	CGContextStrokeRect(ctx,cgr);
	
	CGContextSetRGBFillColor(ctx,0,0,0,1);
	CGContextSelectFont(ctx,kCellFont,kCellFontSize,kCGEncodingMacRoman);
	CGContextSetTextDrawingMode(ctx,kCGTextFill);
	CGContextShowStringAtPoint(	ctx,
								cgr.origin.x+kCellMargin,
								cgr.origin.y+(cgr.size.height*1/3),
								_name,
								kCellFont,
								kCellFontSize);
								
CGPoint	cgp[2];
	cgp[0].x=_o.x+cgr.size.width;
	cgp[0].y=_o.y;
float	cnt;
	if(_lft){
		CGContextSetRGBStrokeColor(ctx,0.1,0.1,0.1,1);
//		cnt=_lft->estimate();
		cnt=_lft->_lref;
		cgp[1].x=cgr.origin.x+cgr.size.width+kCellInterMargin;
		cgp[1].y=	(_rgt)?
					_o.y+((cnt+1)*kCellHeight):
					_o.y;
		CGContextAddLines(ctx,cgp,2);
		CGContextDrawPath(ctx,kCGPathStroke);	
		cgp[1].y+=_go.y;
		cgp[1].x+=_go.x;
		_lft->draw(ctx,cgp[1]);
	}

	if(_rgt){
		CGContextSetRGBStrokeColor(ctx,0.1,0.1,0.1,1);
//		cnt=_rgt->estimate();
		cnt=_rgt->_lref;
		cgp[1].x=cgr.origin.x+cgr.size.width+kCellInterMargin;
		cgp[1].y=	(_lft)?
					_o.y-((cnt+1)*kCellHeight):
					_o.y;
		CGContextAddLines(ctx,cgp,2);
		CGContextDrawPath(ctx,kCGPathStroke);	
		cgp[1].y+=_go.y;
		cgp[1].x+=_go.x;
		_rgt->draw(ctx,cgp[1]);
	}
	
	if(_gsel==this){
/*RGBColor	clr;
	GetThemeBrushAsColor(	kThemeBrushPrimaryHighlightColor,
							32,
							false,
							&clr);
float		r=(float)clr.red/(float)USHRT_MAX;
float		g=(float)clr.green/(float)USHRT_MAX;
float		b=(float)clr.blue/(float)USHRT_MAX;

		CGContextSetRGBStrokeColor(ctx,r,g,b,1);*/

		CGContextSetRGBStrokeColor(ctx,0,0,0,1);
		
		cgr=CGRectInset(cgr,-2,-2);
		CGContextStrokeRect(ctx,cgr);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::rect(CGRect* cgr){
bCoreTextInfo	txt(kCellFont,kCellFontSize,0,0,0,_name);
	cgr->origin.x=_o.x;
	cgr->origin.y=_o.y-(kCellHeight/2.0);
	cgr->size.width=txt.textWidth()+2*kCellMargin;
	cgr->size.height=kCellHeight;
}


// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::read(bGenericXMLBaseElement* root){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::write(bArray& arr,int indent){
	add_cdesc(arr,indent,_name,"");
	if(_lft){
		_lft->write(arr,indent+1);
	}
	if(_rgt){
		_rgt->write(arr,indent+1);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCell::add(bXMapCalcCell* cell){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCell::remove(bXMapCalcCell* cell){
	if(cell->kind()==kXMapCalcEditorKindNone){
		return(false);
	}
	if((_lft)&&(cell==_lft)){
		delete _lft;
		_lft=new bXMapCalcCellDeadEnd(NULL,"");
		_lft->set_parent(this);
		return(true);
	}
	if((_rgt)&&(cell==_rgt)){
		delete _rgt;
		_rgt=new bXMapCalcCellDeadEnd(NULL,"");
		_rgt->set_parent(this);
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bXMapCalcCell::estimate(){
int	l=0,r=0;
	_lref=0;
	if(_lft){
		l=_lft->estimate();
	}
	if(_rgt){
		r=_rgt->estimate();
	}
	if((_par)&&(_rgt)&&(_lft)){
		if(_par->_lft==this){
			_lref=r;
		}
		else if(_par->_rgt==this){
			_lref=l;
		}
	}
	else if((_par)&&(_lft)){
		_lref=_lft->_lref;
	}	
	else if((_par)&&(_rgt)){
		_lref=_rgt->_lref;
	}	
	if((l==0)&&(r==0)){
		l=1;
	}
	return((_cnt==1)?(l+r):(l+r+1));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::set_left(bXMapCalcCell* cell){
	_lft=cell;
	if(cell){
		_lft->set_parent(this);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCell::set_right(bXMapCalcCell* cell){
	_rgt=cell;
	if(cell){
		_rgt->set_parent(this);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellDeadEnd
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellDeadEnd::bXMapCalcCellDeadEnd(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=0.75;
	_g=0.75;
	_b=0.75;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellDeadEnd::~bXMapCalcCellDeadEnd(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellDeadEnd::rect(CGRect* cgr){
	cgr->origin.x=_o.x;
	cgr->origin.y=_o.y-(kCellDeadEndSize/2.0);
	cgr->size.width=kCellDeadEndSize;
	cgr->size.height=kCellDeadEndSize;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellDeadEnd::write(bArray& arr,int indent){
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellOperator
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellOperator::bXMapCalcCellOperator(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=1;
	_g=1;
	_b=0.8;
	_lft=new bXMapCalcCellDeadEnd(gapp,"");
	_lft->set_parent(this);
	_rgt=new bXMapCalcCellDeadEnd(gapp,"");
	_rgt->set_parent(this);
	_knd=kXMapCalcEditorKindOperator;
	_cnt=3;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellOperator::~bXMapCalcCellOperator(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellOperator::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
//fprintf(stderr,"Err1\n");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
//fprintf(stderr,"Err2\n");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
//fprintf(stderr,"Err3\n");
		return(false);
	}

char	name[256];
	root->getclassname(name);
bXMapCalcCellOperator*	elt=new bXMapCalcCellOperator(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellOperator::add(bXMapCalcCell* cell){
//_bTrace_("bXMapCalcCellOperator::add",true);
bXMapCalcCell*	csel=bXMapCalcCell::get_select();

	if((csel)&&(csel==_rgt)){
		if(cell->kind()==_rgt->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_rgt=cell;
		_rgt->select(true);
		return(true);
	}
	if((csel)&&(csel==_lft)){
		if(cell->kind()==_lft->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_lft=cell;
		_lft->select(true);
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellIterator
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellIterator::bXMapCalcCellIterator(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=1;
	_g=1;
	_b=0.25;
	_lft=new bXMapCalcCellDeadEnd(gapp,"");
	_lft->set_parent(this);
	_rgt=new bXMapCalcCellDeadEnd(gapp,"");
	_rgt->set_parent(this);
	_knd=kXMapCalcEditorKindIterator;
	_cnt=3;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellIterator::~bXMapCalcCellIterator(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellIterator::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
//_bTrace_("bXMapCalcCellIterator::parse",true);
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
//_tm_("erreur 1");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
//_tm_("erreur 2");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
//_tm_("erreur 3");
		return(false);
	}

char	name[256];
	root->getclassname(name);
bXMapCalcCellIterator*	elt=new bXMapCalcCellIterator(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellIterator::add(bXMapCalcCell* cell){
bXMapCalcCell*	csel=bXMapCalcCell::get_select();
	if((csel)&&(csel==_rgt)){
		if(cell->kind()==kXMapCalcEditorKindMacro){
			return(false);
		}
		if(cell->kind()==_rgt->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_rgt=cell;
		_rgt->select(true);
		return(true);
	}
	if((csel)&&(csel==_lft)){
		if(cell->kind()!=kXMapCalcEditorKindMacro){
			return(false);
		}
		if(cell->kind()==_lft->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_lft=cell;
		_lft->select(true);
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellFunction
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellFunction::bXMapCalcCellFunction(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=1;
	_g=0.75;
	_b=0;
	_lft=new bXMapCalcCellDeadEnd(gapp,"");
	_lft->set_parent(this);
	_knd=kXMapCalcEditorKindFunction;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellFunction::~bXMapCalcCellFunction(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellFunction::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
		return(false);
	}

char	name[256];
	root->getclassname(name);
bXMapCalcCellFunction*	elt=new bXMapCalcCellFunction(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellFunction::add(bXMapCalcCell* cell){
bXMapCalcCell*	csel=bXMapCalcCell::get_select();
	if((csel)&&(csel==_lft)){
		if(cell->kind()==_lft->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_lft=cell;
		_lft->select(true);
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellConstant
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellConstant::bXMapCalcCellConstant(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=0.75;
	_g=1;
	_b=0.75;
	_knd=kXMapCalcEditorKindConstant;	
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellConstant::~bXMapCalcCellConstant(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellConstant::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
		return(false);
	}

char	name[_names_length_max_];
	root->getclassname(name);
int		idx=RecoverVar(_gapp,name);
	if(idx==0){	
		root->getvalue(name);
		idx=RecoverVar(_gapp,name);
	}
	if(idx==0){
		return(false);
	}
	_gapp->varMgr()->ext_name(idx,name);

bXMapCalcCellConstant*	elt=new bXMapCalcCellConstant(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellConstant::write(bArray& arr,int indent){
bGenericCalc*	ext=(bGenericCalc*)(void*)_gapp->varMgr()->find(_name);
char			rnm[256]="????";
char			sgn[10]="";

	if(ext){
		ext->get_params(rnm,sgn);
	}
	add_cdesc(arr,indent,rnm,sgn);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellField
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellField::bXMapCalcCellField(bGenericMacMapApp* gapp, const char* name, bGenericType* tp)
					:bXMapCalcCell(gapp,name){
	_r=0;
	_g=1;
	_b=0;
	_tp=tp;
	_knd=kXMapCalcEditorKindField;
char	tname[256];
	if(tp){
		_tp->name(tname);
	}
	else{
		strcpy(tname,"????");
	}
	strcat(tname,":");
	strcat(tname,_name);
	strcpy(_name,tname);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellField::~bXMapCalcCellField(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellField::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
//_bTrace_("bXMapCalcCellField::parse",true);
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
//_tm_("erreur 1");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
//_tm_("erreur 2");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
//_tm_("erreur 3");
		return(false);
	}
	
bGenericType*		tp;
char				name[_values_length_max_];
	root->getvalue(name);
char*				adr=strchr(name,':');
bXMapCalcCellField*	elt;

	if(adr){
		adr[0]=0;
		adr++;
		tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(name));
		if(tp==NULL){
			root->getvalue(name);
			elt=new bXMapCalcCellField(NULL,adr,NULL);
		}
		else{
			elt=new bXMapCalcCellField(NULL,adr,tp);
		}
	}
	else{
		elt=new bXMapCalcCellField(NULL,name,NULL);
	}
	
	if((*clrt)){
		if((*clrt)->kind()!=kXMapCalcEditorKindIterator){
			if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
				(*clrt)->left()->select(true);
				(*clrt)->add(elt);
			}
			else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
				(*clrt)->right()->select(true);
				(*clrt)->add(elt);
			}
		}
		else{
			if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
				(*clrt)->right()->select(true);
				(*clrt)->add(elt);
			}
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellField::write(bArray& arr,int indent){
	add_cdesc(arr,indent,"dbfield",_name);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellCalc
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellCalc	::bXMapCalcCellCalc(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=0;
	_g=0.75;
	_b=0;
	_knd=kXMapCalcEditorKindCalc;
fprintf(stderr,"calc %s\n",name);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellCalc::~bXMapCalcCellCalc(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellCalc::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
		return(false);
	}

char	name[_names_length_max_];
	root->getclassname(name);
int		idx=RecoverCalc(_gapp,name);
	if(idx==0){	
		root->getvalue(name);
		idx=RecoverCalc(_gapp,name);
	}
	if(idx==0){
		return(true);
	}
	_gapp->calcMgr()->ext_name(idx,name);

bXMapCalcCellCalc*	elt=new bXMapCalcCellCalc(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellCalc::write(bArray& arr,int indent){
bGenericCalc*	ext=(bGenericCalc*)(void*)_gapp->calcMgr()->find(_name);
char			rnm[256]="????";
char			sgn[10]="";

	if(ext){
		ext->get_params(rnm,sgn);
	}
	add_cdesc(arr,indent,rnm,sgn);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapCalcCellMacro
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcCellMacro::bXMapCalcCellMacro(bGenericMacMapApp* gapp, const char* name)
					:bXMapCalcCell(gapp,name){
	_r=0.75;
	_g=0.75;
	_b=1;
	_lft=new bXMapCalcCellDeadEnd(gapp,"");
	_lft->set_parent(this);
	_knd=kXMapCalcEditorKindMacro;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcCellMacro::~bXMapCalcCellMacro(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellMacro::parse(bGenericXMLBaseElement* root, bXMapCalcCell** clrt){
//_bTrace_("bXMapCalcCellMacro::parse",true);
	if(	((*clrt))					&&
		((*clrt)->left()==NULL)		&&
		((*clrt)->right()==NULL)	){
//_tm_("erreur 1");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right()==NULL)							){
//_tm_("erreur 2");
		return(false);
	}
	if(	((*clrt))											&&
		((*clrt)->left())									&&
		((*clrt)->left()->kind()!=kXMapCalcEditorKindNone)	&&
		((*clrt)->right())									&&
		((*clrt)->right()->kind()!=kXMapCalcEditorKindNone)	){
//_tm_("erreur 3");
		return(false);
	}

char	name[_values_length_max_];
	root->getvalue(name);
int		idx=RecoverMacro(_gapp,name);
	if(idx==0){
		return(false);
	}
	_gapp->macroMgr()->ext_name(idx,name);
	
bXMapCalcCellMacro*	elt=new bXMapCalcCellMacro(NULL,name);
	if((*clrt)){
		if(((*clrt)->left())&&((*clrt)->left()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->left()->select(true);
			(*clrt)->add(elt);
		}
		else if(((*clrt)->right())&&((*clrt)->right()->kind()==kXMapCalcEditorKindNone)){
			(*clrt)->right()->select(true);
			(*clrt)->add(elt);
		}
	}
	(*clrt)=elt;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcCellMacro::write(bArray& arr,int indent){
user_ext_data	prm;
bGenericExt*	ext=_gapp->macroMgr()->find(_name);
char			sgn[10]="????";

	prm.sign=0;
	if(ext){
		ext->process(kExtProcessCallGetData,&prm);
		if(prm.sign!=0){
			sprintf(sgn,"%.4s",&prm.sign);
		}
	}
	add_cdesc(arr,indent,"fmacrouser",sgn);
	if(_lft){
		_lft->write(arr,indent+1);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcCellMacro::add(bXMapCalcCell* cell){
bXMapCalcCell*	csel=bXMapCalcCell::get_select();
	if((csel)&&(csel==_lft)){
		if(cell->kind()!=kXMapCalcEditorKindMacro){
			return(false);
		}
		if(cell->kind()==_lft->kind()){
			if(cell->left()){
				delete(cell->left());
			}
			if(cell->right()){
				delete(cell->right());
			}
			cell->set_left(csel->left());
			cell->set_right(csel->right());
			csel->set_left(NULL);
			csel->set_right(NULL);
		}
		cell->set_parent(this);
		_lft=cell;
		_lft->select(true);
		return(true);
	}
	return(false);
}
