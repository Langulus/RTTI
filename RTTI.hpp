///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include <Langulus.Core.hpp>
#include <stack>
#include <list>
#include <string_view>
#include <string>

namespace Langulus::RTTI
{

	namespace Inner
	{
		/// Static initializer for every translation unit								
		static struct InterfaceInitializer {
			InterfaceInitializer();
			~InterfaceInitializer();
		} InterfaceInitializerInstance;
	}


	///																								
	///	The RTTI interface																	
	///																								
	class Interface {
	friend class Inner::InterfaceInitializer;
	private:
		Interface();
		~Interface();
	};

	///																								
	/// The global RTTI instance, initialized via a schwarz counter				
	///																								
	extern Interface& Instance;

} // namespace Langulus::RTTI
