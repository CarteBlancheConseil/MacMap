//----------------------------------------------------------------------------
// File : bGenericStatusIntf.h
// Project : MacMap
// Purpose : Header file : Root class for status window management (or every kind of status interface)
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
// 17/01/2005 creation.
//----------------------------------------------------------------------------
// 08/04/2014 Création et remplacement de bGenericStatusWind
//----------------------------------------------------------------------------

#ifndef __bGenericStatusIntf__
#define __bGenericStatusIntf__

//----------------------------------------------------------------------------

class bGenericStatusIntf{
public:
	virtual ~bGenericStatusIntf	(	){};	
	virtual void init			(	)=0;
	virtual void writeMessage	(	const char *msg)=0;
	virtual void writeScale		(	const char *msg)=0;
	virtual float height		(	)=0;
};

//----------------------------------------------------------------------------

#endif
