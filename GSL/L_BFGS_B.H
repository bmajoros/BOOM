/********************************************************************
 L-BFGS-B = Limited-memory BFGS with Boundary constraints
 ********************************************************************

 This code is derived from ALGLIB, and is distributed in accordance 
 with the ALGLIB license. For details see:

               http://www.alglib.net/copyrules.php

 Credits for the original code go to:

    NEOS, November 1994. (Latest revision June 1996.)
    Optimization Technology Center.
    Argonne National Laboratory and Northwestern University.

    Written by Ciyou Zhu in collaboration with
    R.H. Byrd, P. Lu-Chen and J. Nocedal.

 This header file is derived from ap.h in the original distribution;
 non-template function definitions have been factored out and are now
 found in LBFGSB.C.

********************************************************************/
#ifndef INCL_L_BFGS_B_H
#define INCL_L_BFGS_B_H


/********************************************************************
Interface to the BOOM/GSL "Optimizer" framework:
********************************************************************/
#include "Optimizer.H"
bool ALGLIB_lbfgsb(GSL::ObjectiveFunction *,const GSL::Vector &initialPoint,
		   const GSL::Vector &lowerBounds,
		   const GSL::Vector &upperBounds,
		   GSL::Vector &optimalPoint,int maxIterations,int M,
		   double epsg=0.01,double epsf=0.01,double epsx=0.01);

#include <stdlib.h>
#include <math.h>

/********************************************************************

The L-BFGS-B function is declared near the end of this file.  The
declaration looks like this:

void  lbfgsbminimize(const int& n,
		     const int& m,
		     ap::real_1d_array& x,
		     const double& epsg,
		     const double& epsf,
		     const double& epsx,
		     const int& maxits,
		     const ap::integer_1d_array& nbd,
		     const ap::real_1d_array& l,
		     const ap::real_1d_array& u,
		     int& info);

The programmer is expected to provide a procedure for evaluating the
value of both the objective function and its derivative:

   void funcgrad(const ap::real_1d_array& x, double& f, 
                 ap::real_1d_array& g);

********************************************************************/




/********************************************************************
Checking of the array boundaries mode.
********************************************************************/
//#define NO_AP_ASSERT
//#define AP_ASSERT

#ifndef AP_ASSERT
#define NO_AP_ASSERT
#endif

#ifdef NO_AP_ASSERT
#ifdef AP_ASSERT
#undef NO_AP_ASSERT
#endif
#endif


/********************************************************************
This symbol is used for debugging. Do not define it and do not remove 
comments. 
********************************************************************/
//#define UNSAFE_MEM_COPY


/********************************************************************
Namespace of a standard library AlgoPascal.
********************************************************************/
namespace ap
{


/********************************************************************
Exception class.
********************************************************************/
class ap_error
{
public:
    static void make_assertion(bool bClause)
        { if(!bClause) throw ap_error(); };
};



/********************************************************************
Class defining a complex number with double precision.
********************************************************************/
class complex;
const complex operator/(const complex& lhs, const complex& rhs);

class complex
{
public:
    complex():x(0.0),y(0.0){};
    complex(const double &_x):x(_x),y(0.0){};
    complex(const double &_x, const double &_y):x(_x),y(_y){};
    complex(const complex &z):x(z.x),y(z.y){};

    complex& operator= (const double& v){ x  = v; y = 0.0; return *this; };
    complex& operator+=(const double& v){ x += v;          return *this; };
    complex& operator-=(const double& v){ x -= v;          return *this; };
    complex& operator*=(const double& v){ x *= v; y *= v;  return *this; };
    complex& operator/=(const double& v){ x /= v; y /= v;  return *this; };

    complex& operator= (const complex& z){ x  = z.x; y  = z.y; return *this; };
    complex& operator+=(const complex& z){ x += z.x; y += z.y; return *this; };
    complex& operator-=(const complex& z){ x -= z.x; y -= z.y; return *this; };
    complex& operator*=(const complex& z)
      { double t = x*z.x-y*z.y; y = x*z.y+y*z.x; x = t; return *this; };
    complex& operator/=(const complex& z)
      { (*this) = (*this)/z; return *this; };

