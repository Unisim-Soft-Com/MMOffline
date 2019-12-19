#pragma once
#include "Dataprovider/TableHandlers.h"



const int PREDEFINED_TABLES_QUANTITY = DocumentEntries+1;
extern TemplatedTableHandler predefinedTables[PREDEFINED_TABLES_QUANTITY];
extern QStringList predefinedDBNames;
extern const QStringList documentFields;
extern const QStringList documentEntryFields;
extern const QStringList fieldsOfProductEntity;
extern const QStringList fieldsOfGroupEntity;
extern const QStringList fieldsOfNamedIdEntity;