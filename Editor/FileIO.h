#ifndef FILEIO_H
#define FILEIO_H

#include <QApplication>
#include <QFileDialog>


//------------------------------------------------------------------------------
/** \brief \ru Класс для ввода вывода из файла в графическом окне. \~
*/
// ---
class FileIO: public QWidget {
public:
  void Open();
  void Save();
};

#endif // FILEIO_H
