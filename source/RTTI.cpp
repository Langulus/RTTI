///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "RTTI.hpp"
#include "MetaData.inl"
#include "MetaVerb.inl"
#include "MetaTrait.inl"
#include "Assumptions.hpp"
#include <cctype>

#define VERBOSE(...) //Logger::Verbose("RTTI: ", __VA_ARGS__)


namespace Langulus::RTTI
{

   Registry Instance {};

   /// Database destruction                                                   
   Registry::~Registry() {
      // If an exception happens here on a delete, then a meta likely   
      // wasn't unregistered upon mod unload. Thank me later            
      for (auto& pair : mMetaData)
         for(auto& meta : pair.second)
            delete meta.second;

      for (auto& pair : mMetaTraits)
         for (auto& meta : pair.second)
            delete meta.second;

      for (auto& pair : mUniqueVerbs)
         for (auto& meta : pair.second)
            delete meta.second;
   }

   /// Get the last, most relevant part of a token that may or may not have   
   /// namespaces in it. Essentially finds last "::" that isn't enclosed in   
   /// a template <>, and skip forward to that                                
   ///   @param token - the token to scan                                     
   ///   @return the last token                                               
   Token ToLastToken(const Token& token) noexcept {
      Count depth = 0;
      for (Offset i = token.size() - 1; i < token.size(); --i) {
         switch (token[i]) {
         case ':':
            // If no depth, then we found it                            
            if (not depth)
               return token.substr(i + 1, token.size() - i - 1);
            break;
         case '>':
            // Open template scope                                      
            ++depth;
            break;
         case '<':
            // Close template scope                                     
            if (depth)
               --depth;
            break;
         default:
            break;
         }
      }

      return token;
   }

   /// Get an existing meta data definition by its token                      
   ///   @attention will search for the definition in the local boundary only 
   ///   @param token - the token of the data definition                      
   ///   @return the definition, or nullptr if not found                      
   DMeta Registry::GetMetaData(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto foundToken = mMetaData.find(lc);
      if (foundToken == mMetaData.end())
         return nullptr;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return nullptr;
      return foundBoundary->second;
   }

   /// Get an existing meta constant definition by its token                  
   ///   @attention will search for the definition in the local boundary only 
   ///   @param token - the token of the constant definition                  
   ///   @return the definition, or nullptr if not found                      
   CMeta Registry::GetMetaConstant(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto foundToken = mMetaConstants.find(lc);
      if (foundToken == mMetaConstants.end())
         return nullptr;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return nullptr;
      return foundBoundary->second;
   }

   /// Get an existing meta trait definition by its token                     
   ///   @attention will search for the definition in the local boundary only 
   ///   @param token - the token of the trait definition                     
   ///   @return the definition, or nullptr if not found                      
   TMeta Registry::GetMetaTrait(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto foundToken = mMetaTraits.find(lc);
      if (foundToken == mMetaTraits.end())
         return nullptr;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return nullptr;
      return foundBoundary->second;
   }

   /// Get an existing meta verb definition by its token                      
   ///   @attention will search for the definition in the local boundary only 
   ///   @param token - the token of the verb definition                      
   ///                  you can search by positive, as well as negative token 
   ///   @return the definition, or nullptr if not found                      
   VMeta Registry::GetMetaVerb(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto foundToken = mMetaVerbs.find(lc);
      if (foundToken == mMetaVerbs.end())
         return nullptr;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return nullptr;
      return foundBoundary->second;
   }

   /// Get an existing meta verb definition by its operator token             
   ///   @attention will search for the definition in the local boundary only 
   ///   @param token - the operator of the verb definition                   
   ///                  you can search by positive, as well as negative       
   ///   @return the definition, or nullptr if not found                      
   VMeta Registry::GetOperator(const Token& token) const noexcept {
      const auto lc = IsolateOperator(token);
      const auto foundToken = mOperators.find(lc);
      if (foundToken == mOperators.end())
         return nullptr;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return nullptr;
      return foundBoundary->second;
   }

   /// Get a list of all the interpretations for a single simple token        
   ///   @param token - the token to search for                               
   ///   @return the list of associated meta definitions                      
   const MetaList& Registry::GetAmbiguousMeta(const Token& token) const noexcept {
      static const MetaList fallback {};
      const auto lc = ToLowercase(ToLastToken(token));
      const auto foundToken = mMetaAmbiguous.find(lc);
      if (foundToken == mMetaAmbiguous.end())
         return fallback;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return fallback;
      return foundBoundary->second;
   }
   
