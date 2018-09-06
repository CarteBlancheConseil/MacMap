//----------------------------------------------------------------------------
// File : bGenericTool.h
// Project : MacMap
// Purpose : Header file : Root class for tools
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
// 21/03/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericTool__
#define __bGenericTool__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExt.h>
#include <MacMapSuite/vx.h>

//----------------------------------------------------------------------------

enum{
//	kToolProcessMenu		=2,
	kToolProcessCocoaMenu	=3
};

enum{
	kToolNav			='tNav',
	kToolGeom			='tGeo',
	kToolPres			='tPrs'
};

typedef struct cocoaMenuPrm{
	CGPoint	location;
	void*	inWindow;
	bool	wdIsCocoa;
}cocoaMenuPrm;

//----------------------------------------------------------------------------

class bGenericTool : public virtual bGenericExt {
public:
	virtual ~bGenericTool		(	){};
	virtual int tool_class		(	)=0;
	virtual CGImageRef symb		(	)=0;
	virtual void activate		(	)=0;
	virtual void deactivate		(	)=0;
	virtual void update			(	bool global)=0;
	
	virtual void get_last		(	CGPoint* loc)=0;		
	virtual void get_last		(	i2dvertex* loc)=0;
	virtual void get_cur		(	CGPoint* loc)=0;		
	virtual void get_cur		(	i2dvertex* loc)=0;
	virtual void get_clic		(	CGPoint* loc)=0;		
	virtual void get_clic		(	i2dvertex* loc)=0;

	virtual void setCursorLock	(	bool flag)=0;
};

//----------------------------------------------------------------------------

#endif
