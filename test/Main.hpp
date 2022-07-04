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

class ImplicitlyReflectedVerb : public Flow::Verb {
public:
	template<class T>
	static OverrideList Of() {
		return {};
	}
};

class ImplicitlyReflectedData {};

class ImplicitlyReflectedDataWithTraits : public ImplicitlyReflectedData {
public:
	int member;
	bool anotherMember;

	inline operator int() const noexcept {
		return 0;
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
	LANGULUS_VERBS(ImplicitlyReflectedVerb);
	LANGULUS_CONVERSIONS(int);
};