    double x, y;
};



/********************************************************************
Non-member operators for the "complex" class
********************************************************************/
  const bool operator==(const complex& lhs, const complex& rhs);
  const bool operator!=(const complex& lhs, const complex& rhs);
  const complex operator+(const complex& lhs);
  const complex operator-(const complex& lhs);
  const complex operator+(const complex& lhs, const complex& rhs);
  const complex operator+(const complex& lhs, const double& rhs);
  const complex operator+(const double& lhs, const complex& rhs);
  const complex operator-(const complex& lhs, const complex& rhs);
  const complex operator-(const complex& lhs, const double& rhs);
  const complex operator-(const double& lhs, const complex& rhs);
  const complex operator*(const complex& lhs, const complex& rhs);
  const complex operator*(const complex& lhs, const double& rhs);
  const complex operator*(const double& lhs, const complex& rhs);
  const complex operator/(const complex& lhs, const complex& rhs);
  const complex operator/(const double& lhs, const complex& rhs);
  const complex operator/(const complex& lhs, const double& rhs);
  const double abscomplex(const complex &z);
  const complex conj(const complex &z);
  const complex csqr(const complex &z);



/********************************************************************
Template defining vector in memory. It is used by the basic 
subroutines of linear algebra.

Vector consists of Length elements of type T, starting from an element, 
which Data is pointed to. Interval between adjacent elements equals 
the value of Step.

The class provides an access for reading only.
********************************************************************/
template<class T>
class const_raw_vector
{
public:
    const_raw_vector(const T *Data, int Length, int Step):
        pData(const_cast<T*>(Data)),iLength(Length),iStep(Step){};
    const T* GetData() const    { return pData; };
    int GetLength() const    { return iLength; };
    int GetStep() const    { return iStep; };
//protected: --had to comment this out to satisfy g++
    T       *pData;
    int     iLength, iStep;
};



/********************************************************************
Template defining vector in memory, derived from const_raw_vector.
It is used by the basic subroutines of linear algebra.

Vector consists of Length elements of type T, starting from an element, 
which Data is pointed to. Interval between adjacent elements equals 
the value of Step.

The class provides an access both for reading and writing.
********************************************************************/
template<class T>
class raw_vector : public const_raw_vector<T>
{
public:
    raw_vector(T *Data, int Length, int Step):const_raw_vector<T>(Data, Length, Step){};

    T* GetData()
    { return const_raw_vector<T>::pData; };
};



/********************************************************************
Scalar product
********************************************************************/
template<class T>
T vdotproduct(const_raw_vector<T> v1, const_raw_vector<T> v2)
{
    ap_error::make_assertion(v1.GetLength()==v2.GetLength());
    if( v1.GetStep()==1 && v2.GetStep()==1 )
    {
        //
        // fast
        //
        T r = 0;
        const T *p1 = v1.GetData();
        const T *p2 = v2.GetData();
        int imax = v1.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            r += (*p1)*(*p2) + p1[1]*p2[1] + p1[2]*p2[2] + p1[3]*p2[3];
            p1+=4;
            p2+=4;
        }
        for(i=0; i<v1.GetLength()%4; i++)
            r += (*(p1++))*(*(p2++));
        return r;
    }
    else
    {
        //
        // general
        //
        int offset11 = v1.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = v2.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T r = 0;
        const T *p1 = v1.GetData();
        const T *p2 = v2.GetData();
        int imax = v1.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            r += (*p1)*(*p2) + p1[offset11]*p2[offset21] + p1[offset12]*p2[offset22] + p1[offset13]*p2[offset23];
            p1+=offset14;
            p2+=offset24;
        }
        for(i=0; i<v1.GetLength()%4; i++)
        {
            r += (*p1)*(*p2);
            p1+=offset11;
            p2+=offset21;
        }
        return r;
    }
}



