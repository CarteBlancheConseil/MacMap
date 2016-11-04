//----------------------------------------------------------------------------
// File : CGUtils.cpp
// Project : MacMap
// Purpose : C++ source file : C utils around CGGeometry, CGImage and CGContext 
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

#include <mox_intf/CGUtils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bCoreTextInfo.h>
#include <mox_intf/bitmap_utils.h>

#include <MacMapSuite/bTrace.h>

#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>


// ---------------------------------------------------------------------------
//
// ------------
int	quad	(	double dh, 
				double dv);
int	cquad	(	int oldQ, 
				int q, 
				int* lq, 
				double* oldDh, 
				double* oldDv, 
				double dh, 
				double dv);

// ---------------------------------------------------------------------------
//
// ------------
/*void CGPoint2PointWithRect(Rect* inRect, CGPoint* cgp, Point* p){
	p->h=round(cgp->x);
	p->v=inRect->bottom-round(cgp->y);
}*/

// ---------------------------------------------------------------------------
//
// ------------
float CGPointsDist(CGPoint* a, CGPoint* b){
float dx=b->x-a->x;
float dy=b->y-a->y;
	return(sqrt(dx*dx+dy*dy));
}

// ---------------------------------------------------------------------------
//
// ------------
/*void XYFloat2RectWithRect(Rect*	inRect, float xmin, float ymin, float xmax, float ymax, Rect* r){
	r->left=round(xmin);
	r->bottom=inRect->bottom-round(ymin);
	r->right=round(xmax);
	r->top=inRect->bottom-round(ymax);
}*/

// ---------------------------------------------------------------------------
//
// ------------
void XYFloat2RectWithCGRect(CGRect*	inRect, float xmin, float ymin, float xmax, float ymax, Rect* r){
	r->left=round(xmin)+inRect->origin.x;
	r->right=round(xmax)+inRect->origin.x;
	r->top=round((inRect->origin.y+inRect->size.height)-ymax);
	r->bottom=round((inRect->origin.y+inRect->size.height)-ymin);
}

// A passer en autre lib
// ---------------------------------------------------------------------------
//
// ------------
float XYDist(float ax, float ay, float bx, float by){
float dx=bx-ax;
float dy=by-ay;
	return(sqrt(dx*dx+dy*dy));
}

// ---------------------------------------------------------------------------
//
// ------------
double XYsDist(float* x, float* y, int n){
double d=0;
	for(int i=0;i<n-1;i++){
		d+=(double)XYDist(x[i],y[i],x[i+1],y[i+1]);
	}
	return(d);
}

// ---------------------------------------------------------------------------
//
// ------------
float XYDist2Vec(	float x, 
					float y, 
					float ax, 
					float ay, 
					float bx, 
					float by){
	
float dx=bx-ax;
float dy=by-ay;
float r;

	if((dx==0)&&(dy==0)){
		dx=ax-x;
		dy=ay-y;
		return(sqrt(dx*dx+dy*dy));
	}
	
	r=((x-ax)*dx+(y-ay)*dy)/(dx*dx+dy*dy);
	
	if(r<=0){
		dx=ax-x;
		dy=ay-y;
		return(sqrt(dx*dx+dy*dy));
	}
	
	if(r>=1){
		dx=bx-x;
		dy=by-y;
		return(sqrt(dx*dx+dy*dy));
	}

float mx=ax+(r*dx);
float my=ay+(r*dy);
		
	dx=mx-x;
	dy=my-y;
	return(sqrt(dx*dx+dy*dy));
}

// ---------------------------------------------------------------------------
//
// ------------
float XYDist2Vecs(	float	x, 
					float	y, 
					float*	xpts, 
					float*	ypts, 
					int		npts){
int		i;
float	d;
float	dx=xpts[0]-x;
float	dy=ypts[0]-y;
float	dmin=sqrt(dx*dx+dy*dy);
	
	for(i=0;i<npts-1;i++){
		d=XYDist2Vec(x,y,xpts[i],ypts[i],xpts[i+1],ypts[i+1]);
		if(d<dmin){
			dmin=d;
		}
		if(dmin==0){
			break;
		}
	}
	return(dmin);
}

