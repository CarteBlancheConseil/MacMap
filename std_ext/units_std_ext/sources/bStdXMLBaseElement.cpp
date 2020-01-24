//----------------------------------------------------------------------------
// File : bStdXMLBaseElement.cpp
// Project : MacMap
// Purpose : C++ source file : XML base class
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
// Classe de base MacMap
//----------------------------------------------------------------------------
// 16/05/2003 creation.
//----------------------------------------------------------------------------

#include "bStdXMLBaseElement.h"

#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericMacMapApp* bStdXMLBaseElement::_gapp=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMLBaseElement	::bStdXMLBaseElement(	bGenericXMLBaseElement* elt,
											bGenericMacMapApp* gapp, 
											CFBundleRef bndl)
					: _elts(sizeof(bGenericXMLBaseElement*)){
	if(!_gapp){
		_gapp=gapp;
	}
	_creator=elt;
	_gapp=gapp;
	_cfname=strdup("");	
	_cfvalue=strdup("");
	_bndl=bndl;
	_clss=kXMLClassGeneric;
	_subclss=kXMLSubClassGeneric;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMLBaseElement::~bStdXMLBaseElement(){
bGenericXMLBaseElement	*elt,*killer;		
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			killer=elt->instance();
			if(!killer){
				continue;
			}
			killer->kill(elt);
		}
	}
	if(_cfvalue){
		free(_cfvalue);
	}
	if(_cfname){
		free(_cfname);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStdXMLBaseElement::instance(){
	return(_creator);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdXMLBaseElement::create(bGenericXMLBaseElement* elt){
	return(new bStdXMLBaseElement(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
void bStdXMLBaseElement::kill(bGenericXMLBaseElement* elt){
	delete elt;
}

// ---------------------------------------------------------------------------
// Décodage XML
// -----------
bool bStdXMLBaseElement::decode(xmlNode* root){
bGenericXMLBaseElement*	child;
xmlNode*                cur_node=NULL;
    
    for(cur_node=root;cur_node;cur_node=cur_node->next){
        if(cur_node->type==XML_TEXT_NODE){
            if(_cfvalue){
                free(_cfvalue);
                _cfvalue=NULL;
            }
            if(cur_node->content){
                _cfvalue=decodeCString((const char*)cur_node->content);
            }
        }
        else if(cur_node->type==XML_ELEMENT_NODE){
            child=_gapp->classMgr()->CreateXMLInstance((const char*)cur_node->name);
            if(child){
                if(!child->decode(cur_node->children)){
                    return(false);
                }
                if(!addelement(child)){
                    return(false);
                }
            }
        }
    }
    return(true);
}

// ---------------------------------------------------------------------------
// Encodage XML
// -----------
void bStdXMLBaseElement::encode(int	indent, bFile* f){
int		i;
char	space[_values_length_max_];
char	name[3*_values_length_max_];

	for(i=0;i<indent;i++){
		space[i]=9;
	}
	space[indent]=0;
	
	if(_elts.count()==0){
		strcpy(name,_cfvalue);
		encodeString(name);
		f->write(space);
		f->write("<");
		f->write(_cfname);
		f->write(">");
		f->write(name);
		f->write("</");
		f->write(_cfname);
		f->write(">\n");
	}
	else{
bGenericXMLBaseElement*	elt;
		f->write(space);
		f->write("<");
		f->write(_cfname);
		f->write(">\n");
		for(i=1;i<=_elts.count();i++){
			if(_elts.get(i,&elt)){
				elt->encode(indent+1,f);
			}
		}
		f->write(space);
		f->write("</");
		f->write(_cfname);
		f->write(">\n");
	}
}

// ---------------------------------------------------------------------------
// Initialisation
// -----------
void bStdXMLBaseElement::init(void *ctx){
bGenericXMLBaseElement*	elt;
	
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			elt->init(ctx);
		}
	}
}

// ---------------------------------------------------------------------------
// Acc√©s au nom de la classe
// -----------
void bStdXMLBaseElement::getclassname(char* name){
	strcpy(name,_cfname);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLBaseElement::setclassname(const char* name){
	if(_cfname){
		free(_cfname);
	}
	_cfname=strdup(name);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdXMLBaseElement::getclass(){
	return(_clss);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdXMLBaseElement::getsubclass(){
	return(_subclss);
}

// ---------------------------------------------------------------------------
// Accès à la valeur
// -----------
void bStdXMLBaseElement::getvalue(char* value){
	if(_cfvalue){
		strcpy(value,_cfvalue);
	}
	else{
		value[0]=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLBaseElement::setvalue(const char* value){
	if(_cfvalue){
		free(_cfvalue);
	}
	_cfvalue=strdup(value);
}

// ---------------------------------------------------------------------------
// Recherche d'une classe par le nom : A VIRER
// -----------
bGenericXMLBaseElement* bStdXMLBaseElement::search(CFStringRef	clssname){
char	cname[_names_length_max_];
	CFStringGetCString(clssname,cname,_names_length_max_,kCFStringEncodingMacRoman);
	return(_gapp->classMgr()->CreateXMLInstance(cname));
}

// ---------------------------------------------------------------------------
// Ajout d'un fils
// -----------
bool bStdXMLBaseElement::addelement(bGenericXMLBaseElement* elt){
	return(_elts.add(&elt));
}

// ---------------------------------------------------------------------------
// Nombre de fils
// -----------
int bStdXMLBaseElement::countelements(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// Acc√©s nieme fils
// -----------
bGenericXMLBaseElement* bStdXMLBaseElement::getelement(int idx){
bStdXMLBaseElement*	elt;
	
	if(!_elts.get(idx,&elt)){
		return(NULL);
	}
	return(elt);
}

// ---------------------------------------------------------------------------
// Acc√©s nieme fils
// -----------
void bStdXMLBaseElement::setelement(int idx, bGenericXMLBaseElement* elt){
	if(!_elts.put(idx,&elt)){
	}
}

// ---------------------------------------------------------------------------
// Retrait d'un fils
// -----------
void bStdXMLBaseElement::rmvelement(int idx){
	if(!_elts.rmv(idx)){
	}
}

// ---------------------------------------------------------------------------
// D√©placement d'un fils
// -----------
void bStdXMLBaseElement::pushelement(int idx, int offset){
	if(!_elts.push(idx,offset)){
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLBaseElement::dotoall(void *prm, int indent, bool(*proc)(bGenericXMLBaseElement*,void*,int)){ 
	if(!(*proc)(this,prm,indent)){
		return(false);
	}
	
bGenericXMLBaseElement* elt;
	
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			if(!elt->dotoall(prm,indent+1,proc)){
				return(false);
			}
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void* bStdXMLBaseElement::getapp(){
	return((void*)_gapp);
}

// ---------------------------------------------------------------------------
// 
// -----------
CFBundleRef bStdXMLBaseElement::getbundle(){
	return(_bndl);
}

// ---------------------------------------------------------------------------
// utilitaire
// ------------
char dumpHex(char c){
	if((c<48)||(c>70)){
		return(0);
	}
	if(c<58){
		return(c-48);
	}
	if(c<65){
		return(0);
	}
	return(c-55);
}

// ---------------------------------------------------------------------------
// utilitaire
// ------------
char* decodeString(CFStringRef	cfs){
int			a,b;
char		buff[1024],num[2];
	
	num[1]=0;
	CFStringGetCString(cfs,buff,1024,kCFStringEncodingMacRoman);
long n=strlen(buff);
	for(long i=n-4;i>-1;i--){
		if((buff[i]==92)&&((buff[i+1]=='x')||(buff[i+1]=='X'))){
			a=dumpHex(buff[i+2]);
			b=dumpHex(buff[i+3]);
			buff[i]=(a*16L+b);
			memmove(&buff[i+1],&buff[i+4],n-(i+3));
		}
	}
	return(strdup(buff));
}

// ---------------------------------------------------------------------------
// utilitaire
// ------------
char* decodeCString(const char* cstr){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,cstr,kCFStringEncodingUTF8);
char*       buf=decodeString(cfs);
    CFRelease(cfs);
    return(buf);
}

// ---------------------------------------------------------------------------
// utilitaire
// ------------
void encodeString(char *name){
// Il faut conserver le encode pour les <, > et & inclus + le cas des chaînes uniquement composées d'espaces
// dans le contenu des balises 
int				i,n;
char			hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
unsigned char	c,f=true;
	
	n=strlen(name);
    for(i=0;i<n;i++){
        if(name[i]!=' '){
            f=false;
            break;
        }
    }
    for(i=n-1;i>-1;i--){
        if(	(name[i]==38)	||	//& 26
            (name[i]==60)	||	//< 3C
            (name[i]==62)	){	//> 3E
            c=name[i];
            memmove(&name[i+4],&name[i+1],(strlen(name)+1)-i);
            name[i]=92;
            name[i+1]='x';
            name[i+2]=hex[c/16];
            name[i+3]=hex[c%16];
        }
        else if(    (f==true)       &&
                    (name[i]==32)	){	//  20
            c=name[i];
            memmove(&name[i+4],&name[i+1],(strlen(name)+1)-i);
            name[i]=92;
            name[i+1]='x';
            name[i+2]=hex[c/16];
            name[i+3]=hex[c%16];
       }
    }

CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
	CFStringGetCString(cfs,name,_values_length_max_*3,kCFStringEncodingUTF8);
    CFRelease(cfs);
}


// ---------------------------------------------------------------------------
// Décodage XML
// -----------
//bool bStdXMLBaseElement::decode(CFXMLTreeRef root){
//int						n,nodetype;
//CFXMLTreeRef			tree;
//CFXMLNodeRef			node;
//CFStringRef				cfs;
//bGenericXMLBaseElement*	child;
//
//	n=CFTreeGetChildCount(root);
//	for(int i=0;i<n;i++){
//		tree=CFTreeGetChildAtIndex(root,i);
//		node=CFXMLTreeGetNode(tree);
//		nodetype=CFXMLNodeGetTypeCode(node);
//		if(nodetype==kCFXMLNodeTypeText){
//			if(_cfvalue){
//				free(_cfvalue);
//				_cfvalue=NULL;
//			}
//			cfs=CFXMLNodeGetString(node);
//			_cfvalue=decodeString(cfs);
//		}
//		else if(nodetype==kCFXMLNodeTypeElement){
//			cfs=CFXMLNodeGetString(node);
//			child=search(cfs);
//			if(child){
//				if(!child->decode(tree)){
//					return(false);
//				}
//				if(!addelement(child)){
//					return(false);
//				}
//			}
//		}
//	}
//	return(true);
//    return false;
//}

