/****************************************************************
 MultiGauss.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 Inspired by open source C version by Ralph dos Santos Silva.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include <complex>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include "MultiGauss.H"
#include "Random.H"
#include "BOOM/Constants.H"
using namespace std;
using namespace GSL;


MultiGauss::MultiGauss()
  : MultivariateDistribution(0,MVD_GAUSS), diagonalOnly(false)
{
  // default ctor
}



MultiGauss::MultiGauss(const MultiGauss &other)
  : MultivariateDistribution(other.dim,MVD_GAUSS),
    means(other.means), covarianceMatrix(other.covarianceMatrix),
    cholesky(other.cholesky), Cinv(other.Cinv), det(other.det),
    diagonalOnly(other.diagonalOnly)
{
  // copy ctor
}


MultiGauss::MultiGauss(const GSL::Vector &means,
		       const GSL::Matrix &covarianceMatrix,
		       bool diagonal)
  : MultivariateDistribution(means.getDim(),MVD_GAUSS), means(means), 
    covarianceMatrix(covarianceMatrix), cholesky(dim,dim),
    Cinv(dim,dim), diagonalOnly(diagonal)
{
  if(diagonalOnly) initAux_diagonal();
  else initAux();
}



void MultiGauss::initAux()
{
  gsl_error_handler_t *handler=gsl_set_error_handler_off();

  // Enforce symmetry
  //covarianceMatrix.symmetrize();

  // Auxiliary variables for random number generation:
  gsl_matrix_memcpy(cholesky.peek(),covarianceMatrix.peek());
  if(gsl_linalg_cholesky_decomp(cholesky.peek())==GSL_EDOM) {
    GSL::Vector work(dim), S(dim);
    GSL::Matrix V(dim,dim);
    gsl_matrix_memcpy(cholesky.peek(),covarianceMatrix.peek());
    gsl_linalg_SV_decomp(cholesky.peek(),V.peek(),S.peek(),work.peek());
    GSL::Matrix diag(dim,dim), cov(dim,dim);
    diag.setAllTo(0.0);
    for(int i=0 ; i<dim ; ++i) {
      diag(i,i)=S[i];
      if(S[i]<0) {
	cout<<"WARNING: negative singular value in covariance matrix: "<<S[i]<<endl;
    cout<<" *** HINT: try adding Gaussian noise ***\n";
	CHECKPOINT;
      }
    }
    GSL::Matrix tmp;
    cholesky.times(diag,tmp);
    cholesky.transpose();
    tmp.times(cholesky,cov);
    gsl_matrix_memcpy(cholesky.peek(),cov.peek());
    if(gsl_linalg_cholesky_decomp(cholesky.peek())==GSL_EDOM) {
      cout<<"WARNING: can't complete cholesky decomposition for covariance matrix"<<endl;
    cout<<" *** HINT: try adding Gaussian noise ***\n";
      cout<<"cholesky="<<cholesky<<endl;
      cout<<"cov="<<covarianceMatrix<<endl;
      CHECKPOINT;
      //INTERNAL_ERROR;
    }
    else {
      /*
      cout<<"WARNING: replacing cov matrix with symmetrized version"<<endl;
      cout.precision(4);
      cout<<covarianceMatrix<<endl;
      exit(0);
      GSL::Matrix diff(dim,dim);
      cov.subtract(covarianceMatrix,diff);
      cout<<diff<<endl;
      covarianceMatrix=cov;//###
      */
    }
  }

  // Auxiliary variables for computing densities:
  int sign;
  GSL::Matrix LU=covarianceMatrix;
  gsl_permutation *perm=gsl_permutation_alloc(dim);

  int err=gsl_linalg_LU_decomp(LU.peek(),perm,&sign);
  if(err) {
    cout<<"LU-decomposition of covariance matrix failed\n";
    cout<<" *** HINT: try adding Gaussian noise ***\n";
    INTERNAL_ERROR;
  }
  det=gsl_linalg_LU_det(LU.peek(),sign);
  if(fabsl(det)==0.0) {
    cout<<"ERROR: covariance matrix has zero determinant"<<endl;
    cout<<" *** HINT: try adding Gaussian noise ***\n";
    cout<<"det="<<det<<endl;
    cout<<"LU="<<LU<<endl;
    cout<<"sign="<<sign<<endl;
    int n=covarianceMatrix.getNumRows();
    double min=covarianceMatrix(0,0);
    for(int i=1 ; i<n ; ++i) 
      if(covarianceMatrix(i,i)<min) min=covarianceMatrix(i,i);
    cout<<"min diagonal element: "<<min<<endl;
    cout<<"diagonals: ";
    for(int i=0 ; i<n ; ++i) cout<<covarianceMatrix(i,i)<<" ";
    cout<<endl;
    if(n<6) cout<<"matrix=\n"<<covarianceMatrix<<endl;
    throw "Noninvertible covariance matrix in MultiGauss";
  }
  gsl_linalg_LU_invert(LU.peek(),perm,Cinv.peek());
  gsl_set_error_handler(handler);
  detFactor=sqrt(det*pow(2.0*M_PI,dim));
  GSL::Matrix vectors;
  GSL::Vector values, imag;
  covarianceMatrix.getEigenVectors(vectors,values,imag,false);
  cout.precision(3);
  cout.precision(10);

  //if(covarianceMatrix.getConditionNumber()>30) cout<<"WARNING!  MULTICOLLINEARITY DETECTED -- CN="<<covarianceMatrix.getConditionNumber()<<endl;
  int n=values.getDim();
  complex<double> cLogDet(0.0,0.0);
  bool neg=false;
  for(int i=0 ; i<n ; ++i) {
    complex<double> c(values[i],imag[i]);
    cLogDet+=log(c);
    if(values[i]<0) neg=true;
  }

  if(neg) {
    cout<<"ERROR: negative determinant\n";
    cout<<" *** HINT: try adding Gaussian noise ***\n";
    cout.precision(4);
    cout<<"BEFORE: "<<covarianceMatrix<<endl;
    cout<<"CinvB4="<<Cinv<<endl;
    cout<<"EIGEN="<<values<<endl;
    cout.precision(10);

    GSL::Vector var;
    covarianceMatrix.getDiagonal(var);
    GSL::Matrix varprod, Cor;
    var.matrixProduct(var,varprod);
    varprod.sqrt();
    covarianceMatrix.divideComponentwise(varprod,Cor);
    Cor.getEigenVectors(vectors,values,imag,false);

    GSL::Matrix &V=vectors, Vt, D(dim,dim), VD, VDV;
    D.setAllTo(0.0);
    for(int i=0 ; i<dim ; ++i) 
      D(i,i)=values[i]>0 ? values[i] : 1e-10;
    V.transpose(Vt);
    V.times(D,VD);
    VD.times(Vt,VDV);
    GSL::Vector S(dim);
    for(int i=0 ; i<dim ; ++i)
      S[i]=1/sqrt(VDV(i,i));
    for(int i=0 ; i<dim ; ++i)
      for(int j=0 ; j<dim ; ++j)
	Cor(i,j)=VDV(i,j);//### *S[i]*S[j];
    Cor.hadamard(varprod,covarianceMatrix);
    covarianceMatrix.symmetrize();

    GSL::Matrix LU=covarianceMatrix;
    gsl_linalg_LU_decomp(LU.peek(),perm,&sign);
    gsl_linalg_LU_invert(LU.peek(),perm,Cinv.peek());
    det=gsl_linalg_LU_det(LU.peek(),sign);//###this is giving wrong values
    cout<<"det="<<det<<" Cinv="<<Cinv<<endl;
    covarianceMatrix.getEigenVectors(vectors,values,imag,false);
    if(covarianceMatrix.getConditionNumber()>30) cout<<"WARNING!  MULTICOLLINEARITY DETECTED -- CN="<<covarianceMatrix.getConditionNumber()<<endl;
    int n=values.getDim();
    cLogDet=complex<double>(0.0,0.0);
    neg=false;
    if(n<0) INTERNAL_ERROR
    for(int i=0 ; i<n ; ++i) {
      if(values[i]<0) continue;
      complex<double> c(values[i],imag[i]);
      cLogDet+=log(c);
      if(values[i]<0) neg=true;
    }
    det=exp(cLogDet.real());
    detFactor=sqrt(det*pow(2.0*M_PI,dim));
    if(neg) cout<<"det still negative"<<endl;
    cout<<"EIGEN="<<values<<endl;
    cout<<"AFTER: "<<covarianceMatrix<<endl;
    cout.precision(10);
    int rowA, rowB; double ratioVar;
    covarianceMatrix.findDependence(rowA,rowB,ratioVar);
    cout<<"DEPENDENCE: "<<rowA<<" "<<rowB<<" "<<ratioVar<<endl;

    // Compute inverse of ill-conditioned matrix (Povey's procedure)
    {
    double maxEigen=values[0];
    for(int i=1 ; i<dim ; ++i) if(values[i]>maxEigen) maxEigen=values[i];
    int K=30; // ###
    double minEigen=maxEigen/K;
    GSL::Matrix A, vectorsT, V(dim,dim), VV;
    V.setAllTo(0.0);
    for(int i=0 ; i<dim ; ++i) V(i,i)=max(values[i],minEigen);
    vectors.times(V,VV);
    vectors.transpose(vectorsT);
    VV.times(vectorsT,A);
    GSL::Matrix U, S, Vt, US;
    A.SVD(U,S,Vt);
    U.transpose(Vt);
    for(int i=0 ; i<dim ; ++i) S(i,i)=1/S(i,i);
    U.times(S,US);
    US.times(Vt,Cinv);
    cout<<"POVEY MATRIX: "<<Cinv<<endl;
    GSL::Matrix I;
    Cinv.times(covarianceMatrix,I);
    Cinv.invert(covarianceMatrix);
    covarianceMatrix.getEigenVectors(vectors,values,imag,false);
    int n=values.getDim();
    cLogDet=complex<double>(0.0,0.0);
    for(int i=0 ; i<n ; ++i) {
      //if(values[i]<0) continue;
      complex<double> c(values[i],imag[i]);
      cLogDet+=log(c);
    }
    det=exp(cLogDet.real());
    detFactor=sqrt(det*pow(2.0*M_PI,dim));
    cout<<"POVEY'S EIGENVALUES: "<<values<<endl;
    }
  }
  if(det<0) {cout<<"EIGN="<<values<<" IMAG="<<imag<<endl;INTERNAL_ERROR;}
  logDetFactor=0.5*(cLogDet.real()+dim*log(2*M_PI));
  if(!isFinite(logDetFactor)) {cout<<"eigen="<<values<<endl;INTERNAL_ERROR;}
  gsl_permutation_free(perm);
}



