///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "RTTI.hpp"
#include "MetaData.hpp"
#include "MetaVerb.hpp"
#include "MetaTrait.hpp"
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
         delete pair.second;
      for (auto& pair : mMetaTraits)
         delete pair.second;
      for (auto& pair : mUniqueVerbs)
         delete pair.second;
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

   /// Isolate and lowercase an operator token                                
   ///   @param token - the operator                                          
   ///   @return the lowercased and isolated operator token                   
   Lowercase IsolateOperator(const Token& token) noexcept {
      // Skip skippable at the front and the back of token              
      auto l = token.data();
      auto r = token.data() + token.size();
      while (l < r and *l <= 32)
         ++l;

      while (r > l and *(r-1) <= 32)
         --r;

      // Lowercase the isolated token                                   
      return ToLowercase(token.substr(l - token.data(), r - l));
   }

   /// Get an existing meta data definition by its token                      
   ///   @param token - the token of the data definition                      
   ///   @return the definition, or nullptr if not found                      
   DMeta Registry::GetMetaData(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaData.find(lc);
      if (found == mMetaData.end())
         return nullptr;
      return found->second;
   }

   /// Get an existing meta constant definition by its token                  
   ///   @param token - the token of the constant definition                  
   ///   @return the definition, or nullptr if not found                      
   CMeta Registry::GetMetaConstant(const Token& token) const noexcept {
      const auto lc = ToLowercase(token);
      const auto found = mMetaConstants.find(lc);
      if (found == mMetaConstants.end())
         return nullptr;
      return found->second;
   }

   /// Get an existing meta trait definition by its token                     
   ///   @param token - the token of the trait definition                     
   ///   @return the definition, or nullptr if not found                      
   TMeta Registry::GetMetaTrait(const Token& token) const noexcept {
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
   VMeta Registry::GetMetaVerb(const Token& token) const noexcept {
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
   VMeta Registry::GetOperator(const Token& token) const noexcept {
      const auto lc = IsolateOperator(token);
      const auto found = mOperators.find(lc);
      if (found != mOperators.end())
         return found->second;
      return nullptr;
   }

   /// Get a list of all the interpretations for a single simple token        
   ///   @param token - the token to search for                               
   ///   @return the list of associated meta definitions                      
   const MetaList& Registry::GetAmbiguousMeta(const Token& token) const noexcept {
      const auto lc = ToLowercase(ToLastToken(token));
      const auto found = mMetaAmbiguous.find(lc);
      if (found == mMetaAmbiguous.end()) {
         static MetaList fallback {};
         return fallback;
      }
      return found->second;
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
            if (not meta_lc_token.find(lowercased))
               continue;

            const auto dmeta = dynamic_cast<DMeta>(meta);
            if (dmeta) {
               if (dmeta->mOrigin)
                  origins.insert(dmeta->mOrigin);
               else
                  origins.insert(dmeta);
            }
            else origins.insert(dmeta);
         }

         LANGULUS_ASSERT(not origins.empty(), Meta,
            "No relevant origins for keyword", ": `", keyword, '`');

         if (origins.size() == 1) {
            // Candidate types reduced to a single relevant origin      
            return *origins.begin();
         }

         // Ambiguity, report error                                     
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

      // No ambiguity, just return                                      
      return *symbols.begin();
   }

   /// Resolve a file extension                                               
   ///   @param extension - the file extension to search for                  
   ///   @return all meta definitions associated with the file extension      
   const MetaList& Registry::ResolveFileExtension(const Token& extension) const {
      const auto lc = ToLowercase(extension);
      const auto found = mFileDatabase.find(lc);
      if (found == mFileDatabase.end()) {
         static MetaList fallback {};
         return fallback;
      }
      return found->second;
   }

   /// Register most relevant token to the ambiguous token map                
   ///   @param token - the token to register                                 
   ///   @param meta - the definition to add                                  
   void Registry::RegisterAmbiguous(const Token& token, const Meta* meta) noexcept {
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
   void Registry::UnregisterAmbiguous(const Token& token, const Meta* meta) noexcept {
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
   DMeta Registry::RegisterData(const Token& token) IF_UNSAFE(noexcept) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaData(lc),
         "Data already registered");

      // If reached, then not found, so insert a new definition         
      const auto newDefinition = new MetaData {};
      mMetaData.insert({::std::move(lc), newDefinition});
      VERBOSE("Data ", token, " registered");

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
      const auto newDefinition = new MetaConst {};
      mMetaConstants.insert({::std::move(lc), newDefinition});
      VERBOSE("Constant ", token, " registered");

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
      const auto newDefinition = new MetaTrait {};
      mMetaTraits.insert({::std::move(lc), newDefinition});
      VERBOSE("Trait ", token, " registered");

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

      const auto newDefinition = new MetaVerb {};
      mUniqueVerbs[cppnamed] = newDefinition;
      VERBOSE("Verb ", token, "/", tokenReverse, " registered");

      mMetaVerbs.insert({lc1, newDefinition});
      if (lc1 != lc2)
         mMetaVerbs.insert({lc2, newDefinition});

      if (not op1.empty()) {
         mOperators.insert({op1, newDefinition});
         VERBOSE("Operator ", op1, " registered");
      }

      if (not op2.empty() and op1 != op2) {
         mOperators.insert({op2, newDefinition});
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
      mFileDatabase[lc].emplace(type);
   }

   /// Runs through all definitions, and destroys all of those, that were     
   /// defined with the given library token                                   
   ///   @param library - the library token to search for                     
   void Registry::UnloadLibrary(const Token& library) {
      VERBOSE(Logger::Push, Logger::Underline, Logger::Red, 
         "Unloading library ", library, Logger::Pop);

      // Unload constants                                               
      for (auto meta = mMetaConstants.begin(); meta != mMetaConstants.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("Constant ", Logger::Push, Logger::Yellow, meta->second->mToken, 
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaConstants.erase(meta);
      }

      // Unload data types                                              
      for (auto meta = mMetaData.begin(); meta != mMetaData.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("Data ", Logger::Push, Logger::Cyan, meta->second->mToken, 
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         if (not meta->second->mFileExtensions.empty()) {
            // Unregister all file extensions, if any                   
            const auto ext = meta->second->mFileExtensions;
            Offset sequential = 0;
            for (Offset e = 0; e < ext.size(); ++e) {
               if (IsSpace(ext[e]) or ext[e] == ',') {
                  if (sequential) {
                     const auto lc = ToLowercase(
                        ext.substr(e - sequential, sequential)
                     );
                     mFileDatabase[lc].erase(meta->second);
                  }

                  sequential = 0;
                  continue;
               }

               ++sequential;
            }

            if (sequential) {
               const auto lc = ToLowercase(
                  ext.substr(ext.size() - sequential, sequential)
               );
               mFileDatabase[lc].erase(meta->second);
            }
         }

         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaData.erase(meta);
      }

      // Unload traits                                                  
      for (auto meta = mMetaTraits.begin(); meta != mMetaTraits.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VERBOSE("Trait ", Logger::Push, Logger::Purple, meta->second->mToken,
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(meta->second->mToken, meta->second);
         delete meta->second;
         meta = mMetaTraits.erase(meta);
      }

      // Unload verbs                                                   
      for (auto meta = mUniqueVerbs.begin(); meta != mUniqueVerbs.end();) {
         if (meta->second->mLibraryName != library) {
            ++meta;
            continue;
         }

         VMeta definition = meta->second;
         const auto lc1 = ToLowercase(definition->mToken);
         const auto lc2 = ToLowercase(definition->mTokenReverse);
         LANGULUS_ASSUME(DevAssumes, definition
            and definition == GetMetaVerb(lc1)
            and definition == GetMetaVerb(lc2),
            "Bad VMeta definition"
         );

         mMetaVerbs.erase(lc1);
         mMetaVerbs.erase(lc2);

         if (definition->mOperator.size()) {
            const auto op1 = IsolateOperator(definition->mOperator);
            VERBOSE("Operator ", Logger::Push, Logger::Green, op1, 
               Logger::Pop, Logger::Red, " unregistered (", library, ")");
            mOperators.erase(op1);
         }

         if (definition->mOperatorReverse.size()) {
            const auto op2 = IsolateOperator(definition->mOperatorReverse);
            VERBOSE("Operator ", Logger::Push, Logger::Green, op2,
               Logger::Pop, Logger::Red, " unregistered (", library, ")");
            mOperators.erase(op2);
         }

         VERBOSE("Verb ", Logger::Push, Logger::Green, 
            definition->mToken, "/", definition->mTokenReverse, 
            Logger::Pop, Logger::Red, " unregistered (", library, ")");

         UnregisterAmbiguous(definition->mToken, meta->second);
         UnregisterAmbiguous(definition->mTokenReverse, meta->second);
         meta = mUniqueVerbs.erase(meta);
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
