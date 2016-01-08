//----------------------------------------------------------------------------
// File : bXMapNetClean.h
// Project : MacMap
// Purpose : Header file : XMap class, network cleaning
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
// 08/11/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapNetClean__
#define __bXMapNetClean__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define kXMapNetCleanConfig					"netclean_config"
#define kXMapNetCleanMessageID				"NetClean"
#define kXMapNetCleanControlField			"CtrlField"
#define kXMapNetCleanControlFieldDefault	"xxxxxxxx"

#define kXMapNetCleanAskAddControlField		"AskAddControlField"

#define kXMapNetCleanNodeCreationMessage	"NodeCreation" 
#define kXMapNetCleanCutEdgesMessage		"CutEdges"
#define kXMapNetCleanJoinOnNodesMessage		"JoinOnNodes"
#define kXMapNetCleanJoinOnEdgesMessage		"JoinOnEdges"
#define kXMapNetCleanCheckNodesMessage		"CheckNodes"
#define kXMapNetCleanCheckEdgesMessage		"CheckEdges"


enum{	
	kXMapNetCleanSignature				='NtCl',
	kXMapNetCleanOptionsNodeCreateID	=1,
	kXMapNetCleanOptionsNodeCreateCmd	='NdCr',
	kXMapNetCleanOptionsEdgeCutID		=2,
	kXMapNetCleanOptionsJoinID			=3,
	kXMapNetCleanOptionsJoinCmd			='Join',
	kXMapNetCleanOptionsJoinPopupID		=4,
	kXMapNetCleanOptionsNodeCheckID		=5,
	kXMapNetCleanOptionsEdgeCheckID		=6
};

//----------------------------------------------------------------------------

class bXMapNetClean : public bStdXMapWithModalIntf{
	public:		
		bXMapNetClean							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapNetClean					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool event						(	EventRef evt);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);

		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
				
	protected:		
		virtual void process_network			(	bool log);
		virtual void process_network			(	bArray& nodes, 
													bArray& edges);
		virtual void process_network			(	netcleanact_prm* prm,
													int* fields,
													bArray& nodes,
													bArray& edges);


		virtual void check_events				(	);

		
		virtual int create_nodes				(	bArray& loop,
													bArray& added,
													int* fields,
													bool report);
		virtual int cut_edges					(	bArray& loop,
													bArray& added,
													int* fields,
													bool report);
		virtual int join_on_nodes				(	bArray& loop,
													int* fields,
													bool report);
		virtual int join_on_edges				(	bArray& loop,
													int* fields,
													bool report);
		virtual int check_nodes					(	bArray& loop,
													int* fields);
		virtual int check_edges					(	bArray& loop,
													int* fields);

		virtual void get_nodes					(	i2dvertex* vx,
													bArray& arr);
		virtual void get_bridges				(	i2dvertex* vx,
													bArray& arr);
		virtual void get_edges					(	ivx_rect* vr,
													bArray& arr);
		virtual void get_edges					(	i2dvertex* vx,
													bArray& arr,
													bool by_end);
		
		virtual void init_fields				(	netcleanact_prm* prm);

		virtual void read_p						(	);
		virtual void write_p					(	);
		virtual bGenericXMLBaseElement* make_p	(	);


	private:
		netcleancfg_prm	_cfg_prm;
		netcleanact_prm	_act_prm;
		bArray			_nodes;
		bArray			_edges;
		bArray			_nitr;
		bArray			_eitr;
		bool			_silent;
};

//----------------------------------------------------------------------------

#endif
