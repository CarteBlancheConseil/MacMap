//----------------------------------------------------------------------------
// File : bStdXMLBaseElement.h
// Project : MacMap
// Purpose : Header file : XML base class
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

#ifndef __bStdXMLBaseElement__
#define __bStdXMLBaseElement__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericMacMapApp.h>

#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

char	dumpHex			(char c);
char*	decodeString	(CFStringRef cfs);
char*	decodeCString	(const char* cstr);
void	encodeString	(char* name);

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

class bStdXMLBaseElement : public virtual bGenericXMLBaseElement{
public:
    bStdXMLBaseElement 							(	bGenericXMLBaseElement* elt,
                                                    bGenericMacMapApp* gapp,
                                                    CFBundleRef bndl);
    virtual ~bStdXMLBaseElement  				(	);
    
    virtual bGenericXMLBaseElement* instance	(	);
    virtual bGenericXMLBaseElement* create		(	bGenericXMLBaseElement* elt);
    virtual void kill							(	bGenericXMLBaseElement* elt);

    virtual bool decode	  						(	xmlNode* root);
    virtual void encode							(	int indent,
                                                    bFile* f);
    
    virtual void init							(	void* ctx);
    
    virtual void getclassname					(	char* name);
    virtual void setclassname					(	const char* name);
    
    virtual int getclass						(	);
    virtual int getsubclass						(	);
    
    virtual void getvalue						(	char *value);
    virtual void setvalue						(	const char *value);
            
    virtual bGenericXMLBaseElement* search		(	CFStringRef	name);// A priori plus utilisé
    
    virtual bool addelement						(	bGenericXMLBaseElement* elt);
    virtual int countelements					(	);
    virtual bGenericXMLBaseElement* getelement	(	int idx);
    virtual void setelement						(	int	idx,
                                                    bGenericXMLBaseElement* elt);
    virtual void rmvelement						(	int idx);
    virtual void pushelement					(	int idx,
                                                    int offset);
    
    virtual bool dotoall						(	void *prm,
                                                    int indent,
                                                    bool (*proc)
                                                    (bGenericXMLBaseElement*,void*,int));
    
    virtual void* getapp						(	);
    virtual CFBundleRef getbundle				(	);

// Utilitaires	
protected:
    char*							_cfname;
    char*							_cfvalue;
    bGenericXMLBaseElement*			_creator;
    static bGenericMacMapApp* 		_gapp;
    CFBundleRef						_bndl;
    bArray							_elts;
    int								_clss;
    int								_subclss;
    
private:
};
									
//----------------------------------------------------------------------------

#endif
