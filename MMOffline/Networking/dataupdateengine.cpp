#include "dataupdateengine.h"
#include "dataupdateengine-http.h"
#include "debugtrace.h"

DataUpdateEngine::DataUpdateEngine(QObject* parent) :
    QObject(parent), class_id(0)
{
#ifdef DEBUG
	//detrace_DCONSTR("DataUpdateEngine");
#endif
}
