#include "abs_parser.h"
#include <QtCore/QTextStream>


namespace RequestParser {
	bool abs_parser::_parseThis()
	{
		return false;
	}
	bool abs_parser::doParsing()
	{
		return _parseThis();
	}
	RequestParser::abs_parser::abs_parser(const QString& Restext, const QString& Errtext)
		: restext(Restext), errtext(Errtext), innerDocument(QJsonDocument::fromJson(Restext.toUtf8())),
		innerObject(), success(false), isErrorMessage(false), mytype(0), uniresult(new uniform_parsing_result())
	{
		innerObject = innerDocument.object();
		if (containsError(&innerObject))
		{
			parsedError pe = makeError(&innerObject);
			if (!pe.isError)
				return;
			isErrorMessage = true;
			errtext += pe.displayable;
			uniresult->alternativeResult = ErrorMessage;
			uniresult->nameOf = QStringLiteral("Error");
			QStringList err;
			err << "displayable" << "full" << "trace";
			uniresult->queryResult.push_back(uniform_json_object_representation(err));
			uniresult->queryResult.last() << pe.displayable << pe.full << pe.trace;
		}
	}

	uniresptr RequestParser::abs_parser::read()
	{
		if (uniresult == nullptr)
		{
			uniresult = uniresptr(new uniform_parsing_result());
			success = doParsing();
		}
		return std::move(uniresult);
	}

	bool RequestParser::abs_parser::isSuccesfull()
	{
		return success;
	}

	int RequestParser::abs_parser::getType()
	{
		return mytype;
	}

	QString RequestParser::abs_parser::getErrors()
	{
		return errtext;
	}
	bool abs_parser::hasError()
	{
		return isErrorMessage;
	}
	bool ConfirmationResponseParser::_parseThis()
	{
		if (isErrorMessage)
			return false;
		if (innerObject.contains(QLatin1String("session")) && innerObject.contains(QLatin1String("user_id")))
		{
			uniform_json_object_representation o;
			o.addField(QStringLiteral("session"), QString::number(innerObject.value("session").toInt()));
			o.addField(QStringLiteral("user_id"), QString::number(innerObject.value("user_id").toInt()));
			uniresult->queryResult.push_back(o);
			return true;
		}
		else
		{
			return false;
		}
	}
	ConfirmationResponseParser::ConfirmationResponseParser(const QString& restext, const QString errtext)
		: abs_parser(restext, errtext)
	{
		uniresult->nameOf = QStringLiteral("Login");
		success = doParsing();
	}
}

QString makeParseresSnapshot(uniresptr* res)
{
	QString temp;
	QTextStream out(&temp);
	out << "Parse result of type "
		<< (*res)->nameOf << " is holding " << (((*res)->alternativeResult == 0) ? "normal result " : "alternative result ")
		<< (*res)->alternativeResult << " and contains entries with length " << ((*res)->queryResult.count()) << "\r\n";
	out << "Heap snapshot holding " << (*res)->queryResult.count() << "\r\n";
	for (int i = 0; i < (*res)->queryResult.count(); ++i)
	{
		out << "entry ¹" << i << " with value " << (*res)->queryResult.at(i).toString() << "\r\n";
	}
	out << "----end of snapshot----" << endl;
	return temp;
}
