/****************************************************************
 Matrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
****************************************************************/
#include <iostream>
#include <fstream>
#include "Matrix.H"
#include "Vector.H"
#include "BOOM/String.H"
#include "BOOM/VectorSorter.H"
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>
#include "../TNT/jama_eig.h"
#include "BOOM/SumLogProbs.H"
#include "BOOM/Array1D.H"
#include "BOOM/Exceptions.H"
#include "BOOM/SummaryStats.H"
#include "BOOM/Constants.H"
using namespace std;
using namespace GSL;


#define DEBUG false


Matrix::Matrix(int rows,int cols)
{
  M=gsl_matrix_alloc(rows,cols);
}



Matrix::Matrix(const GSL::Matrix &other)
{
  M=gsl_matrix_alloc(other.getNumRows(),other.getNumColumns());
  gsl_matrix_memcpy(M,other.M);
}



Matrix::Matrix(const TNT::Array2D<double> &t)
{
  int rows=t.dim1(), cols=t.dim2();
  M=gsl_matrix_alloc(rows,cols);
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      gsl_matrix_set(M,i,j,t[i][j]);
}



Matrix::Matrix(const BOOM::String &filename)
    : M(NULL)
{
    load(filename);
}



Matrix::Matrix(istream &is)
    : M(NULL)
{
    load(is);
}



Matrix::~Matrix()
{
    if(M) gsl_matrix_free(M);
}



double Matrix::operator()(int row,int col) const
{
#ifdef DEBUG
  if(row>=getNumRows() || col>=getNumColumns()) {
    cout<<"GSL::Matrix: asking for ("<<row<<","<<col<<"), size is "
	<<getNumRows()<<"x"<<getNumColumns()<<endl;
    INTERNAL_ERROR;
  }
#endif
  return gsl_matrix_get(M,row,col);
}



double &Matrix::operator()(int row,int col)
{
#ifdef DEBUG
  if(row>=getNumRows() || col>=getNumColumns()) {
    cout<<"GSL::Matrix: asking for ("<<row<<","<<col<<"), size is "
	<<getNumRows()<<"x"<<getNumColumns()<<endl;
    INTERNAL_ERROR;
  }
#endif
  return *gsl_matrix_ptr(M,row,col);
}



const GSL::Matrix &Matrix::operator=(const GSL::Matrix &other)
{
  if(M->size1 != other.M->size1 ||
     M->size2 != other.M->size2)
    resize(other.M->size1,other.M->size2);
  gsl_matrix_memcpy(M,other.M);
  return *this;
}



bool Matrix::invert(GSL::Matrix &resultingMatrix) const
{
  gsl_matrix *LU=gsl_matrix_alloc(M->size1,M->size2);
  gsl_matrix_memcpy(LU,M);
  gsl_permutation *P=gsl_permutation_alloc(M->size1);
  int signum;
  gsl_linalg_LU_decomp(LU,P,&signum);
  if(resultingMatrix.M->size1 != M->size1 ||
     resultingMatrix.M->size2 != M->size2)
    resultingMatrix.resize(M->size1,M->size2);
  gsl_linalg_LU_invert(LU,P,resultingMatrix.M);
  gsl_matrix_free(LU);
  gsl_permutation_free(P);
  return true;
}



bool Matrix::invertDiagonal(GSL::Matrix &result) const
{
  const int n=M->size1;
  if(n!=M->size2) throw "GSL::Matrix::invertDiagonal: matrix is not square";
  if(result.M->size1!=n || result.M->size2!=n) result.resize(n,n);
  result.setAllTo(0.0);
  const Matrix &self=*this;
  for(int i=0 ; i<n ; ++i) result(i,i)=1/self(i,i);
  return true;
}



void Matrix::getColumn(int column,GSL::Vector &into) const
{
  if(into.getDim()!=M->size1) into.resize(M->size1);
  gsl_matrix_get_col(into.peek(),M,column);
}



void Matrix::getRow(int row,GSL::Vector &into) const
{
  if(into.getDim()!=M->size2) into.resize(M->size2);
  gsl_matrix_get_row(into.peek(),M,row);
}



void Matrix::setColumn(int column,const GSL::Vector &v)
{
  gsl_matrix_set_col(M,column,v.peek());
}



void Matrix::setRow(int row,const GSL::Vector &v)
{
  gsl_matrix_set_row(M,row,v.peek());
}



