#ifndef FILEIO_H
#define FILEIO_H
////////////////////////////////////////////////////////////////////////////////
/**
  \file
  Читатель - писатель файла из формы.\~

*/
////////////////////////////////////////////////////////////////////////////////
#include <QApplication>
#include <QFileDialog>
#include "Editor/DisplayedObject.h"
#include <memory>


namespace Editor {
///////////////////////////////////////////////////////////////////////////////
//
/// Чтение текущего состояния из файла,запись текущего состояния в файл.
/**
*/
///////////////////////////////////////////////////////////////////////////////
class FileIO: public QWidget {
public:
  FileIO() = default;
  ~FileIO() = default;

private:
  FileIO( const FileIO &obj ) = delete;
  FileIO& operator=( FileIO &obj ) = delete;

public:
  void Open ( std::vector<std::shared_ptr<Math::Curve>>& inputObj );       ///< Создать диалог открытия файла.
  void Save (const std::vector<std::shared_ptr<Math::Curve>>& savedObj ); ///< Создать диалог сохранения файла.

};
}
#endif // FILEIO_H
