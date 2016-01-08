//----------------------------------------------------------------------------
// File : bXMapCloneDetector.h
// Project : MacMap
// Purpose : Header file : XMap classe, clone detection
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
// 21/06/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapCloneDetector__
#define __bXMapCloneDetector__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define kXMapCloneFoundMessageID	"CloneFound"
#define kXMapNoCloneFoundMessageID	"NoCloneFound"

enum{	
	kXMapCloneDetectorSignature		='ClnD',
	kXMapCloneDetectorNameID		=1,
	kXMapCloneDetectorSubTypeID		=2,
	kXMapCloneDetectorColorID		=3,
	kXMapCloneDetectorDirectionID	=4,
	kXMapCloneDetectorGeomID		=5,
	kXMapCloneDetectorUserFieldsID	=6
};

//----------------------------------------------------------------------------

class bXMapCloneDetector : public bStdXMapWithModalIntf{
	public:		
		bXMapCloneDetector						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapCloneDetector				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
			
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		virtual bool detect						(	clonedetector_prm* prm);
		virtual bool obj_comp					(	bGenericGeoElement* oa, 
													bGenericGeoElement* ob, 
													clonedetector_prm* prm);
						
	private:		
		clonedetector_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
