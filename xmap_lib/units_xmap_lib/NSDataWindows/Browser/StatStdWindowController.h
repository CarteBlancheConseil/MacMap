//----------------------------------------------------------------------------
// File : StatStdWindowController.h
// Project : MacMap
// Purpose : Header file : Window controller classes for statistical panels
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
// 02/06/2014 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bArray.h>
#import <MacMapStats/MMsAnalysis.h>
#import "BrowserWindowController.h"
#import "StatBrowserView.h"
#import "MatrixBrowserView.h"
#import "GraphView.h"

//----------------------------------------------------------------------------

@interface StatStdPanel : BrowserPanel{
	
}

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

@interface StatStdWindowController : BrowserWindowController{
	IBOutlet NSButton*	_sav;
	IBOutlet NSButton*	_cpt;
	IBOutlet NSTabView*	_tab;
	mmx_analysis*		_anap;
	MMsPath*			_lpath;	// Analysis read path
}

//----------------------------------------------------------------------------
// 
//+(NSString*)fileSignature;							// Need to subclass

//----------------------------------------------------------------------------
// Actions
-(IBAction)doSaveAs:(id)sender;
-(IBAction)doCompute:(id)sender;// Need to subclass

//----------------------------------------------------------------------------
// Gestion Interface
-(void)saveAsPanelDidEnd:(NSSavePanel *)sheet 
			  returnCode:(int)returnCode 
			 contextInfo:(void *)contextInfo;

-(void)updateUI;									// Need to subclass

-(BOOL)setExt:(bStdNSXMap*)ext
	   atPath:(const char*)path;
-(long)analysisType;								// Need to subclass
-(NSString*)analysisExt;							// Need to subclass
-(BOOL)saveToPath:(const char*)fpath				// Need to subclass
			 name:(const char*)fname;
-(BOOL)loadFromPath:(const char*)fpath				// Need to subclass
			   name:(const char*)fname
				app:(bGenericMacMapApp*)gapp;
-(void)cleanupSheets;
-(void)copyColumns;
-(BOOL)recoverColumn:(const char*)name;
-(BOOL)recoverPanel:(bGenericXMLBaseElement*)root; // Need to subclass
-(void)initBrowser:(NSString*)rsrc
			bundle:(NSBundle*)bndl
			 label:(NSString*)lbl
			matrix:(MMsMatrix*)mmx;
-(void)initGraph:(NSString*)rsrc
		  bundle:(NSBundle*)bndl
		   label:(NSString*)lbl
		  matrix:(MMsMatrix*)mmx;
-(void)initGraphExtra:(GraphView*)grv;
-(MMsPath*)loadPath;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
