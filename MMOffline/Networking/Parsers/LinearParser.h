#pragma once
#include "abs_parser.h"

namespace RequestParser
{
	class LinearParser : public abs_parser
	{
	protected:
		// Inherited via abs_parser
		virtual bool _parseThis() override;
	public:
		LinearParser(const QString& restext, const QString& errtext);
	};
}