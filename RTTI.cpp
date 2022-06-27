///																									
/// Langulus::RTTI																				
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#include "Reflection.hpp"
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

	/// Get an existing meta data definition by its token								
	///	@param token - the token of the data definition								
	///	@return the definition, or nullptr if not found								
	DMeta Interface::GetMetaData(const Token& token) const noexcept {
		const auto found = mMetaData.find(token);
		if (found == mMetaData.end())
			return nullptr;
		return found->second;
	}

	/// Get an existing meta trait definition by its token							
	///	@param token - the token of the trait definition							
	///	@return the definition, or nullptr if not found								
	TMeta Interface::GetMetaTrait(const Token& token) const noexcept {
		const auto found = mMetaTraits.find(token);
		if (found == mMetaTraits.end())
			return nullptr;
		return found->second;
	}

	/// Get an existing meta verb definition by its token								
	///	@param token - the token of the verb definition								
	///						you can search by positive, as well as negative token	
	///	@return the definition, or nullptr if not found								
	VMeta Interface::GetMetaVerb(const Token& token) const noexcept {
		auto found = mMetaVerbs.find(token);
		if (found != mMetaVerbs.end())
			return found->second;

		found = mMetaVerbsAlt.find(token);
		if (found != mMetaVerbsAlt.end())
			return found->second;

		return nullptr;
	}

	/// Register a data definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta data, if not defined yet, or				
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	DMeta Interface::Register(MetaData&& definition) noexcept {
		const auto found = GetMetaData(definition.mToken);
		if (found) {
			if (*found == definition) {
				// Same type registered													
				++const_cast<MetaData*>(found)->mReferences;
				return found;
			}

			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so emplace a new definition			
		const auto newDefinition = new MetaData {Move(definition)};
		mMetaData.insert({newDefinition->mToken, newDefinition});
		return newDefinition;
	}

	/// Register a trait definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta trait, if not defined yet, or			
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	TMeta Interface::Register(MetaTrait&& definition) noexcept {
		const auto found = GetMetaTrait(definition.mToken);
		if (found) {
			if (*found == definition) {
				// Same type registered													
				++const_cast<MetaTrait*>(found)->mReferences;
				return found;
			}

			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so emplace a new definition			
		const auto newDefinition = new MetaTrait {Move(definition)};
		mMetaTraits.insert({newDefinition->mToken, newDefinition});
		return newDefinition;
	}

	/// Register a verb definition															
	///	@param definition - the definition to register								
	///	@return the newly defined meta verb, if not defined yet, or				
	///			  a pointer to the already registered definition, if same		
	///			  or nullptr if a conflict or out-of-memory occured				
	VMeta Interface::Register(MetaVerb&& definition) noexcept {
		const auto found = GetMetaVerb(definition.mToken);
		if (found) {
			if (*found == definition) {
				// Same type registered													
				++const_cast<MetaVerb*>(found)->mReferences;
				return found;
			}

			// Conflicting type 															
			return nullptr;
		}

		// If reached, then not found, so emplace a new definition			
		const auto newDefinition = new MetaVerb {Move(definition)};
		mMetaVerbs.insert({newDefinition->mToken, newDefinition});
		mMetaVerbsAlt.insert({newDefinition->mTokenReverse, newDefinition});
		return newDefinition;
	}

	/// Unregister a data definition															
	///	@param definition - the definition to remove									
	void Interface::Unregister(DMeta definition) noexcept {
		const auto found = GetMetaData(definition->mToken);
		if (!found)
			return;

		--const_cast<MetaData*>(found)->mReferences;
		if (found->mReferences == 0) {
			mMetaData.erase(definition->mToken);
			delete found;
		}
	}

	/// Unregister a trait definition														
	///	@param definition - the definition to remove									
	void Interface::Unregister(TMeta definition) noexcept {
		const auto found = GetMetaTrait(definition->mToken);
		if (!found)
			return;

		--const_cast<MetaTrait*>(found)->mReferences;
		if (found->mReferences == 0) {
			mMetaTraits.erase(definition->mToken);
			delete found;
		}
	}

	/// Unregister a verb definition															
	///	@param definition - the definition to remove									
	void Interface::Unregister(VMeta definition) noexcept {
		const auto found = GetMetaVerb(definition->mToken);
		if (!found)
			return;

		--const_cast<MetaVerb*>(found)->mReferences;
		if (found->mReferences == 0) {
			mMetaVerbs.erase(definition->mToken);
			mMetaVerbsAlt.erase(definition->mTokenReverse);
			delete found;
		}
	}

} // namespace Langulus::RTTI