void MultiGauss::load(istream &is)
{
  is>>means>>covarianceMatrix;
  dim=means.getDim();
  cholesky.resize(dim,dim);
  Cinv.resize(dim,dim);
  initAux();
}



void MultiGauss::save(ostream &os) const
{
  os<<means<<endl;
  os<<covarianceMatrix<<endl;
}



void MultiGauss::printOn(ostream &os) const
{
  os<<"means: "<<means<<endl;
  os<<"covariance matrix: "<<covarianceMatrix;
}



MultivariateDistribution *MultiGauss::clone() const
{
  return new MultiGauss(*this);
}



void MultiGauss::sample(GSL::Vector &result) const
{
  if(result.getDim()!=dim) result.resize(dim);

  for(int i=0; i<dim; ++i) result[i]=gsl_ran_ugaussian(generator);
  gsl_blas_dtrmv(CblasLower,CblasNoTrans,CblasNonUnit,
		 cholesky.peek(),result.peek());
  gsl_vector_add(result.peek(),means.peek());
}



double MultiGauss::density(const GSL::Vector &x) const
{
  if(dim==0) return 0;
  if(dim==1 && !isFinite(x[0])) return 1; // ### MISSING DATA
  double prod=D_Cinv_Dt(x);
  double density=exp(-prod/2)/detFactor;
  return density;
}



