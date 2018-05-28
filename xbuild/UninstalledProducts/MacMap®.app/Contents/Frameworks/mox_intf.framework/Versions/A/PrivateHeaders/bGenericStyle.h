//----------------------------------------------------------------------------
// File : bGenericStyle.h
// Project : MacMap
// Purpose : Header file : Root class for styles
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
// 02/02/2004 creation.
// 04/09/2014 elimination des appels avec geometrie QD.
//----------------------------------------------------------------------------

#ifndef __bGenericStyle__
#define __bGenericStyle__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bStdWait.h>

//----------------------------------------------------------------------------

enum{
	kNoStyleKind_			= _kNULLSign_,	// Not inited
	kIconStyleKind_			= 'Icon',		// Icon
	kCircleStyleKind_		= 'Circ',		// Circle
	kRectStyleKind_			= 'Rect',		// Rect
	kTextStyleKind_			= 'Text',		// Text
	kLineStyleKind_			= 'Line',		// Polyline
	kSurfStyleKind_			= 'Surf',		// Polygon
	kRasterStyleKind_		= 'Rast'		// Raster (TIFF, JPEG, PNG)
};

//----------------------------------------------------------------------------

class bGenericStyle : public bGenericElement{
	public:
		virtual ~bGenericStyle						(	){};	

// Accès librairie XML
		virtual void setidentification				(	void* elt)=0;
		virtual void setglobals						(	void* elt)=0;
		virtual void setcondition					(	void* elt)=0;
		
		virtual void setdrawingmethod				(	int k)=0;
		virtual void setclassfield					(	const char* name)=0;
		virtual void setpasscount					(	int n)=0;
		virtual void setscaleref					(	double d)=0;
		virtual void setunitcoef					(	double d)=0;
		virtual void setsortkind					(	int k)=0;
		virtual void addclassbound					(	double d)=0;
		virtual void addclassname					(	const char* name)=0;

		virtual void addstylerun					(	void* elt)=0;
		virtual void setrunvalidity					(	void* elt)=0;
		virtual void setrunrender					(	void* elt)=0;
		virtual void setrungeometry					(	void* elt)=0;
		
		virtual void setrunscalemin					(	double d)=0;
		virtual void setrunscalemax					(	double d)=0;
		virtual void setrunclassmin					(	int idx)=0;
		virtual void setrunclassmax					(	int idx)=0;
		virtual void setrunpass						(	int idx)=0;
				
		virtual void settype						(	const char* name)=0;
		virtual void setname						(	const char* name)=0;
		virtual void setvisible						(	bool vis)=0;
		virtual void setselectable					(	bool sel)=0;
		virtual void setroot						(	void* elt)=0;
		virtual void setlayer						(	void* elt)=0;
		
		virtual void setmargin						(	int margin)=0;
		
// Accès noyau/externes		
		virtual void setbounds						(	ivx_rect* bounds)=0;
		virtual void setcontext						(	bGenericGraphicContext* ctx)=0;
		
		virtual void draw							(	bStdWait& wt)=0;
		virtual void draw							(	bGenericGeoElement* o)=0;
		virtual void rect							(	bGenericGeoElement* o, 
														CGRect* bounds)=0;
		virtual bGenericGraphicContext* 
		get_context_for_object						(	bGenericGeoElement* o,
														int pass,
														bool render)=0;
		virtual bGenericGraphicContext* 
		get_context_for_class						(	int clss,
														int pass,
														bool render)=0;
		
		virtual bGenericType* gettype				(	)=0;
		virtual char* getname						(	)=0;
		virtual bool visible						(	)=0;
		virtual bool selectable						(	)=0;

		virtual int getdrawingmethod				(	)=0;
		virtual char* getclassfield					(	)=0;
		virtual int getpasscount					(	)=0;
		virtual double getscaleref					(	)=0;
		virtual double getunitcoef					(	)=0;
		virtual int getsortkind						(	)=0;
		virtual int getclasscount					(	)=0;
		virtual double getclassbound				(	int idx)=0;
		virtual char* getclassname					(	int idx)=0;
		virtual void set_curclass					(	int idx)=0;
		virtual int get_curclass					(	)=0;
		virtual int getmargin						(	)=0;

		virtual void report							(	)=0;
		
		virtual bArray* objsinrect					(	CGRect* cgr, 
														bool strict)=0;
		virtual bArray* objsincircle				(	CGRect* cgr, 
														bool strict)=0;
		virtual bArray* objsinpath					(	CGPoint* cgp, 
														int nb, 
														bool strict)=0;
		virtual bArray* objsatpoint					(	CGPoint* cgp, 
														bool all)=0;
// root of style
		virtual bGenericXMLBaseElement* root		(	)=0;
// styleruns access (returns array of bGenericXMLBaseElement*)
		virtual void get_styleruns					(	bArray& arr,
														bGenericGeoElement* o,
														double scale,
														int pass)=0;
};

//----------------------------------------------------------------------------

#endif
