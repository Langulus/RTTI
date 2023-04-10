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

#define VERBOSE(a) //Logger::Verbose() << a;

namespace Langulus::RTTI
{

   Interface Database {};

   /// Database destruction                                                   
   Interface::~Interface() {
      // If an exception happens here on a delete, then a meta likely   
      // wasn't unregistered upon mod unload. Thank me later            
      for (auto& pair : mMetaData)
         delete pair.second;
      for (auto& pair : mMetaTraits)
         delete pair.second;
      for (auto& pair : mUniqueVerbs)
         delete pair.second;
   }

   /// Convert a token to a lowercase string                                  
   ///   @param token - the token to lowercase                                
   ///   @return the lowercase string                                         
   Lowercase ToLowercase(const Token& token) noexcept {
      Lowercase lc {token};
      ::std::transform(lc.begin(), lc.end(), lc.begin(),
         [](char c) { return static_cast<char>(::std::tolower(c)); }
      );
      return lc;
   }

   /// Get the last, most relevant part of a token that my or may not have    
   /// namespaces in it. Essentially finds last "::" and skip forward to that 
   ///   @param token - the token to scan                                     
   ///   @return the last token                                               
   Token ToLastToken(const Token& token) noexcept {
      const auto found = token.find_last_of(':', token.size());
      if (found != token.npos)
         return token.substr(found + 1, token.size() - found - 1);
      return token;
   }

   /// Isolate and lowercase an operator token                                
   ///   @param token - the operator                                          
   ///   @return the lowercased and isolated operator token                   
   Lowercase IsolateOperator(const Token& token) noexcept {
      // Skip skippable at the front and the back of token              
      auto l = token.data();
      auto r = token.data() + token.size();
      while (l < r && *l <= 32)
         ++l;

      while (r > l && *(r-1) <= 32)
         --r;

      // Lowercase the isolated token                                   
      return ToLowercase(token.substr(l - token.data(), r - l));
   }

   /// Get an existing meta data definition by its token                      
   ///   @param token - the token of the data definition                      
   ///   @return the definition, or nullptr if not found                      
   DMeta Interface::GetMetaData(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaData.find(lc);
      if (found == mMetaData.end())
         return nullptr;
      return found->second;
   }

   /// Get an existing meta constant definition by its token                  
   ///   @param token - the token of the constant definition                  
   ///   @return the definition, or nullptr if not found                      
   CMeta Interface::GetMetaConstant(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaConstants.find(lc);
      if (found == mMetaConstants.end())
         return nullptr;
      return found->second;
   }

   /// Get an existing meta trait definition by its token                     
   ///   @param token - the token of the trait definition                     
   ///   @return the definition, or nullptr if not found                      
   TMeta Interface::GetMetaTrait(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaTraits.find(lc);
      if (found == mMetaTraits.end())
         return nullptr;
      return found->second;
   }

   /// Get an existing meta verb definition by its token                      
   ///   @param token - the token of the verb definition                      
   ///                  you can search by positive, as well as negative token 
   ///   @return the definition, or nullptr if not found                      
   VMeta Interface::GetMetaVerb(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaVerbs.find(lc);
      if (found != mMetaVerbs.end())
         return found->second;
      return nullptr;
   }

   /// Get an existing meta verb definition by its operator token             
   ///   @param token - the operator of the verb definition                   
   ///                  you can search by positive, as well as negative       
   ///   @return the definition, or nullptr if not found                      
   VMeta Interface::GetOperator(const Token& token) const noexcept {
      const auto lc = IsolateOperator(token);
      const auto found = mOperators.find(lc);
      if (found != mOperators.end())
         return found->second;
      return nullptr;
   }

   /// Get a list of all the interpretations for a single simple token        
   ///   @param token - the token to search for                               
   ///   @return the list of associated meta definitions                      
   const MetaList& Interface::GetAmbiguousMeta(const Token& token) const noexcept {
      const auto lc = ToLowercase(ToLastToken(token));
      const auto found = mMetaAmbiguous.find(lc);
      if (found == mMetaAmbiguous.end()) {
         static MetaList fallback {};
         return fallback;
      }
      return found->second;
   }

   /// Register most relevant token to the ambiguous token map                
   ///   @param token - the token to register                                 
   ///   @param meta - the definition to add                                  
   void Interface::RegisterAmbiguous(const Token& token, const Meta* meta) noexcept {
      auto ambiguous = ToLowercase(ToLastToken(token));
      const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
      if (foundAmbiguous == mMetaAmbiguous.end())
         mMetaAmbiguous.insert({::std::move(ambiguous), {meta}});
      else
         foundAmbiguous->second.insert(meta);
   }

