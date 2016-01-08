//----------------------------------------------------------------------------// File : bStdXMLRenderingElement.h// Project : MacMap// Purpose : Header file : XML rendering base class// Author : Benoit Ogier, benoit.ogier@macmap.com//// Copyright (C) 1997-2015 Carte Blanche Conseil.//// This program is free software: you can redistribute it and/or modify// it under the terms of the GNU General Public License as published by// the Free Software Foundation, either version 3 of the License, or// (at your option) any later version.//// This program is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the// GNU General Public License for more details.//// You should have received a copy of the GNU General Public License// along with this program.  If not, see <http://www.gnu.org/licenses/>.//// See the LICENSE.txt file for more information.////----------------------------------------------------------------------------// Classe de base pour éléments XML de rendering//----------------------------------------------------------------------------// 22/10/2003 creation.//----------------------------------------------------------------------------#ifndef __bStdXMLRenderingElement__#define __bStdXMLRenderingElement__//----------------------------------------------------------------------------#include "bStdXMLBaseElement.h"#include <mox_intf/bGenericXMLRenderingElement.h>#include <mox_intf/bGenericStyle.h>#include <mox_intf/bGenericGraphicContext.h>//----------------------------------------------------------------------------class bStdXMLRenderingElement : public bGenericXMLRenderingElement, public bStdXMLBaseElement {public:    bStdXMLRenderingElement 			(	bGenericXMLBaseElement* elt,                                             bGenericMacMapApp* gapp,                                            CFBundleRef bndl);    virtual ~bStdXMLRenderingElement  	(	);        virtual void init					(	void* ctx);                virtual bool action					(	bGenericGraphicContext* ctx);        virtual bool applyforscale			(	bGenericGraphicContext* ctx);    virtual bool applyforclass			(	bGenericGraphicContext* ctx);    virtual bool applyforobject			(	bGenericGraphicContext* ctx);    virtual bool scalecompliant			(	);    virtual bool classcompliant			(	);    virtual bool objectcompliant		(	);        virtual void setscalecompliant		(	bool b);    virtual void setclasscompliant		(	bool b);    virtual void setobjectcompliant		(	bool b);    private:    bool	_scalecmp;    bool	_classcmp;    bool	_objectcmp;		};									//----------------------------------------------------------------------------#endif