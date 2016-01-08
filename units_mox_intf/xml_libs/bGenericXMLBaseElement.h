//----------------------------------------------------------------------------
// File : bGenericXMLBaseElement.h
// Project : MacMap
// Purpose : Header file : Root class for every XML class
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
// Classe de base pour éléments XML
//----------------------------------------------------------------------------
// 16/05/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericXMLBaseElement__
#define __bGenericXMLBaseElement__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/XMLSignatures.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bStdFile.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

//----------------------------------------------------------------------------

#define _names_length_max_	64
#define _values_length_max_	1024

//----------------------------------------------------------------------------

class bGenericXMLBaseElement : public virtual bGenericElement{
public:
	virtual ~bGenericXMLBaseElement					(	){};
	virtual bGenericXMLBaseElement* instance		(	)=0;
	virtual bGenericXMLBaseElement* create			(	bGenericXMLBaseElement* elt)=0;
	virtual void kill								(	bGenericXMLBaseElement* elt)=0;

    virtual bool decode	  							(	xmlNode* root)=0;
	virtual void encode								(	int indent,
														bFile* f)=0;
	
	virtual void init								(	void* ctx)=0;
	
	virtual void getclassname						(	char* name)=0;
	virtual void setclassname						(	const char* name)=0;
	
	virtual int getclass							(	)=0;
	virtual int getsubclass							(	)=0;
	
	virtual void getvalue							(	char *value)=0;
	virtual void setvalue							(	const char *value)=0;
			
	virtual bGenericXMLBaseElement* search		  	(	CFStringRef name)=0;
	
	virtual bool addelement							(	bGenericXMLBaseElement* elt)=0;
	virtual int countelements						(	)=0;
	virtual bGenericXMLBaseElement* getelement		(	int idx)=0;
	virtual void setelement							(	int	idx,
														bGenericXMLBaseElement* elt)=0;
	virtual void rmvelement							(	int idx)=0;
	virtual void pushelement						(	int idx, 
														int offset)=0;
	
	virtual bool dotoall							(	void *prm, 
														int indent,
														bool (*proc)
														(bGenericXMLBaseElement*,void*,int))=0;
	
	
	virtual void* getapp							(	)=0;
	virtual	CFBundleRef	getbundle					(	)=0;	
};
									
//----------------------------------------------------------------------------

#endif
