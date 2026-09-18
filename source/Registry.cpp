///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#include <Langulus/RTTI/Registry.hpp>
#include <Langulus/RTTI/DefinitionData.hpp>
#include <Langulus/RTTI/DefinitionTag.hpp>
#include <Langulus/RTTI/DefinitionConst.hpp>
#include <Langulus/RTTI/DefinitionVerb.hpp>
#include <ranges>

#if not LANGULUS_FEATURE(MANAGED_REFLECTION)
   #error "This file shouldn't be compiled if MANAGED_REFLECTION is disabled"
#endif

#if 0
   #include <Langulus/Logger.hpp>//TODO use the specialized headers for this
   #define VERBOSE_SCOPED(...) const auto scope = Logger::VerboseScoped(__VA_ARGS__);
   #define VERBOSE(...) Logger::Verbose(__VA_ARGS__)
#else
   #define VERBOSE_SCOPED(...)
   #define VERBOSE(...)
#endif


namespace Langulus::RTTI
{
   namespace 
   {
      // Definitions indexed by lowercase reflected name                
      template<class T>
      using MetaMap = ::std::unordered_map<Token, T>;
      //using MetaSet  = ::std::unordered_set<Inner::Definition const*>;
   
      // Definitions indexed by ID                                      
      template<class T>
      using Indexed = ::std::vector<T>;
   
      // @attention order of these containers matters!                  
      // Database for meta data definitions                             
      MetaMap<::std::unique_ptr<DefinitionData>> mMetaDataByCppName;
      MetaMap<DefinitionData const*>  mMetaDataByToken;
      Indexed<DefinitionData const*>  mMetaDataByID;
   
      // Database for named values                                      
      MetaMap<::std::unique_ptr<DefinitionConst>> mMetaConstantsByCppName;
      MetaMap<DefinitionConst const*> mMetaConstantsByToken;
      Indexed<DefinitionConst const*> mMetaConstantsByID;
   
      // Database for meta trait definitions                            
      MetaMap<::std::unique_ptr<DefinitionTag>> mMetaTagsByCppName;
      MetaMap<DefinitionTag const*>   mMetaTagsByToken;
      Indexed<DefinitionTag const*>   mMetaTagsByID;
   
      // Database for meta verb definitions                             
      MetaMap<::std::unique_ptr<DefinitionVerb>> mMetaVerbsByCppName;
      MetaMap<DefinitionVerb const*>  mMetaVerbsByToken;
      Indexed<DefinitionVerb const*>  mMetaVerbsByID;
   
      // Database for ambiguous tokens                                  
      // All definitions indexed by their last lowercased token         
      MetaMap<MetaSet> mMetaAmbiguous;
      // Meta data definitions, indexed by file extensions              
      MetaMap<MetaSet> mFileDatabase;
      
         
      /// Common way to extract something from the registry by ID             
      ///   @param where where to search in                                   
      ///   @param id the id to search for                                    
      ///   @return the found element, or nullptr if not found                
      auto GetMetaByID(const auto& where, size_t id) assumptious {
         LglsAssumeDevAndOptimize(id != 0, "Invalid ID");
         return where[id-1];
      }
   }
   
   /// Get an existing data definition by its CppNameOf                       
   ///   @param token the C++ name of the data definition                     
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaDataByCppName(const Token& token)
   noexcept -> DefinitionData const* {
      const auto foundToken = mMetaDataByCppName.find(token);
      if (foundToken == mMetaDataByCppName.end())
         return nullptr;
      return foundToken->second.get();
   }

   /// Get an existing data definition by its NameOf                          
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the reflected token of the data definition              
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaDataByToken(const Token& token)
   assumptious -> DefinitionData const* {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      const auto foundToken = mMetaDataByToken.find(Inner::ToLowercase(token));
      if (foundToken == mMetaDataByToken.end())
         return nullptr;
      return foundToken->second;
   }

   /// Get an existing constant definition by its CppNameOf                   
   ///   @param token the C++ name of the constant definition                 
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaConstByCppName(const Token& token)
   noexcept -> DefinitionConst const* {
      const auto foundToken = mMetaConstantsByCppName.find(token);
      if (foundToken == mMetaConstantsByCppName.end())
         return nullptr;
      return foundToken->second.get();
   }

   /// Get an existing constant definition by its NameOf                      
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the reflected token of the constant definition          
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaConstByToken(const Token& token)
   assumptious -> DefinitionConst const* {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      const auto foundToken = mMetaConstantsByToken.find(Inner::ToLowercase(token));
      if (foundToken == mMetaConstantsByToken.end())
         return nullptr;
      return foundToken->second;
   }

   /// Get an existing tag definition by its CppNameOf                        
   ///   @param token the C++ name of the tag definition                      
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaTagByCppName(const Token& token)
   noexcept -> DefinitionTag const* {
      const auto foundToken = mMetaTagsByCppName.find(token);
      if (foundToken == mMetaTagsByCppName.end())
         return nullptr;
      return foundToken->second.get();
   }

