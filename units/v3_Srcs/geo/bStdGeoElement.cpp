//----------------------------------------------------------------------------
// File : bStdGeoElement.cpp
// Project : MacMap
// Purpose : C++ source file : Standart MacMap object class
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
// 18/08/2005 creation.
//----------------------------------------------------------------------------

#include "bStdGeoElement.h"
#include "bMacMapApp.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdGeoElement::bStdGeoElement(){
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bStdGeoElement::~bStdGeoElement(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::kill(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::unkill(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::killed(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::selected(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setselect(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setselect(bool b, bool unused){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::contrasted(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setcontrast(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setcontrast(bool b, bool unused){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::masked(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setmasked(bool b, bool report){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::atscreen(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setatscreen(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::needupdate(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::setneedupdate(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::handled(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::sethandled(bool b){
}

/*// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::isnew(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::resetnew(){
}*/

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getID(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getType(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getOffset(){
	return(_offset);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::getBounds(ivx_rect* vxr){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::getVertices(ivertices** vxs){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setVertices(ivertices* vxs){
	return(false);
}

/*// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getIndex(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setIndex(int index){
	return(false);
}*/

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getSubType(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setSubType(int subtype){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getColor(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setColor(int color){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStdGeoElement::getDirection(){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setDirection(int direction){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::getName(char *name){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setName(char *name){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(int fieldIdx, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(char *fieldName, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(int fieldIdx, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(char *fieldName, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(int fieldIdx, int *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(char *fieldName, int *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(int fieldIdx, double *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::getValue(char *fieldName, double *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(int fieldIdx, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(char *fieldName, void *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(int fieldIdx, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(char *fieldName, char *value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(int fieldIdx, int value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(char *fieldName, int value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(int fieldIdx, double value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::setValue(char *fieldName, double value){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::flag1(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::set_flag1(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::flag2(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::set_flag2(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::flag3(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::set_flag3(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::flag4(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::set_flag4(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdGeoElement::is_lock(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::set_lock(bool b){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdGeoElement::inval(int idx){
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bStdGeoElement::get_ref(){
	return(NULL);
}

