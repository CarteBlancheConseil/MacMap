//----------------------------------------------------------------------------
// File : bXMapVecPublisher.h
// Project : MacMap
// Purpose : Header file : XMap classes, graphic vector exports (PDF, KML)
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
// 21/03/2006 creation.
// 22/02/2007 ajout de choose_location pour passer le path en paramètre
//            au PDFGraphicContext.
// 10/02/2015 Passage des fenêtres NAV en NS.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapPDFPublisher__
#define __bXMapPDFPublisher__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

class bXMapVecPublisher : public bStdXMap{
public:
    bXMapVecPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapVecPublisher				(	);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:
    virtual OSStatus choose_location		(	char* path);
    void dump								(	char* path);

    UInt32	_gc;
    
private:
};

//----------------------------------------------------------------------------

class bXMapPDFPublisher : public bXMapVecPublisher{
public:
    bXMapPDFPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapPDFPublisher				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);

protected:
    
private:
};

//----------------------------------------------------------------------------

class bXMapKMLPublisher : public bXMapVecPublisher{
public:		
    bXMapKMLPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapKMLPublisher				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    virtual bool test						(	void* prm);

protected:
    
private:
};

//----------------------------------------------------------------------------

#endif
