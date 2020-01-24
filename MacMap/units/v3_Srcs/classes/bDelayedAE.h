//----------------------------------------------------------------------------
// File : bDelayedAE.h
// Project : MacMap
// Purpose : Header file : System events management (close, quit, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 28/06/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bDelayedAE__
#define __bDelayedAE__

//----------------------------------------------------------------------------

#include "bMacMapApp.h"

//----------------------------------------------------------------------------

class bDelayedAE{
public:		
	bDelayedAE					(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAE			(	);
	
	virtual OSStatus exec		(	AppleEvent* out_ae);
	virtual OSStatus exec		(	);

protected:
	OSStatus	_status;
	bMacMapApp*	_zapp;
	
private:
};

//----------------------------------------------------------------------------

/*class bDelayedAEOpen : public bDelayedAE{
public:		
	bDelayedAEOpen				(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAEOpen		(	);
	
	virtual OSStatus exec		(	AppleEvent* out_ae);
	virtual OSStatus exec		(	);

protected:

private:
	FSSpec	_spec;
};*/

//----------------------------------------------------------------------------

class bDelayedAEOpenWithPath : public bDelayedAE{
public:		
	bDelayedAEOpenWithPath			(	const AppleEvent* ae,
										bMacMapApp* zapp);
	virtual ~bDelayedAEOpenWithPath	(	);
	
	virtual OSStatus exec			(	AppleEvent* out_ae);
	virtual OSStatus exec			(	);
	
protected:
	
private:
	char _path[PATH_MAX];
};

//----------------------------------------------------------------------------

class bDelayedAEClose : public bDelayedAE{
public:		
	bDelayedAEClose				(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAEClose	(	);
	
	virtual OSStatus exec		(	AppleEvent* out_ae);
	virtual OSStatus exec		(	);

protected:

private:
};

//----------------------------------------------------------------------------

class bDelayedAEQuit : public bDelayedAE{
public:		
	bDelayedAEQuit				(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAEQuit		(	);
	
	virtual OSStatus exec		(	AppleEvent* out_ae);
	virtual OSStatus exec		(	);

protected:

private:
};

//----------------------------------------------------------------------------

class bDelayedAECmd : public bDelayedAE{
public:		
	bDelayedAECmd				(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAECmd		(	);
	
	virtual OSStatus exec		(	AppleEvent* out_ae);
	virtual OSStatus exec		(	);

protected:

private:
	UInt32	_cmd;
};

//----------------------------------------------------------------------------

class bDelayedAECmdParam : public bDelayedAECmd{
public:		
	bDelayedAECmdParam			(	const AppleEvent* ae,
									bMacMapApp* zapp);
	virtual ~bDelayedAECmdParam	(	);
	
protected:

private:
};

//----------------------------------------------------------------------------

#endif