/********************************************************************
Copy one vector into another
********************************************************************/
template<class T>
void vmove(raw_vector<T> vdst, const_raw_vector<T> vsrc)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/2;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 = *p2;
            p1[1] = p2[1];
            p1 += 2;
            p2 += 2;
        }
        if(vdst.GetLength()%2 != 0)
            *p1 = *p2;
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 = *p2;
            p1[offset11] = p2[offset21];
            p1[offset12] = p2[offset22];
            p1[offset13] = p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 = *p2;
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Copy one vector multiplied by -1 into another.
********************************************************************/
template<class T>
void vmoveneg(raw_vector<T> vdst, const_raw_vector<T> vsrc)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/2;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 = -*p2;
            p1[1] = -p2[1];
            p1 += 2;
            p2 += 2;
        }
        if(vdst.GetLength()%2 != 0)
            *p1 = -*p2;
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 = -*p2;
            p1[offset11] = -p2[offset21];
            p1[offset12] = -p2[offset22];
            p1[offset13] = -p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 = -*p2;
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Copy one vector multiplied by a number into another vector.
********************************************************************/
template<class T, class T2>
void vmove(raw_vector<T> vdst, const_raw_vector<T> vsrc, T2 alpha)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 = alpha*(*p2);
            p1[1] = alpha*p2[1];
            p1[2] = alpha*p2[2];
            p1[3] = alpha*p2[3];
            p1 += 4;
            p2 += 4;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
            *(p1++) = alpha*(*(p2++));
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 = alpha*(*p2);
            p1[offset11] = alpha*p2[offset21];
            p1[offset12] = alpha*p2[offset22];
            p1[offset13] = alpha*p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 = alpha*(*p2);
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Vector addition
********************************************************************/
template<class T>
void vadd(raw_vector<T> vdst, const_raw_vector<T> vsrc)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 += *p2;
            p1[1] += p2[1];
            p1[2] += p2[2];
            p1[3] += p2[3];
            p1 += 4;
            p2 += 4;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
            *(p1++) += *(p2++);
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 += *p2;
            p1[offset11] += p2[offset21];
            p1[offset12] += p2[offset22];
            p1[offset13] += p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 += *p2;
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Add one vector multiplied by a number to another vector.
********************************************************************/
template<class T, class T2>
void vadd(raw_vector<T> vdst, const_raw_vector<T> vsrc, T2 alpha)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 += alpha*(*p2);
            p1[1] += alpha*p2[1];
            p1[2] += alpha*p2[2];
            p1[3] += alpha*p2[3];
            p1 += 4;
            p2 += 4;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
            *(p1++) += alpha*(*(p2++));
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 += alpha*(*p2);
            p1[offset11] += alpha*p2[offset21];
            p1[offset12] += alpha*p2[offset22];
            p1[offset13] += alpha*p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 += alpha*(*p2);
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Vector subtraction
********************************************************************/
template<class T>
void vsub(raw_vector<T> vdst, const_raw_vector<T> vsrc)
{
    ap_error::make_assertion(vdst.GetLength()==vsrc.GetLength());
    if( vdst.GetStep()==1 && vsrc.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 -= *p2;
            p1[1] -= p2[1];
            p1[2] -= p2[2];
            p1[3] -= p2[3];
            p1 += 4;
            p2 += 4;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
            *(p1++) -= *(p2++);
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        int offset21 = vsrc.GetStep(), offset22 = 2*offset21, offset23 = 3*offset21, offset24 = 4*offset21;
        T *p1 = vdst.GetData();
        const T *p2 = vsrc.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 -= *p2;
            p1[offset11] -= p2[offset21];
            p1[offset12] -= p2[offset22];
            p1[offset13] -= p2[offset23];
            p1 += offset14;
            p2 += offset24;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 -= *p2;
            p1 += vdst.GetStep();
            p2 += vsrc.GetStep();
        }
        return;
    }
}



/********************************************************************
Subtract one vector multiplied by a number from another vector.
********************************************************************/
template<class T, class T2>
void vsub(raw_vector<T> vdst, const_raw_vector<T> vsrc, T2 alpha)
{
    vadd(vdst, vsrc, -alpha);
}



/********************************************************************
In-place vector multiplication
********************************************************************/
template<class T, class T2>
void vmul(raw_vector<T> vdst, T2 alpha)
{
    if( vdst.GetStep()==1 )
    {
        //
        // fast
        //
        T *p1 = vdst.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=imax; i!=0; i--)
        {
            *p1 *= alpha;
            p1[1] *= alpha;
            p1[2] *= alpha;
            p1[3] *= alpha;
            p1 += 4;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
            *(p1++) *= alpha;
        return;
    }
    else
    {
        //
        // general
        //
        int offset11 = vdst.GetStep(), offset12 = 2*offset11, offset13 = 3*offset11, offset14 = 4*offset11;
        T *p1 = vdst.GetData();
        int imax = vdst.GetLength()/4;
        int i;
        for(i=0; i<imax; i++)
        {
            *p1 *= alpha;
            p1[offset11] *= alpha;
            p1[offset12] *= alpha;
            p1[offset13] *= alpha;
            p1 += offset14;
        }
        for(i=0; i<vdst.GetLength()%4; i++)
        {
            *p1 *= alpha;
            p1 += vdst.GetStep();
        }
        return;
    }
}