double MultiGauss::D_Cinv_Dt(const GSL::Vector &x) const
{
  GSL::Vector D(dim), CinvD(dim);
  x.sub(means,D);
  Cinv.multiply(D,CinvD);
  double prod=CinvD.dotProduct(D);
  return prod;
}


double MultiGauss::logDensity(const GSL::Vector &x) const
{
  if(dim==0) return NEGATIVE_INFINITY;
  if(dim==1 && !isFinite(x[0])) return 0; // inf denotes missing data
  if(diagonalOnly) return logDensity_diag(x);
  double prod=D_Cinv_Dt(x);
  double logDens=-prod/2-logDetFactor;
  return logDens;
}



double MultiGauss::logDensity_diag(const GSL::Vector &x) const
{
  GSL::Vector D(dim);
  x.sub(means,D);
  double prod=0;
  for(int i=0 ; i<dim ; ++i) {
    double xi=D[i];
    prod+=CinvDiag[i]*xi*xi;
  }
  double logDens=-prod/2-logDetFactor;
  return logDens;
}



void MultiGauss::initAux_diagonal()
{
  // Auxiliary variables for random number generation:
  if(covarianceMatrix.cholesky(cholesky)==GSL_EDOM) {
    GSL::Matrix U, S, Vt, cov(dim,dim);
    covarianceMatrix.SVD(U,S,Vt);
    GSL::Matrix tmp;
    covarianceMatrix.times(S,tmp);
    covarianceMatrix.transpose(cholesky);
    tmp.times(cholesky,cov);
    if(cov.cholesky(cholesky)==GSL_EDOM) INTERNAL_ERROR;
  }

  // Auxiliary variables for computing densities:
  covarianceMatrix.invertDiagonal(Cinv);
  GSL::Matrix vectors;
  GSL::Vector values, imag;
  covarianceMatrix.getEigenVectors(vectors,values,imag,false);
  cout.precision(3);
  cout.precision(10);
  int n=values.getDim();
  complex<double> cLogDet(0.0,0.0);
  bool neg=false;
  for(int i=0 ; i<n ; ++i) {
    complex<double> c(values[i],imag[i]);
    cLogDet+=log(c);
    if(values[i]<0) neg=true;
  }
  if(neg) {
    cout<<"NEGATIVE EIGENVALUE DETECTED: "<<values<<endl;
    CHECKPOINT;
  }
  logDetFactor=0.5*(cLogDet.real()+dim*log(2*M_PI));
  CinvDiag.resize(dim);
  for(int i=0 ; i<dim ; ++i) CinvDiag[i]=Cinv(i,i);
}



