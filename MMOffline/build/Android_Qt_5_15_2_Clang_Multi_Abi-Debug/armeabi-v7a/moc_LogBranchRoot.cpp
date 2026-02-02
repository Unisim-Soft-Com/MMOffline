/****************************************************************************
** Meta object code from reading C++ file 'LogBranchRoot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Widgets/LogBranch/LogBranchRoot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LogBranchRoot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogBranchRoot_t {
    QByteArrayData data[8];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogBranchRoot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogBranchRoot_t qt_meta_stringdata_LogBranchRoot = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LogBranchRoot"
QT_MOC_LITERAL(1, 14, 17), // "onDocInteractions"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "Document"
QT_MOC_LITERAL(4, 42, 3), // "doc"
QT_MOC_LITERAL(5, 46, 6), // "action"
QT_MOC_LITERAL(6, 53, 11), // "onDocChange"
QT_MOC_LITERAL(7, 65, 11) // "hideCurrent"

    },
    "LogBranchRoot\0onDocInteractions\0\0"
    "Document\0doc\0action\0onDocChange\0"
    "hideCurrent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogBranchRoot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x09 /* Protected */,
       6,    1,   34,    2, 0x09 /* Protected */,
       7,    0,   37,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void LogBranchRoot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogBranchRoot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDocInteractions((*reinterpret_cast< Document(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onDocChange((*reinterpret_cast< Document(*)>(_a[1]))); break;
        case 2: _t->hideCurrent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Document >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Document >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogBranchRoot::staticMetaObject = { {
    QMetaObject::SuperData::link<inframedWidget::staticMetaObject>(),
    qt_meta_stringdata_LogBranchRoot.data,
    qt_meta_data_LogBranchRoot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogBranchRoot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogBranchRoot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogBranchRoot.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "abstractNode"))
        return static_cast< abstractNode*>(this);
    return inframedWidget::qt_metacast(_clname);
}

int LogBranchRoot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = inframedWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