// ---------------------------------------------------------------------------
//
// ------------
float XYDistMax2Vecs(	float	x, 
						float	y, 
						float*	xpts, 
						float*	ypts, 
						int		npts){
int		i;
float	d;
float	dx=xpts[0]-x;
float	dy=ypts[0]-y;
float	dmax=sqrt(dx*dx+dy*dy);
	
	for(i=0;i<npts-1;i++){
		d=XYDist2Vec(x,y,xpts[i],ypts[i],xpts[i+1],ypts[i+1]);
		if(d>dmax){
			dmax=d;
		}
	}
	return(dmax);
}

// ---------------------------------------------------------------------------
//
// ------------
int XYInBounds(	float x, float y, 
				float xmin, float ymin, 
				float xmax, float ymax){
	return((x>=xmin)&&(x<=xmax)&&(y>=ymin)&&(y<=ymax));
}

// ---------------------------------------------------------------------------
//
// ------------
void XYBounds	(	float* xmin, float* ymin, 
					float* xmax, float* ymax,
					float*	xpts, 
					float*	ypts, 
					int		npts){
int	i;
	*xmin=*xmax=xpts[0];
	*ymin=*ymax=ypts[0];
	
	for(i=1;i<npts;i++){
		if(xpts[i]<*xmin){
			*xmin=xpts[i];
		}
		else if(xpts[i]>*xmax){
			*xmax=xpts[i];
		}
		if(ypts[i]<*ymin){
			*ymin=ypts[i];
		}
		else if(ypts[i]>*ymax){
			*ymax=ypts[i];
		}
	}
}

// ---------------------------------------------------------------------------
//
// ------------
int XYInClosedPath	(	float x, float y, 
						float*	xpts, 
						float*	ypts, 
						int		npts,
						int		limit){
int		i,first,last,oq,q;
double	oh,ov;
double	vpx,vpy;
	
	if(npts<3){
		return(0);
	}
	vpx=xpts[0]-x;
	vpy=ypts[0]-y;
	if((vpx==0)&&(vpy==0)){
		return(limit);
	}
	first=quad(vpx,-vpy);
	last=first;
	oq=first;
	oh=vpx;
	ov=vpy;
	for(i=1;i<npts;i++){
		vpx=vpx+(xpts[i]-xpts[i-1]);
		vpy=vpy+(ypts[i]-ypts[i-1]);
		if((vpx==0)&&(vpy==0)){
			return(limit);
		}
		q=quad(vpx,-vpy);
		if(cquad(oq,q,&last,&oh,&ov,vpx,vpy)){
			return(limit);
		}
		oq=q;
		oh=vpx;
		ov=vpy;
	}
	return((labs(last-first)/4)>0);
}

// ---------------------------------------------------------------------------
//
// ------------
void CentroidizeClosedPath(	float* x, float *y, 
							float	*xpts, 
							float	*ypts, 
							int		npts){
int		i;
float	xmin,xmax,ymin,ymax;
double	dx,dy;

	XYBounds(&xmin,&ymin,&xmax,&ymax,xpts,ypts,npts);

	dx=((double)xmin+(double)xmax)/2.0;
	dy=((double)ymin+(double)ymax)/2.0;
	
	if(XYInClosedPath(dx,dy,xpts,ypts,npts,1)){
		*x=dx;
		*y=dy;
//fprintf(stderr,"CentroidizeClosedPath : in\n");
		return;
	}
//fprintf(stderr,"CentroidizeClosedPath : out\n");

	dx=0;
	dy=0;
	for(i=1;i<npts;i++){
		dx+=(double)(xpts[i]-xpts[0]);
		dy+=(double)(ypts[i]-ypts[0]);
	}
	dx=(double)xpts[0]+dx/((double)npts-1);
	dy=(double)ypts[0]+dy/((double)npts-1);
	*x=dx;
	*y=dy;
}

