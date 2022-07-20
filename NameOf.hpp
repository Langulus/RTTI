///																									
/// Langulus::RTTI																			   
/// Copyright(C) 2012 Dimo Markov <langulusteam@gmail.com>							
///																									
/// Distributed under GNU General Public License v3+									
/// See LICENSE file, or https://www.gnu.org/licenses									
///																									
#pragma once
#include "Config.hpp"
#include <string>

namespace Langulus::RTTI
{
	namespace Inner
	{

		/// Pretty function used to wrap a type as a template argument          
		/// Once we wrap and stringify it, we can isolate the typename itself   
		///   @tparam T - the type to get the name of                           
		template<class T>
		constexpr Token AsTemplateArgument() {
			return LANGULUS_FUNCTION();
		}

		/// Pretty function used to wrap a constexpr constant as a template     
		/// argument. Once we wrap and stringify it, we can isolate the name of 
		/// an enum, if it's a named constant                                   
		///   @tparam T - the constant to get the name of                       
		template<auto T>
		constexpr Token AsTemplateArgument() {
			return LANGULUS_FUNCTION();
		}

		/// Filter a literal matching at the front                              
		///   @param str - the string view to modify as constexpr               
		///   @param prefix - the prefix to skip                                
		constexpr void SkipPrefix(Token& str, const Token& prefix) {
			if (str.starts_with(prefix))
				str.remove_prefix(prefix.size());
		}

		/// Filter a literal matching at the back                               
		///   @param str - the string view to modify as constexpr               
		///   @param suffix - the suffix to skip                                
		constexpr void SkipSuffix(Token& str, const Token& suffix) {
			if (str.ends_with(suffix))
				str.remove_suffix(suffix.size());
		}

		/// Skip empty space                                                    
		///   @param str - the string view to modify as constexpr               
		constexpr void SkipSpace(Token& str) {
			while (str.size() > 0 && (str[0] == ' ' || str[0] == '\t'))
				str.remove_prefix(1);
		}

		/// Skip class identifier                                               
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipClass(Token& name) {
			SkipSpace(name);
			SkipPrefix(name, "class");
			SkipSpace(name);
		}

		/// Skip struct identifier                                              
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipStruct(Token& name) {
			SkipSpace(name);
			SkipPrefix(name, "struct");
			SkipSpace(name);
		}

		/// Skip enum identifier                                                
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipEnum(Token& name) {
			SkipSpace(name);
			SkipPrefix(name, "enum");
			SkipSpace(name);
		}

		/// Skip constantly occuring namespaces                                 
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipNamespace(Token& name) {
			SkipSpace(name);
			SkipPrefix(name, "Langulus::");
			SkipSpace(name);
		}

		/// Skip all decorations in front of a type name                        
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipDecorations(Token& name) {
			SkipStruct(name);
			SkipClass(name);
			SkipEnum(name);
			SkipNamespace(name);
		}

		/// Skip all characters after the first encounter of a template bracket	
		/// This will result in a warning if dependent template types are found	
		///   @param name - the string view to modify as constexpr              
		constexpr void SkipTemplateArguments(Token& name) {
			const auto found = name.find_first_of('<', 0);
			if (found == name.npos)
				return;

			name.remove_suffix(name.size() - found);
		}

		/// These definitions might change in future compiler versions          
		/// and will probably need constant maintenance                         
		#if defined(__clang__)
			constexpr Token Prefix = "Langulus::Token Langulus::RTTI::Inner::AsTemplateArgument() [T = ";
			constexpr Token Suffix = "]";
		#elif defined(__GNUC__) && !defined(__clang__)
			constexpr Token Prefix = "constexpr Langulus::Token Langulus::RTTI::Inner::AsTemplateArgument() [with T = ";
			constexpr Token Suffix = "; Langulus::Token = std::basic_string_view<char>]";
		#elif defined(_MSC_VER)
			constexpr Token Prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Langulus::RTTI::Inner::AsTemplateArgument<";
			constexpr Token Suffix = ">(void)";
		#else
			#error "No support for this compiler"
		#endif