   /// Unregister most relevant token from the ambiguous token map            
   ///   @param token - the token to register                                 
   ///   @param meta - the definition to remove                               
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
   ///   @attention assumes token is not yet registered as data               
   ///   @param token - the data token to reserve                             
   ///   @return the newly defined meta data for that token                   
   DMeta Interface::RegisterData(const Token& token) SAFETY_NOEXCEPT() {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, !GetMetaData(lc),
         "Data already registered");

      // If reached, then not found, so insert a new definition         
      const auto newDefinition = new MetaData {};
      mMetaData.insert({::std::move(lc), newDefinition});
      VERBOSE("RTTI: Data " << token << " registered");

      // Insert the last token to the ambiguity map                     
      RegisterAmbiguous(token, newDefinition);

      return newDefinition;
   }

   /// Register a constant definition                                         
   ///   @attention assumes token is not yet registered as constant           
   ///   @param token - the constant token to reserve                         
   ///   @return the newly defined meta constant for that token               
   CMeta Interface::RegisterConstant(const Token& token) SAFETY_NOEXCEPT() {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, !GetMetaConstant(lc),
         "Constant already registered");

      // If reached, then not found, so insert a new definition         
      const auto newDefinition = new MetaConst {};
      mMetaConstants.insert({::std::move(lc), newDefinition});
      VERBOSE("RTTI: Constant " << token << " registered");

      // Insert the last token to the ambiguity map                     
      RegisterAmbiguous(token, newDefinition);
      return newDefinition;
   }

   /// Register a trait definition                                            
   ///   @attention assumes token is not yet registered as trait              
   ///   @param token - the trait token to reserve                            
   ///   @return the newly defined meta trait for that token                  
   TMeta Interface::RegisterTrait(const Token& token) SAFETY_NOEXCEPT() {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, !GetMetaTrait(lc),
         "Trait already registered");

      // If reached, then not found, so emplace a new definition			
      const auto newDefinition = new MetaTrait {};
      mMetaTraits.insert({::std::move(lc), newDefinition});
      VERBOSE("RTTI: Trait " << token << " registered");

      // Insert the last token to the ambiguity map							
      RegisterAmbiguous(token, newDefinition);
      return newDefinition;
   }

   /// Register a verb definition                                             
   ///   @attention assumes tokens are not yet registered                     
   ///   @param cppname - the original verb class name                        
   ///   @param token - the positive verb token to reserve                    
   ///   @param tokenReverse - the negative verb token to reserve             
   ///   @param op - the positive verb operator to reserve (optional)         
   ///   @param opReverse - the negative verb operator to reserve (optional)  
   ///   @return the newly defined meta verb for that token configuration     
   VMeta Interface::RegisterVerb(const Token& cppname, const Token& token, const Token& tokenReverse, const Token& op, const Token& opReverse) SAFETY_NOEXCEPT() {
      const ::std::string cppnamed {cppname};
      LANGULUS_ASSUME(DevAssumes, mUniqueVerbs.find(cppnamed) == mUniqueVerbs.end(),
         "Verb already registered");

      auto lc1 = ToLowercase(token);
      auto lc2 = ToLowercase(tokenReverse);
      LANGULUS_ASSUME(DevAssumes, !GetMetaVerb(lc1) && !GetMetaVerb(lc2),
         "Verb already registered");

      Lowercase op1;
      if (!op.empty()) {
         op1 = IsolateOperator(op);
         LANGULUS_ASSUME(DevAssumes, !GetOperator(op1),
            "Positive operator already registered");
      }

      Lowercase op2;
      if (!opReverse.empty()) {
         op2 = IsolateOperator(opReverse);
         LANGULUS_ASSUME(DevAssumes, !GetOperator(op2),
            "Negative operator already registered");
      }

      const auto newDefinition = new MetaVerb {};
      mUniqueVerbs[cppnamed] = newDefinition;
      VERBOSE("RTTI: Verb " << token << "/" << tokenReverse << " registered");

      mMetaVerbs.insert({lc1, newDefinition});
      if (lc1 != lc2)
         mMetaVerbs.insert({lc2, newDefinition});

      if (!op1.empty()) {
         mOperators.insert({op1, newDefinition});
         VERBOSE("RTTI: Operator " << op1 << " registered");
      }

      if (!op2.empty() && op1 != op2) {
         mOperators.insert({op2, newDefinition});
         VERBOSE("RTTI: Operator " << op1 << " registered");
      }

      RegisterAmbiguous(token, newDefinition);
      RegisterAmbiguous(tokenReverse, newDefinition);
      return newDefinition;
   }

   /// Unregister a data definition                                           
   ///   @attention assumes definition is a valid pointer owned by this       
   ///   @attention definition may no longer be valid pointer after this call 
   ///   @param definition - the definition to remove                         
   void Interface::Unregister(DMeta definition) SAFETY_NOEXCEPT() {
      --const_cast<MetaData*>(definition)->mReferences;
      if (definition->mReferences)
         return;

      // Time to remove the definition entirely                         
      const auto lc = ToLowercase(definition->mToken);
      LANGULUS_ASSUME(DevAssumes, definition && definition == GetMetaData(lc),
         "Bad DMeta definition");

      VERBOSE("RTTI: Data " << definition->mToken << " unregistered");
      mMetaData.erase(lc);
      UnregisterAmbiguous(definition->mToken, definition);
      delete definition;
   }

   /// Unregister a trait definition                                          
   ///   @attention assumes definition is a valid pointer owned by this       
   ///   @attention definition may no longer be valid pointer after this call 
   ///   @param definition - the definition to remove                         
   void Interface::Unregister(TMeta definition) SAFETY_NOEXCEPT() {
      --const_cast<MetaTrait*>(definition)->mReferences;
      if (definition->mReferences)
         return;

      // Time to remove the definition entirely                         
      const auto lc = ToLowercase(definition->mToken);
      LANGULUS_ASSUME(DevAssumes, definition && definition == GetMetaTrait(lc),
         "Bad TMeta definition");

      VERBOSE("RTTI: Trait " << definition->mToken << " unregistered");
      mMetaTraits.erase(lc);
      UnregisterAmbiguous(definition->mToken, definition);
      delete definition;
   }

   /// Unregister a constant definition                                       
   ///   @attention assumes definition is a valid pointer owned by this       
   ///   @attention definition may no longer be valid pointer after this call 
   ///   @param definition - the definition to remove                         
   void Interface::Unregister(CMeta definition) SAFETY_NOEXCEPT() {
      --const_cast<MetaConst*>(definition)->mReferences;
      if (definition->mReferences)
         return;

      // Time to remove the definition entirely                         
      const auto lc = ToLowercase(definition->mToken);
      LANGULUS_ASSUME(DevAssumes, definition && definition == GetMetaConstant(lc),
         "Bad CMeta definition");

      VERBOSE("RTTI: Constant " << definition->mToken << " unregistered");
      mMetaConstants.erase(lc);
      UnregisterAmbiguous(definition->mToken, definition);
      delete definition;
   }

   /// Unregister a verb definition                                           
   ///   @attention assumes definition is a valid pointer owned by this       
   ///   @attention definition may no longer be valid pointer after this call 
   ///   @param definition - the definition to remove                         
   void Interface::Unregister(VMeta definition) SAFETY_NOEXCEPT() {
      --const_cast<MetaVerb*>(definition)->mReferences;
      if (definition->mReferences)
         return;

      // Time to remove the definition entirely                         
      const auto lc1 = ToLowercase(definition->mToken);
      const auto lc2 = ToLowercase(definition->mTokenReverse);
      LANGULUS_ASSUME(DevAssumes, 
         definition && definition == GetMetaVerb(lc1)
                    && definition == GetMetaVerb(lc2),
         "Bad VMeta definition"
      );

      mMetaVerbs.erase(lc1);
      mMetaVerbs.erase(lc2);

      if (definition->mOperator.size()) {
         const auto op1 = IsolateOperator(definition->mOperator);
         VERBOSE("RTTI: Operator " << op1 << " unregistered");
         mOperators.erase(op1);
      }

      if (definition->mOperatorReverse.size()) {
         const auto op2 = IsolateOperator(definition->mOperatorReverse);
         VERBOSE("RTTI: Operator " << op2 << " unregistered");
         mOperators.erase(op2);
      }

      UnregisterAmbiguous(definition->mToken, definition);
      UnregisterAmbiguous(definition->mTokenReverse, definition);

      VERBOSE("RTTI: Verb " << definition->mToken << "/" << definition->mTokenReverse << " unregistered");
      mUniqueVerbs.erase(::std::string {definition->mCppName});
      delete definition;
   }

   /// Unregister an unknown definition                                       
   ///   @attention assumes definition is a valid pointer owned by this       
   ///   @attention definition may no longer be valid pointer after this call 
   ///   @param definition - the definition to remove                         
   void Interface::Unregister(const Meta* definition) SAFETY_NOEXCEPT() {
      auto asdata = dynamic_cast<DMeta>(definition);
      if (asdata) {
         Unregister(asdata);
         return;
      }

      auto asverb = dynamic_cast<VMeta>(definition);
      if (asverb) {
         Unregister(asverb);
         return;
      }

      auto astrait = dynamic_cast<TMeta>(definition);
      if (astrait) {
         Unregister(astrait);
         return;
      }

      auto asconst = dynamic_cast<CMeta>(definition);
      if (asconst) {
         Unregister(asconst);
         return;
      }
   }

   /// Runs through all definitions, and destroys all of those, that were     
   /// defined with the given library token                                   
   ///   @param library - the library token to search for                     
   void Interface::UnloadLibrary(const Token& library) {
      VERBOSE("RTTI: Unloading library " << library);

      for (auto meta = mMetaConstants.begin(); meta != mMetaConstants.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("RTTI: Constant " << meta->second->mToken << " unregistered (" << library << ")");
         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaConstants.erase(meta);
      }

      for (auto meta = mMetaData.begin(); meta != mMetaData.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("RTTI: Data " << meta->second->mToken << " unregistered (" << library << ")");
         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaData.erase(meta);
      }

      for (auto meta = mMetaTraits.begin(); meta != mMetaTraits.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("RTTI: Trait " << meta->second->mToken << " unregistered (" << library << ")");
         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaTraits.erase(meta);
      }

      for (auto meta = mUniqueVerbs.begin(); meta != mUniqueVerbs.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VMeta definition = meta->second;
         const auto lc1 = ToLowercase(definition->mToken);
         const auto lc2 = ToLowercase(definition->mTokenReverse);
         LANGULUS_ASSUME(DevAssumes,
            definition && definition == GetMetaVerb(lc1)
            && definition == GetMetaVerb(lc2),
            "Bad VMeta definition"
         );

         mMetaVerbs.erase(lc1);
         mMetaVerbs.erase(lc2);

         if (definition->mOperator.size()) {
            const auto op1 = IsolateOperator(definition->mOperator);
            VERBOSE("RTTI: Operator " << op1 << " unregistered (" << library << ")");
            mOperators.erase(op1);
         }

         if (definition->mOperatorReverse.size()) {
            const auto op2 = IsolateOperator(definition->mOperatorReverse);
            VERBOSE("RTTI: Operator " << op2 << " unregistered (" << library << ")");
            mOperators.erase(op2);
         }

         VERBOSE("RTTI: Verb " << definition->mToken << "/" << definition->mTokenReverse << " unregistered (" << library << ")");
         UnregisterAmbiguous(definition->mToken, meta->second);
         UnregisterAmbiguous(definition->mTokenReverse, meta->second);
         meta = mUniqueVerbs.erase(meta);
         delete definition;
      }
   }

   /// Get the shortest possible token, that is not ambiguous                 
   ///   @return the token                                                    
   Token Meta::GetShortestUnambiguousToken() const {
      auto& ambiguous = Database.GetAmbiguousMeta(mToken);
      if (ambiguous.size() == 1)
         return ToLastToken(mToken);

      // Collect all origin types, and work with those                  
      MetaList origins;
      for (auto& meta : ambiguous) {
         const auto dmeta = dynamic_cast<DMeta>(meta);
         if (dmeta) {
            if (dmeta->mOrigin)
               origins.insert(dmeta->mOrigin);
            else
               origins.insert(dmeta);
         }
         else origins.insert(dmeta);
      }

      if (origins.size() == 1)
         return ToLastToken(mToken);

      // Start including namespaces, until the resulting token has      
      // exactly one match inside the ambiguous list                    
      auto start = ToLastToken(mToken).data() - 3;
      while (start >= mToken.data()) {
         if (*start == ':') {
            const auto candidate = mToken.substr(start - mToken.data() + 1);
            Count matches {};
            for (auto& meta : origins) {
               if (meta->mToken.ends_with(candidate)) {
                  if (++matches > 1)
                     break;
               }
            }

            if (matches == 1) {
               // Match found                                           
               return candidate;
            }

            start -= 2;
         }

         --start;
      }

      // Full token returned as fallback                                
      return mToken;
   }

} // namespace Langulus::RTTI