// ---------------------------------------------------------------------------
//
// ------------
void CentroidizeLinePath(	float* x, float *y, 
							float*	xpts, 
							float*	ypts, 
							int		npts){
int		i;
double	l=0,midl;
	
	for(i=0;i<npts-1;i++){
		l+=XYDist(xpts[i],ypts[i],xpts[i+1],ypts[i+1]);
	}
	midl=l/2.0;
	l=0;
	for(i=0;i<npts-1;i++){
		l=XYDist(xpts[i],ypts[i],xpts[i+1],ypts[i+1]);
		if(l>midl){
			break;
		}
		midl-=l;
	}
	if(l!=0){
		*x=xpts[i]+(midl/l)*(xpts[i+1]-xpts[i]);
		*y=ypts[i]+(midl/l)*(ypts[i+1]-ypts[i]);
	}
	else{
		*x=xpts[i];
		*y=ypts[i];
	}
}

// ---------------------------------------------------------------------------
//
// ------------
void Centroidize(	float* x, float* y, 
					float*	xpts, 
					float*	ypts, 
					int		npts){
double	sx=xpts[0];
double	sy=ypts[0];
int		i;

	for(i=1;i<npts;i++){
		sx+=(double)xpts[i];
		sy+=(double)ypts[i];
	}
	*x=sx/(double)npts;
	*y=sy/(double)npts;
}

// ---------------------------------------------------------------------------
//
// ------------
float CGPointVec2Angle(CGPoint *pt1, CGPoint *pt2){
float	x,y,a;

	x=pt2->x-pt1->x;
	y=pt2->y-pt1->y;
	if(x==0){
		a=M_PI/2.0;
		if(y<0){ 
			a+=M_PI;
		}
	}
	else{
	 	a=atan(y/x);
	 	if(x<0){
	 		a+=M_PI;
		}
	} 
	if(a<0){
		a+=2.0*M_PI;
	}
	return(a);
}

// ---------------------------------------------------------------------------
//
// ------------
float XYVec2Angle(	float x1, 
					float y1, 
					float x2, 
					float y2){
float	x,y,a;
	x=x2-x1;
	y=y2-y1;
	if(x==0){
		a=M_PI/2.0;
		if(y<0){ 
			a+=M_PI;
		}
	}
	else{
	 	a=atan(y/x);
	 	if(x<0){
	 		a+=M_PI;
		}
	} 
	if(a<0){
		a+=2.0*M_PI;
	}
	return(a);
}

// ---------------------------------------------------------------------------
//
// ------------
void CGPointNewPos(	CGPoint* o, 
					float d, 
					float angle, 
					CGPoint* r){
	r->x=o->x+d*cos(angle);
	r->y=o->y+d*sin(angle);
}

// ---------------------------------------------------------------------------
//
// ------------
void XYNewPos(	float ox,
				float oy,
				float d, 
				float angle, 
				float* rx,
				float* ry){
	*rx=ox+d*cos(angle);
	*ry=oy+d*sin(angle);
}

