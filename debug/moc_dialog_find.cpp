/****************************************************************************
** Meta object code from reading C++ file 'dialog_find.h'
**
** Created: Thu Aug 31 22:15:43 2017
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog_find.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog_find.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FindDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      31,   11,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      70,   11,   11,   11, 0x08,
      96,   91,   11,   11, 0x08,
     125,  123,   11,   11, 0x08,
     155,   91,   11,   11, 0x08,
     194,  192,  187,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FindDialog[] = {
    "FindDialog\0\0btn_find_clicked()\0"
    "btn_print_clicked()\0btn_exit_clicked()\0"
    "btn_update_clicked()\0text\0"
    "edit_text_changed(QString)\0i\0"
    "edit_text_changed_mapper(int)\0"
    "searchkey_text_changed(QString)\0bool\0"
    ",\0eventFilter(QObject*,QEvent*)\0"
};

const QMetaObject FindDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FindDialog,
      qt_meta_data_FindDialog, 0 }
};

const QMetaObject *FindDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FindDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FindDialog))
        return static_cast<void*>(const_cast< FindDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FindDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btn_find_clicked(); break;
        case 1: btn_print_clicked(); break;
        case 2: btn_exit_clicked(); break;
        case 3: btn_update_clicked(); break;
        case 4: edit_text_changed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: edit_text_changed_mapper((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: searchkey_text_changed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: { bool _r = eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
