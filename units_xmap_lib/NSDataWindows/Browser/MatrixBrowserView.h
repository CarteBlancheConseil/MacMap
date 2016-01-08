//----------------------------------------------------------------------------
// File : MatrixBrowserView.h
// Project : MacMap
// Purpose : Header file : Browser view classes for browsers using matrix data
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
#import <MacMapStats/MMsAnalysis.h>
#import <std_ext/bStdNSXMap.h>
#import "BrowserColumn.h"
#import "BrowserView.h"

//----------------------------------------------------------------------------
@interface MatrixBrowserHeaderView : BrowserHeaderView{
	MMsMatrix*			_mmx;
	mmx_analysis*		_ana;
	BrowserHeaderView*	_shd;
}

//----------------------------------------------------------------------------
-(BrowserColumn*)allocator:(bArray*)lins
					 field:(int)fx
					  calc:(bGenericCalc*)cx
					 start:(float)strt
					 width:(float)w
					 label:(char*)str;

-(void)putSource:(BrowserHeaderView*)shd;
-(BrowserHeaderView*)sourceHeader;
-(void)putAnalysis:(mmx_analysis*)ana;
-(void)putMatrix:(MMsMatrix*)mmx;
-(MMsMatrix*)matrix;
-(void)name:(char*)buffer 
	atIndex:(long)idx;

-(void)read_p;
-(void)write_p;
-(bGenericXMLBaseElement*)make_p;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixBrowserNonGeoHeaderView : MatrixBrowserHeaderView{
}

//----------------------------------------------------------------------------
-(void)flushSelectedLines;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixBrowserIndicatorsHeaderView : MatrixBrowserNonGeoHeaderView{
}

//----------------------------------------------------------------------------
-(void)name:(char*)buffer 
	atIndex:(long)idx;
-(void)putLines:(bArray*)lines;
-(void)getPanelInfos:(char*)name
		   signature:(char*)sign;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixBrowserObjView : BrowserObjView{
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixBrowserNameNonGeoLeftView : BrowserNameLeftView{
}

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixBrowserView : BrowserView{
}

//----------------------------------------------------------------------------
-(void)putSource:(BrowserHeaderView*)shd;
-(void)putAnalysis:(mmx_analysis*)ana;
-(void)putMatrix:(MMsMatrix*)mmx;

-(IBAction)doBackDataFill:(id)sender;

-(void)updateUI;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface MatrixNonGeoBrowserView : MatrixBrowserView{
}

//----------------------------------------------------------------------------
-(IBAction)doSelectObjects:(id)sender;

-(void)updateUI;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------

