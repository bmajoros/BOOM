/****************************************************************
 Optimizer.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Optimizer.H"
#include "BOOM/Progress.H"
#include "BOOM/Constants.H"
#include "BOOM/Exceptions.H"
#include <gsl/gsl_errno.h>
#include "L_BFGS_B.H"
using namespace std;
using namespace GSL;


double private_f(const gsl_vector *v,void *params)
{
  ObjectiveFunction *f=reinterpret_cast<ObjectiveFunction*>(params);
  return f->f(GSL::Vector(v));
}



void private_df(const gsl_vector *v,void *params,gsl_vector *df)
{
  ObjectiveFunction *f=reinterpret_cast<ObjectiveFunction*>(params);
  GSL::Vector vWrapper(v), dfWrapper(df);
  f->gradient(vWrapper,dfWrapper);
}



void private_fdf(const gsl_vector *x,void *params,double *f,gsl_vector *df)
{
  *f=private_f(x,params);
  private_df(x,params,df);
}



Optimizer::Optimizer(OptimizerType type,
		     ObjectiveFunction &f,
		     const GSL::Vector &initialPoint,
		     double stepsize,
		     StoppingCriterion criterion,
		     double tolerance,
		     double gradientThreshold,
		     int maxIterations)
  : type(type),
    f(f),
    criterion(criterion),
    stepSize(stepsize),
    tolerance(tolerance),
    gradientThreshold(gradientThreshold),
    maxIterations(maxIterations),
    algorithm(NULL),
    initialPoint(initialPoint),
    iterations(0)
{
  dimension=initialPoint.getDim();
  setAlgorithm();
}



void Optimizer::setAlgorithm()
{
  switch(type)
    {
    case FLETCHER_REEVES:
      algorithm=gsl_multimin_fdfminimizer_conjugate_fr;
      break;
    case POLAK_RIBIERE:
      algorithm=gsl_multimin_fdfminimizer_conjugate_pr;
      break;
    case BFGS:
      algorithm=gsl_multimin_fdfminimizer_vector_bfgs;
      break;
    case STEEPEST_DESCENT:
      algorithm=gsl_multimin_fdfminimizer_steepest_descent;
      break;
    case SIMPLEX:
      throw "SIMPLEX is not implemented yet; sorry about that.";
      //algorithm=gsl_multimin_fminimizer_nmsimplex;
      break;
    }
}



bool Optimizer::run()
{
  if(type==STEEP_DESC_CONSTR) throw "should not happen";
    
  bool success=true;
  gsl_multimin_function_fdf func;
  func.f=&private_f;
  func.df=&private_df;
  func.fdf=&private_fdf;
  func.n=dimension;
  func.params=reinterpret_cast<void*>(&f);

  gsl_multimin_fdfminimizer *s=
    gsl_multimin_fdfminimizer_alloc(algorithm,dimension);

  gsl_multimin_fdfminimizer_set(s,&func,initialPoint.peek(),stepSize,
				tolerance);

  iterations=0;
  bool keepGoing=true;
  BOOM::Progress progress;
  if(criterion==BY_ITERATION_COUNT) progress.start(maxIterations);
  while(keepGoing)
    {
      int status=gsl_multimin_fdfminimizer_iterate(s);
      //cout<<"STATUS="<<gsl_strerror(status)<<endl;
      ++iterations;
      switch(status)
      {
          case GSL_SUCCESS:
              break;
          case GSL_CONTINUE:
              break;
          case GSL_EUNDRFLW:
          case GSL_EOVRFLW:
              success=false;
              keepGoing=false;
              continue;
          case GSL_ENOPROG: // no progress (means convergence...?)
              success=true;
              keepGoing=false;
              continue;
          default:
              cout<<"GSL error: "<<gsl_strerror(status)<<endl;
              continue;
      }

      switch(criterion)
	{
	case BY_THRESHOLD:
	  status=gsl_multimin_test_gradient(s->gradient,gradientThreshold);
	  if(status==GSL_SUCCESS) keepGoing=false;
	  break;
	case BY_ITERATION_COUNT:
	  if(iterations>=maxIterations) keepGoing=false;
	  //cerr<<progress.getProgress(iterations)<<endl;
	  break;
	case BY_EITHER:
	  if(iterations>=maxIterations) keepGoing=false;
	  else
	    {
	      status=gsl_multimin_test_gradient(s->gradient,gradientThreshold);
	      if(status==GSL_SUCCESS) keepGoing=false;
	    }
	  break;
	}
    }
  optimalPoint=GSL::Vector(s->x);
  gsl_multimin_fdfminimizer_free(s);
  return success;
}



const GSL::Vector &Optimizer::getOptimalPoint()
{
  return optimalPoint;
}



ostream &operator<<(ostream &os,OptimizerType type)
{
  switch(type)
    {
    case FLETCHER_REEVES:
      os<<"FLETCHER_REEVES";
      break;
    case POLAK_RIBIERE:
      os<<"POLAK_RIBIERE";
      break;
    case BFGS:
      os<<"BFGS";
      break;
    case STEEPEST_DESCENT:
      os<<"STEEPEST_DESCENT";
      break;
    case SIMPLEX:
      os<<"SIMPLEX";
      break;
    case STEEP_DESC_CONSTR:
      os<<"STEEP_DESC_CONSTR";
      break;
    case L_BFGS_B:
      os<<"L_BFGS_B";
      break;
    }
}



ostream &operator<<(ostream &os,StoppingCriterion criterion)
{
  switch(criterion)
    {
    case BY_THRESHOLD:
      os<<"BY_THRESHOLD";
      break;
    case BY_ITERATION_COUNT:
      os<<"BY_ITERATION_COUNT";
      break;
    case BY_EITHER:
      os<<"BY_EITHER";
      break;
    }
}



StoppingCriterion GSL::stringToStoppingCriterion(const BOOM::String &s)
{
    if(s=="BY_THRESHOLD") return BY_THRESHOLD;
    if(s=="BY_ITERATION_COUNT") return BY_ITERATION_COUNT;
    if(s=="BY_EITHER") return BY_EITHER;
    throw BOOM::String("Invalid stopping criterion: ")+s;
}



size_t Optimizer::iterationsUsed()
{
  return iterations;
}



OptimizerType GSL::stringToOptimizerType(const BOOM::String &s)
{
  if(s=="FLETCHER_REEVES") return FLETCHER_REEVES;
  if(s=="POLAK_RIBIERE") return POLAK_RIBIERE;
  if(s=="BFGS") return BFGS;
  if(s=="STEEPEST_DESCENT") return STEEPEST_DESCENT;
  if(s=="SIMPLEX") return SIMPLEX;
  if(s=="STEEP_DESC_CONSTR") return STEEP_DESC_CONSTR;
  if(s=="L_BFGS_B") return L_BFGS_B;
  throw BOOM::String("Unknown OptimizerType: ")+s;
}



/********************************************************************
  MY SIMPLE "LINEAR" OPTIMIZATION ALGORITHM

  proc minimize(initialPoint,minStepSize) is:
     currentPoint=initialPoint;
     for i=1 up to maxIterations do
        grad=df(currentPoint);
        if(not finite(grad)) then break;
        newPoint=lineMinimization(currentPoint,minStepSize,grad);
        if(newPoint==currentPoint) then break;
        currentPoint=newPoint;
     return currentPoint;
  end.

  proc lineMinimization(initialPoint,minStepSize,grad) is:
     stepSize=minStepSize;
     sign=1;
     currentPoint=initialPoint;
     currentCost=f(currentPoint);
     while(true) do
        newPoint=currentPoint+stepSize*grad;
        newCost=f(newPoint);
        if(newCost<currentCost) then
           currentPoint=newPoint;
           currentCost=newCost;
           stepSize*=2;  // or: stepSize+=sign*minStepSize;
        else
           if(|stepSize|==minStepSize) then break;
           sign=-sign;
           stepSize=sign*minStepSize;
     return currentPoint;
  end.

 ********************************************************************/