   /// Disambiguate a token                                                   
   ///   @param keyword - the token to search for                             
   ///   @return the disambiguated token; throws if not found/ambiguous       
   const Meta* Registry::DisambiguateMeta(const Token& keyword) const {
      auto& symbols = GetAmbiguousMeta(keyword);
      LANGULUS_ASSERT(not symbols.empty(), Meta,
         "Keyword not found", ": `", keyword, '`');

      if (symbols.size() > 1) {
         // Collect all origin types, and work with those               
         // Also, GetAmbiguousMeta works only with the last part of the 
         // keyword, but the keyword might contain hints as to which    
         // ambiguous meta to pick. Discard symbols that do not         
         // contain the provided keyword (not case sensitive)           
         const auto lowercased = ToLowercase(keyword);
         ::std::unordered_set<const Meta*> origins;
         for (auto& meta : symbols) {
            const auto meta_lc_token = ToLowercase(meta->mToken);
            if (meta_lc_token.find(lowercased) == std::string::npos)
               continue;

            const auto dmeta = dynamic_cast<DMeta>(meta);
            if (dmeta) {
               if (dmeta->mOrigin)
                  origins.insert(dmeta->mOrigin);
               else
                  origins.insert(dmeta);
            }
            else origins.insert(meta);
         }

         LANGULUS_ASSERT(not origins.empty(), Meta,
            "No relevant origins for keyword", ": `", keyword, '`');

         if (origins.size() == 1) {
            // Candidate types reduced to a single relevant origin      
            return *origins.begin();
         }
         else for (auto& candidate : origins) {
            // There's a chance, that one of the symbols matches the    
            // lowercased keyword exactly                               
            if (ToLowercase(candidate->mToken) == lowercased)
               return candidate;
         }

         // Ambiguity if reached, report error and throw                
         {
            auto tab = Logger::Error(
               "Ambiguous symbol: `", keyword,
               "`; Could be one of: ", Logger::Tabs {}
            );

            for (auto& meta : origins) {
               switch (meta->GetMetaType()) {
               case Meta::Data:
                  Logger::Verbose(Logger::Red, '`', meta->mCppName,
                     "` (meta data)");
                  break;
               case Meta::Trait:
                  Logger::Verbose(Logger::Red, '`', meta->mCppName,
                     "` (meta trait)");
                  break;
               case Meta::Constant:
                  Logger::Verbose(Logger::Red, '`', meta->mCppName,
                     "` (meta constant)");
                  break;
               default:
                  LANGULUS_THROW(Meta, "Unhandled meta type");
               }
            }
         }

         LANGULUS_THROW(Meta, "Ambiguous symbol");
      }

      // No ambiguity, just return the single result                    
      return *symbols.begin();
   }

   /// Resolve a file extension                                               
   ///   @attention only definitions in current boundary are affected         
   ///   @param extension - the file extension to search for                  
   ///   @return all meta definitions associated with the file extension      
   const MetaList& Registry::ResolveFileExtension(const Token& extension) const {
      static const MetaList fallback {};
      const auto lc = ToLowercase(extension);
      const auto foundToken = mFileDatabase.find(lc);
      if (foundToken == mFileDatabase.end())
         return fallback;
      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         return fallback;
      return foundBoundary->second;
   }

   /// Register most relevant token to the ambiguous token map                
   ///   @attention only definitions in current boundary are affected         
   ///   @param token - the token to register                                 
   ///   @param meta - the definition to add                                  
   void Registry::RegisterAmbiguous(const Token& token, const Meta* meta) noexcept {
      auto ambiguous = ToLowercase(ToLastToken(token));
      const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
      if (foundAmbiguous == mMetaAmbiguous.end()) {
         mMetaAmbiguous.insert({
            ::std::move(ambiguous),
            BoundedMeta<MetaList> {{Boundary, {meta}}}
         });
         return;
      }

      const auto foundBoundary = foundAmbiguous->second.find(Boundary);
      if (foundBoundary == foundAmbiguous->second.end()) {
         foundAmbiguous->second.insert({Boundary, {meta}});
         return;
      }

      foundBoundary->second.insert(meta);
   }

   /// Unregister most relevant token from the ambiguous token map            
   ///   @attention only definitions in current boundary are affected         
   ///   @param boundary - the boundary to unregister from                    
   ///   @param token - the token to unregister                               
   ///   @param meta - the definition to remove                               
   void Registry::UnregisterAmbiguous(const Token& boundary, const Token& token, const Meta* meta) noexcept {
      auto ambiguous = ToLowercase(ToLastToken(token));
      const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
      if (foundAmbiguous == mMetaAmbiguous.end())
         return;
      const auto foundBoundary = foundAmbiguous->second.find(boundary);
      if (foundBoundary == foundAmbiguous->second.end())
         return;

      foundBoundary->second.erase(meta);
      if (foundBoundary->second.empty())
         foundAmbiguous->second.erase(foundBoundary);
      if (foundAmbiguous->second.empty())
         mMetaAmbiguous.erase(foundAmbiguous);
   }

