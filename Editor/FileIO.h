////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru  читатель - записываетель файла\~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef FILEIO_H
#define FILEIO_H

#include <QApplication>
#include <QFileDialog>
#include "Editor/DisplayedObject.h"
#include <memory>


//------------------------------------------------------------------------------
/** \brief \ru чтение текущего состояния из файла
  запись текущего состояния в файл \~
*/
// ---
class FileIO: public QWidget {
public:
  FileIO() = default;
  ~FileIO() = default;

private:
  FileIO( const FileIO &obj ) = delete;
  FileIO& operator=( FileIO &obj ) = delete;

public:
  void Open(); ///< \ru создается диалог открытия файла
  void Save(const std::vector<std::shared_ptr<DisplayedObject>>& savedObj); ///< \ru создается диалог сохранения файла

};

#endif // FILEIO_H