void Matrix::printOn(ostream &os) const
{
  os<<M->size1<<"\t"<<M->size2<<endl;
  for(int i=0 ; i<M->size1 ; ++i) {
    for(int j=0 ; j<M->size2 ; ++j)
      os<<gsl_matrix_get(M,i,j)<<"\t";
    os<<endl;
  }
}



void Matrix::setAllTo(double d)
{
  gsl_matrix_set_all(M,d);
}



void Matrix::times(const GSL::Matrix &other,GSL::Matrix &result) const
{
  if(result.M->size1 != M->size1 ||
     result.M->size2 != other.M->size2)
    result.resize(M->size1,other.M->size2);
  gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,M,other.M,0.0,result.M);
}



void Matrix::transpose(GSL::Matrix &Mt) const
{
  int rows=M->size1, cols=M->size2;
  if(Mt.M->size1!=cols || Mt.M->size2!=rows)
    Mt.resize(cols,rows);
  gsl_matrix_transpose_memcpy(Mt.M,M);
}



void Matrix::transpose()
{
  if(M->size1==M->size2)
    gsl_matrix_transpose(M);
  else
    {
      gsl_matrix *m=gsl_matrix_alloc(M->size2,M->size1);
      gsl_matrix_transpose_memcpy(m,M);
      gsl_matrix_free(M);
      M=m;
    }
}



void Matrix::resize(int nRows,int nCols)
{
    if(M) gsl_matrix_free(M);
    M=gsl_matrix_alloc(nRows,nCols);
}



ostream &GSL::operator<<(ostream &os,const GSL::Matrix &M)
{
  M.printOn(os);
  return os;
}



void Matrix::multiply(const GSL::Vector &by,GSL::Vector &result) const
{
  if(result.getDim()!=M->size1) result.resize(M->size1);
  gsl_blas_dgemv(CblasNoTrans,1.0,M,by.peek(),0.0,result.peek());
}



void Matrix::getDiagonal(GSL::Vector &diag) const
{
  const Matrix &self=*this;
  const int n=M->size1;
  if(M->size2!=n) throw "Not square matrix in GSL::Matrix::getDiagonal";
  diag.resize(n);
  for(int i=0 ; i<n ; ++i) diag[i]=self(i,i);
}



void Matrix::hadamard(const GSL::Matrix &by,GSL::Matrix &result) const
{
  if(result.M->size1!=M->size1 || result.M->size2!=M->size2) 
    result.resize(M->size1,M->size2);
  const Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  for(int i=0 ; i<m ; ++i)
    for(int j=0 ; j<n ; ++j)
      result(i,j)=self(i,j)*by(i,j);
}



void Matrix::sqrt()
{
  Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  for(int i=0 ; i<m ; ++i)
    for(int j=0 ; j<n ; ++j)
      self(i,j)=::sqrt(self(i,j));
}



void Matrix::divideComponentwise(const GSL::Matrix &by,GSL::Matrix &result) 
  const
{
  if(result.M->size1!=M->size1 || result.M->size2!=M->size2) 
    result.resize(M->size1,M->size2);
  const Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  for(int i=0 ; i<m ; ++i)
    for(int j=0 ; j<n ; ++j)
      result(i,j)=self(i,j)/by(i,j);
}



double Matrix::determinant() const
{
  gsl_matrix *LU=gsl_matrix_alloc(M->size1,M->size2);
  gsl_matrix_memcpy(LU,M);
  gsl_permutation *P=gsl_permutation_alloc(M->size1);
  int signum;
  gsl_linalg_LU_decomp(LU,P,&signum);
  double det=gsl_linalg_LU_det(LU,signum);
  gsl_matrix_free(LU);
  return det;
}



void Matrix::becomeIdentity()
{
  gsl_matrix_set_identity(M);
}



bool Matrix::isZero(double epsilon) const
{
  //return gsl_matrix_isnull(M);
  int rows=M->size1, cols=M->size2;
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      {
	double entry=gsl_matrix_get(M,i,j);
	if(fabs(entry)>epsilon) return false;
      }
  return true;
}



bool Matrix::isIdentity(double epsilon) const
{
  int rows=M->size1, cols=M->size2;
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      {
	double entry=gsl_matrix_get(M,i,j);
	if(i==j)
	  {if(fabs(entry)-1.0>epsilon) return false;}
	else
	  {if(fabs(entry)>epsilon) return false;}
      }
  return true;
}



