//----------------------------------------------------------------------------
// File : bXMapStatFieldElement.cpp
// Project : MacMap
// Purpose : C++ source file : Geographic object simulator class for browser using fields or matrix data
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
// 16/04/2008 creation.
//----------------------------------------------------------------------------

#include "bXMapStatFieldElement.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapStatFieldElement::bXMapStatFieldElement(){
	_select=false;
	_name=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bXMapStatFieldElement::~bXMapStatFieldElement(){
	if(_name){
		free(_name);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::kill(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::unkill(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::killed(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::selected(){
	return(_select);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setselect(bool b){
	_select=b;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setselect(bool b, bool report){
	_select=b;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::contrasted(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setcontrast(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setcontrast(bool b, bool report){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::masked(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setmasked(bool b, bool report){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::atscreen(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setatscreen(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::needupdate(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::setneedupdate(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::handled(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::sethandled(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getID(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getType(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getOffset(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::getBounds(ivx_rect* vxr){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::getVertices(ivertices** vxs){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setVertices(ivertices* vxs){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getIndex(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setIndex(int index){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getSubType(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setSubType(int subtype){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getColor(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setColor(int color){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapStatFieldElement::getDirection(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setDirection(int direction){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::getName(char *name){
	if(_name){
		strcpy(name,_name);
	}
	else{
		name[0]=0;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setName(char *name){
	if(_name){
		free(_name);
	}
	_name=strdup(name);
	return(_name!=NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(int fieldIdx, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(char *fieldName, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(int fieldIdx, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(char *fieldName, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(int fieldIdx, int *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(char *fieldName, int *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(int fieldIdx, double *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::getValue(char *fieldName, double *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(int fieldIdx, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(char *fieldName, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(int fieldIdx, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(char *fieldName, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(int fieldIdx, int value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(char *fieldName, int value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(int fieldIdx, double value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::setValue(char *fieldName, double value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::flag1(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::set_flag1(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::flag2(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::set_flag2(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::flag3(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::set_flag3(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::flag4(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::set_flag4(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStatFieldElement::is_lock(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::set_lock(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStatFieldElement::inval(int idx){
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bXMapStatFieldElement::get_ref(){
	return(NULL);
}