   /// Register a data definition                                             
   ///   @attention assumes token is not yet registered as data               
   ///   @param token - the data token to reserve                             
   ///   @return the newly defined meta data for that token                   
   DMeta Registry::RegisterData(const Token& token) IF_UNSAFE(noexcept) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaData(lc),
         "Data already registered");

      // If reached, then not found, so insert a new definition         
      const auto newDefinition = new MetaData;
      mMetaData.insert({
         ::std::move(lc),
         BoundedMeta<DMeta> {{Boundary, newDefinition}}
      });

      // Insert the last token to the ambiguity map                     
      RegisterAmbiguous(token, newDefinition);
      return newDefinition;
   }

   /// Register a constant definition                                         
   ///   @attention assumes token is not yet registered as constant           
   ///   @param token - the constant token to reserve                         
   ///   @return the newly defined meta constant for that token               
   CMeta Registry::RegisterConstant(const Token& token) IF_UNSAFE(noexcept) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaConstant(lc),
         "Constant already registered");

      // If reached, then not found, so insert a new definition         
      const auto newDefinition = new MetaConst;
      mMetaConstants.insert({
         ::std::move(lc),
         BoundedMeta<CMeta> {{Boundary, newDefinition}}
      });

      // Insert the last token to the ambiguity map                     
      RegisterAmbiguous(token, newDefinition);
      return newDefinition;
   }

   /// Register a trait definition                                            
   ///   @attention assumes token is not yet registered as trait              
   ///   @param token - the trait token to reserve                            
   ///   @return the newly defined meta trait for that token                  
   TMeta Registry::RegisterTrait(const Token& token) IF_UNSAFE(noexcept) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaTrait(lc),
         "Trait already registered");

      // If reached, then not found, so emplace a new definition			
      const auto newDefinition = new MetaTrait;
      mMetaTraits.insert({
         ::std::move(lc),
         BoundedMeta<TMeta> {{Boundary, newDefinition}}
      });

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
   VMeta Registry::RegisterVerb(
      const Token& cppname,
      const Token& token,
      const Token& tokenReverse,
      const Token& op,
      const Token& opReverse
   ) IF_UNSAFE(noexcept) {
      const ::std::string cppnamed {cppname};
      LANGULUS_ASSUME(DevAssumes, mUniqueVerbs.find(cppnamed) == mUniqueVerbs.end(),
         "Verb already registered");

      auto lc1 = ToLowercase(token);
      auto lc2 = ToLowercase(tokenReverse);
      LANGULUS_ASSUME(DevAssumes, not GetMetaVerb(lc1) and not GetMetaVerb(lc2),
         "Verb already registered");

      Lowercase op1;
      if (not op.empty()) {
         op1 = IsolateOperator(op);
         LANGULUS_ASSUME(DevAssumes, not GetOperator(op1),
            "Positive operator already registered");
      }

      Lowercase op2;
      if (not opReverse.empty()) {
         op2 = IsolateOperator(opReverse);
         LANGULUS_ASSUME(DevAssumes, not GetOperator(op2),
            "Negative operator already registered");
      }

      const auto newDefinition = new MetaVerb;
      BoundedMeta<VMeta> boundedDefinition {{Boundary, newDefinition}};
      mUniqueVerbs.insert({cppnamed, boundedDefinition});
      VERBOSE("Verb ", token, "/", tokenReverse, " registered");

      mMetaVerbs.insert({lc1, boundedDefinition});
      if (lc1 != lc2)
         mMetaVerbs.insert({lc2, boundedDefinition});

      if (not op1.empty()) {
         mOperators.insert({op1, boundedDefinition});
         VERBOSE("Operator ", op1, " registered");
      }

      if (not op2.empty() and op1 != op2) {
         mOperators.insert({op2, boundedDefinition});
         VERBOSE("Operator ", op1, " registered");
      }

      RegisterAmbiguous(token, newDefinition);
      RegisterAmbiguous(tokenReverse, newDefinition);
      return newDefinition;
   }

   /// Register file extension                                                
   ///   @param token - the file extension token to reserve                   
   ///   @param type - the data to associate file with                        
   void Registry::RegisterFileExtension(const Token& token, DMeta type) IF_UNSAFE(noexcept) {
      LANGULUS_ASSUME(DevAssumes, not token.empty(),
         "Bad file extension");
      LANGULUS_ASSUME(DevAssumes, type,
         "Bad meta data for file extension ", token);

      const auto lc = ToLowercase(token);
      const auto foundToken = mFileDatabase.find(lc);
      if (foundToken == mFileDatabase.end()) {
         mFileDatabase[lc].insert({Boundary, {type}});
         return;
      }

      const auto foundBoundary = foundToken->second.find(Boundary);
      if (foundBoundary == foundToken->second.end())
         foundToken->second.insert({Boundary, {type}});
      else
         foundBoundary->second.insert(type);
   }

   /// Runs through all definitions, and destroys all of those, that were     
   /// defined within the given boundary token                                
   ///   @param library - the boundary token to search for                    
   void Registry::UnloadBoundary(const Token& library) {
      VERBOSE(Logger::Push, Logger::Underline, Logger::Red, 
         "Unloading library ", library, Logger::Pop);

      // Unload constants                                               
      for (auto pair = mMetaConstants.begin(); pair != mMetaConstants.end(); ++pair) {
         auto found = pair->second.find(library);
         if (found == pair->second.end())
            continue;

         VERBOSE("Constant ", Logger::Push, Logger::Yellow, found->second->mToken,
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(library, found->second->mToken, found->second);
         delete found->second;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaConstants.erase(pair);
      }

      // Unload file types                                              
      for (auto pair = mFileDatabase.begin(); pair != mFileDatabase.end(); ++pair) {
         auto found = pair->second.find(library);
         if (found == pair->second.end())
            continue;

         VERBOSE("File ", Logger::Push, Logger::Cyan, found->second->mToken,
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         pair->second.erase(found);
         if (pair->second.empty())
            pair = mFileDatabase.erase(pair);
      }

      // Unload data types                                              
      for (auto pair = mMetaData.begin(); pair != mMetaData.end(); ++pair) {
         auto found = pair->second.find(library);
         if (found == pair->second.end())
            continue;

         VERBOSE("Data ", Logger::Push, Logger::Cyan, found->second->mToken,
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(library, found->second->mToken, found->second);
         delete found->second;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaData.erase(pair);
      }

      // Unload traits                                                  
      for (auto pair = mMetaTraits.begin(); pair != mMetaTraits.end(); ++pair) {
         auto found = pair->second.find(library);
         if (found == pair->second.end())
            continue;

         VERBOSE("Trait ", Logger::Push, Logger::Purple, found->second->mToken,
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(library, found->second->mToken, found->second);
         delete found->second;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaTraits.erase(pair);
      }

      // Unload verbs                                                   
      for (auto pair = mUniqueVerbs.begin(); pair != mUniqueVerbs.end(); ++pair) {
         auto found = pair->second.find(library);
         if (found == pair->second.end())
            continue;

         VMeta definition = found->second;
         const auto lc1 = ToLowercase(definition->mToken);
         const auto lc2 = ToLowercase(definition->mTokenReverse);
         LANGULUS_ASSUME(DevAssumes, definition
            and definition == GetMetaVerb(lc1)
            and definition == GetMetaVerb(lc2),
            "Bad VMeta definition"
         );

         auto foundlc1 = mMetaVerbs.find(lc1);
         if (foundlc1 != mMetaVerbs.end())
            foundlc1->second.erase(library);
         auto foundlc2 = mMetaVerbs.find(lc2);
         if (foundlc2 != mMetaVerbs.end())
            foundlc2->second.erase(library);

         if (definition->mOperator.size()) {
            const auto op1 = IsolateOperator(definition->mOperator);
            VERBOSE("Operator ", Logger::Push, Logger::DarkGreen, op1,
               Logger::Pop, Logger::Red, " unregistered (", library, ")");
            auto foundop1 = mOperators.find(op1);
            if (foundop1 != mOperators.end())
               foundop1->second.erase(library);
         }

         if (definition->mOperatorReverse.size() and definition->mOperatorReverse != definition->mOperator) {
            const auto op2 = IsolateOperator(definition->mOperatorReverse);
            VERBOSE("Operator ", Logger::Push, Logger::DarkGreen, op2,
               Logger::Pop, Logger::Red, " unregistered (", library, ")");
            auto foundop2 = mOperators.find(op2);
            if (foundop2 != mOperators.end())
               foundop2->second.erase(library);
         }

         if (definition->mToken != definition->mTokenReverse) {
            VERBOSE("Verb ", Logger::Push, Logger::DarkGreen,
               definition->mToken, "/", definition->mTokenReverse,
               Logger::Pop, Logger::Red, " unregistered (", library, ")");
         }
         else {
            VERBOSE("Verb ", Logger::Push, Logger::DarkGreen,
               definition->mToken, Logger::Pop, Logger::Red,
               " unregistered (", library, ")");
         }

         UnregisterAmbiguous(library, definition->mToken, definition);
         UnregisterAmbiguous(library, definition->mTokenReverse, definition);
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mUniqueVerbs.erase(pair);
         delete definition;
      }
   }

   /// Get the shortest possible token, that is not ambiguous                 
   ///   @return the token                                                    
   Token Meta::GetShortestUnambiguousToken() const {
      auto& ambiguous = Instance.GetAmbiguousMeta(mToken);
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
