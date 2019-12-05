#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <memory>
#include "ErrorParser.h"
#include <QtCore/QJsonDocument>
#include "JsonUniresult.h"



struct uniform_parsing_result
{
	QVector<uniform_json_object_representation> queryResult;
	QString nameOf;
	int alternativeResult;
};



typedef std::unique_ptr<uniform_parsing_result> uniresptr;
QString makeParseresSnapshot(uniresptr* res);
namespace RequestParser
{
	enum SystemAltResults {ErrorMessage = 10000};
	class abs_parser
	{
	protected:
		QString restext;
		QString errtext;
		QJsonDocument innerDocument;
		QJsonObject innerObject;
		bool success;
		bool isErrorMessage;
		int mytype;
		uniresptr uniresult;

		virtual bool _parseThis();
		bool doParsing();
	public:
		abs_parser( const QString& restext, const QString& errtext);
		uniresptr read();
		bool isSuccesfull();
		int getType();
		QString getErrors();
		bool hasError();
	};
	class ConfirmationResponseParser : public abs_parser
	{
	protected:
		virtual bool _parseThis();
	public:
		ConfirmationResponseParser(const QString& restext, const QString errtext);
	};
}

