//----------------------------------------------------------------------------
// File : bXMapDataFillRun.h
// Project : MacMap
// Purpose : Header file : Utility classes, for datafill XMap
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
// 17/03/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapDataFillRun__
#define __bXMapDataFillRun__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------

class bXMapDataFillRun{
public:		
	bXMapDataFillRun			(	char* text,
									int fld,
									int	sz,
									int kind,
									int decs);
	virtual ~bXMapDataFillRun	(	);

	virtual bool apply			(	bGenericGeoElement* o);
	
	virtual int get_id()		{	return(_id);};
	virtual char* get_text()	{	return(_text);};

protected:
	int			_id;
	int			_dst;
	int			_kind;
	int			_decs;
	void*		_value;
	static int	_lid;
	char		_text[512];
};

//----------------------------------------------------------------------------

class bXMapDataFillRunValue : public bXMapDataFillRun{
public:		
	bXMapDataFillRunValue			(	char* text,
										int fld,
										int	sz,
										int kind,
										int decs,
										char* value);
	virtual ~bXMapDataFillRunValue	(	);

	virtual bool apply				(	bGenericGeoElement* o);

private:
	bool	_good;
};

//----------------------------------------------------------------------------

class bXMapDataFillRunCalc : public bXMapDataFillRun{
public:		
	bXMapDataFillRunCalc			(	char* text,
										int fld,
										int	sz,
										int kind,
										int decs,
										bGenericCalc* clc);
	virtual ~bXMapDataFillRunCalc	(	);

	virtual bool apply				(	bGenericGeoElement* o);

private:
	bGenericCalc*	_src;
};

//----------------------------------------------------------------------------

class bXMapDataFillRunField : public bXMapDataFillRun{
public:		
	bXMapDataFillRunField			(	char* text,
										int fld,
										int	sz,
										int kind,
										int decs,
										int	src,
										int	srcsz,
										int srckind,
										int srcdecs);
	virtual ~bXMapDataFillRunField	(	);

	virtual bool apply				(	bGenericGeoElement* o);

private:
	int		_src;
	int		_srckind;
	int		_srcdecs;
	void*	_srcvalue;
};

//----------------------------------------------------------------------------

class bXMapDataFillRunAskValue : public bXMapDataFillRun{
public:		
	bXMapDataFillRunAskValue			(	char* text,
											int fld,
											int	sz,
											int kind,
											int decs,
											bGenericType* tp);
	virtual ~bXMapDataFillRunAskValue	(	);
	
	virtual bool apply					(	bGenericGeoElement* o);
	
private:
	bool	_good;
};

//----------------------------------------------------------------------------

#endif
