//----------------------------------------------------------------------------
// File : bStdWait.h
// Project : MacMap
// Purpose : Header file : Wait windows utility classes
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
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
// Attention, l'allocation d'un bStdWait est très couteuse !
//----------------------------------------------------------------------------
// 07/10/2003 creation.
// 03/01/2018 creation.
//----------------------------------------------------------------------------

#ifndef __bStdWait__
#define __bStdWait__

//----------------------------------------------------------------------------
// A reprendre en Cocoa

class bStdWait{
public:
    virtual ~bStdWait			(	);

    virtual bool set_progress   (	long index);// Set
    virtual long get_progress	(	);          // 0=stopped

protected:
    bStdWait					(	bool indeterminate,
                                    const char* title,
                                    const char* message,
                                    bool canbreak,
                                    bool show,
                                    long maximum);
    
    bool	_visible;
    bool	_show;
    bool    _canbreak;
    long	_index;
    long	_maximum;
    
    clock_t	_timer;
    
    void*   _ctrlr;
};

//----------------------------------------------------------------------------

class bProgressWait : public bStdWait{
public:
    bProgressWait			(	const char* title,
                                const char* message,
                                bool canbreak,
                                long maximum);
    virtual ~bProgressWait	(	);

protected:
private:
};

//----------------------------------------------------------------------------

class bBarberWait : public bStdWait{
public:
    bBarberWait					(	const char* title,
                                    const char* message,
                                    bool canbreak);
    virtual ~bBarberWait		(	);
    
    virtual bool set_progress	(	long index);

protected:
private:
};

//----------------------------------------------------------------------------

class bCursWait : public bStdWait{
public:
    bCursWait					(	bool canbreak);
    virtual ~bCursWait			(	);

    virtual bool set_progress	(	long index);

protected:
private:
};

//----------------------------------------------------------------------------

#endif
