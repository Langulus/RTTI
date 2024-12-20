///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include "RTTI.hpp"
#include "MetaData.inl"
#include "MetaVerb.inl"
#include "MetaTrait.inl"
#include "MetaConst.inl"
#include "Meta.inl"
#include "Assumptions.hpp"
#include <cctype>

#if 0
   #define VERBOSE(...) Logger::Verbose("RTTI: ", __VA_ARGS__)
#else
   #define VERBOSE(...) LANGULUS(NOOP)
#endif


namespace Langulus::RTTI
{

   Registry Instance {};

   /// Database destruction                                                   
   Registry::~Registry() {
      // If an exception happens here on a delete, then a meta likely   
      // wasn't unregistered upon mod unload. Thank me later            
      for (auto& pair : mMetaData)
         for(auto& meta : pair.second)
            delete meta.second.mMeta;

      for (auto& pair : mMetaTraits)
         for (auto& meta : pair.second)
            delete meta.second.mMeta;

      for (auto& pair : mUniqueVerbs)
         for (auto& meta : pair.second)
            delete meta.second.mMeta;
   }

   /// Common way to extract something from the registry                      
   ///   @param where - where to search in                                    
   ///   @param token - the token to search for                               
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the found element, or nullptr if not found                   
   auto Registry::GetMeta(
      const auto& where, const Token& token, const Token& boundary
   ) const noexcept {
      using R = decltype(where.begin()->second.begin()->second);
      const auto lc = ToLowercase(token);
      const auto foundToken = where.find(lc);
      if (foundToken == where.end())
         return (R) nullptr;

      if (not boundary.empty()) {
         // Search in a specific boundary                               
         const auto foundBoundary = foundToken->second.find(boundary);
         if (foundBoundary == foundToken->second.end())
            return (R) nullptr;
         return foundBoundary->second;
      }
      else {
         // Always prefer the main boundary if available, because it's  
         // more persistent                                             
         const auto foundBoundary = foundToken->second.find(RTTI::MainBoundary);
         if (foundBoundary != foundToken->second.end())
            return foundBoundary->second;
         else if (not foundToken->second.empty())
            return foundToken->second.begin()->second;
         return (R) nullptr;
      }
   }

   /// Get a list of all the interpretations for an ambiguous token           
   /// These can be data types, verbs, traits, or constants                   
   ///   @param token - the token to search for                               
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the list of associated meta definitions                      
   const MetaList& Registry::GetMetaList(
      const auto& where, const Token& token, const Token& boundary
   ) const noexcept {
      static const MetaList fallback {};
      const auto lc = ToLowercase(ToLastToken(token));
      const auto foundToken = where.find(lc);
      if (foundToken == where.end())
         return fallback;

      if (not boundary.empty()) {
         // Search in a specific boundary                               
         const auto foundBoundary = foundToken->second.find(boundary);
         if (foundBoundary == foundToken->second.end())
            return fallback;
         return foundBoundary->second;
      }
      else {
         // Always prefer the MAIN boundary, because it's persistent    
         const auto foundBoundary = foundToken->second.find(RTTI::MainBoundary);
         if (foundBoundary != foundToken->second.end())
            return foundBoundary->second;
         else if (not foundToken->second.empty())
            return foundToken->second.begin()->second;
         return fallback;
      }
   }

   /// Get an existing meta data definition by its token and boundary         
   ///   @param token - the token of the data definition                      
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the definition, or nullptr if not found                      
   DMeta Registry::GetMetaData(const Token& token, const Token& boundary) const noexcept {
      return GetMeta(mMetaData, token, boundary);
   }

   /// Get an existing meta constant definition by its token and boundary     
   ///   @param token - the token of the constant definition                  
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the definition, or nullptr if not found                      
   CMeta Registry::GetMetaConstant(const Token& token, const Token& boundary) const noexcept {
      return GetMeta(mMetaConstants, token, boundary);
   }

