//----------------------------------------------------------------------------
// File : bStdXMLConstant.h
// Project : MacMap
// Purpose : Header file : Base class for constant formula
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
// 17/03/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStdXMLConstant__
#define __bStdXMLConstant__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMLFormulaElement.h>
#include <mox_intf/bStdSolver.h>

//----------------------------------------------------------------------------

class bStdXMLConstant : public bStdXMLFormulaElement{
public:
    bStdXMLConstant             (	bGenericXMLBaseElement* elt,
                                    bGenericMacMapApp* gapp,
                                    CFBundleRef bndl);
    virtual ~bStdXMLConstant	(	);
    
    virtual bool solve			(	bStdSolver* ctx);
    
protected:

};
									
//----------------------------------------------------------------------------

#endif
