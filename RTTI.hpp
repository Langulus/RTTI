///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Config.hpp"
#include <unordered_map>
#include <unordered_set>

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
		using Lowercase = ::std::string;
		using MetaList = ::std::unordered_set<const Meta*>;
		static Lowercase ToLowercase(const Token&) noexcept;
		static Token ToLastToken(const Token&) noexcept;

		Interface();
		~Interface();

		// Database for meta data definitions										
		::std::unordered_map<Lowercase, DMeta> mMetaData;
		// Database for meta trait definitions										
		::std::unordered_map<Lowercase, TMeta> mMetaTraits;
		// Database for meta verb definitions										
		::std::unordered_map<Lowercase, VMeta> mMetaVerbs;
		::std::unordered_map<Lowercase, VMeta> mMetaVerbsAlt;
		// Database for ambiguous tokens												
		::std::unordered_map<Lowercase, MetaList> mMetaAmbiguous;

		void RegisterAmbiguous(const Token&, const Meta*) noexcept;
		void UnregisterAmbiguous(const Token&, const Meta*) noexcept;

	public:
		NOD() DMeta GetMetaData(const Token&) const noexcept;
		NOD() TMeta GetMetaTrait(const Token&) const noexcept;
		NOD() VMeta GetMetaVerb(const Token&) const noexcept;
		NOD() const MetaList& GetAmbiguousMeta(const Token&) const noexcept;

		NOD() DMeta RegisterData(const Token&) noexcept;
		NOD() TMeta RegisterTrait(const Token&) noexcept;
		NOD() VMeta RegisterVerb(const Token&, const Token&) noexcept;

		void Unregister(DMeta) noexcept;
		void Unregister(TMeta) noexcept;
		void Unregister(VMeta) noexcept;
	};

	///																								
	/// The global RTTI instance, initialized via a schwarz counter				
	///																								
	extern Interface& Database;

} // namespace Langulus::RTTI
