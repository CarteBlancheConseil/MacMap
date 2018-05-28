//----------------------------------------------------------------------------
// File : bStdAlert.h
// Project : MacMap
// Purpose : Header file : Alert windows utility classes
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
// 
//----------------------------------------------------------------------------
// 24/01/2005 creation.
// 02/01/2018 Cocoa porting.
//----------------------------------------------------------------------------

#ifndef __bStdAlert__
#define __bStdAlert__

//----------------------------------------------------------------------------
// A reprendre en Cocoa

class bStdAlert{
public:
    bStdAlert				(	int kind, 
                                int def_btn, 
                                bool use_cancel, 
                                const char* error_string, 
                                const char* explanation_string);
    bStdAlert				(	int kind, 
                                int def_btn, 
                                bool use_cancel, 
                                const char* error_string, 
                                const char* explanation_string,
                                bool silent);
    bStdAlert				(	int kind, 
                                int def_btn, 
                                bool use_cancel, 
                                const char* error_string, 
                                const char* explanation_string,
                                bool silent,
                                UInt32 delay);
    bStdAlert				(	int kind, 
                                int def_btn, 
                                bool use_cancel, 
                                const char* error_string, 
                                const char* explanation_string,
                                bool silent,
                                UInt32 delay,
                                const char* btn1, 
                                const char* btn2,
                                const char* btn3);	

    virtual~bStdAlert		(	);
    
    virtual bool result		(	);
    virtual int hit_button	(	){return _hit;};
    
protected:

private:
    bool	_res;
    int		_hit;

};

//----------------------------------------------------------------------------

class bAlertWarningYes : public bStdAlert{
public:
    bAlertWarningYes			(	const char* error_string, 
                                    const char* explanation_string);
    bAlertWarningYes			(	const char* error_string, 
                                    const char* explanation_string,
                                    bool silent);
    bAlertWarningYes			(	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay);
    bAlertWarningYes			(	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay,
                                    const char* btn1, 
                                    const char* btn2,
                                    const char* btn3);
    virtual ~bAlertWarningYes	(	);
    
protected:

private:
};

//----------------------------------------------------------------------------

class bAlertWarningNo : public bStdAlert{
public:
    bAlertWarningNo             (   const char* error_string,
                                    const char* explanation_string);
    bAlertWarningNo             (	const char* error_string,
                                    const char* explanation_string,
                                    bool silent);
    bAlertWarningNo             (	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay);
    bAlertWarningNo             (	const char* error_string,
                                    const char* explanation_string,
                                    bool silent,
                                    UInt32 delay,
                                    const char* btn1,
                                    const char* btn2,
                                    const char* btn3);
    virtual ~bAlertWarningNo    (	);
		
protected:
	
private:
};

//----------------------------------------------------------------------------

class bAlertStop : public bStdAlert{
public:
    bAlertStop			(	const char* error_string,
                            const char* explanation_string);
    bAlertStop			(	const char* error_string, 
                            const char* explanation_string,
                            bool silent);
    bAlertStop			(	const char* error_string, 
                            const char* explanation_string,
                            bool silent,
                            UInt32 delay);
    bAlertStop          (	const char* error_string,
                            const char* explanation_string,
                            bool silent,
                            UInt32 delay,
                            const char* btn1,
                            const char* btn2,
                            const char* btn3);
    virtual ~bAlertStop	(	);
		
protected:
	
private:
};

//----------------------------------------------------------------------------

#endif
