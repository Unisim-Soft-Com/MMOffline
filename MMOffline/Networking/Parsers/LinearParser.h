#pragma once
#include "abs_parser.h"

namespace RequestParser
{
	class LinearParser : public abs_parser
		// This parser looks for plain one-dimensional array of json objects
	{
	protected:
		// Inherited via abs_parser
		virtual bool _parseThis() override;
	public:
		LinearParser(const QString& restext, const QString& errtext);
	};
}