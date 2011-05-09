/*
 * SolverGPBiCG.h
 *
 *  Created on: Jul 24, 2009
 *      Author: goto
 */

#ifndef SOLVERGPBICG_H_
#define SOLVERGPBICG_H_

#include "Solver.h"

namespace pmw
{

class CSolverGPBiCG: public pmw::CSolver
{
public:
	CSolverGPBiCG(int iter_max, double tolerance,
			uint method, uint precondition,
			bool flag_iter, bool flag_time);
	virtual ~CSolverGPBiCG();
private:
	int doSolve(const CAssyMatrix *pA, const CAssyVector *pB, CAssyVector *pX,
			int iter_max, double tolerance,
			bool flag_iter_log, bool flag_time_log);
};

}

#endif /* SOLVERGPBICG_H_ */