bool Optimizer::linear() {
    GSL::Vector currentPoint=initialPoint, grad, newPoint;
    double value=f.f(initialPoint);
    for(int i=0 ; i<maxIterations ; ++i) {
        f.gradient(currentPoint,grad);
        if(!validGradient(grad)) break;
        if(grad.norm()<=gradientThreshold) break;
        double newValue=
	  lineMinimization(currentPoint,value,stepSize,grad,newPoint);
        if(newValue==value) break;
        currentPoint=newPoint;
	value=newValue;
    }
    optimalPoint=currentPoint;
    return true;
}



double Optimizer::lineMinimization(const GSL::Vector &initialPoint,
				   double initialCost,
				   double minStepSize,
				   const GSL::Vector &gradient,
				   GSL::Vector &currentPoint) {
    double sign=-1.0;
    double stepSize=sign*minStepSize;
    currentPoint=initialPoint;
    double currentCost=initialCost; //f.f(currentPoint);
    int steps=0;
    while(true) {
        GSL::Vector step=gradient, newPoint;
        step.scale(stepSize);
        currentPoint.add(step,newPoint);
        double newCost=f.f(newPoint);
        if(BOOM::isFinite(newCost) && newCost<currentCost) {
            currentPoint=newPoint;
            currentCost=newCost;
            stepSize*=2; // or: stepSize+=sign*minStepSize;
            ++steps;
        }
        else {
            if(steps==0) break;
	    steps=0;
            sign=-sign;
            stepSize=sign*minStepSize;
        }
    }
    return currentCost;
}



