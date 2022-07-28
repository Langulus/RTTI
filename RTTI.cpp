///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Reflection.hpp"
#include "RTTI.hpp"
#include <algorithm>

namespace Langulus::RTTI
{

	/// Schwarz counter pattern																
	/// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter			
	/// Zero initialized at load time, guaranteed										
	static int NiftyCounter;

	/// Memory for the meta database instance												
	static ::std::aligned_storage_t<sizeof(Interface), alignof(Interface)> InstanceBuffer;

	/// The global handle shall point to the buffer										
	Interface& Database = reinterpret_cast<Interface&> (InstanceBuffer);

	/// Initialize database together with the first include of the header		
	Inner::InterfaceInitializer::InterfaceInitializer() {
		if (NiftyCounter++ == 0)
			new (&Database) Interface();
	}

	/// Destroy database with the last destruction of the initializer				
	Inner::InterfaceInitializer::~InterfaceInitializer() {
		if (--NiftyCounter == 0)
			Database.~Interface();
	}

	/// Database construction																	
	Interface::Interface() {

	}

	/// Database destruction																	
	Interface::~Interface() {
		for (auto& pair : mMetaData)
			delete pair.second;
		for (auto& pair : mMetaTraits)
			delete pair.second;
		for (auto& pair : mMetaVerbs)
			delete pair.second;
	}

	/// Convert a token to a lowercase string												
	///	@param token - the token to lowercase											
	///	@return the lowercase string														
	typename Interface::Lowercase Interface::ToLowercase(const Token& token) noexcept {
		Lowercase lc {token};
		::std::transform(lc.begin(), lc.end(), lc.begin(),
			[](unsigned char c) { return ::std::tolower(c); }
		);
		return Move(lc);
	}

	/// Get the last, most relevant part of a token that my or may not have		
	/// namespaces in it. Essentially finds last "::" and skip forward to that	
	///	@param token - the token to scan													
	///	@return the last token																
	Token Interface::ToLastToken(const Token& token) noexcept {
		const auto found = token.find_last_of(':', token.size());
		if (found != token.npos)
			return token.substr(found + 1, token.size() - found - 1);
		return token;
	}

	/// Get an existing meta data definition by its token								
	///	@param token - the token of the data definition								
	///	@return the definition, or nullptr if not found								
	DMeta Interface::GetMetaData(const Token& token) const noexcept {
		const auto lc = ToLowercase(token);
		const auto found = mMetaData.find(lc);
		if (found == mMetaData.end())
			return nullptr;
		return found->second;
	}

	/// Get an existing meta trait definition by its token							
	///	@param token - the token of the trait definition							
	///	@return the definition, or nullptr if not found								
	TMeta Interface::GetMetaTrait(const Token& token) const noexcept {
		const auto lc = ToLowercase(token);
		const auto found = mMetaTraits.find(lc);
		if (found == mMetaTraits.end())
			return nullptr;
		return found->second;
	}

	/// Get an existing meta verb definition by its token								
	///	@param token - the token of the verb definition								
	///						you can search by positive, as well as negative token	
	///	@return the definition, or nullptr if not found								
	VMeta Interface::GetMetaVerb(const Token& token) const noexcept {
		const auto lc = ToLowercase(token);
		auto found = mMetaVerbs.find(lc);
		if (found != mMetaVerbs.end())
			return found->second;

		found = mMetaVerbsAlt.find(lc);
		if (found != mMetaVerbsAlt.end())
			return found->second;

		return nullptr;
	}

	/// Get a list of all the interpretations for a single simple token			
	///	@param token - the token to search for											
	///	@return the list of associated meta definitions								
	const typename Interface::MetaList& Interface::GetAmbiguousMeta(const Token& token) const noexcept {
		const auto lc = ToLowercase(ToLastToken(token));
		const auto found = mMetaAmbiguous.find(lc);
		if (found == mMetaAmbiguous.end()) {
			static Interface::MetaList fallback {};
			return fallback;
		}
		return found->second;
	}