// ---------------------------------------------------------------------------
//
// ------------
void XYDecal(	float*	xpts, 
				float*	ypts, 
				int		npts, 
				float	dst,
				float**	xnew, 
				float**	ynew,
				int* 	nnew){
int 	closed;
int		i, n, cur;
float	a,a1,a2,r,d;
float	x,x1,x2=0,x3,y,y1,y2=0,y3;

	n=npts-1;
	closed=((xpts[0]==xpts[npts-1])&&(ypts[0]==ypts[npts-1]));
	for(;;){
		(*xnew)=(float*)malloc(n*2L*sizeof(float));
		if(!(*xnew)){
			return;
		}
		(*ynew)=(float*)malloc(n*2L*sizeof(float));
		if(!(*ynew)){
			return;
		}
		
		cur=0;
		for(i=0;i<=n;i++){
			x=xpts[i];
			y=ypts[i];
			
			if(i!=0){
				x1=xpts[i-1];
				y1=ypts[i-1];
			}
			else if(closed){
				x1=xpts[n-1];
				y1=ypts[n-1];
			}
			if(i!=n){
				x2=xpts[i+1];
				y2=ypts[i+1];
			}
			else if(closed){
				x2=xpts[1];
				y2=ypts[1];
			}
			
			if((i==0)&&(!closed)){
				d=dst;
				a=XYVec2Angle(x2,y2,x,y)-M_PI/2.0;
				if(a>2.0*M_PI){
					a-=2.0*M_PI;
				}
			}
			else if((i==n)&&(!closed)){
				d=dst;
				a=XYVec2Angle(x,y,x1,y1)-M_PI/2.0;
				if(a>2.0*M_PI){
					a-=2.0*M_PI;
				}
			}
			else{
				a1=XYVec2Angle(x1,y1,x,y);
				a2=XYVec2Angle(x2,y2,x,y);
				if(fabs(a1-a2)<0.1){
					r=M_PI/4.0;
					d=dst/fabs(sin(r));
					XYNewPos(x,y,d,a2+5.0*M_PI/4.0,&x3,&y3);
					a=a2+3.0*M_PI/4.0;
				}
				else{
					r=(a2-a1)/2.0;
					d=dst/fabs(sin(r));
					if(a2<a1){
						a1+=2.0*M_PI;
					}
					a=(a1+a2)/2.0;
				}
			}
			XYNewPos(x,y,d,a,&x3,&y3);
			(*xnew)[cur]=x3;
			(*ynew)[cur]=y3;
			cur++;
		}
		(*nnew)=cur;
		return;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
float	XYProjOnSeg(float	inx,
					float	iny,
					float	xa,
					float	ya,
					float	xb,
					float	yb,
					float*	outx,
					float*	outy,
					float*	r){

float	dh=xb-xa;
float	dv=yb-ya;

	if((dh==0)&&(dv==0)){
	    *r=0;
		*outx=xa;
		*outy=ya;
		return(XYDist(xa,ya,inx,iny));
	}
	
	*r=((inx-xa)*dh+(iny-ya)*dv)/(dh*dh+dv*dv);
	if(*r<=0){
		*outx=xa;
		*outy=ya;
		return(XYDist(xa,ya,inx,iny));
	}
	
	if(*r>=1){
		*outx=xb;
		*outy=yb;
		return(XYDist(xb,yb,inx,iny));
	}
	
	dh=xa+(*r*dh)-inx;
	dv=ya+(*r*dv)-iny;
	*outx=inx+dh;
	*outy=iny+dv;
	return(sqrt(dh*dh+dv*dv));
}

// ---------------------------------------------------------------------------
// 
// -----------
float	XYProjOnPath(	float	inx,
						float	iny,
						float*	pathx,
						float*	pathy,
						int		pathn,
						float*	outx,
						float*	outy,
						int*	idx,	// in [0..n-1]
						float*	r){
int			j;
float		r1;
double		dst,d1;
float		x,y;

	dst=0x7FFFFFFF;
	dst*=2.0;
	for(j=0;j<pathn-1;j++){
		d1=XYProjOnSeg(inx,iny,pathx[j],pathy[j],pathx[j+1],pathy[j+1],&x,&y,&r1);
		if(d1<dst){
			if(r1<=0){
				*idx=j;
			}
			else if(r1>=1){
				*idx=j+1;
			}
			else{
				*idx=j+1;
			}
			*outx=x;	
			*outy=y;	
			*r=r1;
			dst=d1;
		}
	}
	return(dst);
}

// ---------------------------------------------------------------------------
//
// ------------
int XYSIAM(	float x0, 
			float y0, 
			float x1, 
			float y1, 
			float x2, 
			float y2){
float dx1=x1-x0;
float dy1=y1-y0;
float dx2=x2-x0;
float dy2=y2-y0;
	if(dx1*dy2>dy1*dx2){
		return(1);
	}
	if(dx1*dy2<dy1*dx2){
		return(-1);
	}
	if((dx1*dx2<0)||(dy1*dy2<0)){
		return(-1);
	}
	if((dx1*dx1+dy1*dy1)<(dx2*dx2+dy2*dy2)){
		return(1);
	}	
	return(0);
}

// ---------------------------------------------------------------------------
//
// ------------
int XYLegible(	float*	xpts, 
				float*	ypts, 
				int		npts){
int			i;
double		s=0,d,a;

	for(i=1;i<npts;i++){
		d=XYDist(xpts[i-1],ypts[i-1],xpts[i],ypts[i]);
		a=XYVec2Angle(xpts[i-1],ypts[i-1],xpts[i],ypts[i]);
		if((a>M_PI/2.0)&&(a<=3.0*M_PI/2.0)){
			s-=d;
		}
		else{
			s+=d;
		}
	}
	return(s>=0);
}

// ---------------------------------------------------------------------------
//
// ------------
CGRect CGScaleRect(	CGRect bound, 
					CGRect r){
float	cx,cy,c;
	cx=bound.size.width/r.size.width;
	cy=bound.size.height/r.size.height;
	c=(cx<cy)?cx:cy;
CGRect	res;
	res.size.width=c*r.size.width;
	res.size.height=c*r.size.height;
	res.origin.x=(bound.origin.x+bound.size.width/2.0)-(res.size.width/2.0);
	res.origin.y=(bound.origin.y+bound.size.height/2.0)-(res.size.height/2.0);
	return(res);
}

// ---------------------------------------------------------------------------
//
// ------------
int	quad(double dh, double dv){
	if((dh>=0)&&(dv>0)){
		return(0);
	}
	else if((dh>0)&&(dv<=0)){
		return(1);
	}
	else if((dh<=0)&&(dv<0)){
		return(2);
	}
	else{
		return(3);
	}
}

// ---------------------------------------------------------------------------
//
// ------------
int	cquad(int oq, int q, int* lq, double* oh, double* ov, double dh, double dv){
double	e;
	
	switch(q-oq){
		case -3:
		case 1:
			(*lq)++;
			break;
		case -2:
		case 2:
			e=-(*oh)*dv+(*ov)*dh;
			if(e<0)
				(*lq)+=2;
			else if(e>0){
				(*lq)-=2;
			}
			else{
				return(1);
			}
			break;
		case -1:
		case 3:
			(*lq)--;
			break;
		case 0:
			break;
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGImageRef CGImageCreateFromData(void* data, 
								 int sz, 
								 OSType unused){
_bTrace_("CGImageCreateFromData",false);
CFDataRef           cfData=NULL;
CGImageSourceRef    source=NULL;
CGImageRef          image=NULL;
    
    for(;;){
        cfData=CFDataCreate(kCFAllocatorDefault,(const UInt8*)data,sz);
        if(cfData==NULL){
_te_("CFDataCreateMutable failed");
            break;
        }
        source=CGImageSourceCreateWithData(cfData,NULL);
        if(source==NULL){
_te_("CGImageSourceCreateWithData failed");
            break;
        }
        image=CGImageSourceCreateImageAtIndex(source,0,NULL);
        if(image==NULL){
_te_("CGImageSourceCreateImageAtIndex failed");
            break;
        }
        break;
    }
    if(source){
        CFRelease(source);
    }
    if(cfData){
        CFRelease(cfData);
    }
    return(image);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus GetImageInfoFromData(	void* data, 
								int sz, 
								OSType unused,
								int* hsz,
								int* vsz,
								double* hr, 
								double* vr){
_bTrace_("GetImageInfoFromData",false);
OSStatus            status=-1;
CFDataRef           cfData=NULL;
CGImageSourceRef    source=NULL;
NSDictionary*       prop=NULL;
CFDictionaryRef     dict=NULL;
CFNumberRef         value;
    
    for(;;){
        cfData=CFDataCreate(kCFAllocatorDefault,(const UInt8*)data,sz);
        if(cfData==NULL){
_te_("CFDataCreateMutable failed");
            break;
        }
        source=CGImageSourceCreateWithData(cfData,NULL);
        if(source==NULL){
_te_("CGImageSourceCreateWithData failed");
            break;
        }
        prop=(NSDictionary*)CGImageSourceCopyPropertiesAtIndex(source,0,NULL);
        if(prop==NULL){
_te_("CGImageSourceCopyPropertiesAtIndex failed");
            break;
        }
        dict=(__bridge CFDictionaryRef)prop;
        if(CFDictionaryGetValueIfPresent(dict,kCGImagePropertyPixelHeight,(const void**)&value)){
            CFNumberGetValue(value,kCFNumberSInt32Type,vsz);
        }
        else{
_te_("kCGImagePropertyPixelHeight not found");
            break;
        }
        if(CFDictionaryGetValueIfPresent(dict,kCGImagePropertyPixelWidth,(const void**)&value)){
            CFNumberGetValue(value,kCFNumberSInt32Type,hsz);
        }
        else{
_te_("kCGImagePropertyPixelWidth found");
            break;
        }
        if(CFDictionaryGetValueIfPresent(dict,kCGImagePropertyDPIHeight,(const void**)&value)){
            CFNumberGetValue(value,kCFNumberDoubleType,vr);
        }
        else{
_tw_("kCGImagePropertyDPIHeight found");
            *vr=72;
            //break;
        }
       if(CFDictionaryGetValueIfPresent(dict,kCGImagePropertyDPIWidth,(const void**)&value)){
            CFNumberGetValue(value,kCFNumberDoubleType,hr);
       }
       else{
_tw_("kCGImagePropertyDPIWidth found");
           *hr=72;
           //break;
       }
        status=0;
        break;
    }
    if(source){
        CFRelease(source);
    }
    if(cfData){
        CFRelease(cfData);
    }
    if(dict){
        CFRelease(dict);
    }
    return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPDFDocumentRef PDFCreate(void* data, int sz){
CGDataProviderRef	dp=CGDataProviderCreateWithData(NULL,data,sz,NULL);
CGPDFDocumentRef	pdf=CGPDFDocumentCreateWithProvider(dp);
	CGDataProviderRelease(dp);
	return(pdf);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus PDFConvert(void* data, int sz, UInt32 codec, void** outdata, int* outsz){
_bTrace_("PDFConvert",false);
CGRect					cgr;
OSStatus                status=-1;
size_t                  buf;
CGPDFDocumentRef		pdf=PDFCreate(data,sz);
CGContextRef            context=NULL;
    
    *outsz=0;
    *outdata=NULL;
    if(!pdf){
_te_("PDFCreate failed");
        return status;
    }
    
    for(;;){
CGPDFPageRef    pg=CGPDFDocumentGetPage(pdf,1);
        if(!pg){
_te_("CGPDFDocumentGetPage failed");
            break;
        }
        cgr=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);

        context=CreateBitmapContextWithSize(cgr.size.width,cgr.size.height);
        if(!context){
_te_("CreateBitmapContextWithSize failed");
            break;
        }
	
        CGContextBeginPage(context,&cgr);
        CGContextDrawPDFPage(context,pg);
        CGContextEndPage(context);

        if(CGBitmapContextGetOutputData(context,
                                        codec,
                                        72,
                                        3,
                                        outdata,
                                        &buf)==0){
            (*outsz)=buf;
        }
        else{
_te_("CGBitmapContextGetOutputData failed");
            break;
        }
        status=0;
        break;
    }
    if(pdf){
        CGPDFDocumentRelease(pdf);
    }
    if(context){
        CGContextRelease(context);
    }
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
//void BevelButtonReplaceResourceByPng(ControlRef c, CFBundleRef bndl){
//ControlButtonContentInfo	info;
//		
//	info.contentType=kControlContentCGImageRef;
//	GetBevelButtonContentInfo(c,&info);
//	if(	(info.contentType==kControlContentCIconRes)		||
//		(info.contentType==kControlContentPictRes)		||
//		(info.contentType==kControlContentICONRes)		||
//		(info.contentType==kControlContentIconSuiteRes)	){
//CFStringRef	cfs=CFStringCreateWithFormat(kCFAllocatorDefault,NULL,CFSTR("%d.png"),info.u.resID);
//CGImageRef	img=LoadPNGImageFromBundle(bndl,cfs);
//		CFRelease(cfs);
//		if(!img){
//			return;
//		}
//		BevelButtonSetImageRef(c,img);
//		CGImageRelease(img);
//	}
//}

// ---------------------------------------------------------------------------
// 
// ------------
void CGContextAddRoundedRect(CGContextRef c,
							 CGRect rect,
							 float radius){
	if(radius>(rect.size.height/2.0)){
		radius=(rect.size.height/2.0);
	}
	if(radius>(rect.size.width/2.0)){
		radius=(rect.size.width/2.0);
	}
	CGContextBeginPath(c);
	CGContextMoveToPoint(c,CGRectGetMinX(rect)+radius,CGRectGetMinY(rect));
    CGContextAddArc(c,CGRectGetMaxX(rect)-radius,CGRectGetMinY(rect)+radius,radius,3*M_PI/2,0,0);
    CGContextAddArc(c,CGRectGetMaxX(rect)-radius,CGRectGetMaxY(rect)-radius,radius,0,M_PI/2,0);
    CGContextAddArc(c,CGRectGetMinX(rect)+radius,CGRectGetMaxY(rect)-radius,radius,M_PI/2,M_PI,0);
    CGContextAddArc(c,CGRectGetMinX(rect)+radius,CGRectGetMinY(rect)+radius,radius,M_PI,3*M_PI/2,0);	
    CGContextClosePath(c);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CGContextShowStringAtPoint(CGContextRef c,
								CGFloat	x,
								CGFloat	y,
								const char* str,
								const char* font,
								CGFloat size){
bCoreTextInfo	ti(	(char*)font, 
					size, 
					0, 
					0, 
					0, 
					(char*)str);
	CGContextShowGlyphsAtPoint(c,x,y,ti.glyphs(),ti.nGlyphs());
}

// ---------------------------------------------------------------------------
// 
// -----------
void CGContextSetFillColorWithComponents64(CGContextRef c, 
										   double* components, 
										   bool cmyk){
	if(cmyk){
		CGContextSetCMYKFillColor(c,
								  components[0],
								  components[1],
								  components[2],
								  components[3],
								  components[4]);
	}
	else{
		CGContextSetRGBFillColor(c,
								 components[0],
								 components[1],
								 components[2],
								 components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void CGContextSetStrokeColorWithComponents64(CGContextRef c, 
											 double* components, 
											 bool cmyk){
	if(cmyk){
		CGContextSetCMYKStrokeColor(c,
									components[0],
									components[1],
									components[2],
									components[3],
									components[4]);
	}
	else{
		CGContextSetRGBStrokeColor(c,
								   components[0],
								   components[1],
								   components[2],
								   components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void CGContextSetEmptyStyle(CGContextRef c, 
							CGRect rect){
	CGContextSaveGState(c);
	CGContextSetLineWidth(c,0.75);
	CGContextSetRGBStrokeColor(c,0,0,0,1);
CGRect	shape=CGRectInset(rect,1,1);
	CGContextMoveToPoint(c,CGRectGetMinX(shape),CGRectGetMinY(shape));
	CGContextAddLineToPoint(c,CGRectGetMaxX(shape),CGRectGetMaxY(shape));
	CGContextMoveToPoint(c,CGRectGetMaxX(shape),CGRectGetMinY(shape));
	CGContextAddLineToPoint(c,CGRectGetMinX(shape),CGRectGetMaxY(shape));
	
	CGContextStrokePath(c);
	
	CGContextRestoreGState(c);
}




// ---------------------------------------------------------------------------
//
// ------------
//CGImageRef CGImageCreateFromData(void* data,
//                                 int sz,
//                                 OSType imgtype){
//OSErr					err;
//GraphicsImportComponent imp;
//Handle					h=NULL;
//CGImageRef				image;
//     
//    if((err=OpenADefaultComponent(GraphicsImporterComponentType,imgtype,&imp))){
//fprintf(stderr,"CGUtils::CGImageCreateFromData::OpenADefaultComponent error %d\n",err);
//fflush(stderr);
//        return(NULL);
//    }
//    h=NewHandle(sz);
//    if(!h){
//fprintf(stderr,"CGUtils::CGImageCreateFromData::NewHandle returns NULL\n");
//fflush(stderr);
//        CloseComponent(imp);
//        return(NULL);
//    }
//    memmove(*h,data,sz);
//    if(err=GraphicsImportSetDataHandle(imp,h)){
//fprintf(stderr,"CGUtils::CGImageCreateFromData::GraphicsImportSetDataHandle error %d\n",err);
//fflush(stderr);
//        CloseComponent(imp);
//        DisposeHandle(h);
//        return(NULL);
//    }
//    if((err=GraphicsImportCreateCGImage(imp,&image,kGraphicsImportCreateCGImageUsingCurrentSettings))){
//fprintf(stderr,"CGUtils::CGImageCreateFromData::GraphicsImportCreateCGImage error %d\n",err);
//fflush(stderr);
//        CloseComponent(imp);
//        DisposeHandle(h);
//        return(NULL);
//    }
//    CloseComponent(imp);
//    DisposeHandle(h);
//    return(image);
//}

// ---------------------------------------------------------------------------
//
// ------------
//OSStatus GetImageInfoFromData(	void* data,
//                              int sz,
//                              OSType imgtype,
//                              int* hsz,
//                              int* vsz,
//                              double* hr,
//                              double* vr){
//OSStatus				status;
//GraphicsImportComponent imp;
//ImageDescriptionHandle	imageDescH=NULL;
//Handle					h=NULL;
//
//	if((status=OpenADefaultComponent(GraphicsImporterComponentType,imgtype,&imp))){
//fprintf(stderr,"CGUtils::GetImageInfoFromData::OpenADefaultComponent error %ld\n",status);
//fflush(stderr);
//		return(-1);
//	}
//	h=NewHandle(sz);
//	if(!h){
//fprintf(stderr,"CGUtils::GetImageInfoFromData::NewHandle returns NULL\n");
//fflush(stderr);
//		CloseComponent(imp);
//		return(-1);
//	}
//	memmove(*h,data,sz);
//	if((status=GraphicsImportSetDataHandle(imp,h))){
//fprintf(stderr,"CGUtils::GetImageInfoFromData::GraphicsImportSetDataHandle error %ld\n",status);
//fflush(stderr);
//		CloseComponent(imp);
//		DisposeHandle(h);
//		return(status);
//	}
//	if((status=GraphicsImportGetImageDescription(imp,&imageDescH))){
//fprintf(stderr,"CGUtils::GetImageInfoFromData::GraphicsImportGetImageDescription error %ld\n",status);
//fflush(stderr);
//		CloseComponent(imp);
//		DisposeHandle(h);
//		return(status);
//	}
//	if(imageDescH==NULL){
//fprintf(stderr,"CGUtils::GetImageInfoFromData::imageDescH==NULL\n");
//fflush(stderr);
//		CloseComponent(imp);
//		DisposeHandle(h);
//		return(-2);
//	}
//	*hsz=(**imageDescH).width;
//	*vsz=(**imageDescH).height;
//	*hr=Fix2X((**imageDescH).hRes);
//	*vr=Fix2X((**imageDescH).vRes);
//	CloseComponent(imp);	
//	DisposeHandle((Handle)imageDescH);
//	DisposeHandle(h);
//    return(noErr);
//}

// ---------------------------------------------------------------------------
//
// ------------
//OSStatus PDFConvert2(void* data, int sz, UInt32 codec, void** outdata, int* outsz){
//CGRect					cgr;
//OSStatus				err=-1;
//GraphicsExportComponent	exporter=NULL;
//Handle					h;
//unsigned long			asw;
//CGPDFDocumentRef		pdf=PDFCreate(data,sz);
//CGPDFPageRef            pg=NULL;
//CGContextRef            context=NULL;
//    
//    if(!pdf){
//fprintf(stderr,"PDFConvert::PDFCreate failed (%ld)\n",err);
//        return err;
//    }
//    
//    pg=CGPDFDocumentGetPage(pdf,1);
//    cgr=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
//    context=CreateBitmapContextWithSize(cgr.size.width,cgr.size.height);
//
//    CGContextBeginPage(context,&cgr);
//    CGContextDrawPDFPage(context,pg);
//    CGContextEndPage(context);
//
//	err=OpenADefaultComponent(GraphicsExporterComponentType,codec,&exporter);
//	if(err==noErr){
//		h=NewHandle(0);
//		(void)GraphicsExportSetInputCGBitmapContext(exporter,context);
//		(void)GraphicsExportSetOutputHandle(exporter,h);
//		(void)GraphicsExportDoExport(exporter,&asw);
//		*outsz=GetHandleSize(h);
//		*outdata=malloc(*outsz);
//		HLock(h);
//		memmove(*outdata,*h,*outsz);
//		HUnlock(h);
//		DisposeHandle(h);
//		CloseComponent(exporter);
//	}
//	else{
//fprintf(stderr,"PDFConvert::OpenADefaultComponent failed (%ld)\n",err);
//	}
//
//    CGPDFDocumentRelease(pdf);
//    CGContextRelease(context);
//    
//    return(noErr);
//}
