/****************************************************************************
** Meta object code from reading C++ file 'StartingScreen.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Widgets/ModeDefining/StartingScreen.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StartingScreen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StartingScreen_t {
    QByteArrayData data[11];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StartingScreen_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StartingScreen_t qt_meta_stringdata_StartingScreen = {
    {
QT_MOC_LITERAL(0, 0, 14), // "StartingScreen"
QT_MOC_LITERAL(1, 15, 25), // "documentCreationInitiated"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "logsRequired"
QT_MOC_LITERAL(4, 55, 13), // "toOnlineLogin"
QT_MOC_LITERAL(5, 69, 10), // "toSettings"
QT_MOC_LITERAL(6, 80, 11), // "hideCurrent"
QT_MOC_LITERAL(7, 92, 12), // "userLoggedIn"
QT_MOC_LITERAL(8, 105, 5), // "login"
QT_MOC_LITERAL(9, 111, 19), // "translationHappened"
QT_MOC_LITERAL(10, 131, 12) // "setInfoLabel"

    },
    "StartingScreen\0documentCreationInitiated\0"
    "\0logsRequired\0toOnlineLogin\0toSettings\0"
    "hideCurrent\0userLoggedIn\0login\0"
    "translationHappened\0setInfoLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StartingScreen[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   56,    2, 0x09 /* Protected */,
       5,    0,   57,    2, 0x09 /* Protected */,
       6,    0,   58,    2, 0x09 /* Protected */,
       7,    1,   59,    2, 0x09 /* Protected */,
       9,    1,   62,    2, 0x09 /* Protected */,
      10,    0,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void StartingScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StartingScreen *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->documentCreationInitiated(); break;
        case 1: _t->logsRequired(); break;
        case 2: _t->toOnlineLogin(); break;
        case 3: _t->toSettings(); break;
        case 4: _t->hideCurrent(); break;
        case 5: _t->userLoggedIn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->translationHappened((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setInfoLabel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StartingScreen::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StartingScreen::documentCreationInitiated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (StartingScreen::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StartingScreen::logsRequired)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StartingScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<inframedWidget::staticMetaObject>(),
    qt_meta_stringdata_StartingScreen.data,
    qt_meta_data_StartingScreen,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StartingScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StartingScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StartingScreen.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "abstractNode"))
        return static_cast< abstractNode*>(this);
    return inframedWidget::qt_metacast(_clname);
}

int StartingScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = inframedWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void StartingScreen::documentCreationInitiated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void StartingScreen::logsRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