	/// Register most relevant token to the ambiguous token map						
	///	@param token - the token to register											
	///	@param meta - the definition to add												
	void Interface::RegisterAmbiguous(const Token& token, const Meta* meta) noexcept {
		auto ambiguous = ToLowercase(ToLastToken(token));
		const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
		if (foundAmbiguous == mMetaAmbiguous.end())
			mMetaAmbiguous.insert({::std::move(ambiguous), {meta}});
		else
			foundAmbiguous->second.insert(meta);
	}

	/// Unregister most relevant token from the ambiguous token map				
	///	@param token - the token to register											
	///	@param meta - the definition to remove											
	void Interface::UnregisterAmbiguous(const Token& token, const Meta* meta) noexcept {
		auto ambiguous = ToLowercase(ToLastToken(token));
		const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
		if (foundAmbiguous != mMetaAmbiguous.end()) {
			foundAmbiguous->second.erase(meta);
			if (foundAmbiguous->second.empty())
				mMetaAmbiguous.erase(foundAmbiguous);
		}
	}

	/// Register a data definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta data, if not defined yet, or				
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	DMeta Interface::RegisterData(const Token& token) noexcept {
		auto lc = ToLowercase(token);
		const auto found = GetMetaData(lc);
		if (found) {
			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so insert a new definition			
		const auto newDefinition = new MetaData {};
		mMetaData.insert({::std::move(lc), newDefinition});

		// Insert the last token to the ambiguity map							
		RegisterAmbiguous(token, newDefinition);

		return newDefinition;
	}

	/// Register a trait definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta trait, if not defined yet, or			
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	TMeta Interface::RegisterTrait(const Token& token) noexcept {
		auto lc = ToLowercase(token);
		const auto found = GetMetaTrait(lc);
		if (found) {
			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so emplace a new definition			
		const auto newDefinition = new MetaTrait {};
		mMetaTraits.insert({::std::move(lc), newDefinition});

		// Insert the last token to the ambiguity map							
		RegisterAmbiguous(token, newDefinition);

		return newDefinition;
	}

	/// Register a verb definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta verb, if not defined yet, or				
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	VMeta Interface::RegisterVerb(const Token& token, const Token& tokenReverse) noexcept {
		auto lc1 = ToLowercase(token);
		const auto found = GetMetaVerb(lc1);
		if (found) {
			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so emplace a new definition			
		auto lc2 = ToLowercase(tokenReverse);
		const auto newDefinition = new MetaVerb {};
		mMetaVerbs.insert({::std::move(lc1), newDefinition});
		mMetaVerbsAlt.insert({::std::move(lc2), newDefinition});

		RegisterAmbiguous(token, newDefinition);
		RegisterAmbiguous(tokenReverse, newDefinition);

		return newDefinition;
	}

	/// Unregister a data definition															
	///	@param definition - the definition to remove									
	void Interface::Unregister(DMeta definition) noexcept {
		const auto lc = ToLowercase(definition->mToken);
		const auto found = GetMetaData(lc);
		if (!found)
			return;

		mMetaData.erase(lc);
		UnregisterAmbiguous(definition->mToken, definition);
		delete found;
	}

	/// Unregister a trait definition														
	///	@param definition - the definition to remove									
	void Interface::Unregister(TMeta definition) noexcept {
		const auto lc = ToLowercase(definition->mToken);
		const auto found = GetMetaTrait(lc);
		if (!found)
			return;

		mMetaTraits.erase(lc);
		UnregisterAmbiguous(definition->mToken, definition);
		delete found;
	}

	/// Unregister a verb definition															
	///	@param definition - the definition to remove									
	void Interface::Unregister(VMeta definition) noexcept {
		const auto lc1 = ToLowercase(definition->mToken);
		const auto found = GetMetaVerb(lc1);
		if (!found)
			return;

		const auto lc2 = ToLowercase(definition->mTokenReverse);
		mMetaVerbs.erase(lc1);
		mMetaVerbsAlt.erase(lc2);
		UnregisterAmbiguous(definition->mToken, definition);
		UnregisterAmbiguous(definition->mTokenReverse, definition);
		delete found;
	}

} // namespace Langulus::RTTI
