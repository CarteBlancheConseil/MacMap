//----------------------------------------------------------------------------
// File : bXMapsRasterPublisher.h
// Project : MacMap
// Purpose : Header file : XMap classes, raster exports (JPG, TIF, PNG)
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
// 12/04/2007 creation.
// 10/02/2015 Passage des fenêtres NAV en NS.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapsRasterPublisher__
#define __bXMapsRasterPublisher__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define kXMapMultiPublisherProcessMessageID	"Processing"

//----------------------------------------------------------------------------

class bXMapsRasterPublisher : public bStdXMap{
public:
    bXMapsRasterPublisher					(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapsRasterPublisher			(	);
    
    virtual void open						(	int* flags);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    virtual void editOptions				(	);
    virtual void putCodec                   (	UInt32 codec);
    
protected:
    raster_export_prm	_prm;
    
    virtual bool raster_export				(	);
    virtual bool raster_export				(	raster_export_prm* ep);
    
private:
};

//----------------------------------------------------------------------------

class bXMapJPGPublisher : public bXMapsRasterPublisher{
public:
    bXMapJPGPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapJPGPublisher				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
protected:
    
private:
};

//----------------------------------------------------------------------------

class bXMapTIFFPublisher : public bXMapsRasterPublisher{
public:
    bXMapTIFFPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapTIFFPublisher				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
protected:
    
private:
};

//----------------------------------------------------------------------------

class bXMapPNGPublisher : public bXMapsRasterPublisher{
public:
    bXMapPNGPublisher						(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapPNGPublisher				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
protected:
    
private:
};

//----------------------------------------------------------------------------

#endif
