///                                                                           
/// Langulus::RTTI                                                            
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "MetaData.hpp"

namespace Langulus::RTTI
{

   ///                                                                        
   ///   Meta trait                                                           
   ///                                                                        
   struct MetaTrait final : public Meta {
      friend struct Member;
      LANGULUS(NAME) "TMeta";
      LANGULUS_BASES(Meta);

      static constexpr Token DefaultToken = "NoTrait";

      MetaType GetMetaType() const noexcept final { return Meta::Trait; }

      // Data filter for the trait (optional)                           
      DMeta mDataType {};

   public:
      template<CT::Void T>
      NOD() static constexpr TMeta Of();
      template<CT::Decayed T>
      NOD() static TMeta Of();
      
      NOD() bool Is(TMeta) const noexcept;
      template<CT::Data T>
      NOD() bool Is() const;

      bool operator == (const MetaTrait&) const noexcept;

   protected:
      template<CT::Data T>
      static constexpr Token GetReflectedToken() noexcept;
   };

} // namespace Langulus::RTTI