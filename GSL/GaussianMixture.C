/****************************************************************
 GaussianMixture.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GaussianMixture.H"
#include "Random.H"
#include "BOOM/Constants.H"
#include "BOOM/SumLogProbs.H"
using namespace std;
using namespace BOOM;
using namespace GSL;

GaussianMixture::GaussianMixture(int numComponents)
  : coef(numComponents), logCoef(numComponents), distr(numComponents)
{
  zero();
}



GaussianMixture::GaussianMixture(const GaussianMixture &other)
  : coef(other.coef), logCoef(other.logCoef), distr(other.distr)
{
  // copy ctor
}



void GaussianMixture::zero()
{
  coef.setAllTo(0.0);
}



int GaussianMixture::getNumComponents() const
{
  return coef.size();
}



void GaussianMixture::resize(int numComponents)
{
  coef.resize(numComponents);
  logCoef.resize(numComponents);
  distr.resize(numComponents);
  zero();
}



double GaussianMixture::getCoef(int i) const
{
  return coef[i];
}



double GaussianMixture::getLogCoef(int i) const
{
  return logCoef[i];
}



MultiGauss &GaussianMixture::getDistr(int i)
{
  return distr[i];
}



void GaussianMixture::setCoef(int i,double d)
{
  coef[i]=d;
  logCoef[i]=log(d);
}



void GaussianMixture::setDistr(int i,const MultiGauss &d)
{
  distr[i]=d;
}



double GaussianMixture::density(const GSL::Vector &v) const
{
  double val=0.0;
  int n=getNumComponents();
  for(int i=0 ; i<n ; ++i) {
    double c=coef[i];
    if(c>0.0) val+=c*distr[i].density(v);
  }
  return n>0 ? val : 1.0;
}



double GaussianMixture::logDensity(const GSL::Vector &v) const
{
  int n=getNumComponents();
  BOOM::Vector<double> V;
  for(int i=0 ; i<n ; ++i) {
    double c=logCoef[i];
    if(isFinite(c)) {
      double density=distr[i].logDensity(v);
      V.push_back(c+density);
    }
  }
  return n>0 ? sumLogProbs(V) : 0.0;
}



int GaussianMixture::mostProbableComponent(const GSL::Vector &v,
					   double &bestScore) const
{
  bestScore=0.0;
  int n=getNumComponents(), best=0;
  for(int i=0 ; i<n ; ++i) {
    double score=coef[i]*distr[i].density(v);
    if(score>bestScore) {
      bestScore=score;
      best=i;
    }
  }
  return best;
}



void GaussianMixture::normalizeCoefs()
{
  int n=getNumComponents();
  double sum=0.0;
  for(int i=0 ; i<n ; ++i) sum+=coef[i];
  for(int i=0 ; i<n ; ++i) coef[i]/=sum;
  initLogCoef();
}



void GaussianMixture::initLogCoef()
{
  int n=getNumComponents();
  logCoef.resize(n);
  for(int i=0 ; i<n ; ++i) logCoef[i]=log(coef[i]);
}



void GaussianMixture::save(ostream &os) const
{
  os<<coef<<endl;
  int n=distr.size();
  for(int i=0 ; i<n ; ++i)
    distr[i].save(os);
}



void GaussianMixture::load(istream &is)
{
  is>>coef;
  int n=coef.size();
  distr.resize(n);
  for(int i=0 ; i<n ; ++i)
    distr[i].load(is);
  initLogCoef();
}



void GaussianMixture::sample(GSL::Vector &v) const
{
  // First, select a random component
  double r=Random::random0to1();
  int n=getNumComponents(), i;
  for(i=0 ; i<n ; ++i) 
    if((r-=coef[i])<=0.0) break;
  if(i>=n) i=n-1;

  // Now sample a point from that component
  distr[i].sample(v);
}



void GaussianMixture::printOn(ostream &os) const
{
  int n=getNumComponents();
  for(int i=0 ; i<n ; ++i)
    os<<"component #"<<i<<": coef="<<coef[i]<<" distr="<<distr[i]<<endl;
}



MultivariateDistribution *GaussianMixture::clone() const
{
  return new GaussianMixture(*this);
}



void GaussianMixture::addVariate()
{
  int n=getNumComponents();
  for(int i=0 ; i<n ; ++i) distr[i].addVariate(0,1);
}



void GaussianMixture::addComponent()
{
  int n=coef.size();
  coef.safeResize(n+1);
  logCoef.safeResize(n+1);
  distr.safeResize(n+1);
  coef[n]=0;
  logCoef[n]=NEGATIVE_INFINITY;
  if(n>0) distr[n]=distr[n-1];
}



void GaussianMixture::dropVariate(int whichVariate)
{
  int n=distr.size();
  for(int i=0 ; i<n ; ++i) 
    distr[i].dropVariate(whichVariate);
}