bool Matrix::sameSize(const GSL::Matrix &R) const
{
  return R.M->size1==M->size1 && R.M->size2==M->size2;
}



void Matrix::getEigenVectors(GSL::Matrix &vectors,
			     BOOM::Vector< complex<double> > &eigenvalues,
			     bool sort)
{
  GSL::Vector real, imag;
  getEigenVectors(vectors,real,imag,sort);
  int n=M->size1;
  for(int i=0 ; i<n ; ++i)
    eigenvalues.push_back(complex<double>(real[i],imag[i]));
}



void Matrix::getEigenVectors(GSL::Matrix &vectors,GSL::Vector &values,
			     GSL::Vector &imagParts,bool sort) const
{
  // Convert to TNT format
  int n=M->size1;
  TNT::Array2D<double> *tA=convertToTNT(), tVectors(n,n);
  TNT::Array1D<double> tRealValues(n), tImagValues(n);

  // Compute eigenvalues and eigenvectors using TNT/JAMA
  JAMA::Eigenvalue<double> E(*tA);
  E.getV(tVectors);
  E.getRealEigenvalues(tRealValues);
  E.getImagEigenvalues(tImagValues);
  delete tA;

  // Convert back to our preferred classes
  vectors=tVectors;
  values=tRealValues;
  imagParts=tImagValues;

  // Sort eigenvalues & eigenvectors
  if(sort) sortEigen(vectors,values);
}



void Matrix::sortEigen(GSL::Matrix &vectors,GSL::Vector &values)
{
  // First, compute a permutation array using QuickSort
  BOOM::Vector<double> v;
  int n=values.getDim();
  v.resize(n);
  for(int i=0 ; i<n ; ++i) v[i]=values[i];
  BOOM::DirectComparator<double> cmp;
  BOOM::VectorSorter<double> sorter(v,cmp);
  BOOM::Vector<int> &permutation=*sorter.sortDescendByIndex();

  // Apply the permutation to the eigenvalues
  GSL::Vector t=values;
  for(int i=0 ; i<n ; ++i) 
    values[i]=t[permutation[i]];

  // Apply the permutation to the eigenvectors
  GSL::Matrix m=vectors;
  GSL::Vector ev(n);
  for(int i=0 ; i<n ; ++i)
    {
      m.getColumn(permutation[i],ev);
      vectors.setColumn(i,ev);
    }

  // Clean up
  delete &permutation;
}



void Matrix::sortEigenAbs(GSL::Matrix &vectors,GSL::Vector &values)
{
  throw "sortEigenAbs -- under construction";
}


 
TNT::Array2D<double> *Matrix::convertToTNT() const
{
  int rows=M->size1, cols=M->size2;
  TNT::Array2D<double> &t=*new TNT::Array2D<double>(rows,cols);
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      t[i][j]=gsl_matrix_get(M,i,j);
  return &t;
}



GSL::Matrix &Matrix::operator=(const TNT::Array2D<double> &t)
{
  int rows=t.dim1(), cols=t.dim2();
  resize(rows,cols);
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      gsl_matrix_set(M,i,j,t[i][j]);
  return *this;
}



void Matrix::applyThreshold(double epsilon)
{
  Matrix &self=*this;
  int rows=M->size1, cols=M->size2;
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      if(fabs(self(i,j))<epsilon) self(i,j)=0.0;
}



void Matrix::add(const GSL::Matrix &other,GSL::Matrix &result) const
{
  const Matrix &self=*this;
  int rows=M->size1, cols=M->size2;
  if(result.M->size1!=rows || result.M->size2!=cols)
    result.resize(rows,cols);
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      result(i,j)=self(i,j)+other(i,j);
}



void Matrix::subtract(const GSL::Matrix &other,GSL::Matrix &result) const
{
  const Matrix &self=*this;
  int rows=M->size1, cols=M->size2;
  if(result.M->size1!=rows || result.M->size2!=cols)
    result.resize(rows,cols);
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      result(i,j)=self(i,j)-other(i,j);
}



void Matrix::scale(double factor)
{
  Matrix &self=*this;
  int rows=M->size1, cols=M->size2;
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      self(i,j)*=factor;
}



void Matrix::power(int power,GSL::Matrix &result)
{
    if(power<1) throw "Matrix::power() : power must be greater than zero";
    if(power==1) result=*this;
    else
    {
        GSL::Matrix temp=*this;
        int n=power-2;
        for(int i=0 ; i<n ; ++i)
        {
            times(temp,result);
            temp=result;
        }
        times(temp,result);
    }
}