   /// Get an existing tag definition by its NameOfTag                        
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the reflected token of the tag definition               
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaTagByToken(const Token& token)
   assumptious -> DefinitionTag const* {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      const auto foundToken = mMetaTagsByToken.find(Inner::ToLowercase(token));
      if (foundToken == mMetaTagsByToken.end())
         return nullptr;
      return foundToken->second;
   }
   
   /// Get an existing verb definition by its CppNameOf                       
   ///   @param token the C++ name of the verb definition                     
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaVerbByCppName(const Token& token)
   noexcept -> DefinitionVerb const* {
      const auto foundToken = mMetaVerbsByCppName.find(token);
      if (foundToken == mMetaVerbsByCppName.end())
         return nullptr;
      return foundToken->second.get();
   }

   /// Get an existing verb definition by NameOfVerb/NameOfVerbReverse        
   /// or OperatorOfVerb/OperatorOfVerbReverse                                
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the reflected token of the verb definition              
   ///      you can search by positive, as well as negative token             
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaVerbByToken(const Token& token)
   assumptious -> DefinitionVerb const* {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      const auto foundToken = mMetaVerbsByToken.find(Inner::ToLowercase(token));
      if (foundToken == mMetaVerbsByToken.end())
         return nullptr;
      return foundToken->second;
   }
   
   /// Get an existing data definition by unpacking an ID                     
   ///   @param id the ID                                                     
   ///   @param sparse is the data type sparse?                               
   ///   @param constant is the data type constant?                           
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaDataByID(size_t id, bool sparse, bool constant)
   assumptious -> DefinitionData const* {
      LglsAssumeUserAndOptimize(id != 0, "Invalid ID");
      DefinitionData const* found = GetMetaByID(mMetaDataByID, id);
      LglsAssumeDevAndOptimize(found, "ID wasn't found");

      if (sparse and not found->mPtrIncludedInID) {
         LglsAssumeDevAndOptimize(found->mAddPtr,
            "An indirection ID for a type exists, "
            "but no such type has been reflected yet: ", found->mNameOf
         );
         found = found->mAddPtr;
      }

      if (constant) {
         LglsAssumeDevAndOptimize(found->mAddConst,
            "A constant ID for a type exists, "
            "but no such type has been reflected yet: ", found->mNameOf
         );
         found = found->mAddConst;
      }
      
      return found;
   }

   /// Get an existing tag definition by unpacking an ID                      
   ///   @param id the ID                                                     
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaTagByID(size_t id)
   assumptious -> DefinitionTag const* {
      LglsAssumeUserAndOptimize(id != 0, "Invalid ID");
      return GetMetaByID(mMetaTagsByID, id);
   }

   /// Get an existing verb definition by unpacking an ID                     
   ///   @param id the ID                                                     
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaVerbByID(size_t id)
   assumptious -> DefinitionVerb const* {
      LglsAssumeUserAndOptimize(id != 0, "Invalid ID");
      return GetMetaByID(mMetaVerbsByID, id);
   }

   /// Get an existing constant definition by unpacking an ID                 
   ///   @param id the ID                                                     
   ///   @return the definition, or nullptr if not found                      
   auto Registry::GetMetaConstByID(size_t id)
   assumptious -> DefinitionConst const* {
      LglsAssumeUserAndOptimize(id != 0, "Invalid ID");
      return GetMetaByID(mMetaConstantsByID, id);
   }

   /// Get a list of all the interpretations for an ambiguous token           
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the token to search for                                 
   ///   @return the set of associated meta definitions                       
   auto Registry::GetAmbiguousMeta(const Token& token)
   assumptious -> const MetaSet& {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      static const MetaSet fallback {};
      const auto foundToken = mMetaAmbiguous.find(Inner::ToLowercase(token));
      if (foundToken == mMetaAmbiguous.end())
         return fallback;
      return foundToken->second;
   }
   