/********************************************************************
Template of a dynamical one-dimensional array
********************************************************************/
template<class T>
class template_1d_array
{
public:
    template_1d_array()
    {
        m_Vec=0;
        m_iVecSize = 0;
    };

    ~template_1d_array()
    {
        if(m_Vec)
            delete[] m_Vec;
    };

    template_1d_array(const template_1d_array &rhs)
    {
        m_iVecSize = rhs.m_iVecSize;
        m_iLow = rhs.m_iLow;
        m_iHigh = rhs.m_iHigh;
        if(rhs.m_Vec)
        {
            m_Vec = new T[m_iVecSize];
            #ifndef UNSAFE_MEM_COPY
            for(int i=0; i<m_iVecSize; i++)
                m_Vec[i] = rhs.m_Vec[i];
            #else
            memcpy(m_Vec, rhs.m_Vec, m_iVecSize*sizeof(T));
            #endif
        }
        else
            m_Vec=0;
    };

    
    const template_1d_array& operator=(const template_1d_array &rhs)
    {
        if( this==&rhs )
            return *this;

        m_iLow = rhs.m_iLow;
        m_iHigh = rhs.m_iHigh;
        m_iVecSize = rhs.m_iVecSize;
        if(m_Vec)
            delete[] m_Vec;
        if(rhs.m_Vec)
        {
            m_Vec = new T[m_iVecSize];
            #ifndef UNSAFE_MEM_COPY
            for(int i=0; i<m_iVecSize; i++)
                m_Vec[i] = rhs.m_Vec[i];
            #else
            memcpy(m_Vec, rhs.m_Vec, m_iVecSize*sizeof(T));
            #endif
        }
        else
            m_Vec=0;
        return *this;
    };

    
    const T& operator()(int i) const
    {
        #ifndef NO_AP_ASSERT
        ap_error::make_assertion(i>=m_iLow && i<=m_iHigh);
        #endif
        return m_Vec[ i-m_iLow ];
    };

    
    T& operator()(int i)
    {
        #ifndef NO_AP_ASSERT
        ap_error::make_assertion(i>=m_iLow && i<=m_iHigh);
        #endif
        return m_Vec[ i-m_iLow ];
    };

    
    void setbounds( int iLow, int iHigh )
    {
        if(m_Vec)
            delete[] m_Vec;
        m_iLow = iLow;
        m_iHigh = iHigh;
        m_iVecSize = iHigh-iLow+1;
        m_Vec = new T[m_iVecSize];
    };

    
    void setcontent( int iLow, int iHigh, const T *pContent )
    {
        setbounds(iLow, iHigh);
        for(int i=iLow; i<=iHigh; i++)
            (*this)(i) = pContent[i-iLow];
    };

    
    T* getcontent()
    {
        return m_Vec;
    };

    const T* getcontent() const
    {
        return m_Vec;
    };

    
    int getlowbound(int iBoundNum = 0) const
    {
        return m_iLow;
    };

    
    int gethighbound(int iBoundNum = 0) const
    {
        return m_iHigh;
    };

    raw_vector<T> getvector(int iStart, int iEnd)
    {
        #ifndef NO_AP_ASSERT
        if( iEnd>=iStart )
            ap_error::make_assertion(m_iLow<=iStart && iEnd<=m_iHigh);
        #endif
        if( iStart>iEnd )
            return raw_vector<T>(0, 0, 1);
        else
            return raw_vector<T>(m_Vec+iStart-m_iLow, iEnd-iStart+1, 1);
    };

    
    const_raw_vector<T> getvector(int iStart, int iEnd) const
    {
        #ifndef NO_AP_ASSERT
        if( iEnd>=iStart )
            ap_error::make_assertion(m_iLow<=iStart && iEnd<=m_iHigh);
        #endif
        if( iStart>iEnd )
            return const_raw_vector<T>(0, 0, 1);
        else
            return const_raw_vector<T>(m_Vec+iStart-m_iLow, iEnd-iStart+1, 1);
    };
private:
    T         *m_Vec;
    long      m_iVecSize;
    long      m_iLow, m_iHigh;
};



/********************************************************************
Template of a dynamical two-dimensional array
********************************************************************/
template<class T>
class template_2d_array
{
public:
    template_2d_array()
    {
        m_Vec=0;
        m_iVecSize=0;
    };