void Matrix::load(istream &is)
{
    Matrix &self=*this;
    int rows, columns;
    is>>rows>>columns;
    resize(rows,columns);
    for(int row=0 ; row<rows ; ++row)
        for(int col=0 ; col<columns ; ++col)
            is>>self(row,col);
}



void Matrix::load(const BOOM::String &filename)
{
    ifstream is(filename.c_str());
    if(!is.good()) throw BOOM::String("Error opening file ")+filename;
    load(is);
}



istream &GSL::operator>>(istream &is,GSL::Matrix &m)
{
    m.load(is);
    return is;
}



void Matrix::convertToLogs()
{
  Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  for(int i=0 ; i<m ; ++i)
    for(int j=0 ; j<n ; ++j)
      self(i,j)=log(self(i,j));
}



void Matrix::multiplyInLogSpace(const GSL::Vector &by,
				GSL::Vector &result) const
{
  const Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  if(by.getDim()!=n) 
    throw "Matrix::multiplyInLogSpace(): vector is not of proper size";
  if(result.getDim()!=n) result.resize(n);
  BOOM::Array1D<double> logProbs(n);
  for(int i=0 ; i<m ; ++i) {
    for(int j=0 ; j<n ; ++j)
      logProbs[j]=self(i,j)+by[j];
    result[i]=BOOM::sumLogProbs<double>(logProbs);
  }
}



void Matrix::swapCells(int row1,int col1,int row2,int col2)
{
  Matrix &self=*this;
  double &a=self(row1,col1), &b=self(row2,col2);
  double temp=a;
  a=b;
  b=temp;
}



void Matrix::findDependence(int &rowA,int &rowB,double &ratioVariance) const
{
  const Matrix &self=*this;
  int m=getNumRows(), n=getNumColumns();
  rowA=rowB=-1;
  ratioVariance=BOOM::POSITIVE_INFINITY;
  for(int i=0 ; i<m ; ++i)
    for(int j=i+1 ; j<m ; ++j) {
      BOOM::Vector<double> V;
      bool nonzero=false;
      for(int k=0 ; k<n ; ++k) {
	double a=self(i,k), b=self(j,k);
	if(b!=0) {
	  double ratio=a/b;
	  if(ratio!=0) nonzero=true;
	  V.push_back(ratio);
	}
      }
      if(!nonzero) continue;
      BOOM::SummaryStats SS(V);
      double var=SS.getVar(), mean=SS.getMean();
      var/=mean;
      if(rowA<0 || var<ratioVariance) {
	rowA=i;
	rowB=j;
	ratioVariance=var;
      }
    }
}



void Matrix::symmetrize(void)
{
  const int dim=getNumRows();
  if(dim!=getNumColumns()) throw "Matrix is not square, in symmetrize()";
  GSL::Matrix T(dim,dim), sum(dim,dim);
  transpose(T);
  add(T,sum);
  *this=sum;
  scale(0.5);
}



int Matrix::SVD(GSL::Matrix &U,GSL::Matrix &S_matrix,GSL::Matrix &Vt) const
{
  const int dim1=getNumRows(), dim2=getNumColumns();
  GSL::Vector work(dim2), S(dim2);
  Vt.resize(dim2,dim2);
  U=*this;
  int retval=gsl_linalg_SV_decomp(U.peek(),Vt.peek(),S.peek(),work.peek());
  Vt.transpose();
  S_matrix.resize(dim2,dim2);
  S_matrix.setAllTo(0);
  for(int i=0 ; i<dim2 ; ++i) S_matrix(i,i)=S[i];
  return retval;
}



int Matrix::cholesky(GSL::Matrix &L_Lt) const
{
  gsl_error_handler_t *handler=gsl_set_error_handler_off();
  L_Lt=*this;
  int retval=gsl_linalg_cholesky_decomp(L_Lt.peek());
  gsl_set_error_handler(handler);
  return retval;
}



