/*

   Copyright (c) 2012, The Chinese University of Hong Kong

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

#include "operation.hpp"

void operation::addition_task(const size_t index, const path& p, const size_t seed)
{
	// Initialize a Mersenne Twister random number generator.
	mt19937eng eng(seed);

	// Initialize random number generators for obtaining a random fragment and a random elitist.
	using boost::random::variate_generator;
	using boost::random::uniform_int_distribution;
	variate_generator<mt19937eng&, uniform_int_distribution<size_t>> uniform_elitist_gen(eng, uniform_int_distribution<size_t>(0, num_elitists - 1));
	variate_generator<mt19937eng&, uniform_int_distribution<size_t>> uniform_fragment_gen(eng, uniform_int_distribution<size_t>(0, num_fragments - 1));

	// Create a child ligand by addition.
	do
	{
		// Obtain references to the two parent ligands.
		ligand& l1 = ligands[uniform_elitist_gen()];
		ligand l2 = ligand_flyweight(fragments[uniform_fragment_gen()]);
		while (!(l1.addition_feasible() && l2.addition_feasible()))
		{
			l1 = ligands[uniform_elitist_gen()];
			l2 = ligand_flyweight(fragments[uniform_fragment_gen()]);
		}

		// Obtain a random mutable atom from the two parent ligands respectively.
		const size_t g1 = variate_generator<mt19937eng&, uniform_int_distribution<size_t>>(eng, uniform_int_distribution<size_t>(0, l1.mutable_atoms.size() - 1))();
		const size_t g2 = variate_generator<mt19937eng&, uniform_int_distribution<size_t>>(eng, uniform_int_distribution<size_t>(0, l2.mutable_atoms.size() - 1))();

		ligands.replace(index, new ligand(p, l1, l2, g1, g2));
		if (v(ligands[index]))
		{
			// Save the newly created child ligand.
			ligands[index].save();
			return;
		}
	} while (++num_failures < max_failures);
}

void operation::subtraction_task(const size_t index, const path& p, const size_t seed)
{
	// Initialize a Mersenne Twister random number generator.
	mt19937eng eng(seed);

	// Initialize random number generators for obtaining a random fragment and a random elitist.
	using boost::random::variate_generator;
	using boost::random::uniform_int_distribution;
	variate_generator<mt19937eng&, uniform_int_distribution<size_t>> uniform_elitist_gen(eng, uniform_int_distribution<size_t>(0, num_elitists - 1));

	// Create a child ligand by subtraction.
	do
	{
		// Obtain reference to the parent ligand.
		ligand& l1 = ligands[uniform_elitist_gen()];
		while (!l1.subtraction_feasible())
		{
			l1 = ligands[uniform_elitist_gen()];
		}

		// Obtain a random mutable atom from the two parent ligands respectively.
		const size_t g1 = variate_generator<mt19937eng&, uniform_int_distribution<size_t>>(eng, uniform_int_distribution<size_t>(1, l1.num_rotatable_bonds))();

		ligands.replace(index, new ligand(p, l1, g1));
		if (v(ligands[index]))
		{
			// Save the newly created child ligand.
			ligands[index].save();
			return;
		}
	} while (++num_failures < max_failures);
}

void operation::crossover_task(const size_t index, const path& p, const size_t seed)
{
	// Initialize a Mersenne Twister random number generator.
	mt19937eng eng(seed);

	// Initialize random number generators for obtaining a random fragment and a random elitist.
	using boost::random::variate_generator;
	using boost::random::uniform_int_distribution;
	variate_generator<mt19937eng&, uniform_int_distribution<size_t>> uniform_elitist_gen(eng, uniform_int_distribution<size_t>(0, num_elitists - 1));

	// Create a child ligand by crossover.
	do
	{
		// Obtain constant references to the two parent ligands.
		ligand& l1 = ligands[uniform_elitist_gen()];
		ligand& l2 = ligands[uniform_elitist_gen()];
		while (!(l1.crossover_feasible() && l2.crossover_feasible()))
		{
			l1 = ligands[uniform_elitist_gen()];
			l2 = ligands[uniform_elitist_gen()];
		}

		// Obtain a random mutable atom from the two parent ligands respectively.
		const size_t g1 = variate_generator<mt19937eng&, uniform_int_distribution<size_t>>(eng, uniform_int_distribution<size_t>(1, l1.num_rotatable_bonds))();
		const size_t g2 = variate_generator<mt19937eng&, uniform_int_distribution<size_t>>(eng, uniform_int_distribution<size_t>(1, l2.num_rotatable_bonds))();

		ligands.replace(index, new ligand(p, l1, l2, g1, g2, true));
		if (v(ligands[index]))
		{
			// Save the newly created child ligand.
			ligands[index].save();
			return;
		}
	} while (++num_failures < max_failures);
}