   /// Get an existing meta trait definition by its token and boundary        
   ///   @param token - the token of the trait definition                     
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the definition, or nullptr if not found                      
   TMeta Registry::GetMetaTrait(const Token& token, const Token& boundary) const noexcept {
      return GetMeta(mMetaTraits, token, boundary);
   }

   /// Get an existing meta verb definition by its token and boundary         
   ///   @param token - the token of the verb definition                      
   ///                  you can search by positive, as well as negative token 
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the definition, or nullptr if not found                      
   VMeta Registry::GetMetaVerb(const Token& token, const Token& boundary) const noexcept {
      return GetMeta(mMetaVerbs, token, boundary);
   }

   /// Get an existing meta verb definition by its operator token and boundary
   ///   @param token - the operator of the verb definition                   
   ///                  you can search by positive, as well as negative       
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the definition, or nullptr if not found                      
   VMeta Registry::GetOperator(const Token& token, const Token& boundary) const noexcept {
      const auto lc = IsolateOperator(token);
      return GetMeta(mOperators, lc, boundary);
   }

   /// Get a list of all the interpretations for an ambiguous token           
   /// These can be data types, verbs, traits, or constants                   
   ///   @param token - the token to search for                               
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the list of associated meta definitions                      
   const MetaList& Registry::GetAmbiguousMeta(
      const Token& token, const Token& boundary
   ) const noexcept {
      return GetMetaList(mMetaAmbiguous, token, boundary);
   }
   
