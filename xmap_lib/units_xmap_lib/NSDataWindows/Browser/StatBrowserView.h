//----------------------------------------------------------------------------
// File : StatBrowserView.h
// Project : MacMap
// Purpose : Header file : Browser view classes for stat browsers
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
#import "BrowserView.h"

//----------------------------------------------------------------------------
@interface MMsPath : NSObject{
	char*	_fpath;
	char*	_fname;
}

//----------------------------------------------------------------------------
-(id)init:(const char*)fpath
	 name:(const char*)fname;
-(void)getPath:(char*)fpath
		  name:(char*)fname;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface StatBrowserHeaderView : BrowserHeaderView{
	NSMutableArray*	_acol;	// Analysis columns
}

//----------------------------------------------------------------------------
-(BrowserColumn*)allocator:(bArray*)lins
					 field:(int)fx
					  calc:(void*)cx
					 start:(float)strt
					 width:(float)w;
-(void)getPanelInfos:(char*)name
		   signature:(char*)sign;
-(MMsPath*)loadPath;
-(NSMutableArray*)analysisColumns;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
@interface StatBrowserView : BrowserView{
	
}

//----------------------------------------------------------------------------
-(id)initWithFrame:(NSRect)frameRect;
-(MMsMatrix*)matrix;
-(MMsMatrix*)onlySelectedMatrix;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
