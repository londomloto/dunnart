/*
 * $Revision: 1.17 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2008-01-17 22:56:41 +0100 (Thu, 17 Jan 2008) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of CrossingMinimization Module, an interface for crossing minimization algorithms
 * 
 * \author Markus Chimani
 * 
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2007
 * 
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 * 
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * \par
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 * 
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#ifndef OGDF_CROSSING_MINIMIZATION_MODULE_H
#define OGDF_CROSSING_MINIMIZATION_MODULE_H



#include <ogdf/planarity/PlanRep.h>
#include <ogdf/planarity/PlanarModule.h>
#include <ogdf/basic/Module.h>
#include <ogdf/basic/Logger.h>
#include <ogdf/basic/Timeouter.h>


namespace ogdf {

/**
 * \brief Interface for crossing minimization algorithms.
 *
 */
class OGDF_EXPORT CrossingMinimizationModule : public Module, public Timeouter
{
public:
	//! Initializes a crossing minimization module.
	CrossingMinimizationModule() { }

	// destruction
	virtual ~CrossingMinimizationModule() { }

	/**
	 * \brief Computes a planarized representation of the input graph.
	 *
	 * @param PG represents the input graph as well as the computed planarized
	 *        representation after the call. \a PG has to be initialzed as a
	 *        PlanRep of the input graph and is modified to obatain the planarized
	 *        representation (crossings are replaced by dummy vertices with degree
	 *        four.
	 * @param cc is the number of the connected component in \a PG that is considered.
	 * @param crossingNumber is assigned the number of crossings.
	 * @param forbid points to an edge array indicating which edges are not allowed
	 *        to be crossed, i.e., (*forbid)[e] = true. If forbid = 0, no edges are
	 *        forbidden.
	 * @param cost points to an edge array that gives the cost of each edge. If cost
	 *        = 0, all edges have cost 1.
	 * @param subgraphs
	 * \return the status of the result.
	 */
	ReturnType call(PlanRep &PG,
			int cc,
			int&  crossingNumber,
			const EdgeArray<int>  * cost = 0,
			const EdgeArray<bool> * forbid = 0, 
			const EdgeArray<unsigned int> * subgraphs = 0)
	{
		m_useCost = (cost != 0);
		m_useForbid = (forbid != 0);
		m_useSubgraphs = (subgraphs != 0);

		if(!useCost())      cost      = OGDF_NEW EdgeArray<int> (PG.original(), 1);
		if(!useForbid())    forbid    = OGDF_NEW EdgeArray<bool> (PG.original(), 0);
		if(!useSubgraphs()) subgraphs = OGDF_NEW EdgeArray<unsigned int> (PG.original(), 1);

		ReturnType R = doCall(PG, cc, *cost, *forbid, *subgraphs, crossingNumber);

		if(!useCost())      delete cost;
		if(!useForbid())    delete forbid;
		if(!useSubgraphs()) delete subgraphs;
		return R;
	};

	//! Computes a planarized representation of the input graph (shorthand for call)
	ReturnType operator()(PlanRep &PG,
			int cc,
			int&  crossingNumber,
			const EdgeArray<int>  * cost = 0,
			const EdgeArray<bool> * forbid = 0, 
			const EdgeArray<unsigned int> * const subgraphs = 0) {
		return call(PG, cc, crossingNumber, cost, forbid, subgraphs);
	};

	//! Returns true iff edge costs are given.
	bool useCost() const { return m_useCost; }

	//! Returns true iff forbidden edges are given.
	bool useForbid() const { return m_useForbid; }

	bool useSubgraphs() const { return m_useSubgraphs; }

protected:
	/**
	 * \brief Actual algorithm call that needs to be implemented by derived classed.
	 *
	 * @param PG represents the input graph as well as the computed planarized
	 *        representation after the call. \a PG is initialzed as a
	 *        PlanRep of the input graph and has to be modified so that it represents
	 *        the planarized representation (crossings are replaced by dummy vertices
	 *        with degree four).
	 * @param cc is the number of the connected component in \a PG that is considered.
	 * @param crossingNumber is assigned the number of crossings.
	 * @param forbid points to an edge array indicating which edges are not allowed
	 *        to be crossed, i.e., (*forbid)[e] = true.
	 * @param cost points to an edge array that gives the cost of each edge.
	 * @param subgraphs
	 * \return the status of the result.
	 */
	virtual ReturnType doCall(PlanRep &PG,
		int cc,
		const EdgeArray<int>  &cost,
		const EdgeArray<bool> &forbid, 
		const EdgeArray<unsigned int> &subgraphs, 
		int& crossingNumber) = 0;
		
	OGDF_MALLOC_NEW_DELETE
	
private:
	bool m_useCost; //!< True iff edge costs are given.
	bool m_useForbid; //!< True iff forbidden edges are given.
	bool m_useSubgraphs;
};

} // end namespace ogdf

#endif
