#pragma once
#include "json.h"

namespace SleepyDiscord {
	namespace json {
		std::vector<std::string> getArray(const std::string* _source);
		//void JSON_getValues(const char* source, const char ** names, std::string * targets, const unsigned int numOfValues);
		std::vector<std::string> getValues(const char* source, std::initializer_list<const char *const> const &names);
		std::string getValue(const char* source, const char * name);

		const std::string createJSON(std::initializer_list<std::pair<std::string, std::string>> json);
		const std::string createJSONArray(std::vector<std::string> source);
		const std::string string(std::string s);
		const std::string UInteger(const uint64_t num);
		const std::string optionalUInteger(const uint64_t num);
		const std::string integer(const int64_t num);
		const std::string optionalInteger(const int64_t num);
		const std::string boolean(const bool boolean);

		struct BaseArrayWrapper {
			virtual operator const std::string&() const = 0;
			operator const std::string() const { return operator const std::string&(); }

			inline std::vector<std::string> vectorStrings() { return getArray(&operator const std::string&()); }
			inline std::string* cArrayStrings() { return &vectorStrings()[0]; }
		};

		struct ArrayStringWrapper : public BaseArrayWrapper {
			const std::string rawJSON; //to do change this to s pointer
			ArrayStringWrapper(const std::string json) : rawJSON(json) {}
			operator const std::string&() const { return rawJSON; }
		};

		template<class TypeToConvertTo, class Base = ArrayStringWrapper>
		struct ArrayWrapper : public Base {
			using Base::Base;
			template<template<class...> class Container, typename Type = TypeToConvertTo>
			Container<Type> get() {
				std::vector<std::string> jsonArray = Base::vectorStrings();
				const unsigned int size = jsonArray.size();
				std::vector<std::string*> jsonPointers(size);
				for (unsigned int i = 0; i < size; ++i)
					jsonPointers[i] = &jsonArray[i];
				return Container<Type>(jsonPointers.begin(), jsonPointers.end());

				//this doesn't work because the consturctors need to be using &
				//currently objects are using *
				//return Container<Type>(jsonArray.begin(), jsonArray.end());
			}
			template<template<class...> class Container>
			inline Container<std::string> getStrings() { return get<Container, std::string>(); }

			inline std::vector<TypeToConvertTo> vector() { return get<std::vector>(); }

			//c arrays
			inline TypeToConvertTo* cArray() { return &vector()[0]; }

			operator std::vector<std::string>() { return Base::vectorStrings(); }
			operator std::vector<TypeToConvertTo>() { return vector(); }
		};

		template<class Base>
		struct ArrayWrapper<std::string, Base> : public Base {
			using Base::Base;
			operator std::vector<std::string>() {
				return Base::vectorStrings();
			}
		};
	}
}