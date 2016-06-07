#ifndef __PLASP__UTILS__PARSER_WARNING_H
#define __PLASP__UTILS__PARSER_WARNING_H

#include <exception>
#include <string>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserWarning
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserWarning: public std::exception
{
	public:
		explicit ParserWarning(const utils::Parser &parser)
		:	ParserWarning(parser, "Unspecified parser warning")
		{
		}

		explicit ParserWarning(const utils::Parser &parser, const char *message)
		:	ParserWarning(parser, static_cast<std::string>(message))
		{
		}

		explicit ParserWarning(const utils::Parser &parser, const std::string &message)
		:	m_message{parser.fileName() + ":" + std::to_string(parser.row()) + ":" + std::to_string(parser.column()) + " " + message}
		{
		}

		~ParserWarning() throw()
		{
		}

		const char *what() const throw()
		{
			if (m_message.empty())
				return "Unspecified parser warning";

			return m_message.c_str();
		}

	private:
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