   /// Disambiguate a token. Works in the following way:                      
   ///   1. Checks keyword for an exact match (not case-sensitive)            
   ///      If such is found, the meta is returned directly                   
   ///   2. If multiple keywords match partially:                             
   ///      a. Meta-data and meta-tags are always with higher priority than   
   ///         meta-verbs and meta-constants.                                 
   ///      b. A keyword starting with a capital letter is always hinted as   
   ///         meta-data, instead of meta-tag.                                
   ///   3. If after all these disambiguation attempts there's still ambiguity
   ///      throw an exception - the ambiguity has to be manually fixed       
   ///   @param keyword the token to search for                               
   ///   @return the disambiguated token; throws if not found/ambiguous       
   auto Registry::DisambiguateMeta(const Token& keyword)
   -> Inner::Definition const* {
      auto& symbols = GetAmbiguousMeta(keyword);
      LglsAssert(not symbols.empty(), "Keyword not found", ": `", keyword, '`');
      
      if (symbols.size() == 1) {
         // No ambiguity, just return the single result (1)             
         return *symbols.begin();
      }

      // Collect all origin types, and work with those                  
      // Also, GetAmbiguousMeta works only with the last part of the    
      // keyword, but the keyword might contain hints as to which       
      // ambiguous meta to pick. Discard symbols that do not            
      // contain the provided keyword (not case sensitive)              
      const auto lowercased = Inner::ToLowercase(keyword);
      MetaSet origins;
      for (auto& meta : symbols) {
         LglsAssumeDevAndOptimize(meta, "Bad meta");
         if (not meta->mNameOfLowercased.contains(lowercased))
            continue;
         
         if (auto dmeta = dynamic_cast<DefinitionData const*>(meta)) {
            if (dmeta->mOrigin)
               origins.insert(dmeta->mOrigin);
            else
               origins.insert(meta);
         }
         else origins.insert(meta);
      }

      LglsAssert(not origins.empty(),
         "No relevant origins for keyword", ": `", keyword, '`');

      DefinitionData const* meta_data             IF_SAFE(= nullptr);
      DefinitionData const* meta_data_exact_match IF_SAFE(= nullptr);
      DefinitionTag  const* meta_tag              IF_SAFE(= nullptr);
      size_t meta_data_encountered = 0;
      size_t meta_tag_encountered  = 0;

      if (origins.size() == 1) {
         // Candidate types reduced to a single relevant origin (1)     
         return *origins.begin();
      }

      for (auto& candidate : origins) {
         // There's a chance, that one of the symbols matches the       
         // lowercased keyword exactly (1)                              
         auto dmeta = dynamic_cast<DefinitionData const*>(candidate);
         if (candidate->mNameOfLowercased == lowercased and dmeta)
            meta_data_exact_match = dmeta;

         if (dmeta) {
            meta_data = dmeta;
            ++meta_data_encountered;
         }
         else if (auto tmeta = dynamic_cast<DefinitionTag const*>(candidate)) {
            meta_tag = tmeta;
            ++meta_tag_encountered;
         }
      }

      // If there are data/traits available, discard verbs/consts (2.a) 
      if (meta_data_encountered and meta_tag_encountered) {
         // Both data and traits encountered, check first letter (2.b)  
         if (IsLowercase(keyword[0])) {
            if (meta_tag_encountered == 1)
               return meta_tag;
         }
         else {
            if (meta_data_encountered == 1)
               return meta_data;

            if (meta_data_exact_match)
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
      else if (meta_tag_encountered == 1) {
         // No data, just meta traits                                   
         // If it's just one, directly return it (2.a)                  
         return meta_tag;
      }

      // Unfixable ambiguity reached, report error and throw (3)        
      const auto tab = Logger::ErrorScoped(
         "Ambiguous symbol: `", keyword, "`; Could be one of: "
      );
      
      for (auto& meta : origins) {
         Logger::Line('`', Logger::PushDarkYellow,
            meta->mNameOf, Logger::Pop, '`');
         
         if (dynamic_cast<DefinitionData const*>(meta))
            Logger::Append(" (data)");
         else if (dynamic_cast<DefinitionTag const*>(meta))
            Logger::Append(" (tag)");
         else if (dynamic_cast<DefinitionVerb const*>(meta))
            Logger::Append(" (verb)");
         else if (dynamic_cast<DefinitionConst const*>(meta))
            Logger::Append(" (constant)");
      }
      
      throw MetaException {"Ambiguous symbol", HERE()};
   }

   /// Resolve a file extension                                               
   ///   @attention assumes token doesn't contain spaces                      
   ///   @param token the file extension to search for                        
   ///   @return all meta definitions associated with the file extension      
   auto Registry::ResolveFileExtension(const Token& token)
   assumptious -> const MetaSet& {
      LglsAssumeUser(not token.contains(' '), "Token shouldn't contain spaces");
      static const MetaSet fallback {};
      const auto foundToken = mFileDatabase.find(Inner::ToLowercase(token));
      if (foundToken == mFileDatabase.end())
         return fallback;
      return foundToken->second;
   }

   /// Register a data definition                                             
   ///   @attention assumes token is not yet registered                       
   ///   @param cppname the C++ type name to register                         
   ///   @param token_messy the custom token used in scripting                
   ///   @return the newly defined meta data for that name                    
   auto Registry::RegisterData(const Token& cppname, const Token& token_messy) -> DefinitionData& {
      const ::std::string cppname_s {cppname};
      LglsAssumeDev(not mMetaDataByCppName.contains(cppname_s),
         "Data with this name is already registered: ", cppname);
      
      LglsAssert(not mMetaTagsByCppName.contains(cppname_s),
         "Data type already registered as tag: ", cppname);
      LglsAssert(not mMetaVerbsByCppName.contains(cppname_s),
         "Data type already registered as verb: ", cppname);
      LglsAssert(not mMetaConstantsByCppName.contains(cppname_s),
         "Data type already registered as constant: ", cppname);

      // Make sure scripting token doesn't conflict with other metas    
      // that use a capital first letter, such as constants             
      const auto token = Inner::NormalizeAtRuntime(token_messy);
      LglsAssert(not token.empty(),
         "Invalid data token is not allowed - "
         "you have equipped your type (or its base) with an empty CTTI_Named. "
         "The type in question is: ", cppname
      );
      
      auto lowercased_token = Inner::ToLowercase(token);
      if (mMetaDataByToken.contains(lowercased_token)) {
         LglsError("Data token conflict", " between ", cppname, " and ",
            mMetaDataByToken.at(lowercased_token)->mCppNameOf);
      }
      if (mMetaConstantsByToken.contains(lowercased_token)) {
         LglsError("Token conflict", " between data ", cppname, " and constant ",
            mMetaConstantsByToken.at(lowercased_token)->mCppNameOf);
      }

      // If reached, then not found, so insert a new definition         
      auto new_meta = ::std::make_unique<DefinitionData>(cppname);
      auto meta = new_meta.get();

      // Index by C++ name                                              
      mMetaDataByCppName[meta->mCppNameOf] = LglsMov(new_meta);
      
      // Index by lowercased token                                      
      meta->mNameOf = token;
      meta->mNameOf[0] = ToUppercase(token[0]);
      meta->mNameOfLowercased = LglsMov(lowercased_token);
      // @important notice how key is made from heap-allocated          
      // member variable. This guarantees, that if a boundary is        
      // unloaded, the name data remains untouched on the heap          
      mMetaDataByToken[meta->mNameOfLowercased] = meta;

      // Index by last lowercase token                                  
      mMetaAmbiguous[Inner::ToLastToken(meta->mNameOfLowercased)].insert(meta);
      return *meta;
   }

   /// Reserves a data ID for more compact representation of metadata.        
   /// Used in packed pointers to definitions.                                
   ///   @param meta the definition to reserve ID for                         
   ///   @attention assumes meta definition is stripped from a single level   
   ///      of indirection, constness and volatileness                        
   ///   @return the new ID                                                   
   auto Registry::ReserveDataID(DefinitionData const* meta) -> size_t {
      mMetaDataByID.push_back(meta);
      return mMetaDataByID.size();
   }

   /// Register a constant definition                                         
   ///   @attention assumes token is not yet registered                       
   ///   @param cppname the C++ type name to register                         
   ///   @param token_messy the custom token used in scripting                
   ///   @return the newly defined meta constant for that token               
   auto Registry::RegisterConst(const Token& cppname, const Token& token_messy) -> DefinitionConst& {
      const ::std::string cppname_s {cppname};
      LglsAssumeDev(not mMetaConstantsByCppName.contains(cppname_s),
         "Constant with this name is already registered: ", cppname);

      LglsAssert(not mMetaDataByCppName.contains(cppname_s),
         "Constant already registered as data: ", cppname);
      LglsAssert(not mMetaTagsByCppName.contains(cppname_s),
         "Constant already registered as tag: ", cppname);
      LglsAssert(not mMetaVerbsByCppName.contains(cppname_s),
         "Constant already registered as verb: ", cppname);

      // Make sure scripting token doesn't conflict with other metas    
      // that use a capital first letter, such as data definitions      
      auto token = Inner::NormalizeAtRuntime(token_messy);
      LglsAssert(not token.empty(),
         "Invalid constant token is not allowed - "
         "you have equipped your constant with an empty CTTI::NamedValue. "
         "The constant in question is: ", cppname
      );

      auto lowercased_token = Inner::ToLowercase(token);
      if (mMetaDataByToken.contains(lowercased_token)) {
         LglsError("Token conflict", " between constant ", cppname, " and data ",
            mMetaDataByToken.at(lowercased_token)->mCppNameOf);
      }
      if (mMetaConstantsByToken.contains(lowercased_token)) {
         LglsError("Constant token conflict", " between ", cppname, " and ",
            mMetaConstantsByToken.at(lowercased_token)->mCppNameOf);
      }

      // If reached, then not found, so insert a new definition         
      auto new_meta = ::std::make_unique<DefinitionConst>(cppname);
      auto meta = new_meta.get();

      // Index by C++ name                                              
      mMetaConstantsByCppName[meta->mCppNameOf] = LglsMov(new_meta);

      // Index by ID                                                    
      mMetaConstantsByID.push_back(meta);
      meta->mID = mMetaConstantsByID.size();

      // Index by lowercased token                                      
      meta->mNameOf = LglsMov(token);
      meta->mNameOf[0] = ToUppercase(meta->mNameOf[0]);
      meta->mNameOfLowercased = LglsMov(lowercased_token);
      // @important notice how key is made from heap-allocated          
      // member variable. This guarantees, that if a boundary is        
      // unloaded, the name data remains untouched on the heap          
      mMetaConstantsByToken[meta->mNameOfLowercased] = meta;

      // Index by last lowercase token                                  
      mMetaAmbiguous[Inner::ToLastToken(meta->mNameOfLowercased)].insert(meta);
      return *meta;
   }

   /// Register a tag definition                                              
   ///   @attention assumes token is not yet registered                       
   ///   @param cppname the C++ type name to register                         
   ///   @param token_messy the custom token used in scripting                
   ///   @return the newly defined meta trait for that token                  
   auto Registry::RegisterTag(const Token& cppname, const Token& token_messy) -> DefinitionTag& {
      const ::std::string cppname_s {cppname};
      LglsAssumeDev(not mMetaTagsByCppName.contains(cppname_s),
         "Tag with this name is already registered: ", cppname);

      LglsAssert(not mMetaDataByCppName.contains(cppname_s),
         "Tag already registered as data: ", cppname);
      LglsAssert(not mMetaConstantsByCppName.contains(cppname_s),
         "Tag already registered as constant: ", cppname);
      LglsAssert(not mMetaVerbsByCppName.contains(cppname_s),
         "Tag already registered as verb: ", cppname);

      // Make sure scripting token doesn't conflict with other metas    
      // that use a lowercase first letter, such as verbs               
      const auto token = Inner::ToLowercase(token_messy);
      LglsAssert(not token.empty(),
         "Invalid tag token is not allowed - "
         "you have equipped your type (or its base) with an empty CTTI_DefineTag. "
         "The type in question is: ", cppname
      );

      auto lowercased_token = Inner::ToLowercase(token);
      if (mMetaTagsByToken.contains(lowercased_token)) {
         LglsError("Tag token conflict", " between ", cppname, " and ",
            mMetaTagsByToken.at(lowercased_token)->mCppNameOf);
      }
      if (mMetaVerbsByToken.contains(lowercased_token)) {
         LglsError("Token conflict", " between tag ", cppname, " and verb ",
            mMetaVerbsByToken.at(lowercased_token)->mCppNameOf);
      }

      // If reached, then not found, so insert a new definition         
      auto new_meta = ::std::make_unique<DefinitionTag>(cppname);
      auto meta = new_meta.get();

      // Index by C++ name                                              
      mMetaTagsByCppName[meta->mCppNameOf] = LglsMov(new_meta);

      // Index by ID                                                    
      mMetaTagsByID.push_back(meta);
      meta->mID = mMetaTagsByID.size();

      // Index by lowercased token                                      
      meta->mNameOf = LglsMov(lowercased_token);
      meta->mNameOfLowercased = meta->mNameOf;
      // @important notice how key is made from heap-allocated          
      // member variable. This guarantees, that if a boundary is        
      // unloaded, the name data remains untouched on the heap          
      mMetaTagsByToken[meta->mNameOfLowercased] = meta;

      // Index by last lowercase token                                  
      mMetaAmbiguous[Inner::ToLastToken(meta->mNameOf)].insert(meta);
      return *meta;
   }

   /// Register a verb definition                                             
   ///   @attention assumes tokens are not yet registered                     
   ///   @param cppname the C++ type name to register                         
   ///   @param token_messy positive verb token                               
   ///   @param tokenRev_messy negative verb token (optional)                 
   ///   @param op positive verb operator (optional)                          
   ///   @param opRev negative verb operator (optional)                       
   ///   @return the newly defined meta verb for that token configuration     
   auto Registry::RegisterVerb(
      Token const& cppname,
      Token const& token_messy,
      Token const& tokenRev_messy,
      Token const& op,
      Token const& opRev
   ) -> DefinitionVerb& {
      const ::std::string cppname_s {cppname};
      LglsAssumeDev(not mMetaVerbsByCppName.contains(cppname_s),
         "Verb with this name is already registered: ", cppname);

      LglsAssert(not mMetaDataByCppName.contains(cppname_s),
         "Verb already registered as data: ", cppname);
      LglsAssert(not mMetaConstantsByCppName.contains(cppname_s),
         "Verb already registered as constant: ", cppname);
      LglsAssert(not mMetaTagsByCppName.contains(cppname_s),
         "Verb already registered as tag: ", cppname);

      const auto token = Inner::ToLowercase(token_messy);
      LglsAssert(not token.empty(),
         "Invalid verb token is not allowed - "
         "you have equipped your verb (or its base) with an empty CTTI_DefineVerb. "
         "The verb in question is: ", cppname);
      
      const auto tokenRev = Inner::ToLowercase(tokenRev_messy);
      LglsAssert(token != tokenRev,
         "Verb can't have the same positive and negative tokens for: ", cppname);
      
      LglsAssert(op != opRev or op.empty(),
         "Verb can't have the same positive and negative operators for: ", cppname);
      LglsAssert(IsASCII(op),
         "Verb positive operator isn't ASCII for: ", cppname);
      LglsAssert(IsASCII(opRev),
         "Verb reverse operator isn't ASCII for: ", cppname);

      // Make sure scripting token doesn't conflict with other metas    
      // that use a lowercase first letter, such as tags                
      auto lowercased_token = Inner::ToLowercase(token);
      if (mMetaVerbsByToken.contains(lowercased_token)) {
         LglsError("Verb positive token conflict", " between ", cppname, " and ",
            mMetaVerbsByToken.at(lowercased_token)->mCppNameOf);
      }
      if (mMetaTagsByToken.contains(lowercased_token)) {
         LglsError("Token conflict", " between verb positive token in ", cppname, " and tag ",
            mMetaTagsByToken.at(lowercased_token)->mCppNameOf);
      }

      auto lowercased_token_rev = Inner::ToLowercase(tokenRev);
      if (not tokenRev.empty() and mMetaVerbsByToken.contains(lowercased_token_rev)) {
         LglsError("Verb negative token conflict", " between ", cppname, " and ",
            mMetaVerbsByToken.at(lowercased_token_rev)->mCppNameOf);
      }
      if (not tokenRev.empty() and mMetaTagsByToken.contains(lowercased_token_rev)) {
         LglsError("Token conflict", " between verb negative token in ", cppname, " and tag ",
            mMetaTagsByToken.at(lowercased_token_rev)->mCppNameOf);
      }

      auto lowercased_op = Inner::ToLowercase(op);
      auto stripped_op = Inner::StripSpaces(lowercased_op);
      if (not stripped_op.empty() and mMetaVerbsByToken.contains(stripped_op)) {
         LglsError("Verb positive operator conflict", " between ", cppname, " and ",
            mMetaVerbsByToken.at(stripped_op)->mCppNameOf);
      }
      if (not stripped_op.empty() and mMetaTagsByToken.contains(stripped_op)) {
         LglsError("Token conflict", " between verb positive operator in ", cppname, " and tag ",
            mMetaTagsByToken.at(stripped_op)->mCppNameOf);
      }

      auto lowercased_op_rev = Inner::ToLowercase(opRev);
      auto stripped_op_rev = Inner::StripSpaces(lowercased_op_rev);
      if (not stripped_op_rev.empty() and mMetaVerbsByToken.contains(stripped_op_rev)) {
         LglsError("Verb negative operator conflict", " between ", cppname, " and ",
            mMetaVerbsByToken.at(stripped_op_rev)->mCppNameOf);
      }
      if (not stripped_op_rev.empty() and mMetaTagsByToken.contains(stripped_op_rev)) {
         LglsError("Token conflict", " between verb negative operator in ", cppname, " and tag ",
            mMetaTagsByToken.at(stripped_op_rev)->mCppNameOf);
      }

      // If reached, then not found, so insert a new definition         
      auto new_meta = ::std::make_unique<DefinitionVerb>(cppname);
      auto meta = new_meta.get();

      // Index by C++ name                                              
      mMetaVerbsByCppName[meta->mCppNameOf] = LglsMov(new_meta);

      // Index by ID                                                    
      mMetaVerbsByID.push_back(meta);
      meta->mID = mMetaVerbsByID.size();

      // Index by lowercased tokens                                     
      meta->mNameOf = LglsMov(lowercased_token);
      meta->mNameOfReverse = LglsMov(lowercased_token_rev);
      meta->mOperator = LglsMov(lowercased_op);
      meta->mOperatorReverse = LglsMov(lowercased_op_rev);
      meta->mOperatorStripped = Inner::StripSpaces(meta->mOperator);
      meta->mOperatorReverseStripped = Inner::StripSpaces(meta->mOperatorReverse);
      // Amalgamate all tokens in this one                              
      // Verb disambiguation is a bit more complex                      
      meta->mNameOfLowercased = meta->mNameOf
                        + " " + meta->mNameOfReverse
                        + " " + ::std::string(meta->mOperatorStripped)
                        + " " + ::std::string(meta->mOperatorReverseStripped);

      // @important notice how key is made from heap-allocated          
      // member variable. This guarantees, that if a boundary is        
      // unloaded, the name data remains untouched on the heap          
      mMetaVerbsByToken[meta->mNameOf] = meta;
      if (not meta->mNameOfReverse.empty())
         mMetaVerbsByToken[meta->mNameOfReverse] = meta;
      if (not meta->mOperatorStripped.empty())
         mMetaVerbsByToken[meta->mOperatorStripped] = meta;
      if (not meta->mOperatorReverseStripped.empty())
         mMetaVerbsByToken[meta->mOperatorReverseStripped] = meta;

      // Index by last lowercase token                                  
      mMetaAmbiguous[Inner::ToLastToken(meta->mNameOf)].insert(meta);
      if (not meta->mNameOfReverse.empty())
         mMetaAmbiguous[Inner::ToLastToken(meta->mNameOfReverse)].insert(meta);
      if (not meta->mOperatorStripped.empty())
         mMetaAmbiguous[meta->mOperatorStripped].insert(meta);
      if (not meta->mOperatorReverseStripped.empty())
         mMetaAmbiguous[meta->mOperatorReverseStripped].insert(meta);
      return *meta;
   }

   /// Register file extension                                                
   ///   @param token the file extension token to reserve                     
   ///   @param type the data to associate file with                          
   void Registry::RegisterFileExtension(const Token& token, DefinitionData* type)
   assumptious {
      LglsAssumeDev(not token.empty(), "Bad file extension");
      LglsAssumeDevAndOptimize(type, "Bad meta data for file extension: ", token);

      const auto lc = Inner::ToLowercase(token);
      const auto foundToken = mFileDatabase.find(lc);
      if (foundToken == mFileDatabase.end())
         mFileDatabase[lc].insert({type});
      else
         foundToken->second.insert(type);
   }

   /// Runs through all definitions and destroys all of those, that were      
   /// defined only within the given boundary token                           
   ///   @param boundary the boundary token to search for                     
   void Registry::UnloadBoundary(const Token& boundary) {
      LglsAssumeDev(not boundary.empty(), "Can't unload main boundary");
      VERBOSE_SCOPED(Logger::Red, Logger::Underline, 
         "Unloading boundary ", boundary);

      //                                                                
      // Unload constants                                               
      for (auto pair = mMetaConstantsByCppName.begin(); pair != mMetaConstantsByCppName.end();) {
         auto definition = pair->second.get();
         if (not definition->mBoundaries.erase(::std::string{boundary})) {
            // Boundary is irrelevant for this definition               
            ++pair;
            continue;
         }

         if (not definition->mBoundaries.empty()) {
            // Definition is still used in other boundaries             
            ++pair;
            continue;
         }

         // If this is reached, then it is time to destroy the          
         // definition - it is no longer in use                         
         VERBOSE(
            "Constant ", Logger::Yellow, definition->mNameOf,
            Logger::Red, " unregistered"
         );

         // Remove from indexing by ID                                  
         if (mMetaConstantsByID[definition->mID] == definition)
            mMetaConstantsByID[definition->mID] = nullptr;

         // Remove from the ambiguity map                               
         const auto ambiguous = mMetaAmbiguous.find(definition->mNameOfLowercased);
         ambiguous->second.erase(definition);
         if (ambiguous->second.empty())
            mMetaAmbiguous.erase(ambiguous);

         // Finally, remove it from registry (delete unique_ptr)        
         mMetaConstantsByToken.erase(definition->mNameOfLowercased);
         pair = mMetaConstantsByCppName.erase(pair);
      }

      //                                                                
      // Unload file types (must be done before deleting meta data)     
      for (auto pair = mFileDatabase.begin(); pair != mFileDatabase.end();) {
         for (auto def = pair->second.begin(); def != pair->second.end();) {
            if ((*def)->mBoundaries.size() == 1 and (*def)->mBoundaries.contains(::std::string{boundary}))
               def = pair->second.erase(def);
            else
               ++def;
         }

         VERBOSE(
            "File ", Logger::Push, pair->first, Logger::Red, " unregistered"
         );
         
         if (pair->second.empty())
            pair = mFileDatabase.erase(pair);
         else
            ++pair;
      }

      //                                                                
      // Unload data types                                              
      for (auto pair = mMetaDataByCppName.begin(); pair != mMetaDataByCppName.end();) {
         auto definition = pair->second.get();
         if (not definition->mBoundaries.erase(::std::string{boundary})) {
            // Boundary is irrelevant for this definition               
            ++pair;
            continue;
         }

         if (not definition->mBoundaries.empty()) {
            // Definition is still used in other boundaries, make sure  
            // we pick new function pointers                            
            definition->mOtherBoundaries.erase(::std::string{boundary});
            definition->mCurrentBoundary = definition->mOtherBoundaries.begin()->second;
            ++pair;
            continue;
         }

         VERBOSE(
            "Data ", Logger::Cyan, definition->mNameOf,
            Logger::Red, " unregistered"
         );

         // Remove from indexing by ID                                  
         if (mMetaDataByID[definition->mID] == definition)
            mMetaDataByID[definition->mID] = nullptr;

         // Remove from the ambiguity map                               
         const auto ambiguous = mMetaAmbiguous.find(definition->mNameOfLowercased);
         ambiguous->second.erase(definition);
         if (ambiguous->second.empty())
            mMetaAmbiguous.erase(ambiguous);

         // Finally, remove it from registry (destroys unique_ptr)      
         mMetaDataByToken.erase(definition->mNameOfLowercased);
         pair = mMetaDataByCppName.erase(pair);
      }

      //                                                                
      // Unload tags                                                    
      for (auto pair = mMetaTagsByCppName.begin(); pair != mMetaTagsByCppName.end();) {
         auto definition = pair->second.get();
         if (not definition->mBoundaries.erase(::std::string{boundary})) {
            // Boundary is irrelevant for this definition               
            ++pair;
            continue;
         }

         if (not definition->mBoundaries.empty()) {
            // Definition is still used in other boundaries             
            ++pair;
            continue;
         }

         VERBOSE(
            "Tag ", Logger::Purple, definition->mNameOf,
            Logger::Red, " unregistered"
         );

         // Remove from indexing by ID                                  
         if (mMetaTagsByID[definition->mID] == definition)
            mMetaTagsByID[definition->mID] = nullptr;

         // Remove from the ambiguity map                               
         const auto ambiguous = mMetaAmbiguous.find(definition->mNameOfLowercased);
         ambiguous->second.erase(definition);
         if (ambiguous->second.empty())
            mMetaAmbiguous.erase(ambiguous);

         // Finally, remove it from registry (destroys unique_ptr)      
         mMetaTagsByToken.erase(definition->mNameOfLowercased);
         pair = mMetaTagsByCppName.erase(pair);
      }

      //                                                                
      // Unload verbs                                                   
      for (auto pair = mMetaVerbsByCppName.begin(); pair != mMetaVerbsByCppName.end();) {
         auto definition = pair->second.get();
         if (not definition->mBoundaries.erase(::std::string{boundary})) {
            // Boundary is irrelevant for this definition               
            ++pair;
            continue;
         }

         if (not definition->mBoundaries.empty()) {
            // Definition is still used in other boundaries             
            ++pair;
            continue;
         }

         if (not definition->mNameOfReverse.empty()) {
            VERBOSE("Verb ", Logger::DarkGreen,
               definition->mNameOf, "/", definition->mNameOfReverse,
               Logger::Red, " unregistered"
            );
         }
         else {
            VERBOSE("Verb ", Logger::DarkGreen,
               definition->mNameOf, Logger::Red, " unregistered"
            );
         }

         // Remove from indexing by ID                                  
         if (mMetaVerbsByID[definition->mID] == definition)
            mMetaVerbsByID[definition->mID] = nullptr;

         // Remove from the ambiguity map                               
         const auto ambiguous = mMetaAmbiguous.find(definition->mNameOf);
         ambiguous->second.erase(definition);
         if (ambiguous->second.empty())
            mMetaAmbiguous.erase(ambiguous);

         if (not definition->mNameOfReverse.empty()) {
            const auto ambiguous_rev = mMetaAmbiguous.find(definition->mNameOfReverse);
            ambiguous_rev->second.erase(definition);
            if (ambiguous_rev->second.empty())
               mMetaAmbiguous.erase(ambiguous_rev);
         }

         // Remove from the token map                                   
         mMetaVerbsByToken.erase(definition->mNameOf);
         if (not definition->mNameOfReverse.empty())
            mMetaVerbsByToken.erase(definition->mNameOfReverse);
         
         if (not definition->mOperator.empty()) {
            VERBOSE(
               "Operator ", Logger::DarkGreen, definition->mOperator,
               Logger::Red, " unregistered"
            );
            mMetaVerbsByToken.erase(definition->mOperator);
         }
         
         if (not definition->mOperatorReverse.empty()) {
            VERBOSE(
               "Operator ", Logger::DarkGreen, definition->mOperatorReverse,
               Logger::Red, " unregistered"
            );
            mMetaVerbsByToken.erase(definition->mOperatorReverse);
         }

         // Finally, remove it from registry (destroys unique_ptr)      
         pair = mMetaVerbsByCppName.erase(pair);
      }
   }

   /// Get the shortest possible unambiguous token                            
   ///   @return the token                                                    
   /*Token Inner::Definition::GetShortestUnambiguousToken() const {
      auto& ambiguous = Instance.GetAmbiguousMeta(mToken);
      if (ambiguous.size() == 1)
         return Inner::ToLastToken(mToken);

      // Collect all origin types, and work with those                  
      int datas = 0;
      int traits = 0;
      MetaList origins;
      for (auto meta : ambiguous) {
         auto dmeta = dynamic_cast<DefinitionData const*>(meta);
         if (dmeta and dmeta->mOrigin) {
            origins.insert(dmeta->mOrigin);
            ++datas;
         }
         else {
            origins.insert(meta);

            if (dmeta)
               ++datas;
            else if (dynamic_cast<DefinitionTag const*>(meta))
               ++traits;
         }
      }

      // Some easy to do disambiguations                                
      // Meta datas/tags always win over verbs/constants                
      if (origins.size() == 1)
         return ToLastToken(mToken);
      
      if ((datas  == 1 and traits == 0 and Kind() == Meta::Data)
      or  (traits == 1 and datas  == 0 and Kind() == Meta::Trait))
         return ToLastToken(mToken);
      
      if (datas == 1 and traits == 1) {
         if (Kind() == Meta::Data) {
            // Token should be starting with a capital letter           
            return static_cast<const MetaData*>(this)->mTokenSanitized;
         }

         if (Kind() == Meta::Trait) {
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
            size_t matches = 0;
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
   }*/
}

#undef VERBOSE//TODO see above
#undef VERBOSE_SCOPED