   /// Disambiguate a token                                                   
   /// Works in the following way:                                            
   ///   1. Checks keyword for an exact match (not case-sensitive)            
   ///      If such is found, the meta is returned directly                   
   ///   2. If multiple keywords match partially:                             
   ///      a. Meta-data and meta-traits are always with higher priority than 
   ///         meta-verbs and meta-constants.                                 
   ///      b. A keyword starting with a capital letter is always hinted as   
   ///         meta-data, instead of meta-trait.                              
   ///   3. If after all these disambiguation attempts there's still ambiguity
   ///      throw a Meta exception - the ambiguity has to be manually fixed   
   ///   @param keyword - the token to search for                             
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return the disambiguated token; throws if not found/ambiguous       
   AMeta Registry::DisambiguateMeta(const Token& keyword, const Token& boundary) const {
      auto& symbols = GetAmbiguousMeta(keyword, boundary);
      LANGULUS_ASSERT(not symbols.empty(), Meta,
         "Keyword not found", ": `", keyword, '`');
      if (symbols.size() == 1) {
         // No ambiguity, just return the single result (1)             
         return *symbols.begin();
      }

      // Collect all origin types, and work with those                  
      // Also, GetAmbiguousMeta works only with the last part of the    
      // keyword, but the keyword might contain hints as to which       
      // ambiguous meta to pick. Discard symbols that do not            
      // contain the provided keyword (not case sensitive)              
      const auto lowercased = ToLowercase(keyword);
      MetaList origins;
      for (auto& meta : symbols) {
         LANGULUS_ASSUME(DevAssumes, meta, "Bad meta");
         const auto meta_lc_token = ToLowercase(meta->mToken);
         if (meta_lc_token.find(lowercased) == std::string::npos)
            continue;

         const auto dmeta = meta.As<DMeta>();
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

      DMeta meta_data;
      DMeta meta_data_exact_match;
      Count meta_data_encountered = 0;
      TMeta meta_trait;
      Count meta_trait_encountered = 0;

      if (origins.size() == 1) {
         // Candidate types reduced to a single relevant origin (1)     
         return *origins.begin();
      }
      else for (auto& candidate : origins) {
         // There's a chance, that one of the symbols matches the       
         // lowercased keyword exactly (1)                              
         if (ToLowercase(candidate->mToken) == lowercased)
            meta_data_exact_match = candidate;

         if (candidate.Kind() == Meta::Data) {
            meta_data = candidate;
            ++meta_data_encountered;
         }
         else if (candidate.Kind() == Meta::Trait) {
            meta_trait = candidate;
            ++meta_trait_encountered;
         }
      }

      // If there are data/traits available, discard verbs/consts (2.a) 
      if (meta_data_encountered and meta_trait_encountered) {
         // Both data and traits encountered, check first letter (2.b)  
         if (::std::islower(keyword[0])) {
            if (meta_trait_encountered == 1)
               return meta_trait;
         }
         else {
            if (meta_data_encountered == 1)
               return meta_data;
            else if (meta_data_exact_match)
               return meta_data_exact_match;
         }
      }
      else if (meta_data_encountered == 1) {
         // No traits, just meta data                                   
         // If it's just one, directly return it (2.a)                  
         return meta_data;
      }
      else if (meta_data_exact_match) {
         // If there was an exact match - now's the time to return it   
         return meta_data_exact_match;
      }
      else if (meta_trait_encountered == 1) {
         // No data, just meta traits                                   
         // If it's just one, directly return it (2.a)                  
         return meta_trait;
      }

      // Unfixable ambiguity reached, report error and throw (3)        
      const auto tab = Logger::ErrorTab(
         "Ambiguous symbol: `", keyword, "`; Could be one of: "
      );
      for (auto& meta : origins) {
         Logger::Line('`', Logger::PushDarkYellow,
            meta->mToken, Logger::Pop, "` (", meta.Kind(), ')');
      }
      LANGULUS_THROW(Meta, "Ambiguous symbol");
   }

   /// Resolve a file extension                                               
   ///   @param token - the file extension to search for                      
   ///   @param boundary - the boundary to search in (optional)               
   ///   @return all meta definitions associated with the file extension      
   const MetaList& Registry::ResolveFileExtension(
      const Token& token, const Token& boundary
   ) const {
      return GetMetaList(mFileDatabase, token, boundary);
   }
   
   /// Register most relevant token to the ambiguous token map                
   ///   @param boundary - the boundary to register in                        
   ///   @param token - the token to register                                 
   ///   @param meta - the definition to add                                  
   void Registry::RegisterAmbiguous(
      const Token& boundary, const Lowercase& token, AMeta meta
   ) noexcept {
      Lowercase ambiguous {ToLastToken(token)};
      const auto foundAmbiguous = mMetaAmbiguous.find(ambiguous);
      if (foundAmbiguous == mMetaAmbiguous.end()) {
         mMetaAmbiguous.insert({std::move(ambiguous), {{boundary, {meta}}}});
         return;
      }

      const auto foundBoundary = foundAmbiguous->second.find(boundary);
      if (foundBoundary == foundAmbiguous->second.end())
         foundAmbiguous->second.insert({boundary, {meta}});
      else
         foundBoundary->second.insert(meta);
   }

   /// Unregister most relevant token from the ambiguous token map            
   ///   @attention only definitions in current boundary are affected         
   ///   @param boundary - the boundary to unregister from                    
   ///   @param token - the token to unregister                               
   ///   @param meta - the definition to remove                               
   void Registry::UnregisterAmbiguous(
      const Token& boundary, const Lowercase& token, AMeta meta
   ) noexcept {
      Lowercase ambiguous {ToLastToken(token)};
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

   /// A common function to register a specific type in a specific place      
   ///   @tparam REGISTER_AMBIGUOUS - whether or not to register 'lc' in the  
   ///                                ambiguity map                           
   ///   @param meta - the new meta definition to register                    
   ///   @param where - place to insert the definition                        
   ///   @param lc - the lowercase token to reserve                           
   ///   @param boundary - the boundary to register in                        
   ///   @return the newly defined meta for that token                        
   template<bool REGISTER_AMBIGUOUS>
   auto Registry::Register(
      auto meta, auto& where, const Lowercase& lc, const Token& boundary
   ) IF_UNSAFE(noexcept) {
      LANGULUS_ASSUME(DevAssumes, not boundary.empty(), "Bad boundary");

      // If reached, then not found, so insert a new definition         
      const auto foundToken = where.find(lc);
      if (foundToken == where.end())
         where.insert({lc, {{boundary, meta}}});
      else
         foundToken->second.insert({boundary, meta});

      // Insert the last token to the ambiguity map                     
      if constexpr (REGISTER_AMBIGUOUS)
         RegisterAmbiguous(boundary, lc, meta);
      return meta;
   }

   /// Register a data definition                                             
   ///   @attention assumes token is not yet registered in the given boundary 
   ///   @param token - the data token to reserve                             
   ///   @param boundary - the boundary to register in                        
   ///   @return the newly defined meta data for that token                   
   DMeta Registry::RegisterData(
      const Token& token, const Token& boundary
   ) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaData(lc, boundary),
         "Data with this name is already registered: ", token);

      LANGULUS_ASSERT(not GetMetaTrait(lc), Meta,
         "Data name conflicts with trait: ", token);
      LANGULUS_ASSERT(not GetMetaVerb(lc), Meta,
         "Data name conflicts with verb: ", token);
      LANGULUS_ASSERT(not GetMetaConstant(lc), Meta,
         "Data name conflicts with constant: ", token);

      return Register(new MetaData {token}, mMetaData, lc, boundary);
   }

