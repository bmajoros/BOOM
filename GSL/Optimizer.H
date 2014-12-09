/****************************************************************
 Optimizer.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Optimizer_H
#define INCL_Optimizer_H
#include <iostream>
#include <gsl/gsl_multimin.h>
#include "Vector.H"
#include "../String.H"
using namespace std;

namespace GSL {

/****************************************************************
              (1) Choose optimization algorithm
 */
enum OptimizerType
  {
  // These require gradients:
    FLETCHER_REEVES,  // Fletcher-Reeves conjugate gradient
    POLAK_RIBIERE,    // Polak-Ribiere conjugate gradient
    BFGS,             // Broyden-Fletcher-Goldfarb-Shanno conjugate gradient
    STEEPEST_DESCENT, // inefficient!
    STEEP_DESC_CONSTR,// steepest descent, but with boundary constraints
    L_BFGS_B,         // Limited-memory BFGS with Boundary constraints
    
  // This does NOT require gradients:
    SIMPLEX           // Nelder-Mead
  };
ostream &operator<<(ostream &,OptimizerType);
OptimizerType stringToOptimizerType(const BOOM::String &);

enum StoppingCriterion
  {
    BY_THRESHOLD,
    BY_ITERATION_COUNT,
    BY_EITHER
  };
ostream &operator<<(ostream &,StoppingCriterion);
StoppingCriterion stringToStoppingCriterion(const BOOM::String &);


/****************************************************************
        (2)  Overload virtual functions for your objective
 */
struct ObjectiveFunction
{
  virtual double f(const GSL::Vector &currentPoint)=0;
  virtual void gradient(const GSL::Vector &currentPoint,
			GSL::Vector &gradient) {}
};



/****************************************************************
              (3) Minimize the objective function
 */

// THIS CLASS IS FOR *UNCONSTRAINED* OPTIMIZATION...SEE BELOW FOR
// CONSTRAINED OPTIMIZATION

class Optimizer // unconstrained
{
public:
  Optimizer(OptimizerType,
	    ObjectiveFunction &,
	    const GSL::Vector &initialPoint,
	    double stepSize=0.01,
	    StoppingCriterion=BY_EITHER,
	    double tolerance=0.1,
	    double gradientThreshold=1.0,
	    int maxIterations=100);
  virtual bool run();
  const GSL::Vector &getOptimalPoint();
  size_t iterationsUsed();
protected:
  OptimizerType type;
  ObjectiveFunction &f;
  StoppingCriterion criterion;
  double tolerance; // see GSL documentation; usually affects line search
  double stepSize;
  double gradientThreshold; // how small the gradient's norm has to become
  size_t iterations;
  int maxIterations;
  int dimension;
  GSL::Vector initialPoint;
  GSL::Vector optimalPoint;
  const gsl_multimin_fdfminimizer_type *algorithm;
  
  void setAlgorithm();
  
  // Methods for my "linear" optimizer:
  bool linear();
  bool validGradient(const GSL::Vector &gradient) const;
  double lineMinimization(const GSL::Vector &initialPoint,
			  double initialValue,
			  double minStepSize,
			  const GSL::Vector &gradient,
			  GSL::Vector &newPoint);
};
    


//--------------------------------------------------------
// THIS CLASS IS FOR *CONSTRAINED* OPTIMIZATION
class ConstrainedOptimizer : public Optimizer
{
public:
  ConstrainedOptimizer(OptimizerType,
		       ObjectiveFunction &,
		       const GSL::Vector &initialPoint,
		       const GSL::Vector &lowerBounds,
		       const GSL::Vector &upperBounds,
		       double stepSize=0.01,
		       StoppingCriterion=BY_EITHER,
		       double gradientThreshold=1,
		       int maxIterations=100);
  virtual bool run();
  double getOptimalValue() const {return optimalValue;}
  double getInitialValue() const {return initialValue;}
protected:
  GSL::Vector lowerBounds, upperBounds;
  double optimalValue, initialValue;

  // Methods for steepest-descent:
  bool steepestDescent();
  double lineMinimization(const GSL::Vector &initialPoint,
			  double initialValue,
			  double minStepSize,
			  const GSL::Vector &gradient,
			  GSL::Vector &newPoint);
  bool checkBounds(GSL::Vector &);

  // Methods and data for L-BFGS-B:
  bool lbfgsb();
};
    
    
}
#endif