bool Optimizer::validGradient(const GSL::Vector &gradient) const {
    int n=gradient.getDim();
    for(int i=0 ; i<n ; ++i)
        if(!BOOM::isFinite(gradient[i])) return false;
    return true;
}



//=======================================================================


ConstrainedOptimizer::ConstrainedOptimizer(OptimizerType type,
					   ObjectiveFunction &f,
					   const GSL::Vector &initialPoint,
					   const GSL::Vector &lowerBounds,
					   const GSL::Vector &upperBounds,
					   double stepsize,
					   StoppingCriterion criterion,
					   double threshold,
					   int maxIterations)
  : Optimizer(type,f,initialPoint,stepsize,criterion,threshold,threshold,
	      maxIterations),
    lowerBounds(lowerBounds),
    upperBounds(upperBounds)
{
}



bool ConstrainedOptimizer::run() 
{
  switch(type) {
    case STEEP_DESC_CONSTR:
      return steepestDescent();
    case L_BFGS_B:
      return lbfgsb();
  }
}



bool ConstrainedOptimizer::steepestDescent()
{
  GSL::Vector currentPoint=initialPoint, grad, newPoint;
  double value=initialValue=f.f(initialPoint);
  for(iterations=0 ; iterations<maxIterations ; ++iterations) {
    f.gradient(currentPoint,grad);
    if(!validGradient(grad)) break;
    double newValue=
      lineMinimization(currentPoint,value,stepSize,grad,newPoint);
    if(newValue==value) break;
    double deltaScore=value-newValue;
    currentPoint=newPoint;
    value=newValue;
    if(deltaScore<tolerance) break;
  }
  optimalPoint=currentPoint;
  optimalValue=value;
  return true;
}



double ConstrainedOptimizer::lineMinimization(const GSL::Vector &initialPoint,
					      double initialCost,
					      double minStepSize,
					      const GSL::Vector &gradient,
					      GSL::Vector &currentPoint) {
  double sign=-1.0;
  double stepSize=sign*minStepSize;
  double currentCost=initialCost;
  currentPoint=initialPoint;
  if(!checkBounds(currentPoint)) currentCost=f.f(currentPoint);
  int steps=0;
  while(true) {
    GSL::Vector step=gradient, newPoint;
    step.scale(stepSize);
    currentPoint.add(step,newPoint);
    checkBounds(newPoint);
    double newCost=f.f(newPoint);
    if(BOOM::isFinite(newCost) && newCost<currentCost) {
      currentPoint=newPoint;
      currentCost=newCost;
      stepSize*=2; // or: stepSize+=sign*minStepSize;
      ++steps;
    }
    else {
      if(steps==0) break;
      steps=0;
      sign=-sign;
      stepSize=sign*minStepSize;
    }
  }
  return currentCost;
}



bool ConstrainedOptimizer::checkBounds(GSL::Vector &v) {
  bool ok=true;
  for(int i=0 ; i<dimension ; ++i) {
    double a=lowerBounds[i], b=upperBounds[i], &c=v[i];
    if(c<a) {c=a; ok=false;}
    else if(c>b) {c=b; ok=false;}
  }
  return false;
}



bool ConstrainedOptimizer::lbfgsb() {
  /*
   bool ALGLIB_lbfgsb(GSL::ObjectiveFunction *f,GSL::Vector &initialPoint,
		   GSL::Vector &lowerBounds,GSL::Vector &upperBounds,
		   GSL::Vector &optimalPoint,int maxIterations,int M,
		   double epsg=0.01,double epsf=0.01,double epsx=0.01)

   */

  int M=dimension; // num corrections in BFGS' Hessian update
  if(M>7) M=7; 
  double epsg=0.01;
  double epsf=0.01;
  double epsx=0.01;
  optimalValue=0; // ### ALGLIB doesn't provide a way to get this...

  return ALGLIB_lbfgsb(&f,initialPoint,lowerBounds,upperBounds,
		       optimalPoint,maxIterations,M,epsg,epsf,epsx);
}



