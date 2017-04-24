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
  void Open(); ///< \ru создается диалог открытия файла
  void Save(); ///< \ru создается диалог сохранения файла
};

#endif // FILEIO_H
