//----------------------------------------------------------------------------
// File : bEventLog.cpp
// Project : MacMap
// Purpose : C++ source file : Event creation utility class 
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
// 02/10/2003 creation.
//----------------------------------------------------------------------------

#include "bEventLog.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bEventLog::bEventLog(	bGenericMacMapApp*	gapp,
						CFBundleRef bndl,
						const char* msg_id,
						int BigEndianCreator){
	_gapp=gapp;
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(msg_id,msg,bndl,1);
	_gapp->eventMgr()->init_log(BigEndianCreator,msg);
	_closed=false;
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bEventLog::bEventLog(bGenericMacMapApp*	gapp,
                     bGenericExt*	ext){
    _gapp=gapp;
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
    GetName(ext,msg);
    _gapp->eventMgr()->init_log(GetSignature(ext),msg);
    _closed=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bEventLog::~bEventLog(){
	if(_closed){
		_gapp->eventMgr()->close_log();
	}
	else{
		_gapp->eventMgr()->reset_log();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bEventLog::close(){
	_closed=true;
}
