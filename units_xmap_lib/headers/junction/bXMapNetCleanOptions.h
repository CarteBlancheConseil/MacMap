//----------------------------------------------------------------------------
// File : bXMapNetCleanOptions.h
// Project : MacMap
// Purpose : Header file : XMap class, network cleaning options
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
// 08/11/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapNetCleanOptions__
#define __bXMapNetCleanOptions__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

enum{	
	kXMapNetCleanOptionsSignature			='NtCO',
	
	kXMapNetCleanOptionsNodesID				=10,
	kXMapNetCleanOptionsNodesCmd			='tNod',
	kXMapNetCleanOptionsEdgesID				=11,
	kXMapNetCleanOptionsEdgesCmd			='tLnk',

	kXMapNetCleanOptionsNodesCrTypeID		=20,
	kXMapNetCleanOptionsNodesCrTypeCmd		='tCrN',
	kXMapNetCleanOptionsNodesCrSubtypeID	=21,
	kXMapNetCleanOptionsNodesCrSubtypeCmd	='sCrN',
	kXMapNetCleanOptionsNodesCrNameID		=23,

	kXMapNetCleanOptionsBridgesTypeID		=40,
	kXMapNetCleanOptionsBridgesTypeCmd		='tEdg',
	kXMapNetCleanOptionsBridgesSubtypeID	=41,
	kXMapNetCleanOptionsBridgesSubtypeCmd	='sEdg',

	kXMapNetCleanOptionsDistNodeID			=31,
	kXMapNetCleanOptionsDistNodeUnitID		=32,
	kXMapNetCleanOptionsDistBridgeID		=34,
	kXMapNetCleanOptionsDistBridgeUnitID	=35,


	kXMapNetCleanOptionsAutoID				=50
};

//----------------------------------------------------------------------------

class bXMapNetCleanOptions : public bStdXMapWithModalIntf{
	public:		
		bXMapNetCleanOptions					(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapNetCleanOptions			(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		
	private:
		virtual void param_get					(	);
		virtual void param_put					(	);
		virtual void check_intf					(	UInt32 sign);
		
		
		netcleancfg_prm	_prm;

};

//----------------------------------------------------------------------------

#endif