int Matrix::bilmesCholesky(GSL::Matrix &L,GSL::Matrix &diag,GSL::Matrix &Lt) 
  const
{
  // This function factors a positive definite matrix A into A=LDL', where
  // D is a diagonal of positive values and L is a unit lower triangular
  // (meaning that its diagonal contains only 1's).  Used by Jeff Bilmes for
  // partially tying of sparse inverse covariance matrices.

  // Perform cholesky
  const int errval=cholesky(L);

  // Zero out upper triangle
  const int n=getNumRows();
  for(int i=0 ; i<n ; ++i)
    for(int j=i+1 ; j<n ; ++j)
      L(i,j)=0.0;

  // Factor out the diagonal
  diag.resize(n,n);
  diag.setAllTo(0.0);
  for(int i=0 ; i<n ; ++i) {
    const double d=L(i,i);
    diag(i,i)=d*d;
    for(int j=i ; j<n ; ++j) {
      L(j,i)/=d; // yes, I mean (j,i), not (i,j)!
    }
  }

  // Compute transpose (technically, this should be the conjugate transpose)
  L.transpose(Lt);

  return errval;
}



int Matrix::LU(GSL::Matrix &LU,GSL::Permutation &P,int &sign,int &det) const
{
  LU=*this;
  int retval=gsl_linalg_LU_decomp(LU.peek(),P.peek(),&sign);
  //gsl_linalg_LU_invert(LU.peek(),P.peek(),inv.peek());
  det=gsl_linalg_LU_det(LU.peek(),sign);
  return retval;
}


/*
int Matrix::QR(GSL::Matrix &Q,GSL::Matrix &R) const
{
  R=*this;
  const int m=getNumRows(), n=getNumColumns();
  GSL::Vector tau(min(m,n));
  int retval=gsl_linalg_QR_decomp(R.peek(),tau.peek());
  
  ...need to unpack R and form the Q matrix...

  return retval;
}
*/



double Matrix::getConditionNumber() const
{
  GSL::Matrix U, S, Vt;
  SVD(U,S,Vt);
  GSL::Vector singularValues;
  S.getDiagonal(singularValues);
  double first=singularValues[0];
  double maximum=first, minimum=first;
  int n=singularValues.getDim();
  for(int i=1 ; i<n ; ++i) 
    if(singularValues[i]<minimum) minimum=singularValues[i];
    else if(singularValues[i]>maximum) maximum=singularValues[i];
  return ::sqrt(maximum-minimum);
}



bool Matrix::isSquare() const
{
  return getNumRows()==getNumColumns();
}



double Matrix::determinantDiag() const
{
  const int dim=getNumRows();
  if(!isSquare()) throw "GSL::Matrix::determinantDiag(): not square matrix";
  const Matrix &self=*this;
  double det=1;
  for(int i=0 ; i<dim ; ++i)
    det*=self(i,i);
  return det;
}



double Matrix::logDeterminantDiag() const
{
  const int dim=getNumRows();
  if(!isSquare()) throw "GSL::Matrix::determinantDiag(): not square matrix";
  const Matrix &self=*this;
  double det=0;
  for(int i=0 ; i<dim ; ++i)
    det+=log(self(i,i));
  return det;
}



int Matrix::tridiag(GSL::Matrix &U,GSL::Matrix &D,GSL::Matrix &U_transpose) 
  const
{
  Matrix A=*this;
  const int dim=getNumRows();
  GSL::Vector tau(dim), diag(dim), subdiag(dim);
  int errval=gsl_linalg_symmtd_decomp(A.peek(),tau.peek());
  gsl_linalg_symmtd_unpack(A.peek(),tau.peek(),U.peek(),diag.peek(),
			   subdiag.peek());
  D.setAllTo(0.0);
  for(int i=0 ; i<dim ; ++i) D(i,i)=diag[i];
  U.transpose(U_transpose);
  return errval;
}



void Matrix::covToCor(GSL::Matrix &cor,
		      GSL::Vector &sd) const
{
  const Matrix &self=*this;
  const int D=getNumRows();
  cor.resize(D,D);
  sd.resize(D);
  for(int d=0 ; d<D ; ++d) sd[d]=::sqrt(self(d,d));
  for(int c=0 ; c<D ; ++c)
    for(int d=0 ; d<D ; ++d)
      cor(c,d)=self(c,d)*sd[c]*sd[d];
}



void Matrix::rebuildCovMatrix(const GSL::Matrix &cor,
			      const GSL::Vector &sd)
{
  Matrix &self=*this;
  const int D=getNumRows();
  for(int c=0 ; c<D ; ++c)
    for(int d=0 ; d<D ; ++d)
      self(c,d)=cor(c,d)/(sd[c]*sd[d]);
}



const GSL::Matrix &Matrix::operator+=(const GSL::Matrix &other)
{
  gsl_matrix_add(M,other.M);
}


