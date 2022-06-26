///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "RTTI.hpp"

namespace Langulus::RTTI
{

	/// Schwarz counter pattern																
	/// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter			
	/// Zero initialized at load time, guaranteed										
	static int NiftyCounter;

	/// Memory for the meta database instance												
	static ::std::aligned_storage_t<sizeof(Interface), alignof(Interface)> InstanceBuffer;

	/// The global handle shall point to the buffer										
	Interface& Instance = reinterpret_cast<Interface&> (InstanceBuffer);

	/// Initialize database together with the first include of the header		
	Inner::InterfaceInitializer::InterfaceInitializer() {
		if (NiftyCounter++ == 0)
			new (&Instance) Interface();
	}

	/// Destroy database with the last destruction of the initializer				
	Inner::InterfaceInitializer::~InterfaceInitializer() {
		if (--NiftyCounter == 0)
			Instance.~Interface();
	}

	/// Database construction																	
	Interface::Interface() {

	}

	/// Database destruction																	
	Interface::~Interface() { }

} // namespace Langulus::RTTI
