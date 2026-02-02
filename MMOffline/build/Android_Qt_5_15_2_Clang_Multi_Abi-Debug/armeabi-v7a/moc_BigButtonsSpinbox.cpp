/****************************************************************************
** Meta object code from reading C++ file 'BigButtonsSpinbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Widgets/ElementWidgets/BigButtonsSpinbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BigButtonsSpinbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BigButtonsSpinbox_t {
    QByteArrayData data[18];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BigButtonsSpinbox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BigButtonsSpinbox_t qt_meta_stringdata_BigButtonsSpinbox = {
    {
QT_MOC_LITERAL(0, 0, 17), // "BigButtonsSpinbox"
QT_MOC_LITERAL(1, 18, 13), // "ivalueChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "timeChanged"
QT_MOC_LITERAL(4, 45, 1), // "t"
QT_MOC_LITERAL(5, 47, 12), // "valueChanged"
QT_MOC_LITERAL(6, 60, 16), // "dateValueChanged"
QT_MOC_LITERAL(7, 77, 1), // "d"
QT_MOC_LITERAL(8, 79, 13), // "dvalueChanged"
QT_MOC_LITERAL(9, 93, 13), // "returnPressed"
QT_MOC_LITERAL(10, 107, 12), // "backRequired"
QT_MOC_LITERAL(11, 120, 15), // "intValueChanged"
QT_MOC_LITERAL(12, 136, 16), // "timeValueChanged"
QT_MOC_LITERAL(13, 153, 18), // "doubleValueChanged"
QT_MOC_LITERAL(14, 172, 11), // "editingDone"
QT_MOC_LITERAL(15, 184, 11), // "backRequire"
QT_MOC_LITERAL(16, 196, 11), // "dateChanged"
QT_MOC_LITERAL(17, 208, 8) // "setFocus"

    },
    "BigButtonsSpinbox\0ivalueChanged\0\0"
    "timeChanged\0t\0valueChanged\0dateValueChanged\0"
    "d\0dvalueChanged\0returnPressed\0"
    "backRequired\0intValueChanged\0"
    "timeValueChanged\0doubleValueChanged\0"
    "editingDone\0backRequire\0dateChanged\0"
    "setFocus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BigButtonsSpinbox[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       3,    1,   87,    2, 0x06 /* Public */,
       5,    1,   90,    2, 0x06 /* Public */,
       6,    1,   93,    2, 0x06 /* Public */,
       8,    1,   96,    2, 0x06 /* Public */,
       9,    0,   99,    2, 0x06 /* Public */,
      10,    0,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,  101,    2, 0x08 /* Private */,
      12,    1,  104,    2, 0x08 /* Private */,
      13,    1,  107,    2, 0x08 /* Private */,
      14,    0,  110,    2, 0x08 /* Private */,
      15,    0,  111,    2, 0x08 /* Private */,
      16,    1,  112,    2, 0x08 /* Private */,
      17,    0,  115,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QTime,    4,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QDate,    7,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QTime,    4,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    2,
    QMetaType::Void,

       0        // eod
};

void BigButtonsSpinbox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BigButtonsSpinbox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ivalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->timeChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 2: _t->valueChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->dateValueChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 4: _t->dvalueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->returnPressed(); break;
        case 6: _t->backRequired(); break;
        case 7: _t->intValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->timeValueChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 9: _t->doubleValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->editingDone(); break;
        case 11: _t->backRequire(); break;
        case 12: _t->dateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 13: _t->setFocus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BigButtonsSpinbox::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::ivalueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)(const QTime & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::timeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::valueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)(const QDate & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::dateValueChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::dvalueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::returnPressed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BigButtonsSpinbox::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigButtonsSpinbox::backRequired)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BigButtonsSpinbox::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_BigButtonsSpinbox.data,
    qt_meta_data_BigButtonsSpinbox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BigButtonsSpinbox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BigButtonsSpinbox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BigButtonsSpinbox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BigButtonsSpinbox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void BigButtonsSpinbox::ivalueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BigButtonsSpinbox::timeChanged(const QTime & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BigButtonsSpinbox::valueChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BigButtonsSpinbox::dateValueChanged(const QDate & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BigButtonsSpinbox::dvalueChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BigButtonsSpinbox::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BigButtonsSpinbox::backRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
