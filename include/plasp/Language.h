#ifndef __PLASP__LANGUAGE_H
#define __PLASP__LANGUAGE_H

#include <string>

namespace plasp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Language
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Language
{
	public:
		enum class Type
		{
			Unknown,
			Automatic,
			PDDL,
			SAS
		};

		static Language::Type fromString(const std::string &languageName);

	public:
		Language() = delete;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