    ~template_2d_array()
    {
        if(m_Vec)
            delete[] m_Vec;
    };

    template_2d_array(const template_2d_array &rhs)
    {
        m_iVecSize = rhs.m_iVecSize;
        m_iLow1 = rhs.m_iLow1;
        m_iLow2 = rhs.m_iLow2;
        m_iHigh1 = rhs.m_iHigh1;
        m_iHigh2 = rhs.m_iHigh2;
        m_iConstOffset = rhs.m_iConstOffset;
        m_iLinearMember = rhs.m_iLinearMember;
        if(rhs.m_Vec)
        {
            m_Vec = new T[m_iVecSize];
            #ifndef UNSAFE_MEM_COPY
            for(int i=0; i<m_iVecSize; i++)
                m_Vec[i] = rhs.m_Vec[i];
            #else
            memcpy(m_Vec, rhs.m_Vec, m_iVecSize*sizeof(T));
            #endif
        }
        else
            m_Vec=0;
    };
    const template_2d_array& operator=(const template_2d_array &rhs)
    {
        if( this==&rhs )
            return *this;

        m_iLow1 = rhs.m_iLow1;
        m_iLow2 = rhs.m_iLow2;
        m_iHigh1 = rhs.m_iHigh1;
        m_iHigh2 = rhs.m_iHigh2;
        m_iConstOffset = rhs.m_iConstOffset;
        m_iLinearMember = rhs.m_iLinearMember;
        m_iVecSize = rhs.m_iVecSize;
        if(m_Vec)
            delete[] m_Vec;
        if(rhs.m_Vec)
        {
            m_Vec = new T[m_iVecSize];
            #ifndef UNSAFE_MEM_COPY
            for(int i=0; i<m_iVecSize; i++)
                m_Vec[i] = rhs.m_Vec[i];
            #else
            memcpy(m_Vec, rhs.m_Vec, m_iVecSize*sizeof(T));
            #endif
        }
        else
            m_Vec=0;
        return *this;
    };

    const T& operator()(int i1, int i2) const
    {
        #ifndef NO_AP_ASSERT
        ap_error::make_assertion(i1>=m_iLow1 && i1<=m_iHigh1);
        ap_error::make_assertion(i2>=m_iLow2 && i2<=m_iHigh2);
        #endif
        return m_Vec[ m_iConstOffset + i2 +i1*m_iLinearMember];
    };

    T& operator()(int i1, int i2)
    {
        #ifndef NO_AP_ASSERT
        ap_error::make_assertion(i1>=m_iLow1 && i1<=m_iHigh1);
        ap_error::make_assertion(i2>=m_iLow2 && i2<=m_iHigh2);
        #endif
        return m_Vec[ m_iConstOffset + i2 +i1*m_iLinearMember];
    };

    void setbounds( int iLow1, int iHigh1, int iLow2, int iHigh2 )
    {
        if(m_Vec)
            delete[] m_Vec;
        m_iVecSize = (iHigh1-iLow1+1)*(iHigh2-iLow2+1);
        m_Vec = new T[m_iVecSize];
        m_iLow1  = iLow1;
        m_iHigh1 = iHigh1;
        m_iLow2  = iLow2;
        m_iHigh2 = iHigh2;
        m_iConstOffset = -m_iLow2-m_iLow1*(m_iHigh2-m_iLow2+1);
        m_iLinearMember = (m_iHigh2-m_iLow2+1);
    };

    void setcontent( int iLow1, int iHigh1, int iLow2, int iHigh2, const T *pContent )
    {
        setbounds(iLow1, iHigh1, iLow2, iHigh2);
        for(int i=0; i<m_iVecSize; i++)
            m_Vec[i]=pContent[i];
    };

    T* getcontent()
    {
        return m_Vec;
    };

    const T* getcontent() const
    {
        return m_Vec;
    };

    int getlowbound(int iBoundNum) const
    {
        return iBoundNum==1 ? m_iLow1 : m_iLow2;
    };

    int gethighbound(int iBoundNum) const
    {
        return iBoundNum==1 ? m_iHigh1 : m_iHigh2;
    };