   /// Register a constant definition                                         
   ///   @attention assumes token is not yet registered in the given boundary 
   ///   @param token - the constant token to reserve                         
   ///   @param boundary - the boundary to register in                        
   ///   @return the newly defined meta constant for that token               
   CMeta Registry::RegisterConstant(
      const Token& token, const Token& boundary
   ) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaConstant(lc, boundary),
         "Constant with this name is already registered: ", token);

      LANGULUS_ASSERT(not GetMetaTrait(lc), Meta,
         "Constant name conflicts with trait: ", token);
      LANGULUS_ASSERT(not GetMetaVerb(lc), Meta,
         "Constant name conflicts with verb: ", token);
      LANGULUS_ASSERT(not GetMetaData(lc), Meta,
         "Constant name conflicts with data: ", token);

      return Register(new MetaConst {token}, mMetaConstants, lc, boundary);
   }

   /// Register a trait definition                                            
   ///   @attention assumes token is not yet registered in the given boundary 
   ///   @param token - the trait token to reserve                            
   ///   @param boundary - the boundary to register in                        
   ///   @return the newly defined meta trait for that token                  
   TMeta Registry::RegisterTrait(
      const Token& token, const Token& boundary
   ) {
      auto lc = ToLowercase(token);
      LANGULUS_ASSUME(DevAssumes, not GetMetaTrait(lc, boundary),
         "Trait with this name is already registered: ", token);

      LANGULUS_ASSERT(not GetMetaConstant(lc), Meta,
         "Trait name conflicts with constant: ", token);
      LANGULUS_ASSERT(not GetMetaVerb(lc), Meta,
         "Trait name conflicts with verb: ", token);
      LANGULUS_ASSERT(not GetMetaData(lc), Meta,
         "Trait name conflicts with data: ", token);

      return Register(new MetaTrait {token}, mMetaTraits, lc, boundary);
   }

   /// Register a verb definition                                             
   ///   @attention assumes tokens are not yet registered                     
   ///   @param cppname - the original verb class name                        
   ///   @param token - the positive verb token to reserve                    
   ///   @param tokenReverse - the negative verb token to reserve             
   ///   @param op - the positive verb operator to reserve                    
   ///   @param opReverse - the negative verb operator to reserve             
   ///   @param boundary - the boundary to register in                        
   ///   @return the newly defined meta verb for that token configuration     
   VMeta Registry::RegisterVerb(
      const Token& cppname,
      const Token& token,
      const Token& tokenReverse,
      const Token& op,
      const Token& opReverse,
      const Token& boundary
   ) {
      LANGULUS_ASSUME(DevAssumes, not boundary.empty(),
         "Bad boundary provided");
      const auto cppnamelc = ToLowercase(cppname);

      IF_SAFE(const auto uniqueFound = mUniqueVerbs.find(cppnamelc));
      LANGULUS_ASSUME(DevAssumes, uniqueFound == mUniqueVerbs.end()
         or uniqueFound->second.find(boundary) == uniqueFound->second.end(),
         "Verb already registered for that boundary");

      auto lc1 = ToLowercase(token);
      auto lc2 = ToLowercase(tokenReverse);
      LANGULUS_ASSUME(DevAssumes, not GetMetaVerb(lc1, boundary)
                              and not GetMetaVerb(lc2, boundary),
         "Verb already registered with one of the following tokens: ",
         token, ", ", tokenReverse
      );

      LANGULUS_ASSERT(not GetMetaConstant(token), Meta,
         "Verb positive token conflicts with constant: ", token);
      LANGULUS_ASSERT(not GetMetaTrait(token), Meta,
         "Verb positive token conflicts with trait: ", token);
      LANGULUS_ASSERT(not GetMetaData(token), Meta,
         "Verb positive token conflicts with data: ", token);

      LANGULUS_ASSERT(not GetMetaConstant(tokenReverse), Meta,
         "Verb negative token conflicts with constant: ", tokenReverse);
      LANGULUS_ASSERT(not GetMetaTrait(tokenReverse), Meta,
         "Verb negative token conflicts with trait: ", tokenReverse);
      LANGULUS_ASSERT(not GetMetaData(tokenReverse), Meta,
         "Verb negative token conflicts with data: ", tokenReverse);

      Lowercase op1;
      if (not op.empty()) {
         op1 = IsolateOperator(op);
         LANGULUS_ASSUME(DevAssumes, not GetOperator(op1, boundary),
            "Positive operator already registered");

         LANGULUS_ASSERT(not GetMetaConstant(op1), Meta,
            "Verb positive operator conflicts with constant: ", op1);
         LANGULUS_ASSERT(not GetMetaTrait(op1), Meta,
            "Verb positive operator conflicts with trait: ", op1);
         LANGULUS_ASSERT(not GetMetaData(op1), Meta,
            "Verb positive operator conflicts with data: ", op1);
      }

      Lowercase op2;
      if (not opReverse.empty()) {
         op2 = IsolateOperator(opReverse);
         LANGULUS_ASSUME(DevAssumes, not GetOperator(op2, boundary),
            "Negative operator already registered");

         LANGULUS_ASSERT(not GetMetaConstant(op2), Meta,
            "Verb positive operator conflicts with constant: ", op2);
         LANGULUS_ASSERT(not GetMetaTrait(op2), Meta,
            "Verb positive operator conflicts with trait: ", op2);
         LANGULUS_ASSERT(not GetMetaData(op2), Meta,
            "Verb positive operator conflicts with data: ", op2);
      }

      const auto meta = Register<false>(
         new MetaVerb {token, tokenReverse, op, opReverse},
         mUniqueVerbs, cppnamelc, boundary
      );
      VERBOSE("Verb ", token, "/", tokenReverse, " registered");

      Register(meta, mMetaVerbs, lc1, boundary);
      if (lc1 != lc2)
         Register(meta, mMetaVerbs, lc2, boundary);

      if (not op1.empty()) {
         Register<false>(meta, mOperators, op1, boundary);
         VERBOSE("Operator ", op1, " registered");
      }

      if (not op2.empty() and op1 != op2) {
         Register<false>(meta, mOperators, op2, boundary);
         VERBOSE("Operator ", op1, " registered");
      }

      return meta;
   }

   /// Register file extension                                                
   ///   @param token - the file extension token to reserve                   
   ///   @param type - the data to associate file with                        
   ///   @param boundary - the boundary to register in                        
   void Registry::RegisterFileExtension(
      const Token& token, DMeta type, const Token& boundary
   ) IF_UNSAFE(noexcept) {
      LANGULUS_ASSUME(DevAssumes, not token.empty(),
         "Bad file extension");
      LANGULUS_ASSUME(DevAssumes, type,
         "Bad meta data for file extension ", token);
      LANGULUS_ASSUME(DevAssumes, not boundary.empty(),
         "Bad boundary provided");

      const auto lc = ToLowercase(token);
      const auto foundToken = mFileDatabase.find(lc);
      if (foundToken == mFileDatabase.end()) {
         mFileDatabase[lc].insert({boundary, {type.mMeta}});
         return;
      }

      const auto foundBoundary = foundToken->second.find(boundary);
      if (foundBoundary == foundToken->second.end())
         foundToken->second.insert({boundary, {type.mMeta}});
      else
         foundBoundary->second.insert(type.mMeta);
   }

   /// Runs through all definitions, and destroys all of those, that were     
   /// defined within the given boundary token                                
   ///   @param boundary - the boundary token to search for                   
   void Registry::UnloadBoundary(const Token& boundary) {
      VERBOSE(Logger::PushRed, Logger::Underline, 
         "Unloading library ", boundary, Logger::Pop);

      // Unload constants                                               
      for (auto pair = mMetaConstants.begin(); pair != mMetaConstants.end();) {
         auto found = pair->second.find(boundary);
         if (found == pair->second.end()) {
            ++pair;
            continue;
         }

         VERBOSE("Constant ", Logger::PushYellow, found->second->mToken,
            Logger::PopRed, " unregistered (", boundary, ")");

         UnregisterAmbiguous(boundary, pair->first, found->second.mMeta);
         delete found->second.mMeta;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaConstants.erase(pair);
         else ++pair;
      }

      // Unload file types                                              
      for (auto pair = mFileDatabase.begin(); pair != mFileDatabase.end();) {
         auto found = pair->second.find(boundary);
         if (found == pair->second.end()) {
            ++pair;
            continue;
         }

         VERBOSE("File ", Logger::PushCyan, pair->first,
            Logger::PopRed, " unregistered (", boundary, ")");

         pair->second.erase(found);
         if (pair->second.empty())
            pair = mFileDatabase.erase(pair);
         else ++pair;
      }

      // Unload data types                                              
      for (auto pair = mMetaData.begin(); pair != mMetaData.end();) {
         auto found = pair->second.find(boundary);
         if (found == pair->second.end()) {
            ++pair;
            continue;
         }

         VERBOSE("Data ", Logger::PushCyan, found->second->mToken,
            Logger::PopRed, " unregistered (", boundary, ")");

         UnregisterAmbiguous(boundary, pair->first, found->second.mMeta);
         delete found->second.mMeta;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaData.erase(pair);
         else ++pair;
      }

      // Unload traits                                                  
      for (auto pair = mMetaTraits.begin(); pair != mMetaTraits.end();) {
         auto found = pair->second.find(boundary);
         if (found == pair->second.end()) {
            ++pair;
            continue;
         }

         VERBOSE("Trait ", Logger::PushPurple, found->second->mToken,
            Logger::PopRed, " unregistered (", boundary, ")");

         UnregisterAmbiguous(boundary, pair->first, found->second.mMeta);
         delete found->second.mMeta;
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mMetaTraits.erase(pair);
         else ++pair;
      }

      // Unload verbs                                                   
      for (auto pair = mUniqueVerbs.begin(); pair != mUniqueVerbs.end();) {
         auto found = pair->second.find(boundary);
         if (found == pair->second.end()) {
            ++pair;
            continue;
         }

         VMeta definition = found->second;
         const auto lc1 = ToLowercase(definition->mToken);
         const auto lc2 = ToLowercase(definition->mTokenReverse);
         LANGULUS_ASSUME(DevAssumes, definition
            and definition == GetMetaVerb(lc1, boundary)
            and definition == GetMetaVerb(lc2, boundary),
            "Bad VMeta definition"
         );

         auto foundlc1 = mMetaVerbs.find(lc1);
         if (foundlc1 != mMetaVerbs.end())
            foundlc1->second.erase(boundary);
         auto foundlc2 = mMetaVerbs.find(lc2);
         if (foundlc2 != mMetaVerbs.end())
            foundlc2->second.erase(boundary);

         if (definition->mOperator.size()) {
            const auto op1 = IsolateOperator(definition->mOperator);
            VERBOSE("Operator ", Logger::PushDarkGreen, op1,
               Logger::PopRed, " unregistered (", boundary, ")");
            auto foundop1 = mOperators.find(op1);
            if (foundop1 != mOperators.end())
               foundop1->second.erase(boundary);
         }

         if (definition->mOperatorReverse.size()
         and definition->mOperatorReverse != definition->mOperator) {
            const auto op2 = IsolateOperator(definition->mOperatorReverse);
            VERBOSE("Operator ", Logger::PushDarkGreen, op2,
               Logger::PopRed, " unregistered (", boundary, ")");
            auto foundop2 = mOperators.find(op2);
            if (foundop2 != mOperators.end())
               foundop2->second.erase(boundary);
         }

         if (definition->mToken != definition->mTokenReverse) {
            VERBOSE("Verb ", Logger::PushDarkGreen,
               definition->mToken, "/", definition->mTokenReverse,
               Logger::PopRed, " unregistered (", boundary, ")");
         }
         else {
            VERBOSE("Verb ", Logger::PushDarkGreen,
               definition->mToken, Logger::PopRed,
               " unregistered (", boundary, ")");
         }

         UnregisterAmbiguous(boundary, lc1, definition.mMeta);
         UnregisterAmbiguous(boundary, lc2, definition.mMeta);
         pair->second.erase(found);
         if (pair->second.empty())
            pair = mUniqueVerbs.erase(pair);
         else ++pair;

         delete definition.mMeta;
      }
   }

   /// Get the shortest possible unambiguous token                            
   ///   @return the token                                                    
   Token Meta::GetShortestUnambiguousToken() const {
      auto& ambiguous = Instance.GetAmbiguousMeta(mToken);
      if (ambiguous.size() == 1)
         return ToLastToken(mToken);

      // Collect all origin types, and work with those                  
      Count datas = 0;
      Count traits = 0;
      MetaList origins;
      for (auto& meta : ambiguous) {
         const auto dmeta = meta.As<DMeta>();
         if (dmeta and dmeta->mOrigin) {
            origins.insert(dmeta->mOrigin);
            ++datas;
         }
         else {
            origins.insert(meta);

            if (dmeta)
               ++datas;
            else if (meta.Kind() == Meta::Trait)
               ++traits;
         }
      }

      // Some easy to do disambiguations                                
      // Meta datas/traits always win over verbs/constants              
      if (origins.size() == 1)
         return ToLastToken(mToken);
      else if ((datas  == 1 and traits == 0 and Kind() == Meta::Data)
           or  (traits == 1 and datas  == 0 and Kind() == Meta::Trait))
         return ToLastToken(mToken);
      else if (datas == 1 and traits == 1) {
         if (Kind() == Meta::Data) {
            // Token should be starting with a capital letter           
            return static_cast<const MetaData*>(this)->mTokenSanitized;
         }
         else if (Kind() == Meta::Trait) {
            // Token should be starting with a lower letter             
            return static_cast<const MetaTrait*>(this)->mTokenSanitized;
         }
      }

      // Start including namespaces, until the resulting token has      
      // exactly one match inside the ambiguous list                    
      auto start = ToLastToken(mToken).data() - 3;
      while (start >= mToken.data()) {
         if (*start == ':') {
            const auto candidate = mToken.substr(start - mToken.data() + 1);
            Count matches = 0;
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
