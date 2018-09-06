//----------------------------------------------------------------------------
// File : bXMapsScripting.h
// Project : MacMap
// Purpose : Header file : XMap classes, scripting utilities
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
// 31/12/2008 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapsScripting__
#define __bXMapsScripting__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

#define kNbScriptDictionaryChilCount	8

//----------------------------------------------------------------------------

class bXMapScriptingDictionary : public bStdXMap{
public:		
	bXMapScriptingDictionary				(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapScriptingDictionary		(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
protected:
	
private:
	bStdXMap*	_child[kNbScriptDictionaryChilCount];
};

//----------------------------------------------------------------------------

class bXMapExecMacro : public bStdXMap{
public:		
	bXMapExecMacro							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapExecMacro					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	virtual bool test						(	void* prm);
	
protected:
	virtual bool exec						(	char* name);
	
private:
};

//----------------------------------------------------------------------------

class bXMapRefreshScreen : public bStdXMap{
public:		
	bXMapRefreshScreen						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapRefreshScreen				(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapInitLog : public bStdXMap{
public:		
	bXMapInitLog							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapInitLog					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapCloseLog : public bStdXMap{
public:		
	bXMapCloseLog							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapCloseLog					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapSetLog : public bStdXMap{
public:		
	bXMapSetLog								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapSetLog					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapHideStyle : public bStdXMap{
public:		
	bXMapHideStyle							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapHideStyle					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapShowStyle : public bStdXMap{
public:		
	bXMapShowStyle							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bXMapShowStyle					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual bool process					(	int msg, 
												void* prm);
	
protected:
	
private:
};

//----------------------------------------------------------------------------

class bXMapLockScreen : public bStdXMap{
public:
    bXMapLockScreen							(	bGenericXMLBaseElement* elt,
                                                bGenericMacMapApp* gapp,
                                                CFBundleRef bndl);
    virtual ~bXMapLockScreen                (	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg,
                                                void* prm);
    
protected:
    
private:
};

//----------------------------------------------------------------------------

#endif