void MultiGauss::addVariate(float mean,float variance)
{
  GSL::Vector oldMeans=means;
  GSL::Matrix oldCov=covarianceMatrix;
  int oldN=oldMeans.getDim();
  int N=oldN+1;
  means.resize(N);
  covarianceMatrix.resize(N,N);
  for(int i=0 ; i<oldN ; ++i) means[i]=oldMeans[i];
  means[oldN]=mean;
  covarianceMatrix.setAllTo(0);
  for(int i=0 ; i<oldN ; ++i)
    for(int j=0 ; j<oldN ; ++j)
      covarianceMatrix(i,j)=oldCov(i,j);
  covarianceMatrix(oldN,oldN)=variance;
  ++dim;
  if(diagonalOnly) initAux_diagonal();
  else initAux();
}



MultiGauss *MultiGauss::dropVariate(const MultiGauss &source,int whichVariate)
{
  int D=source.means.getDim();
  const int newD=D-1;
  GSL::Vector means(newD);
  for(int i=0 ; i<newD ; ++i)
    means[i]=source.means[i<whichVariate ? i : i+1];
  GSL::Matrix cov(newD,newD);
  for(int i=0 ; i<newD ; ++i) {
    int fi=i<whichVariate ? i : i+1;
    for(int j=0 ; j<newD ; ++j) {
      int fj=j<whichVariate ? j : j+1;
      cov(i,j)=source.covarianceMatrix(fi,fj);
    }
  }
  return new MultiGauss(means,cov);
}



void MultiGauss::dropVariate(const MultiGauss &source,int whichVariate,
			     MultiGauss &into)
{
  MultiGauss *G=dropVariate(source,whichVariate);
  into=*G;
  delete G;
}



void MultiGauss::dropVariate(int whichVariate)
{
  dropVariate(*this,whichVariate,*this);
}








