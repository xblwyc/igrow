/*

   Copyright (c) 2011, The Chinese University of Hong Kong

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 */

#pragma once
#ifndef IGROW_LIGAND_HPP
#define IGROW_LIGAND_HPP

#include <boost/filesystem/path.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
#include <boost/flyweight/no_tracking.hpp>
#include "atom.hpp"

namespace igrow
{

// Choose the appropriate Mersenne Twister engine for random number generation on 32-bit or 64-bit platform.
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__) || defined(_M_X64) || defined(_M_AMD64)
	typedef boost::random::mt19937_64 mt19937eng;
#else
	typedef boost::random::mt19937 mt19937eng;
#endif

	/// Represents a ROOT or a BRANCH in PDBQT structure.
	class frame
	{
	public:
		size_t parent; ///< Frame array index pointing to the parent of current frame. For ROOT frame, this field is not used.
		size_t rotorX; ///< Index pointing to the parent frame atom which forms a rotatable bond with the first atom of current frame, a.k.a. rotor Y.
		vector<size_t> branches; ///< Child branches.
		vector<atom> atoms; ///< Heavy atoms.

		/// Constructs a frame, and relates it to its parent frame.
		explicit frame(const size_t parent) : parent(parent)
		{
			branches.reserve(5); // A frame typically consists of <= 5 branch frames.
			atoms.reserve(20); // A frame typically consists of <= 20 atoms.
		}
		
		/// Copy constructor.
		frame(const frame& f) : parent(f.parent), rotorX(f.rotorX), branches(f.branches), atoms(f.atoms) {}
		
		/// Move constructor.
		frame(frame&& f) : parent(f.parent), rotorX(f.rotorX), branches(static_cast<vector<size_t>&&>(f.branches)), atoms(static_cast<vector<atom>&&>(f.atoms)) {}
	};

	/// Represents the index to a hydrogen or a halogen together with the index to its neighbor heavy atom.
	class mutation_point
	{
	public:
		size_t frame; ///< The index to the frame to which the mutation point belongs.
		size_t point; ///< The index to a mutation point, e.g. hydrogen or halogen.
		size_t neighbor; ///< The index to the neighbor of the current mutation point.

		explicit mutation_point(const size_t frame, const size_t point, const size_t neighbor) : frame(frame), point(point), neighbor(neighbor) {}
	};
	
	using boost::filesystem::path;

	// Represents a ligand.
	class ligand
	{
	public:
		const path p; ///< The path to the fragment.
		vector<frame> frames; ///< Ligand frames.
		vector<mutation_point> mutation_points; ///< Hydrogens or halogens.
		size_t num_heavy_atoms; ///< Number of heavy atoms.
		size_t num_hb_donors; ///< Number of hydrogen bond donors.
		size_t num_hb_acceptors; ///< Number of hydrogen bond acceptors.
		fl mw; ///< Molecular weight.		
		fl logp; ///< Predicted LogP obtained by external XLOGP3.
		fl free_energy; ///< Predicted free energy obtained by external docking.
		fl efficacy; ///< Ligand efficacy

		ligand() {}
		explicit ligand(const path& p);
		
		/// Saves the current ligand to a file in pdbqt format.
		void save(const path& p) const;

		/// Mutates the current ligand.
		ligand* mutate(const ligand& other, const mt19937eng& eng) const;

		/// Recalculates ligand efficacy, defined as free_energy / num_heavy_atoms.
		void evaluate_efficacy();

		/// For sorting ptr_vector<ligand>.
		const bool operator<(const ligand& lig) const
		{
			return efficacy < lig.efficacy;
		}
	};

	/// For extracting the path out of a ligand.
	class ligand_path_extractor
	{
	public:
		const path& operator()(const ligand& lig) const
		{
			return lig.p;
		}
	};

	/// Define flyweight type for ligand.
	using namespace boost::flyweights;
	typedef	flyweight<key_value<path, ligand, ligand_path_extractor>, no_tracking> ligand_flyweight;
}

#endif
