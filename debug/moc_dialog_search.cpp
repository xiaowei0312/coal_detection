/****************************************************************************
** Meta object code from reading C++ file 'dialog_search.h'
**
** Created: Thu Aug 31 22:01:37 2017
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog_search.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog_search.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SearchDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      39,   13,   13,   13, 0x08,
      58,   13,   13,   13, 0x08,
      77,   13,   13,   13, 0x08,
      95,   13,   13,   13, 0x08,
     117,   13,   13,   13, 0x08,
     147,  135,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SearchDialog[] = {
    "SearchDialog\0\0editTextChanged(QString)\0"
    "searchBtnClicked()\0firstPageClicked()\0"
    "lastPageClicked()\0previousPageClicked()\0"
    "nextPageClicked()\0item,column\0"
    "itemDbClicked(QTreeWidgetItem*,int)\0"
};

const QMetaObject SearchDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SearchDialog,
      qt_meta_data_SearchDialog, 0 }
};

const QMetaObject *SearchDialog::metaObject() const
{
    return &staticMetaObject;
}

void *SearchDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchDialog))
        return static_cast<void*>(const_cast< SearchDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SearchDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: searchBtnClicked(); break;
        case 2: firstPageClicked(); break;
        case 3: lastPageClicked(); break;
        case 4: previousPageClicked(); break;
        case 5: nextPageClicked(); break;
        case 6: itemDbClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
