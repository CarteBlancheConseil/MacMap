//----------------------------------------------------------------------------
// File : BrowserView.h
// Project : MacMap
// Purpose : Header file : Base browser view classes
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

#import <Cocoa/Cocoa.h>
#import <std_ext/bStdNSXMap.h>
#import "BrowserColumn.h"

// ---------------------------------------------------------------------------
#define kBrowserMsgEmptyCol	"empty_col"
#define kBrowserMsgDataLoad	"data_load"
#define kBrowserMsgSortPrgr	"sort_prog"

//----------------------------------------------------------------------------
@interface BrowserHeaderView : NSView{
	bStdNSXMap*			_ext;	// MacMap extern instance
	bGenericType*		_tp;	// Type for selection
	NSMutableArray*		_cols;	// Columns
	bArray*				_csels;	// Selected columns
	bArray*				_lines;	// Rows (MacMap objects)
	NSPoint				_o;		// Origin
	long				_cur;	// Resizing column index
	IBOutlet NSView*	_obj;	// ObjView
}

//----------------------------------------------------------------------------
-(BrowserColumn*)allocator:(bArray*)lins
					 field:(int)fx
					  calc:(bGenericCalc*)cx
					 start:(float)strt
					 width:(float)w;
	
-(void)putExt:(bStdNSXMap*)ext;
-(void)putLines:(bArray*)lines;
-(bStdNSXMap*)ext;
-(bGenericMacMapApp*)app;
-(bGenericType*)type;
-(NSMutableArray*)columns;
-(bArray*)selectedColumns;
-(bArray*)lines;

-(void)flushSelectedLines;

-(void)getParamsInfos:(char*)directory 
				 name:(char*)prmName;
-(void)getPanelInfos:(char*)name
		  signature:(char*)sign;

-(void)read_p;
-(void)write_p;
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname;
-(bGenericXMLBaseElement*)make_p;

-(void)initType;
-(void)resetType;

//---------------------------------------------------------------------------
// Columns parse utilitie 
//---------------------------------------------------------------------------
typedef struct col_p{
	char				nam[_names_length_max_];
	long				knd;
	long				fx;
	bGenericCalc*		cx;
	double				strt;
	double				h;
	NSMutableArray*		arr;
	bArray*				objs;
	bGenericType*		tp;
	bGenericExtMgr*		mgr;
	bGenericMacMapApp*	gapp;
	BrowserHeaderView*	x;
	bStdWait*			wt;
}col_p;

bool readProc(bGenericXMLBaseElement* elt,
			  void* prm,
			  int indent);
	
//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface BrowserLeftView : NSView{
	NSPoint						_o;
	IBOutlet BrowserHeaderView*	_hdr;
    IBOutlet NSView*            _obj;
}

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface BrowserNameLeftView : BrowserLeftView{
}

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface BrowserObjView : NSView{ //NSScrollView{ //NSView{
	IBOutlet BrowserHeaderView*	_hdr;
	IBOutlet BrowserLeftView*	_lft;
	long	_cidx;
}

//----------------------------------------------------------------------------
-(id)initWithFrame:(NSRect)frameRect;
-(void)dealloc;
-(BOOL)abort:(BOOL)closing;
-(void)putBounds;
-(void)endEdit;
-(void)updateField:(long)fieldId;
-(void)removeField:(long)fieldId;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface BrowserCornerView : NSView{
}

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface BrowserView : NSView{
	IBOutlet BrowserCornerView*	_crn;
	IBOutlet BrowserHeaderView*	_hdr;
	IBOutlet BrowserLeftView*	_lft;
	IBOutlet BrowserObjView*	_obj;
	
	IBOutlet NSPopUpButton*		_mnu;
	IBOutlet NSPopUpButton*		_fmt;
	IBOutlet NSPanel*			_srt;
	IBOutlet NSPopUpButton*		_col;
	IBOutlet NSPopUpButton*		_knd;
	IBOutlet NSPopUpButton*		_ord;

	NSSavePanel*				_sav;
}

//----------------------------------------------------------------------------
-(void)putExt:(bStdNSXMap*)ext;
-(void)putLines:(bArray*)lines;
-(bGenericMacMapApp*)app;
-(bGenericType*)type;
-(NSMutableArray*)columns;
-(bArray*)selectedColumns;
-(bArray*)lines;
-(BrowserHeaderView*)header;

-(void)putBounds;
-(void)save;
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname;
-(BOOL)abort;
-(BOOL)checkMacMapEvent;

//----------------------------------------------------------------------------
-(IBAction)doInsertColumn:(id)sender;
-(IBAction)doRemoveColumn:(id)sender;
-(IBAction)doSelectColumns:(id)sender;
-(IBAction)doResetColumns:(id)sender;
-(IBAction)doInsertObjects:(id)sender;
-(IBAction)doRemoveObjects:(id)sender;
-(IBAction)doSelectObjects:(id)sender;

-(IBAction)doStartSort:(id)sender;
-(IBAction)doChooseColumn:(id)sender;
-(IBAction)doChooseSortKind:(id)sender;
-(IBAction)doChooseSortOrder:(id)sender;
-(IBAction)doSortOk:(id)sender;
-(IBAction)doSortCancel:(id)sender;

-(IBAction)doExport:(id)sender;
-(IBAction)doPutFileType:(id)sender;

//----------------------------------------------------------------------------
-(void)updateUI;

//----------------------------------------------------------------------------

@end


//----------------------------------------------------------------------------
