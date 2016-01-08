//----------------------------------------------------------------------------
// File : GraphView.h
// Project : MacMap
// Purpose : Header file : Base graph view classes
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
// 27/05/2014 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <std_ext/bStdNSXMap.h>
#import <MacMapStats/MMsAnalysis.h>
#import <MacMapStats/MMsClassification.h>
#import <xmap_lib/BrowserColumn.h>
#import <xmap_lib/BrowserView.h>

// ---------------------------------------------------------------------------
#define _MARGIN_				5
#define	_LEFTSPACE_				100
#define	_BOTTOMSPACE_			25
#define	_DEFAULTSTROKEWIDTH_	0.5
#define	_DEFAULTSTROKECOLOR_	[NSColor blackColor]

//----------------------------------------------------------------------------
@interface GraphObjView : NSView{
	bStdNSXMap*				_ext;
	bGenericType*			_typ;
	bArray*					_lines;
	long					_grpf;
	long					_a1;
	
	BrowserHeaderView*		_shd;
	mmx_analysis*			_ana;
	MMsMatrix*				_mmx;
	
	dvx_rect				_vbnds;
	double					_rx;
	double					_ry;
	NSPoint					_start;
	NSPoint					_end;
	BOOL					_onDrag;

	IBOutlet NSPopUpButton*	_mnu;
}

//----------------------------------------------------------------------------
+(void)setHiliteFill;
+(void)setHiliteStroke;
-(void)putExt:(bStdNSXMap*)ext;
-(void)putBounds;
-(void)putAnalysis:(mmx_analysis*)ana;
-(void)putMatrix:(MMsMatrix*)mmx;
-(void)putAxis:(long)x 
		 yAxis:(long)y;
-(void)putHCAClass:(hca_clss**)clss
			 count:(long)n;
-(void)putRoot:(bGenericXMLBaseElement*)root;
-(void)putLines:(bArray*)lines;
-(void)putSource:(BrowserHeaderView*)shd;
-(void)putGroupField:(long)fld;

-(bStdNSXMap*)ext;
-(bGenericMacMapApp*)app;
-(bGenericType*)type;
-(bArray*)lines;
-(long)groupField;

-(void)getParamsInfos:(char*)directory 
				 name:(char*)prmName;
-(void)getPanelInfos:(char*)name
		  signature:(char*)sign;
-(NSRect)objRect;

-(void)read_p;
-(void)write_p;
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname;
-(bGenericXMLBaseElement*)make_p;

-(double)zoomXFactor;
-(double)zoomYFactor;

-(void)zoomIn;
-(void)zoomOut;

-(void)drawExtra:(NSRect)rect;
-(void)drawFields:(NSRect)rect;
-(void)drawObjects:(NSRect)rect;
-(void)drawSelect:(NSRect)rect;
-(void)drawPath:(NSRect)rect;

-(void)updateUI;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface HistogramGraphObjView : GraphObjView{
	bArray*	_objects;
	
	double	_hr;
	double	_hg;
	double	_hb;
	double	_ha;
	
	double	_lr;
	double	_lg;
	double	_lb;
	double	_la;
	
	double	_sr;
	double	_sg;
	double	_sb;
	double	_sa;
	
	double	_mid;
	double	_min;
	double	_max;
	
	BOOL	_saved;
	
	IBOutlet NSPanel*		_dwd;
	IBOutlet NSTextField*	_dnc;
	IBOutlet NSPopUpButton*	_dmt;

	IBOutlet NSPanel*		_pwd;
	IBOutlet NSColorWell*	_phc;
	IBOutlet NSColorWell*	_plc;
	IBOutlet NSColorWell*	_psc;
	IBOutlet NSTextField*	_pmd;
	IBOutlet NSTextField*	_pmn;
	IBOutlet NSTextField*	_pmx;
}

//----------------------------------------------------------------------------
-(void)putClass;

//----------------------------------------------------------------------------
-(IBAction)doStartPresentation:(id)sender;
-(IBAction)doPresentationOk:(id)sender;
-(IBAction)doPresentationCancel:(id)sender;

-(IBAction)doStartDiscretization:(id)sender;
-(IBAction)doDiscretizationOk:(id)sender;
-(IBAction)doDiscretizationCancel:(id)sender;
-(IBAction)doKillDiscretization:(id)sender;

//----------------------------------------------------------------------------
@end



//----------------------------------------------------------------------------
@interface RepDiagramGraphObjView : GraphObjView{	
	double	_r;
	double	_g;
	double	_b;
	double	_a;
		
	IBOutlet NSPanel*		_pwd;
	IBOutlet NSColorWell*	_pcl;
}

//----------------------------------------------------------------------------
-(IBAction)doStartPresentation:(id)sender;
-(IBAction)doPresentationOk:(id)sender;
-(IBAction)doPresentationCancel:(id)sender;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface GraphView : NSView{
	IBOutlet GraphObjView*	_obj;
	
	IBOutlet NSPopUpButton*	_mnu;
	IBOutlet NSButton*		_zmi;
	IBOutlet NSButton*		_zmo;

	NSSavePanel*			_sav;
}

//----------------------------------------------------------------------------
-(void)putExt:(bStdNSXMap*)ext;
-(void)putAnalysis:(mmx_analysis*)ana;
-(void)putMatrix:(MMsMatrix*)mmx;
-(void)putAxis:(long)x 
		 yAxis:(long)y;
-(void)putHCAClass:(hca_clss**)clss
			 count:(long)n;
-(void)putRoot:(bGenericXMLBaseElement*)root;
-(void)putLines:(bArray*)lines;
-(void)putSource:(BrowserHeaderView*)shd;
-(bGenericType*)type;

-(void)putBounds;
-(void)save;
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname;

//----------------------------------------------------------------------------
-(IBAction)doPublish:(id)sender;
-(IBAction)doSelectAll:(id)sender;
-(IBAction)doChooseGroupField:(id)sender;
-(IBAction)doZoomIn:(id)sender;
-(IBAction)doZoomOut:(id)sender;

//----------------------------------------------------------------------------
-(void)updateUI;

//----------------------------------------------------------------------------
@end

//----------------------------------------------------------------------------
