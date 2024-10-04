///                                                                           
/// Langulus::RTTI                                                            
/// Copyright (c) 2012 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: MIT                                              
///                                                                           
#pragma once
#include "MetaData.hpp"
#include "MetaTrait.hpp"
#include "MetaVerb.hpp"
#include "MetaConst.hpp"


namespace Langulus::RTTI
{

   ///                                                                        
   /// Meta Data                                                              
   ///                                                                        
   LANGULUS(INLINED)
   constexpr DMeta::DMeta(const MetaData* meta) noexcept
      : AMeta {meta} {}

   /// Get the shortest unambiguous token that represents the data type       
   LANGULUS(INLINED)
   constexpr Token DMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaData::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaData::DefaultToken;
   #endif
   }

   LANGULUS(INLINED)
   const MetaData* DMeta::operator -> () const noexcept {
      return static_cast<const MetaData*>(mMeta);
   }

   LANGULUS(INLINED)
   const MetaData& DMeta::operator *  () const noexcept {
      return *static_cast<const MetaData*>(mMeta);
   }

   /// Compare if two data types match exactly                                
   ///   @attention includes qualifiers and sparsity                          
   LANGULUS(INLINED)
   constexpr bool DMeta::operator == (const DMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->IsExact(rhs));
   }

   /// Compare if two data types are similar                                  
   ///   @attention discards qualifiers only                                  
   LANGULUS(INLINED)
   constexpr bool DMeta::operator & (const DMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->IsSimilar(rhs));
   }

   /// Compare if two data types' origins match                               
   ///   @attention discards both qualifiers and sparsity                     
   LANGULUS(INLINED)
   constexpr bool DMeta::operator | (const DMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->Is(rhs));
   }
   

   ///                                                                        
   /// Meta Trait                                                             
   ///                                                                        
   LANGULUS(INLINED)
   constexpr TMeta::TMeta(const MetaTrait* meta) noexcept
      : AMeta {meta} {}

   /// Get the shortest unambiguous token that represents the trait type      
   LANGULUS(INLINED)
   constexpr Token TMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaTrait::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaTrait::DefaultToken;
   #endif
   }

   LANGULUS(INLINED)
   const MetaTrait* TMeta::operator -> () const noexcept {
      return static_cast<const MetaTrait*>(mMeta);
   }

   LANGULUS(INLINED)
   const MetaTrait& TMeta::operator *  () const noexcept {
      return *static_cast<const MetaTrait*>(mMeta);
   }

   /// Check if two trait definitions match                                   
   LANGULUS(INLINED)
   constexpr bool TMeta::operator == (const TMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->Is(rhs));
   }


   ///                                                                        
   /// Meta Const                                                             
   ///                                                                        
   LANGULUS(INLINED)
   constexpr CMeta::CMeta(const MetaConst* meta) noexcept
      : AMeta {meta} {}

   /// Get the shortest unambiguous token that represents the constant        
   LANGULUS(INLINED)
   constexpr Token CMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaConst::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaConst::DefaultToken;
   #endif
   }

   LANGULUS(INLINED)
   const MetaConst* CMeta::operator -> () const noexcept {
      return static_cast<const MetaConst*>(mMeta);
   }

   LANGULUS(INLINED)
   const MetaConst& CMeta::operator *  () const noexcept {
      return *static_cast<const MetaConst*>(mMeta);
   }

   /// Check if two constants definitions match                               
   LANGULUS(INLINED)
   constexpr bool CMeta::operator == (const CMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->Is(rhs));
   }


   ///                                                                        
   /// Meta Verb                                                              
   ///                                                                        
   LANGULUS(INLINED)
   constexpr VMeta::VMeta(const MetaVerb* meta) noexcept
      : AMeta {meta} {}

   /// Get the shortest unambiguous token that represents the verb            
   LANGULUS(INLINED)
   constexpr Token VMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : MetaVerb::DefaultToken;
   #else
      return mMeta ? mMeta->mToken : MetaVerb::DefaultToken;
   #endif
   }

   LANGULUS(INLINED)
   const MetaVerb* VMeta::operator -> () const noexcept {
      return static_cast<const MetaVerb*>(mMeta);
   }

   LANGULUS(INLINED)
   const MetaVerb& VMeta::operator *  () const noexcept {
      return *static_cast<const MetaVerb*>(mMeta);
   }

   /// Check if two verb definitions match                                    
   LANGULUS(INLINED)
   constexpr bool VMeta::operator == (const VMeta& rhs) const noexcept {
      return mMeta == rhs.mMeta or (mMeta and (*this)->Is(rhs));
   }


   ///                                                                        
   /// Meta                                                                   
   ///                                                                        
   LANGULUS(INLINED)
   constexpr Token AMeta::GetToken() const noexcept {
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      return mMeta ? mMeta->GetShortestUnambiguousToken() : "null";
   #else
      return mMeta ? mMeta->mToken : "null";
   #endif
   }

   constexpr bool AMeta::operator == (const AMeta& rhs) const noexcept {
      if (mMeta == rhs.mMeta)
         return true;

      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      const DMeta rhs_dmeta = dynamic_cast<const MetaData*>(rhs.mMeta);
      if (lhs_dmeta and rhs_dmeta)
         return lhs_dmeta == rhs_dmeta;

      const TMeta lhs_tmeta = dynamic_cast<const MetaTrait*>(mMeta);
      const TMeta rhs_tmeta = dynamic_cast<const MetaTrait*>(rhs.mMeta);
      if (lhs_tmeta and rhs_tmeta)
         return lhs_tmeta == rhs_tmeta;

      const VMeta lhs_vmeta = dynamic_cast<const MetaVerb*>(mMeta);
      const VMeta rhs_vmeta = dynamic_cast<const MetaVerb*>(rhs.mMeta);
      if (lhs_vmeta and rhs_vmeta)
         return lhs_vmeta == rhs_vmeta;

      const CMeta lhs_cmeta = dynamic_cast<const MetaConst*>(mMeta);
      const CMeta rhs_cmeta = dynamic_cast<const MetaConst*>(rhs.mMeta);
      if (lhs_cmeta and rhs_cmeta)
         return lhs_cmeta == rhs_cmeta;

      return false;
   }
   
   constexpr bool AMeta::operator & (const AMeta& rhs) const noexcept {
      if (mMeta == rhs.mMeta)
         return true;

      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      const DMeta rhs_dmeta = dynamic_cast<const MetaData*>(rhs.mMeta);
      if (lhs_dmeta and rhs_dmeta)
         return lhs_dmeta & rhs_dmeta;

      return false;
   }

   constexpr bool AMeta::operator | (const AMeta& rhs) const noexcept {
      if (mMeta == rhs.mMeta)
         return true;

      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      const DMeta rhs_dmeta = dynamic_cast<const MetaData*>(rhs.mMeta);
      if (lhs_dmeta and rhs_dmeta)
         return lhs_dmeta | rhs_dmeta;

      return false;
   }

   constexpr bool AMeta::operator == (const VMeta& rhs) const noexcept {
      if (mMeta == static_cast<const Meta*>(rhs.mMeta))
         return true;

      const VMeta lhs_vmeta = dynamic_cast<const MetaVerb*>(mMeta);
      const VMeta rhs_vmeta = dynamic_cast<const MetaVerb*>(rhs.mMeta);
      if (lhs_vmeta and rhs_vmeta)
         return lhs_vmeta == rhs_vmeta;

      return false;
   }

   constexpr bool AMeta::operator == (const TMeta& rhs) const noexcept {
      if (mMeta == static_cast<const Meta*>(rhs.mMeta))
         return true;

      const TMeta lhs_tmeta = dynamic_cast<const MetaTrait*>(mMeta);
      const TMeta rhs_tmeta = dynamic_cast<const MetaTrait*>(rhs.mMeta);
      if (lhs_tmeta and rhs_tmeta)
         return lhs_tmeta == rhs_tmeta;

      return false;
   }

   constexpr bool AMeta::operator == (const CMeta& rhs) const noexcept {
      if (mMeta == static_cast<const Meta*>(rhs.mMeta))
         return true;

      const CMeta lhs_cmeta = dynamic_cast<const MetaConst*>(mMeta);
      const CMeta rhs_cmeta = dynamic_cast<const MetaConst*>(rhs.mMeta);
      if (lhs_cmeta and rhs_cmeta)
         return lhs_cmeta == rhs_cmeta;

      return false;
   }

   LANGULUS(INLINED)
   constexpr bool AMeta::operator == (const DMeta& rhs) const noexcept {
      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      return lhs_dmeta ? lhs_dmeta == rhs : false;
   }

   LANGULUS(INLINED)
   constexpr bool AMeta::operator & (const DMeta& rhs) const noexcept {
      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      return lhs_dmeta ? lhs_dmeta & rhs : false;
   }

   LANGULUS(INLINED)
   constexpr bool AMeta::operator | (const DMeta& rhs) const noexcept {
      const DMeta lhs_dmeta = dynamic_cast<const MetaData*>(mMeta);
      return lhs_dmeta ? lhs_dmeta | rhs : false;
   }

   template<class T> LANGULUS(INLINED)
   constexpr T AMeta::As() const noexcept {
      if (not mMeta)
         return {};

      if constexpr (CT::Exact<T, DMeta>)
         return dynamic_cast<const MetaData*>(mMeta);
      else if constexpr (CT::Exact<T, TMeta>)
         return dynamic_cast<const MetaTrait*>(mMeta);
      else if constexpr (CT::Exact<T, VMeta>)
         return dynamic_cast<const MetaVerb*>(mMeta);
      else if constexpr (CT::Exact<T, CMeta>)
         return dynamic_cast<const MetaConst*>(mMeta);
      else if constexpr (CT::Exact<T, AMeta>)
         return *this;
      else LANGULUS_ERROR(
         "T is not a definition interchange type, "
         "has to be one of the following: DMeta, TMeta, CMeta, VMeta, AMeta"
      );
   }

   LANGULUS(INLINED)
   constexpr AMeta::operator DMeta() const noexcept {
      return As<DMeta>();
   }

   LANGULUS(INLINED)
   constexpr AMeta::operator TMeta() const noexcept {
      return As<TMeta>();
   }

   LANGULUS(INLINED)
   constexpr AMeta::operator CMeta() const noexcept {
      return As<CMeta>();
   }

   LANGULUS(INLINED)
   constexpr AMeta::operator VMeta() const noexcept {
      return As<VMeta>();
   }

   constexpr Token AMeta::Kind() const noexcept {
      if (not mMeta)
         return Meta::Unknown;

      if (dynamic_cast<const MetaData*>(mMeta))
         return Meta::Data;
      else if (dynamic_cast<const MetaTrait*>(mMeta))
         return Meta::Trait;
      else if (dynamic_cast<const MetaVerb*>(mMeta))
         return Meta::Verb;
      else if (dynamic_cast<const MetaConst*>(mMeta))
         return Meta::Constant;
      else 
         return Meta::Unknown;
   }
   
   /// Construct an abstract meta definition by setting token and hashing it  
   ///   @param name - token                                                  
   LANGULUS(INLINED)
   Meta::Meta(const Token& name)
      : mHash  {HashOf(name)}
      , mToken {name} {}

} // namespace Langulus::RTTI