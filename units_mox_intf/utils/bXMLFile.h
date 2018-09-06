//----------------------------------------------------------------------------
// File : bXMLFile.h
// Project : MacMap
// Purpose : Header file : xml file class (parse xml for MacMap)
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
// 08/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMLFile__
#define __bXMLFile__

//----------------------------------------------------------------------------

#include <MacMapSuite/bStdFile.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericClassesMgr.h>

//----------------------------------------------------------------------------

class bXMLFile : public bStdFile{
public:
    bXMLFile			(const char* path,
                         const char* perm);
    virtual ~bXMLFile	(	);
    
    virtual void mount	(char** buffer,
                         int* sz);
    virtual void mount	(bGenericClassesMgr* mgr,
                         bGenericXMLBaseElement** root);
protected:
private:
};

//----------------------------------------------------------------------------

#endif
