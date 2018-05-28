//----------------------------------------------------------------------------
// File : bGraphicParams.h
// Project : MacMap
// Purpose : Header file : Graphic params storage class. Used for tool trace, selection and contrasts drawing.
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
// 20/03/2014 creation.
//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

#define kStdGraphicParamsSelectionName	"selection"
#define kStdGraphicParamsContrastesName	"contrastes"
#define kStdGraphicParamsToolTraceName	"tooltrace"

//----------------------------------------------------------------------------

#ifndef __bGraphicParams__
#define __bGraphicParams__

//----------------------------------------------------------------------------

class bGraphicParams{
public:
	bGraphicParams								(	const char* name,
													bGenericMacMapApp* gapp);
	bGraphicParams								(	bGenericXMLBaseElement* root, 
													bGenericMacMapApp* gapp);

	virtual ~bGraphicParams						(	);

	virtual bool update							(	bArray& arr, 
													long indent);
	virtual bool update							(	const char* name,
													bGenericMacMapApp* gapp);

	virtual void apply							(	CGContextRef ctx);
	
	virtual int	status							(	){return(_status);};
	
	virtual CGFloat	width						(	){return(_width);};
	virtual bool fill							(	){return(_fill[0]>0);};
	virtual bool stroke							(	){return(_stroke[0]>0);};
	virtual CGPathDrawingMode pathDrawingMode	(	);

	virtual void setWidth						(	float w);
	virtual void setStroke						(	const float* clr);
	virtual void setFill						(	const float* clr);

protected:
	virtual void parse							(	bGenericXMLBaseElement* root, 
													bGenericMacMapApp* gapp);
	int		_status;
	CGFloat	_width;
	CGFloat	_stroke[4];
	CGFloat _fill[4];
	
private:
	
};

//----------------------------------------------------------------------------

#endif
