//----------------------------------------------------------------------------
// File : bXMapsGo.h
// Project : MacMap
// Purpose : Header file : XMap classes, scrolls screen to top, bottom, left, right, or to the cursor location.
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
// 21/02/2007 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapsGo__
#define __bXMapsGo__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

class bXMapGoRight : public bStdXMap{
public:
    bXMapGoRight							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapGoRight					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapGoLeft : public bStdXMap{
public:		
    bXMapGoLeft								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapGoLeft					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapGoUp : public bStdXMap{
public:		
    bXMapGoUp								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapGoUp						(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapGoDown : public bStdXMap{
public:		
    bXMapGoDown								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapGoDown					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapGoCurs : public bStdXMap{
public:		
    bXMapGoCurs								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapGoCurs					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapZoomInCurs : public bStdXMap{
public:		
    bXMapZoomInCurs							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapZoomInCurs				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bXMapZoomOutCurs : public bStdXMap{
public:		
    bXMapZoomOutCurs						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapZoomOutCurs				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
};

//----------------------------------------------------------------------------

#endif