		/// Skip all decorations in front of a type name                        
		///   @return the modified constexpr string view                        
		template<class T>
		constexpr Token IsolateTypename() {
			auto original = AsTemplateArgument<Decay<T>>();
			SkipPrefix(original, Prefix);
			SkipSuffix(original, Suffix);
			SkipDecorations(original);
			return original;
		}

		/// Skip all decorations in front of a constant, named or not           
		/// Also, skips all namespaces for enums, since those need to pass      
		/// through the name splitter anyways                                   
		///   @return the modified constexpr string view                        
		template<auto T>
		constexpr Token IsolateConstant() {
			auto original = AsTemplateArgument<T>();
			SkipPrefix(original, Prefix);
			SkipSuffix(original, Suffix);
			SkipDecorations(original);

			// Skip ALL namespaces, these will be put back together later  
			auto found = original.find_last_of(':', original.size());
			if (found == original.npos)
				return original;
			
			original.remove_prefix(found + 1);
			return original;
		}

		template<class HEAD, class... TAIL>
		constexpr Size NameSplitAndEstimate() noexcept;
		template<class HEAD, class... TAIL>
		constexpr Size NameSplitAndAppend(char*) noexcept;

		template<class T>
		struct Overrider {};

		template<template<class...> class T, class... TArgs>
		constexpr Size NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept;
		template<class T>
		constexpr Size NameOfEstimate(Overrider<T>&&) noexcept;

		/// Correctly handles templated types, but isn't constexpr due to static   
		/// string buffer, that is required for substitution of fundamental types  
		///   @tparam T - the templated type                                       
		///   @return TArgs - the template arguments                               
		template<template<class...> class T, class... TArgs>
		Token NameOf(Overrider<T<TArgs...>>&&) noexcept {
			auto original = IsolateTypename<T<TArgs...>>();

			// Corner case, where the splitter doesn't work							
			#if LANGULUS(SAFE)
				const auto found = original.find_first_of('<', 0);
				int nested = 1;
				for (auto i = found + 1; i < original.size() && found != original.npos; ++i) {
					if (original[i] == '<') {
						if (0 == nested) {
							// Templates started again, which means a dependent	
							// template type was encountered. This behavior is		
							// not covered by NameOf yet									
							Logger::Warning() <<
								"NameOf can't guarantee, that parent template "
								"arguments are consistently named fundamentals "
								"- if that's probable, you should either move "
								"your type outside the templated namespace, or "
								"avoid implicit naming by manually defining the "
								"reflected token via LANGULUS(NAME). "
								"The type that triggered this warning is: "
								<< Logger::Red << original;
							break;
						}

						++nested;
					}
					else if (original[i] == '>')
						--nested;
				}
			#endif

			SkipTemplateArguments(original);
			static char buffer[NameOfEstimate(Overrider<T<TArgs...>>{})] = {};
			::std::memcpy(buffer, original.data(), original.size());
			auto i = original.size();
			buffer[i] = '<'; ++i;
			i += NameSplitAndAppend<TArgs...>(buffer + i);
			buffer[i] = '>'; ++i;
			return Token {buffer, i};
		}

		/// Gets the name of T, without the decorations                            
		/// This is an important step for consistent fundamental names, since      
		/// different compilers and architectures name types differently in their  
		/// function name macros. This will hopefully be improved in future C++    
		/// standards, and made completely constexpr, as it should be              
		template<class T>
		constexpr Token NameOf(Overrider<T>&&) noexcept {
			// Some fundamental types are stringified differently on different
			// compilers, and we need to make sure their names cross-match    
			if constexpr (CT::Same<T, ::std::int8_t>)
				return "int8";
			else if constexpr (CT::Same<T, ::std::int16_t>)
				return "int16";
			else if constexpr (CT::Same<T, ::std::int32_t>)
				return "int32";
			else if constexpr (CT::Same<T, ::std::int64_t>)
				return "int64";
			else if constexpr (CT::Same<T, ::std::uint8_t>)
				return "uint8";
			else if constexpr (CT::Same<T, ::std::uint16_t>)
				return "uint16";
			else if constexpr (CT::Same<T, ::std::uint32_t>)
				return "uint32";
			else if constexpr (CT::Same<T, ::std::uint64_t>)
				return "uint64";
			else
				return IsolateTypename<T>();
		}

