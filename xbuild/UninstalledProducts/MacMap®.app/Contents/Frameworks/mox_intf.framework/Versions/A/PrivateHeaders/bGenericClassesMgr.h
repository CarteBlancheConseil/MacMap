//----------------------------------------------------------------------------
// File : bGenericClassesMgr.h
// Project : MacMap
// Purpose : Header file : Root class for XML classes manager
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
// 04/02/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericClassesMgr__
#define __bGenericClassesMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bStdFile.h>

//----------------------------------------------------------------------------

typedef struct xmlelt{
	int						indent;
	char*					name;
	char*					value;
	bGenericXMLBaseElement*	element;
}xmlelt;

//----------------------------------------------------------------------------

class bGenericClassesMgr : public bGenericMgr{
public:
    virtual ~bGenericClassesMgr								(	){};
    virtual bGenericXMLBaseElement* CreateXMLInstance		(	const char* name)=0;
    virtual bGenericXMLBaseElement* CreateXMLInstance		(	const char* name, 
                                                                const char* value, 
                                                                bGenericXMLBaseElement* parent)=0;
    virtual bGenericXMLBaseElement* FindXMLInstance			(	int sign)=0;
    virtual bool GetXMLInstanceName							(	int sign, 
                                                                char* name)=0;
    virtual void ReleaseXMLInstance							(	bGenericXMLBaseElement* elt)=0;
    virtual bGenericXMLBaseElement* ParseXMLData			(	void* data,
                                                                size_t datasz)=0;
    virtual bGenericXMLBaseElement* ParseXMLDescriptors		(	bArray* xml)=0;
    virtual bool AddXMLClass								(	bGenericXMLBaseElement* elt)=0;

    virtual bGenericXMLBaseElement* NthElement				(	bGenericXMLBaseElement* root, 
                                                                int idx,
                                                                const char *name)=0;
    virtual int CountElements								(	bGenericXMLBaseElement* root, 
                                                                const char *name)=0;
    virtual void FindXMLInstanceByClass						(	bArray& arr, 
                                                                int sign)=0;
    virtual void FindXMLInstanceBySubClass					(	bArray& arr, 
                                                                int sign)=0;
};

//----------------------------------------------------------------------------

#endif
