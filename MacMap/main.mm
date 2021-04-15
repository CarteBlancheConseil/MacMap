//----------------------------------------------------------------------------
// File : main.mm
// Project : MacMap
// Purpose : Objective C++ source file : MacMap main proc
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2019 Carte Blanche Conseil.
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
// 05/11/2018 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/MacMapSuiteC.h>

#include "mm_config.h"
#include "bMacMapApp.h"

#include <sys/time.h>
#include <sys/resource.h>

// ---------------------------------------------------------------------------
//
// -----------
int main(int argc, const char * argv[]) {
struct rlimit lim;
lim.rlim_cur=4096;
lim.rlim_max=4096;
setrlimit(RLIMIT_NOFILE,&lim);
_VERBOSE_=0;
_USE_STDERR_=_DEVELOPMENT_VERS_;
_USE_STDERR_=1;
    
_bTrace_("MacMap",true);

    return NSApplicationMain(argc, argv);
}
