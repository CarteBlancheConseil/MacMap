//----------------------------------------------------------------------------
// File : bStyle.h
// Project : MacMap
// Purpose : Header file : Style class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 02/02/2004 creation
// 27/03/2007 fonctionne avec les tris inverses
// 17/04/2007 prise en compte des false retournés dans les procédures applyfor
// 04/09/2014 elimination des appels avec geometrie QD
// 18/07/2017 simplification de la boucle de chargement des objets
//----------------------------------------------------------------------------

#ifndef __bStyle__
#define __bStyle__

//----------------------------------------------------------------------------

#include "bStyleRun.h"
#include "bScreenObj.h"

#include <mox_intf/bGenericStyle.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericXMLRenderingElement.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGenericLayersMgr.h>

#include <MacMapSuite/bArray.h>

/**/
//#include <map>
//#include <list>
//#include <vector>
/**/


//----------------------------------------------------------------------------

/**/
//typedef std::multimap   <double, bScreenObj*>	MultiMapScreenObj;
//typedef std::pair		<double, bScreenObj*>	PairScreenObj;
/**/

typedef	void	(*drawProc)(bGenericGraphicContext*);
typedef	float	(*distProc)(bGenericGraphicContext*,CGPoint*);
typedef	void	(*boundProc)(bGenericGraphicContext*);

//----------------------------------------------------------------------------

class bStyle : public bGenericStyle{
public:
	bStyle 										(	bGenericLayersMgr* mgr);
	virtual ~bStyle   							(	);

// Accès librairie XML
	virtual void setidentification				(	void *elt);
	virtual void setglobals						(	void *elt);
	virtual void setcondition					(	void *elt);
	
	virtual void setdrawingmethod				(	int k);
	virtual void setclassfield					(	const char *name);
	virtual void setpasscount					(	int n);
	virtual void setscaleref					(	double d);
	virtual void setunitcoef					(	double d);
	virtual void setsortkind					(	int k);
	virtual void addclassbound					(	double d);
	virtual void addclassname					(	const char* name);
	
	virtual void addstylerun					(	void *elt);
	virtual void setrunvalidity					(	void *elt);
	virtual void setrunrender					(	void *elt);
	virtual void setrungeometry					(	void *elt);
	
	virtual void setrunscalemin					(	double d);
	virtual void setrunscalemax					(	double d);
	virtual void setrunclassmin					(	int idx);
	virtual void setrunclassmax					(	int idx);
	virtual void setrunpass						(	int idx);
	
	virtual void settype						(	const char *name);
	virtual void setname						(	const char *name);
	virtual void setvisible						(	bool vis);
	virtual void setselectable					(	bool sel);
	virtual void setroot						(	void *elt);
	virtual void setlayer						(	void *elt);
	
	virtual void setmargin						(	int margin);

// Accès noyau/externes		
	virtual void setbounds						(	ivx_rect* bounds);
	virtual void setcontext						(	bGenericGraphicContext* ctx);
				
	virtual void draw							(	bStdWait& wt);
	virtual void draw							(	bGenericGeoElement* o);
	virtual void rect							(	bGenericGeoElement* o, 
													CGRect* bounds);
	virtual bGenericGraphicContext* 
	get_context_for_object						(	bGenericGeoElement* o,
													int pass,
													bool render);
	virtual bGenericGraphicContext* 
	get_context_for_class						(	int clss,
													int pass,
													bool render);
	
	virtual bGenericType* gettype				(	);
	virtual char* getname						(	);
	virtual bool visible						(	);
	virtual bool selectable						(	);
	
	virtual int getdrawingmethod				(	);
	virtual char* getclassfield					(	);
	virtual int getpasscount					(	);
	virtual double getscaleref					(	);
	virtual double getunitcoef					(	);
	virtual int getsortkind						(	);
	virtual int getclasscount					(	);
	virtual double getclassbound				(	int idx);
	virtual char* getclassname					(	int idx);
	virtual void set_curclass					(	int idx);
	virtual int get_curclass					(	);

	virtual int getmargin						(	);

	virtual void report							(	);

	virtual bArray* objsinrect					(	CGRect* cgr, 
													bool strict);
	virtual bArray* objsincircle				(	CGRect* cgr, 
													bool strict);
	virtual bArray* objsinpath					(	CGPoint* cgp, 
													int nb, 
													bool strict);
	virtual bArray* objsatpoint					(	CGPoint* cgp, 
													bool all);
	
