#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <memory>
#include "ErrorParser.h"
#include <QtCore/QJsonDocument>
#include "JsonUniresult.h"

/*
	This file contains abstract parser class, which is created for unification of parsing process
	and polymorthic changes of parsing object. Currently most functionality here is renundant, 
	except error message check. 
*/

struct uniform_parsing_result
	// This structure is returned from any child parser
{
	// objects in result
	QVector<uniform_json_object_representation> queryResult;
	// name of contained entity\entities. Usually it equals to container
	QString nameOf;
	// this variable is equal to roles in model. It notes that result is not standard.
	int alternativeResult;
	// example: instead of list of users parser obtained flag, which tells to skip picking user phase
};

typedef std::unique_ptr<uniform_parsing_result> uniresptr;

// converts parse result into string suitable to debug output
QString makeParseresSnapshot(uniresptr* res);

namespace RequestParser
{
	enum SystemAltResults { ErrorMessage = 10000 };
	class abs_parser
		// This class is basic for all parsers
	{
	protected:
		QString restext;	//	request result text
		QString errtext;	//	request error text
		QJsonDocument innerDocument;	//	document to extract data from
		QJsonObject innerObject;	//	root object of the document
		bool success;		//	true if parsing successfull
		bool isErrorMessage;	//	true if restext is containing error message
		int mytype;			//	type of the parser
		uniresptr uniresult;	//	result to return

		virtual bool _parseThis();	//	virtual parsing method, must be overrided in inheritors
		bool doParsing();	//	performs parsing, but checks for errors first
	public:
		// constructor performs parsing by calling doParsing
		abs_parser(const QString& restext, const QString& errtext);

		// returns ptr to result and clears pointer
		uniresptr read();	
		//	getter for success
		bool isSuccesfull();
		// getter for mytype
		int getType();
		// returns string with errors if there are some
		QString getErrors();
		// returns true if parsing was not succesfull due to request error, not parsing error
		bool hasError();
	};
	class ConfirmationResponseParser : public abs_parser
		// simpliest parser. used for login. Looks for session and user_id
	{
	protected:
		virtual bool _parseThis();
	public:
		ConfirmationResponseParser(const QString& restext, const QString errtext);
	};
}
