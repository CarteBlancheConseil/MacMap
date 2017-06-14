//----------------------------------------------------------------------------
// File : bStdXMap.h
// Project : MacMap
// Purpose : Header file : Base class for XMaps
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdXMap__
#define __bStdXMap__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <mox_intf/bGenericXMap.h>

//----------------------------------------------------------------------------

#define	kStdXMapConfigSign	'xmap'

//----------------------------------------------------------------------------

class bStdXMap : public virtual bGenericXMap, public bStdExt {
public:		
	bStdXMap				(	bGenericXMLBaseElement* elt, 
								bGenericMacMapApp* gapp, 
								CFBundleRef bndl);
	virtual ~bStdXMap		(	);
			
	virtual void open		(	int* flags);
	virtual void close		(	);
/*> A supprimer */
	virtual bool event		(	EventRef evt);
/*> A supprimer */
	virtual bool nsevent	(	void* nsevt);
	virtual void idle		(	void* prm);
	virtual bool process	(	int msg, 
								void* prm);
	virtual bool test		(	void* prm);
	virtual bool edit		(	void* prm);
	
	virtual void set_flags	(	int flg);
	
protected:

private:
	int	_flgs;
};

//----------------------------------------------------------------------------

#endif
