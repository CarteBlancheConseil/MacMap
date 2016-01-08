//----------------------------------------------------------------------------
// File : bXMapTypeNew.h
// Project : MacMap
// Purpose : Header file : XMap class, type creation
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
// 22/03/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapTypeNew__
#define __bXMapTypeNew__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

enum{
	kXMapTypeNewConnPrmSignature	= 'conx',
	kXMapTypeNewConnPrmDataBaseID	= 3,
	kXMapTypeNewConnPrmHostID		= 4,
	kXMapTypeNewConnPrmPortID		= 5,
	kXMapTypeNewConnPrmUserID		= 6,
	kXMapTypeNewConnPrmPasswordID	= 7

};

#define	kXMapTypeNewSignature	'Tnew'
#define	kXMapTypeNewMessageID	"new type"

//----------------------------------------------------------------------------

class bXMapTypeNew : public bStdXMap{
public:
    bXMapTypeNew							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapTypeNew					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
    OSStatus choose_location				(	char* path, 
                                                char* name, 
                                                int* kind, 
                                                int* tbl,
                                                bool* atv);

    bool get_conn_prm						(	char* data);
    static OSStatus conn_evt				(	EventHandlerCallRef hdlr, 
                                                EventRef evt, 
                                                void *up);
    bool get_string							(	int cid, 
                                                char* str);
    void set_focus							(	int cid);

    WindowRef	_wd;
};

//----------------------------------------------------------------------------

#endif
