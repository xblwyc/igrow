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

#ifndef IGROW_TEE_HPP
#define IGROW_TEE_HPP

#include <iostream>
#include <iomanip>
#include "file.hpp"

namespace igrow
{
	/// Represents a log with both stdout and a custom log file as output.
	class tee
	{
	public:
		ofile file; ///< Custom log file.

		/// Constructs a log and sets up the floating point format.
		explicit tee(const path& p) : file(p)
		{
			using namespace std;
			cout.setf(ios::fixed, ios::floatfield);
			file.setf(ios::fixed, ios::floatfield);
		}

		/// Logs a generic left-value reference.
		template<typename T>
		tee& operator<<(const T& x)
		{
			std::cout << x;
			file << x;
			return *this;
		}

		/// Logs a generic right-value reference.
		template<typename T>
		tee& operator<<(T&& x)
		{
			std::cout << x;
			file << x;
			return *this;
		}
	};
}

#endif