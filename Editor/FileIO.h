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
/** \brief Чтение текущего состояния из файла.
  Запись текущего состояния в файл. \~
*/
// ---
namespace Editor {
class FileIO: public QWidget {
public:
  FileIO() = default;
  ~FileIO() = default;

private:
  FileIO( const FileIO &obj ) = delete;
  FileIO& operator=( FileIO &obj ) = delete;

public:
  void Open();                                                              ///< Создается диалог открытия файла.
  void Save(const std::vector<std::shared_ptr<DisplayedObject>>& savedObj); ///< Создается диалог сохранения файла.

};
}
#endif // FILEIO_H
