/****************************************************************************
** Meta object code from reading C++ file 'EventsAndFilters.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Widgets/utils/EventsAndFilters.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EventsAndFilters.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_filters__NoKeyEvents_t {
    QByteArrayData data[7];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filters__NoKeyEvents_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filters__NoKeyEvents_t qt_meta_stringdata_filters__NoKeyEvents = {
    {
QT_MOC_LITERAL(0, 0, 20), // "filters::NoKeyEvents"
QT_MOC_LITERAL(1, 21, 14), // "numberObtained"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 14), // "returnObtained"
QT_MOC_LITERAL(4, 52, 12), // "backRequired"
QT_MOC_LITERAL(5, 65, 15), // "unknownObtained"
QT_MOC_LITERAL(6, 81, 13) // "eraseRequired"

    },
    "filters::NoKeyEvents\0numberObtained\0"
    "\0returnObtained\0backRequired\0"
    "unknownObtained\0eraseRequired"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filters__NoKeyEvents[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    0,   42,    2, 0x06 /* Public */,
       4,    0,   43,    2, 0x06 /* Public */,
       5,    0,   44,    2, 0x06 /* Public */,
       6,    0,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void filters::NoKeyEvents::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NoKeyEvents *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->numberObtained((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->returnObtained(); break;
        case 2: _t->backRequired(); break;
        case 3: _t->unknownObtained(); break;
        case 4: _t->eraseRequired(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NoKeyEvents::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NoKeyEvents::numberObtained)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NoKeyEvents::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NoKeyEvents::returnObtained)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NoKeyEvents::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NoKeyEvents::backRequired)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NoKeyEvents::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NoKeyEvents::unknownObtained)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NoKeyEvents::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NoKeyEvents::eraseRequired)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject filters::NoKeyEvents::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_filters__NoKeyEvents.data,
    qt_meta_data_filters__NoKeyEvents,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filters::NoKeyEvents::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filters::NoKeyEvents::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filters__NoKeyEvents.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filters::NoKeyEvents::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void filters::NoKeyEvents::numberObtained(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void filters::NoKeyEvents::returnObtained()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void filters::NoKeyEvents::backRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void filters::NoKeyEvents::unknownObtained()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void filters::NoKeyEvents::eraseRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
struct qt_meta_stringdata_filters__CaptureBackFilter_t {
    QByteArrayData data[3];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filters__CaptureBackFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filters__CaptureBackFilter_t qt_meta_stringdata_filters__CaptureBackFilter = {
    {
QT_MOC_LITERAL(0, 0, 26), // "filters::CaptureBackFilter"
QT_MOC_LITERAL(1, 27, 12), // "backRequired"
QT_MOC_LITERAL(2, 40, 0) // ""

    },
    "filters::CaptureBackFilter\0backRequired\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filters__CaptureBackFilter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void filters::CaptureBackFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CaptureBackFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backRequired(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CaptureBackFilter::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureBackFilter::backRequired)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject filters::CaptureBackFilter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_filters__CaptureBackFilter.data,
    qt_meta_data_filters__CaptureBackFilter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filters::CaptureBackFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filters::CaptureBackFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filters__CaptureBackFilter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filters::CaptureBackFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void filters::CaptureBackFilter::backRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_filters__LineEditHelper_t {
    QByteArrayData data[1];
    char stringdata0[24];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filters__LineEditHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filters__LineEditHelper_t qt_meta_stringdata_filters__LineEditHelper = {
    {
QT_MOC_LITERAL(0, 0, 23) // "filters::LineEditHelper"

    },
    "filters::LineEditHelper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filters__LineEditHelper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void filters::LineEditHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject filters::LineEditHelper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_filters__LineEditHelper.data,
    qt_meta_data_filters__LineEditHelper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filters::LineEditHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filters::LineEditHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filters__LineEditHelper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filters::LineEditHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_filters__SpinboxHelper_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filters__SpinboxHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filters__SpinboxHelper_t qt_meta_stringdata_filters__SpinboxHelper = {
    {
QT_MOC_LITERAL(0, 0, 22) // "filters::SpinboxHelper"

    },
    "filters::SpinboxHelper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filters__SpinboxHelper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void filters::SpinboxHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject filters::SpinboxHelper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_filters__SpinboxHelper.data,
    qt_meta_data_filters__SpinboxHelper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filters::SpinboxHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filters::SpinboxHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filters__SpinboxHelper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filters::SpinboxHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_filters__GeneralPurposeFilter_t {
    QByteArrayData data[7];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filters__GeneralPurposeFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filters__GeneralPurposeFilter_t qt_meta_stringdata_filters__GeneralPurposeFilter = {
    {
QT_MOC_LITERAL(0, 0, 29), // "filters::GeneralPurposeFilter"
QT_MOC_LITERAL(1, 30, 13), // "returnPressed"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 11), // "backPressed"
QT_MOC_LITERAL(4, 57, 11), // "scanPressed"
QT_MOC_LITERAL(5, 69, 13), // "numberPressed"
QT_MOC_LITERAL(6, 83, 13) // "symbolPressed"

    },
    "filters::GeneralPurposeFilter\0"
    "returnPressed\0\0backPressed\0scanPressed\0"
    "numberPressed\0symbolPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filters__GeneralPurposeFilter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       6,    1,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void filters::GeneralPurposeFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GeneralPurposeFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->returnPressed(); break;
        case 1: _t->backPressed(); break;
        case 2: _t->scanPressed(); break;
        case 3: _t->numberPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->symbolPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GeneralPurposeFilter::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeneralPurposeFilter::returnPressed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GeneralPurposeFilter::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeneralPurposeFilter::backPressed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GeneralPurposeFilter::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeneralPurposeFilter::scanPressed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GeneralPurposeFilter::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeneralPurposeFilter::numberPressed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GeneralPurposeFilter::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeneralPurposeFilter::symbolPressed)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject filters::GeneralPurposeFilter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_filters__GeneralPurposeFilter.data,
    qt_meta_data_filters__GeneralPurposeFilter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filters::GeneralPurposeFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filters::GeneralPurposeFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filters__GeneralPurposeFilter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filters::GeneralPurposeFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void filters::GeneralPurposeFilter::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void filters::GeneralPurposeFilter::backPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void filters::GeneralPurposeFilter::scanPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void filters::GeneralPurposeFilter::numberPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void filters::GeneralPurposeFilter::symbolPressed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