    raw_vector<T> getcolumn(int iColumn, int iRowStart, int iRowEnd)
    {
        #ifndef NO_AP_ASSERT
        if( iRowEnd>=iRowStart )
        {
            ap_error::make_assertion(m_iLow1<=iRowStart && iRowEnd<=m_iHigh1);
            ap_error::make_assertion(m_iLow2<=iColumn && iColumn<=m_iHigh2);
        }
        #endif
        if( iRowStart>iRowEnd )
            return raw_vector<T>(0, 0, 1);
        else
            return raw_vector<T>(&((*this)(iRowStart, iColumn)), iRowEnd-iRowStart+1, m_iLinearMember);
    };

    raw_vector<T> getrow(int iRow, int iColumnStart, int iColumnEnd)
    {
        #ifndef NO_AP_ASSERT
        if( iColumnEnd>=iColumnStart )
        {
            ap_error::make_assertion(m_iLow2<=iColumnStart && iColumnEnd<=m_iHigh2);
            ap_error::make_assertion(m_iLow1<=iRow && iRow<=m_iHigh1);
        }
        #endif
        if( iColumnStart>iColumnEnd )
            return raw_vector<T>(0, 0, 1);
        else
            return raw_vector<T>(&((*this)(iRow, iColumnStart)), iColumnEnd-iColumnStart+1, 1);
    };

    const_raw_vector<T> getcolumn(int iColumn, int iRowStart, int iRowEnd) const
    {
        #ifndef NO_AP_ASSERT
        if( iRowEnd>=iRowStart )
        {
            ap_error::make_assertion(m_iLow1<=iRowStart && iRowEnd<=m_iHigh1);
            ap_error::make_assertion(m_iLow2<=iColumn && iColumn<=m_iHigh2);
        }
        #endif
        if( iRowStart>iRowEnd )
            return const_raw_vector<T>(0, 0, 1);
        else
            return const_raw_vector<T>(&((*this)(iRowStart, iColumn)), iRowEnd-iRowStart+1, m_iLinearMember);
    };

    const_raw_vector<T> getrow(int iRow, int iColumnStart, int iColumnEnd) const
    {
        #ifndef NO_AP_ASSERT
        if( iColumnEnd>=iColumnStart )
        {
            ap_error::make_assertion(m_iLow2<=iColumnStart && iColumnEnd<=m_iHigh2);
            ap_error::make_assertion(m_iLow1<=iRow && iRow<=m_iHigh1);
        }
        #endif
        if( iColumnStart>iColumnEnd )
            return const_raw_vector<T>(0, 0, 1);
        else
            return const_raw_vector<T>(&((*this)(iRow, iColumnStart)), iColumnEnd-iColumnStart+1, 1);
    };
private:
    T           *m_Vec;
    long        m_iVecSize;
    long        m_iLow1, m_iLow2, m_iHigh1, m_iHigh2;
    long        m_iConstOffset, m_iLinearMember;
};


typedef template_1d_array<int>     integer_1d_array;
typedef template_1d_array<double>  real_1d_array;
typedef template_1d_array<complex> complex_1d_array;
typedef template_1d_array<bool>    boolean_1d_array;
typedef template_2d_array<int>     integer_2d_array;
typedef template_2d_array<double>  real_2d_array;
typedef template_2d_array<complex> complex_2d_array;
typedef template_2d_array<bool>    boolean_2d_array;


