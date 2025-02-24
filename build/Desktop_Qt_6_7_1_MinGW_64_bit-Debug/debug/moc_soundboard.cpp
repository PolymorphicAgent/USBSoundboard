/****************************************************************************
** Meta object code from reading C++ file 'soundboard.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../soundboard.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soundboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSoundboardENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSoundboardENDCLASS = QtMocHelpers::stringData(
    "Soundboard",
    "refreshSerialPorts",
    "",
    "connectToSerialPort",
    "selectSound",
    "index",
    "readSerialData",
    "playSound",
    "saveConfig",
    "loadConfig",
    "loadConfigAtStartup",
    "isProgramInStartup",
    "toggleStartup",
    "enabled",
    "openAudioDeviceDialog",
    "findAudioDevice",
    "QAudioDevice",
    "QAudioDevice::Mode"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSoundboardENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    1,   88,    2, 0x08,    3 /* Private */,
       6,    0,   91,    2, 0x08,    5 /* Private */,
       7,    1,   92,    2, 0x08,    6 /* Private */,
       8,    0,   95,    2, 0x08,    8 /* Private */,
       9,    0,   96,    2, 0x08,    9 /* Private */,
      10,    0,   97,    2, 0x08,   10 /* Private */,
      11,    0,   98,    2, 0x08,   11 /* Private */,
      12,    1,   99,    2, 0x08,   12 /* Private */,
      14,    0,  102,    2, 0x08,   14 /* Private */,
      15,    2,  103,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,    2,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject Soundboard::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSSoundboardENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSoundboardENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSoundboardENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Soundboard, std::true_type>,
        // method 'refreshSerialPorts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToSerialPort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'readSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'saveConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadConfigAtStartup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'isProgramInStartup'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'toggleStartup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'openAudioDeviceDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'findAudioDevice'
        QtPrivate::TypeAndForceComplete<QAudioDevice, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAudioDevice::Mode, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Soundboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Soundboard *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshSerialPorts(); break;
        case 1: _t->connectToSerialPort(); break;
        case 2: _t->selectSound((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->readSerialData(); break;
        case 4: _t->playSound((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->saveConfig(); break;
        case 6: _t->loadConfig(); break;
        case 7: _t->loadConfigAtStartup(); break;
        case 8: { bool _r = _t->isProgramInStartup();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->toggleStartup((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->openAudioDeviceDialog(); break;
        case 11: { QAudioDevice _r = _t->findAudioDevice((*reinterpret_cast< std::add_pointer_t<QAudioDevice::Mode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QAudioDevice*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *Soundboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Soundboard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSoundboardENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Soundboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
