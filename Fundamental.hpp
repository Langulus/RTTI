///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
/// Include this only when building standalone											
#pragma once
#include "Reflection.hpp"

namespace Langulus::A
{

	///																								
	/// The following abstract types are implicitly added as bases					
	/// when reflecting fundamental types. They are linked to their				
	/// corresponding concepts, so you can use them at runtime, to check if		
	/// a type is compatible with the given concept via type->CastsTo				
	///																								

	/// Check if a type is compatible with CT::Number									
	/// concept at runtime, via meta->InterpretsAs<ANumber>							
	class Number {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::Langulus::Real;
		~Number() = delete;
	};

	/// Check if a type is compatible with CT::Integer									
	/// concept at runtime, via meta->InterpretsAs<AInteger>							
	class Integer {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::std::intptr_t;
		LANGULUS_BASES(Number);
		~Integer() = delete;
	};

	/// Check if a type is compatible with CT::Signed									
	/// concept at runtime, via meta->InterpretsAs<ASigned>							
	class Signed {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::Langulus::Real;
		LANGULUS_BASES(Number);
		~Signed() = delete;
	};

	/// Check if a type is compatible with IsUnsigned									
	/// concept at runtime, via meta->InterpretsAs<AUnsigned>						
	class Unsigned {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::std::uintptr_t;
		LANGULUS_BASES(Number);
		~Unsigned() = delete;
	};

	/// Check if a type is compatible with CT::UnsignedInteger concept at		
	/// runtime, via meta->InterpretsAs<AUnsignedInteger>								
	class UnsignedInteger {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::std::uintptr_t;
		LANGULUS_BASES(Unsigned, Integer);
		~UnsignedInteger() = delete;
	};

	/// Check if a type is compatible with CT::Real										
	/// concept at runtime, via meta->InterpretsAs<AReal>								
	class Real {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::Langulus::Real;
		LANGULUS_BASES(Signed);
		~Real() = delete;
	};

	/// Check if a type is compatible with CT::SignedInteger							
	/// concept at runtime, via meta->InterpretsAs<ASignedInteger>					
	class SignedInteger {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) ::std::intptr_t;
		LANGULUS_BASES(Signed, Integer);
		~SignedInteger() = delete;
	};

	/// Check if a type is compatible with CT::Character								
	/// concept at runtime, via meta->InterpretsAs<AText>								
	class Text {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) char8_t;
		~Text() = delete;
	};

	/// Check if a type is compatible with CT::Bool										
	/// concept at runtime, via meta->InterpretsAs<ABool>								
	class Bool {
		LANGULUS(ABSTRACT) true;
		LANGULUS(CONCRETIZABLE) bool;
		~Bool() = delete;
	};

} // namespace Langulus::A
