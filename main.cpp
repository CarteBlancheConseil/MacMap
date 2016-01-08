//----------------------------------------------------------------------------
// File : main.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap main proc
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
// 01/01/2002 creation.
//----------------------------------------------------------------------------

#include "bMacMapApp.h"
#include "unistd.h"

#include <MacMapSuite/bTrace.h>

#include <sys/time.h>
#include <sys/resource.h>

// ---------------------------------------------------------------------------
// 
// ------------ 
static int	app_main(void){
struct rlimit lim;
lim.rlim_cur=4096;
lim.rlim_max=4096;
setrlimit(RLIMIT_NOFILE,&lim);

_USE_STDERR_=_DEVELOPMENT_VERS_;
//_USE_STDERR_=0;
//_USE_STDERR_=1;
bMacMapApp*	app=(bMacMapApp*)bMacMapApp::instance("MacMap®");
OSStatus	status=app->run();
	delete app;
//#warning PROBLEME RAINBOW EVE3 2.4.0.2 SUR PPC OU INTEL SOUS ROSETTA
//	_exit(0);
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
__attribute__((visibility("default"))) int main(int argc, char** argv){
	return(app_main());
}


