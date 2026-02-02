/****************************************************************************
** Meta object code from reading C++ file 'SyncMenuWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Widgets/Syncing/SyncMenuWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SyncMenuWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SyncMenuWidget_t {
    QByteArrayData data[8];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SyncMenuWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SyncMenuWidget_t qt_meta_stringdata_SyncMenuWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SyncMenuWidget"
QT_MOC_LITERAL(1, 15, 13), // "syncCompleted"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "sendData"
QT_MOC_LITERAL(4, 39, 11), // "receiveData"
QT_MOC_LITERAL(5, 51, 12), // "makeFullSync"
QT_MOC_LITERAL(6, 64, 19), // "operate_on_response"
QT_MOC_LITERAL(7, 84, 11) // "was_timeout"

    },
    "SyncMenuWidget\0syncCompleted\0\0sendData\0"
    "receiveData\0makeFullSync\0operate_on_response\0"
    "was_timeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SyncMenuWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SyncMenuWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SyncMenuWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->syncCompleted(); break;
        case 1: _t->sendData(); break;
        case 2: _t->receiveData(); break;
        case 3: _t->makeFullSync(); break;
        case 4: _t->operate_on_response(); break;
        case 5: _t->was_timeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SyncMenuWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SyncMenuWidget::syncCompleted)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SyncMenuWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<inframedWidget::staticMetaObject>(),
    qt_meta_stringdata_SyncMenuWidget.data,
    qt_meta_data_SyncMenuWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SyncMenuWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SyncMenuWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SyncMenuWidget.stringdata0))
        return static_cast<void*>(this);
    return inframedWidget::qt_metacast(_clname);
}

int SyncMenuWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = inframedWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SyncMenuWidget::syncCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
