///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include <Langulus.RTTI.hpp>
#include <cstdint>
#include <cstddef>
#include <vector>

namespace Langulus::Flow
{
	class Verb {};
}

namespace Langulus::Anyness
{
	class Block {};
}

namespace Langulus::Anyness
{
	class Trait : public Block {};
}

using namespace Langulus;
using namespace Langulus::RTTI;

//#define LANGULUS_STD_BENCHMARK

#define CATCH_CONFIG_ENABLE_BENCHMARKING

using uint = unsigned int;
template<class T>
using some = std::vector<T>;

namespace One::Two::Three
{
	struct TypeDeepIntoNamespaces;

	template<class T>
	struct TemplatedTypeDeepIntoNamespaces;

	template<class T>
	struct VeryComplexTemplate;
}

namespace Verbs
{
	///																								
	/// A testing verb, similar to the ones used in Langulus::Flow					
	///																								
	struct Create : public Flow::Verb {
		LANGULUS(POSITIVE_VERB) "Create";
		LANGULUS(NEGATIVE_VERB) "Destroy";
		LANGULUS(INFO)
			"Used for allocating new elements. "
			"If the type you're creating has	a producer, "
			"you need to execute the verb in a matching producer, "
			"or that producer will be created automatically for you, if possible";

		/// Check if the verb is available in a type, and with given arguments	
		///	@return true if verb is available in T with arguments A...			
		template<CT::Data T, CT::Data... A>
		static constexpr bool AvailableFor() noexcept {
			if constexpr (sizeof...(A) == 0)
				return requires (T & t, Verb & v) { t.Create(v); };
			else
				return requires (T & t, Verb & v, A... a) { t.Create(v, a...); };
		}

		/// Get the verb functor for the given type and arguments					
		///	@return the function, or nullptr if not available						
		template<CT::Data T, CT::Data... A>
		static constexpr auto Of() noexcept {
			if constexpr (!Create::AvailableFor<T, A...>()) {
				return nullptr;
			}
			else if constexpr (CT::Constant<T>) {
				return [](const void* context, Flow::Verb& verb, A... args) {
					auto typedContext = static_cast<const T*>(context);
					typedContext->Create(verb, args...);
				};
			}
			else {
				return [](void* context, Flow::Verb& verb, A... args) {
					auto typedContext = static_cast<T*>(context);
					typedContext->Create(verb, args...);
				};
			}
		}

		template<CT::Data T>
		static bool ExecuteIn(T&, Verb&);

		static bool ExecuteDefault(const Anyness::Block&, Verb&) {
			return true;
		}

		static bool ExecuteDefault(Anyness::Block&, Verb&) {
			return false;
		}

		static bool ExecuteStateless(Verb&) {
			return false;
		}
	};
}

class ImplicitlyReflectedData {};

class ImplicitlyReflectedDataWithTraits : public ImplicitlyReflectedData {
public:
	int member;
	bool anotherMember;

	inline operator int() const noexcept {
		return 0;
	}

	void Create(Flow::Verb& v) {
		
	}

	LANGULUS(NAME) "MyType";
	LANGULUS(INFO) "Info about MyType";
	LANGULUS(FILES) "txt, pdf";
	LANGULUS(VERSION_MAJOR) 2;
	LANGULUS(VERSION_MINOR) 1;
	LANGULUS(DEEP) true;
	LANGULUS(POD) true;
	LANGULUS(NULLIFIABLE) true;
	LANGULUS(POOL_TACTIC) PoolTactic::Size;
	LANGULUS(CONCRETIZABLE) ImplicitlyReflectedData;
	LANGULUS(UNINSERTABLE) true;
	LANGULUS(ALLOCATION_PAGE) 250;
	LANGULUS(ABSTRACT) true;
	LANGULUS_PROPERTY(member);
	LANGULUS_PROPERTY(anotherMember);
	LANGULUS_BASES(ImplicitlyReflectedData);
	LANGULUS_VERBS(Verbs::Create);
	LANGULUS_CONVERSIONS(int);
};