		template<auto E>
		Token NameOfEnum() noexcept;

	} // namespace Inner

	/// Get the name of a type, templated or not, with consistently named      
	/// template arguments, even if nested, if such are required               
	/// Unfortunately, this can't be constexpr, because it may require name    
	/// substitution. But is as statically optimized as much as possible!      
	///   @tparam T - the type to get the name of                              
	///   @return the type name                                                
	template<class T>
	Token NameOf() noexcept {
		return Inner::NameOf(Inner::Overrider<Decay<T>>{});
	}

	/// Get the name of a named constant, or any constexpr value in fact       
	/// The type that owns the enum will be passed through the above NameOf    
	/// so that it will too have consistent template arguments if fundamental  
	///   @tparam T - the constant to get the name of                          
	///   @return the name of the constant                                     
	template<auto T>
	Token NameOf() noexcept {
		return Inner::NameOfEnum<T>();
	}

	template<template<class...> class T, class... TArgs>
	constexpr Size Inner::NameOfEstimate(Overrider<T<TArgs...>>&&) noexcept {
		return Inner::IsolateTypename<T<TArgs...>>().size() + NameSplitAndEstimate<TArgs...>() + 3;
	}

	template<class T>
	constexpr Size Inner::NameOfEstimate(Overrider<T>&& a) noexcept {
		return Inner::NameOf(Overrider<T>{}).size();
	}

	template<class HEAD, class... TAIL>
	constexpr Size Inner::NameSplitAndEstimate() noexcept {
		constexpr auto original = NameOfEstimate(Overrider<HEAD>{});
		if constexpr (sizeof...(TAIL) > 0)
			return original + 2 + NameSplitAndEstimate<TAIL...>();
		else
			return original;
	}

	template<class HEAD, class... TAIL>
	constexpr Size Inner::NameSplitAndAppend(char* buffer) noexcept {
		auto original = ::Langulus::RTTI::NameOf<HEAD>();
		if constexpr (sizeof...(TAIL) > 0) {
			int i = 0;
			for (; i < original.size(); ++i)
				buffer[i] = original[i];
			buffer[i] = ','; ++i;
			buffer[i] = ' '; ++i;
			return original.size() + 2 + NameSplitAndAppend<TAIL...>(buffer + i);
		}
		else {
			for (int i = 0; i < original.size(); ++i)
				buffer[i] = original[i];
			return original.size();
		}
	}

	/// Correctly handles templated types, but isn't constexpr due to static   
	/// string buffer, that is required for substitution of fundamental types  
	/// The owning class will be appended to the final enum name               
	///   @tparam T - the type of the constant                                 
	///   @tparam E - the constant                                             
	template<auto E>
	Token Inner::NameOfEnum() noexcept {
		using EnumType = decltype(E);
		const auto base = NameOf(Inner::Overrider<EnumType>{});
		constexpr auto name = IsolateConstant<E>();

		// Corner case, where the splitter doesn't work                   
		#if LANGULUS(SAFE)
			const auto template_start = base.find_first_of('<', 0);
			if (template_start != base.npos)
				Logger::Warning() <<
				"NameOfEnum can't guarantee, that parent template arguments "
				"are consistently named fundamentals - if that's probable, "
				"you should move your enum outside the templated namespace: "
				"The constant that triggered this warning is: "
				<< Logger::Red << base << "::" << name;
		#endif

		constexpr auto size = NameOfEstimate(Overrider<EnumType>{}) + name.size() + 2;
		static char buffer[size] = {};
		::std::memcpy(buffer, base.data(), base.size());
		buffer[base.size()] = buffer[base.size() + 1] = ':';
		::std::memcpy(buffer + base.size() + 2, name.data(), name.size());
		return Token {buffer, size};
	}

} // namespace Langulus::RTTI
