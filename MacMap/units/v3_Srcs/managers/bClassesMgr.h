//----------------------------------------------------------------------------
// File : bClassesMgr.h
// Project : MacMap
// Purpose : Header file : XML classes management and parse class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2003-2018 Carte Blanche Conseil.
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
// Classe pour gestion des classes externes
//----------------------------------------------------------------------------
// 16/05/2003 creation.
// 07/11/2018 64bits
//----------------------------------------------------------------------------

#ifndef __bClassesMgr__
#define __bClassesMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericClassesMgr.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bClassesMgr : public bGenericClassesMgr{
public:
    bClassesMgr												(	);
    virtual ~bClassesMgr									(	);
    
    virtual int load										(	);
    virtual int unload										(	);
    
    virtual bGenericXMLBaseElement* CreateXMLInstance		(	const char *name);
    virtual bGenericXMLBaseElement* CreateXMLInstance		(	const char* name, 
                                                                const char* value, 
                                                                bGenericXMLBaseElement* parent);
    virtual bGenericXMLBaseElement* FindXMLInstance			(	int sign);
    virtual bool GetXMLInstanceName							(	int sign, 
                                                                char* name);																	
    virtual void ReleaseXMLInstance							(	bGenericXMLBaseElement* elt);							
    virtual bGenericXMLBaseElement* ParseXMLData			(	void* data,
                                                                size_t datasz);
    virtual bGenericXMLBaseElement* ParseXMLDescriptors		(	bArray* xml);
    virtual bool AddXMLClass								(	bGenericXMLBaseElement* elt);
    
    virtual bGenericXMLBaseElement* NthElement				(	bGenericXMLBaseElement* root, 
                                                                long idx, 
                                                                const char *name);
    virtual long CountElements								(	bGenericXMLBaseElement* root, 
                                                                const char *name);
    
    virtual void FindXMLInstanceByClass						(	bArray& arr, 
                                                                int sign);
    virtual void FindXMLInstanceBySubClass					(	bArray& arr, 
                                                                int sign);

private:
    bGenericXMLBaseElement* findParent						(	bArray* xml, 
                                                                int idx, 
                                                                int indent);
    bArray	_elts;
    bArray	_extelts;
    bool	_loaded;
};

// ---------------------------------------------------------------------------

#endif