/*************************************************************************
void lbfgsbminimize(...)

The  subroutine  minimizes  the  function  F(x) of N arguments with simple
constraints using a quasi-Newton method (LBFGS scheme) which is  optimized
to use a minimum amount of memory.

The subroutine generates the approximation of an inverse Hessian matrix by
using information about the last M steps of the algorithm (instead  of N).
It lessens a required amount of memory from a value  of  order  N^2  to  a
value of order 2*N*M.

This subroutine uses the FuncGrad subroutine which calculates the value of
the function F and gradient G in point X. The programmer should define the
FuncGrad subroutine by himself.  It should be noted  that  the  subroutine
doesn't need to waste  time for memory allocation of array G, because  the
memory is allocated in calling the  subroutine.  Setting  a  dimension  of
array G each time when calling a subroutine will excessively slow down  an
algorithm.

The programmer could also redefine the LBFGSNewIteration subroutine  which
is called on each new step. The current point X, the function value F  and
the gradient G are passed  into  this  subroutine.  It  is  reasonable  to
redefine the subroutine for better debugging, for  example,  to  visualize
the solution process.

Input parameters:
    N       -   problem dimension. N>0
    M       -   number of  corrections  in  the  BFGS  scheme  of  Hessian
                approximation  update.  Recommended value:  3<=M<=7.   The
                smaller value causes worse convergence,  the  bigger  will
                not  cause  a  considerably  better  convergence, but will
                cause a fall in the performance. M<=N.
    X       -   initial solution approximation.
                Array whose index ranges from 1 to N.
    EpsG    -   positive number which defines a precision of  search.  The
                subroutine finishes its work if the condition ||G|| < EpsG
                is satisfied, where ||.|| means Euclidian norm, G - gradient
                projection onto a feasible set, X - current approximation.
    EpsF    -   positive number which defines a precision of  search.  The
                subroutine  finishes  its  work if on iteration number k+1
                the condition |F(k+1)-F(k)| <= EpsF*max{|F(k)|, |F(k+1)|, 1}
                is satisfied.
    EpsX    -   positive number which defines a precision of  search.  The
                subroutine  finishes  its  work if on iteration number k+1
                the condition |X(k+1)-X(k)| <= EpsX is satisfied.
    MaxIts  -   maximum number of iterations.
                If MaxIts=0, the number of iterations is unlimited.
    NBD     -   constraint type. If NBD(i) is equal to:
                * 0, X(i) has no constraints,
                * 1, X(i) has only lower boundary,
                * 2, X(i) has both lower and upper boundaries,
                * 3, X(i) has only upper boundary,
                Array whose index ranges from 1 to N.
    L       -   lower boundaries of X(i) variables.
                Array whose index ranges from 1 to N.
    U       -   upper boundaries of X(i) variables.
                Array whose index ranges from 1 to N.

Output parameters:
    X       -   solution approximation.
Array whose index ranges from 1 to N.
    Info    -   a return code:
                    * -2 unknown internal error,
                    * -1 wrong parameters were specified,
                    * 0 interrupted by user,
                    * 1 relative function decreasing is less or equal to EpsF,
                    * 2 step is less or equal to EpsX,
                    * 4 gradient norm is less or equal to EpsG,
                    * 5 number of iterations exceeds MaxIts.

    NEOS, November 1994. (Latest revision June 1996.)
    Optimization Technology Center.
    Argonne National Laboratory and Northwestern University.

    Written by Ciyou Zhu in collaboration with
    R.H. Byrd, P. Lu-Chen and J. Nocedal.
*************************************************************************/

void  lbfgsbminimize(const int& n,
		     const int& m,
		     ap::real_1d_array& x,
		     const double& epsg,
		     const double& epsf,
		     const double& epsx,
		     const int& maxits,
		     const ap::integer_1d_array& nbd,
		     const ap::real_1d_array& l,
		     const ap::real_1d_array& u,
		     int& info);



/*-----------------------------------------------
This routines must be defined by the programmer:

void funcgrad(const ap::real_1d_array& x, double& f, ap::real_1d_array& g);
-----------------------------------------------*/
void funcgrad(const ap::real_1d_array& x, double& f, ap::real_1d_array& g);




/********************************************************************
Constants and functions introduced for compatibility with AlgoPascal
********************************************************************/
static double machineepsilon = 5E-16;
static double maxrealnumber = 1E300;
static double minrealnumber = 1E-300;

static int sign(double x)
{
    if( x>0 ) return  1;
    if( x<0 ) return -1;
    return 0;
}

static double randomreal()
{
    int i = rand();
    while(i==RAND_MAX)
        i =rand();
    return double(i)/double(RAND_MAX);
}

static int randominteger(int maxv)
{  return rand()%maxv; }

static int round(double x)
{ return int(floor(x+0.5)); }

static int trunc(double x)
{ return int(x>0 ? floor(x) : ceil(x)); }

static int ifloor(double x)
{ return int(floor(x)); }

static int iceil(double x)
{ return int(ceil(x)); }

static double pi()
{ return 3.14159265358979323846; }

static double sqr(double x)
{ return x*x; }

static int maxint(int m1, int m2)
{
    return m1>m2 ? m1 : m2;
}

static int minint(int m1, int m2)
{
    return m1>m2 ? m2 : m1;
}

static double maxreal(double m1, double m2)
{
    return m1>m2 ? m1 : m2;
}

static double minreal(double m1, double m2)
{
    return m1>m2 ? m2 : m1;
}


};//namespace ap


void gslToAlglib(const GSL::Vector &,ap::real_1d_array &);
void alglibToGsl(const ap::real_1d_array &,GSL::Vector &);


#endif
