//----------------------------------------------------------------------------
// File : bBrowserSortValue.h
// Project : MacMap
// Purpose : Header file : Value classes for sort in browser context
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
// 5/10/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bBrowserSortValue__
#define __bBrowserSortValue__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bStdWait.h>

//----------------------------------------------------------------------------

class bBrowserSortValue{
public:		
	bBrowserSortValue				(	int ord);
	virtual ~bBrowserSortValue		(	);
	
	virtual int compare				(	bBrowserSortValue* b);
	
	virtual void dump				(	);

protected:
	int	_ord;
	
private:
};

//----------------------------------------------------------------------------

class bBrowserDoubleSortValue : public bBrowserSortValue{
public:		
	bBrowserDoubleSortValue					(	int ord,
												double val);
	virtual ~bBrowserDoubleSortValue		(	);
	
	virtual int compare						(	bBrowserSortValue* b);

	virtual void dump						(	);

protected:
	
private:
	double	_val;
};

//----------------------------------------------------------------------------

class bBrowserCharSortValue : public bBrowserSortValue{
public:		
	bBrowserCharSortValue					(	int ord,
												char* val);
	virtual ~bBrowserCharSortValue			(	);
	
	virtual int compare						(	bBrowserSortValue* b);

	virtual void dump						(	);

protected:
	
private:
	char*	_val;
};

//----------------------------------------------------------------------------

class bBrowserStackValue{
public:		
	bBrowserStackValue				(	int dim);
	virtual ~bBrowserStackValue		(	);
	
	virtual void put_ref			(	void* ref);
	virtual void* get_ref			(	);
	
	virtual void put				(	int idx,
										bBrowserSortValue* val);
	
	static int compare				(	const void* a,
										const void* b);
										
	static void put_wt				(	bStdWait* wt);

	void dump						(	);
protected:
	
private:
	static int			_i,_r,_n,_prg;
	static bStdWait*	_wt;
	bBrowserSortValue**	_values;
	void*				_ref;
};

//----------------------------------------------------------------------------

#endif
