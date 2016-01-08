//----------------------------------------------------------------------------
// File : bXMLFile.cpp
// Project : MacMap
// Purpose : C++ source file : xml file class (parse xml for MacMap)
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
// 08/02/2005 creation.
//----------------------------------------------------------------------------

#include "bXMLFile.h"
#include "bMacMapApp.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMLFile	::bXMLFile(const char* path, const char* perm)
			:bStdFile(path,perm){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMLFile::~bXMLFile(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMLFile::mount(char** buffer, int* sz){
	bStdFile::mount(buffer,sz);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMLFile::mount(bGenericClassesMgr* mgr,
					 bGenericXMLBaseElement** root){
_bTrace_("bXMLFile::mount",false);
UInt8*	buff;
int		sz;
	
	(*root)=NULL;
	mount((char**)&buff,&sz);
	if(status()){
_te_("status =="+status());
		return;
	}
	if(sz>0){
		(*root)=mgr->ParseXMLData(buff,sz);
        free(buff);
	}
	else{
_te_("sz<=0");
	}
}
