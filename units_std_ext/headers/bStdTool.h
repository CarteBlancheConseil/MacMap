//----------------------------------------------------------------------------
// File : bStdTool.h
// Project : MacMap
// Purpose : Header file : Base class for Tools
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
// 27/12/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStdTool__
#define __bStdTool__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <mox_intf/bGenericTool.h>
#include <mox_intf/bGraphicParams.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/NSCursorWrappers.h>

#include <MacMapSuite/bTrace.h>

//----------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define	kStdToolConfigSign		'loot'
#else
#define	kStdToolConfigSign		'tool'
#endif

#define	kCurrentToolParamVers	"3.1.0"

#define	kStdToolMenuConfig		"toolmenu"

#define	sqr(x)			((x)*(x))

//----------------------------------------------------------------------------

class bStdTool : public virtual bGenericTool, public bStdExt {
public:		
	bStdTool								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdTool						(	);
			
	virtual void open						(	int* flags);
	virtual void close						(	);
/*> A supprimer */
	virtual bool event						(	EventRef evt);//CC
/*< A supprimer */
	virtual bool nsevent					(	void* nsevt);
	virtual void idle						(	void* prm);
	virtual bool process					(	int msg, 
												void* prm);
	
	virtual int	tool_class					(	);
	virtual CGImageRef symb					(	);
	
	virtual void activate					(	);
	virtual void deactivate					(	);

	
	virtual bool edit						(	void* prm);
	virtual void pop						(	cocoaMenuPrm prm);
	virtual void clic						(	i2dvertex loc, 
												int count);
	virtual void clic						(	CGPoint loc,
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
			
	virtual void drag						(	CGPoint pt);
	virtual void set_use_drag				(	bool b);
	virtual bool get_use_drag				(	);
	
	virtual bool get_on_drag				(	);
    
    virtual bool get_on_edit				(	){return _on_edit;};
    virtual void set_on_edit				(	bool on_edit){_on_edit=on_edit;};
    
	virtual bool get_active					(	);

	virtual void set_mnu_cmd				(	int cmd);
	virtual int get_mnu_cmd					(	);
	
	virtual bool set_key					(	int k);
	virtual bool is_key						(	int k);
	virtual void set_modifiers				(	int k);
	virtual bool is_modifiers				(	int k);
	
	virtual CursorRef load_curs				(	const char* name);

	virtual void set_curs					(	CursorRef curs);
	virtual void set_curs					(	);
	virtual void set_arrow					(	);
	
	virtual void set_obj					(	bGenericGeoElement* o);
	virtual bGenericGeoElement*	get_obj		(	);
	
	virtual void set_use_track				(	bool b);
	virtual bool get_use_track				(	);
	
	virtual void set_use_join				(	bool b);
	virtual bool get_use_join				(	);
	
	virtual void set_use_locs				(	bool b);
	virtual bool get_use_locs				(	);
	virtual bArray* get_locs				(	);

// Angle		
	virtual void set_use_angle				(	bool b);
	virtual bool get_use_angle				(	);
	virtual void set_angle					(	double	d);
	virtual double get_angle				(	);

	virtual void set_use_sel				(	bool b);
	virtual bool get_use_sel				(	);
	
//		
	virtual void set_limit					(	double	d);
	virtual double get_limit				(	);
	virtual double get_scaled_limit			(	);
	
	virtual void get_last					(	CGPoint* loc);		
	virtual void get_last					(	i2dvertex* loc);
	virtual void set_last					(	CGPoint* loc);		
	virtual void set_last					(	i2dvertex* loc);
	virtual void reset_last					(	);

	virtual void get_cur					(	CGPoint* loc);		
	virtual void get_cur					(	i2dvertex* loc);
	virtual void set_cur					(	CGPoint* loc);		
	virtual void set_cur					(	i2dvertex* loc);
	virtual void reset_cur					(	);
	virtual void constraint_cur				(	);
	
	virtual void get_clic					(	CGPoint* loc);		
	virtual void get_clic					(	i2dvertex* loc);
	virtual void set_clic					(	CGPoint* loc);		
	virtual void set_clic					(	i2dvertex* loc);
	virtual void reset_clic					(	);
	
	virtual void push						(	);
	
	virtual void draw_rect					(	);
	virtual void draw_c_rect				(	);
	virtual void draw_circle				(	);
	virtual void draw_c_circle				(	);
	virtual void draw_poly					(	bool use_handles,
												bool addLast);
	virtual void draw_poly					(	ivertices* vxs,
												bool use_handles);

	virtual void track_obj					(	);
	virtual void track_draw					(	);
	virtual void track_join					(	);
	virtual void action						(	int count);

/*> A supprimer */
	virtual bool edit_event					(	EventRef evt,		//CC
												WindowRef wd);		//CC
	virtual void edit_init					(	WindowRef wd);		//CC
/*< A supprimer */
	
	virtual bGenericXMLBaseElement* load	(	);
	virtual void save						(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);
	virtual bGenericXMLBaseElement* 
			get_param						(	bGenericXMLBaseElement* root, 
												const char* cfg_name);

	virtual void hilite_oval				(	CGContextRef ctx, 
												CGRect r);
	virtual void hilite_rect				(	CGContextRef ctx, 
												CGRect r,
												bool paint, 
												bool frame);
	virtual void hilite_node				(	CGContextRef ctx, 
												CGPoint pt);
	virtual void hilite_cross				(	CGContextRef ctx, 
												CGPoint pt);

	virtual void pop_tools					(	CGPoint	location,
												WindowRef inWindow);


	virtual CGContextRef getTempPathContext	(	);
	virtual CGRect getTempPathContextRect	(	);
	virtual void clearTempPathContext		(	bool utrc);
	virtual void validTempPathContext		(	);
	virtual void set_use_trace				(	char val){_use_trace=val;};
    virtual void setCursorLock				(	bool val){_curs_lock=val;};

protected:

/*> A supprimer */
	pascal static OSStatus edit_evt_hdlr	(	EventHandlerCallRef hdlr, //CC
												EventRef evt, 
												void *up);
/*< A supprimer */

private:
	CursorRef			_ccurs;		
	CursorRef			_curs;

/*> A supprimer */
	WindowRef			_edit_wd;	//CC
/*< A supprimer */
	
	int					_clic_count;
	
	int					_mnu_cmd;
	
	int					_key;
	int					_mod;
	
	bool				_on;
	bool				_on_drag;
	bool				_on_edit;
	bool				_use_drag;
	bool				_use_track;
	bool				_use_join;

	bool				_use_angle;
	bool				_use_sel;
	bool				_use_locs;
	bool				_reset_locs;
	
	double				_limit;
	double				_angle;
	
	i2dvertex			_last_loc;
	i2dvertex			_cur_loc;
	i2dvertex			_clic_loc;
	
	bArray*				_locs;

	bGenericGeoElement*	_o;		
	EventMouseButton	_btn;	//CC
	
	char				_use_trace;
	static bool			_curs_lock;

protected:
	bGraphicParams*		_gprm;
};

//----------------------------------------------------------------------------

#endif