	virtual bGenericXMLBaseElement* root		(	);
	virtual void get_styleruns					(	bArray& arr,
													bGenericGeoElement* o,
													double scale,
													int pass);

// Privé		
	virtual bool load							(	);
	virtual bool save							(	);

	virtual int objcount						(	);
	virtual bScreenObj* objget					(	int idx);
	
	virtual int findclass						(	double val);
	virtual int findclass						(	bGenericGeoElement* o);
	
	virtual int first							(	int clss);
	virtual int last							(	int clss);
	
	virtual bGenericXMLBaseElement* getlayer	(	);
	virtual void flushscreen					(	);

	virtual bool is_virtual						(	);
	
protected:
	virtual float dist							(	bScreenObj* scr, 
													CGPoint* cgp);
	
	static int boundsComp						(	const void *a,
													const void *b);
	static int boundsCompInv					(	const void *a, 
													const void *b);
	static int classComp						(	const void *a, 
													const void *b);
	static int classCompInv						(	const void *a, 
													const void *b);
	static int fullComp							(	const void *a, 
													const void *b);
	static int fullCompInv						(	const void *a, 
													const void *b);
	
	static int fill								(	void *prm,
													void *o);
	
	void flushstyleruns							(	);
	
	bool applyglobals							(	);
	bool applyconditions						(	);
	bool applystylesruns						(	int ir);
	bool applystylesruns						(	bScreenObj* scr,
													int ir);
	bool applygeometry							(	int ir);
	bool applygeometry							(	bScreenObj* scr,
													int ir);
	void findgoodstylesruns						(	int pass, 
													int clss);
	
	static void drawIcon						(	bGenericGraphicContext* ctx);	
	static void drawCircle						(	bGenericGraphicContext* ctx);
	static void drawRect						(	bGenericGraphicContext* ctx);
	static void drawText						(	bGenericGraphicContext* ctx);
	static void drawLine						(	bGenericGraphicContext* ctx);
	static void drawSurf						(	bGenericGraphicContext* ctx);
	static void drawRaster						(	bGenericGraphicContext* ctx);
	static void drawNothing						(	bGenericGraphicContext* ctx);
	
	static float distToIcon						(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToCircle					(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToRect						(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToText						(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToLine						(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distMaxToLine					(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToSurf						(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToRaster					(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	static float distToNothing					(	bGenericGraphicContext* ctx, 
													CGPoint* cgp);
	
	static void boundsForIcon					(	bGenericGraphicContext* ctx);
	static void boundsForCircle					(	bGenericGraphicContext* ctx);
	static void boundsForRect					(	bGenericGraphicContext* ctx);
	static void boundsForText					(	bGenericGraphicContext* ctx);
	static void boundsForLine					(	bGenericGraphicContext* ctx);
	static void boundsForSurf					(	bGenericGraphicContext* ctx);
	static void boundsForRaster					(	bGenericGraphicContext* ctx);
	static void boundsForNothing				(	bGenericGraphicContext* ctx);
	
	bGenericGraphicContext*			_ctx;

	char*							_typename;
	int								_typeindex;
	char*							_name;
	int								_sign;
	bool							_vis;
	bool							_sel;
	bool							_lock;
	
	char*							_fieldname;
	int								_fieldindex;
	int								_nbpass;
	double							_scaleref;
	double							_unitcoef;
	int								_sort;
	
	double							_scale;
	
	ivx_rect 						_area;
	int		 						_nobjects;

	int								_drawmethod;
	drawProc						_drawp;
	distProc						_distp;
	boundProc						_boundp;
	
	bArray*							_screenobjs;
//    MultiMapScreenObj               _mmscreenobjs;
    
	bArray*							_valbounds;
	bArray*							_valnames;
	bArray*							_offsets;
	bArray*							_counts;
	double							_minbound;
	double							_maxbound;
	
	bGenericXMLBaseElement*			_root;
	bGenericXMLBaseElement*			_layer;
	bGenericXMLBaseElement*			_identifier;
	
	bGenericXMLRenderingElement*	_globals;
	bGenericXMLRenderingElement*	_condition;
	bArray*							_styleruns;
	bArray*							_goodstyleruns;
	
	bStyleRun*						_curstylerun;
	
	int								_curclass;
	
	int								_margin;

    bGenericLayersMgr*              _mgr;
    
private:

};

//----------------------------------------------------------------------------

#endif
