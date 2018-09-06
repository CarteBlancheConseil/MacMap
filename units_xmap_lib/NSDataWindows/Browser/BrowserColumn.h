//----------------------------------------------------------------------------
// File : BrowserColumn.h
// Project : MacMap
// Purpose : Header file : Browser column classes
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
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#import <AppKit/AppKit.h>
#import <mox_intf/bGenericMacMapApp.h>
#import <mox_intf/bGenericGeoElement.h>
#import <mox_intf/bGenericType.h>
#import <mox_intf/bGenericCalc.h>
#import <mox_intf/ext_utils.h>
#import <MacMapStats/MacMapStats.h>

//----------------------------------------------------------------------------
enum{
	kBrowserPanelContentSignature					='Brow',
	
	kStatBrowserPanelContentSignature				='SBrw',
	kStatBrowserCoordinatePanelContentSignature		='SBCd',
	kStatBrowserContributionPanelContentSignature	='SBCn',
	kStatBrowserDistancePanelContentSignature		='SBDt',
	kStatBrowserCorrelationPanelContentSignature	='SBCr',
	kStatBrowserCovariancePanelContentSignature		='SBCv',
	kStatBrowserIndicatorPanelContentSignature		='SBIn',
	kStatBrowserFactorPanelContentSignature			='SBFc',
	kStatBrowserCenterPanelContentSignature			='SBCt',
	
	kStatGraphHistogramPanelContentSignature		='SGHs',
	kStatGraphDistribDiagramPanelContentSignature	='SGDd',
	kStatGraphMappingPanelContentSignature			='SGMa',
	kStatGraphScatterPlotPanelContentSignature		='SGSp',
	kStatGraphScatterPlotIRAPanelContentSignature	='SGSi',
	kStatGraphDendrogramPanelContentSignature		='SGDn',
	
	kStatVisualMatrixPanelContentSignature			='SVMx'
	
};

enum{
	kNoSortNum		=0,
	kSortNum		=1,
	kSortAlpha		=2,
	kSortAlphaNum	=3
};

#define	kBrowserMessageID "browser"

//----------------------------------------------------------------------------
// Base classe
//----------------------------------------------------------------------------
@interface BrowserColumn : NSObject{
	NSRect	_frame;
	BOOL	_sel;
	int		_srt;
	int		_ord;
}

//----------------------------------------------------------------------------

+(void)frameHeader:(NSRect)rect;
+(void)setHeaderBackground;
+(void)setHiliteColor;
+(void)setHeaderHiliteColor;
+(CGFloat)stdLineHeight;
+(CGFloat)stdColumnWidth;
+(NSMutableDictionary*)stringAttributes;
+(CGFloat)stdFontSize;
+(NSString*)stdNSFont;


-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	   application:(bGenericMacMapApp*)gapp;

-(void)setSelected:(BOOL)flg 
		   inArray:(bArray*)cells;
-(BOOL)selected;
-(void)setLineSelected:(long)lin 
			   inArray:(bArray*)lines 
				status:(BOOL)flg;
-(BOOL)lineSelected:(long)lin
			inArray:(bArray*)lines;

-(void)setFrame:(NSRect)frame;
-(NSRect)frame;

-(void)name:(char*)buffer 
	 forLine:(long)lin 
	 inArray:(bArray*)arr;
-(void)value:(char*)buffer 
	 forLine:(long)lin 
	 inArray:(bArray*)arr;
-(double)value:(long)lin 
	 inArray:(bArray*)arr;
-(void*)ref:(long)lin
       inArray:(bArray*)arr;

-(void)name:(char*)buffer;
-(long)kind;
-(long)len;
-(long)decs;
-(long)colKind;

-(long)sortKind;
-(void)setSortKind:(long)sort;
-(long)sortOrder;
-(void)setSortOrder:(long)order;

-(void)drawHeader:(NSRect)rect 
		   offset:(CGFloat)off;
-(void)drawRight:(NSRect)rect;

-(void)drawLine:(long)lin 
		inArray:(bArray*)arr
		   rect:(NSRect)frame;

// Managing mouse clic
-(BOOL)mouse:(NSView*)view 
		line:(long)lin 
	 inArray:(bArray*)arr;
-(BOOL)rightMouse:(NSView*)view		
			 line:(long)lin 
		  inArray:(bArray*)arr;
// Managing escape
-(BOOL)abort:(BOOL)closing;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Field column classe
//----------------------------------------------------------------------------
@interface BrowserFieldColumn : BrowserColumn{
	long			_field;
	bGenericType*	_gtp;
}

//----------------------------------------------------------------------------

-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
			field:(long)fld
			 type:(bGenericType*)typ;

-(void)value:(char*)buffer 
	 forLine:(long)lin 
	   inArray:(bArray*)arr;

-(void)name:(char*)buffer;
-(long)kind;
-(long)len;
-(long)decs;
-(long)colKind;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Static text Field column classe
//----------------------------------------------------------------------------
@interface BrowserFieldStaticColumn : BrowserFieldColumn{
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Check Field column classe
//----------------------------------------------------------------------------
@interface BrowserFieldCheckColumn : BrowserFieldColumn{
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Popup menu Field column classe
//----------------------------------------------------------------------------
@interface BrowserFieldPopupColumn : BrowserFieldColumn{
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// EditText Field column classe
//----------------------------------------------------------------------------
@interface BrowserFieldEditTextColumn : BrowserFieldColumn{
	NSTextField*		_fld;
	bGenericGeoElement* _geo;
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Calc column classe
//----------------------------------------------------------------------------
@interface BrowserCalcColumn : BrowserColumn{
	bGenericCalc*	_calc;
}

//----------------------------------------------------------------------------

-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
			calc:(bGenericCalc*)clc;

-(void)value:(char*)buffer 
	 forLine:(long)lin 
	 inArray:(bArray*)arr;

-(void)name:(char*)buffer;
-(long)kind;
-(long)len;
-(long)decs;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Matrix column classe
//----------------------------------------------------------------------------
@interface BrowserMatrixColumn : BrowserColumn{
	MMsMatrix*	_mmx;
	long		_idx;
	char*		_name;
}

//----------------------------------------------------------------------------

-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
		   matrix:(MMsMatrix*)mmx
			index:(long)idx
			 name:(char*)nam;

-(void)value:(char*)buffer 
	 forLine:(long)lin 
	 inArray:(bArray*)arr;
-(void*)ref:(long)lin
    inArray:(bArray*)arr;
-(void)name:(char*)buffer;
-(long)kind;
-(long)len;
-(long)decs;
-(long)colKind;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end








