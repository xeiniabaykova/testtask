#include "FileIO.h"
#include "Editor/DisplayedObject.h"


namespace Editor {
//------------------------------------------------------------------------------
// \ru создается диалоговое окно открытия файла
// ---
void FileIO::Open()
{
  QFileDialog::getOpenFileName( this, tr("Open Document"), QDir::currentPath(),
                                tr("Document files (All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );

  QString filename = QFileDialog::getOpenFileName( this, tr("Open Document"), QDir::currentPath(),
    tr( "All files (*.*)") );

  if( !filename.isNull() )
  {
    qDebug( filename.toUtf8() );
  }
}


//------------------------------------------------------------------------------
// \ru создается диалоговое окно сохранения файла
// ---
void FileIO::Save(const std::vector<std::shared_ptr<DisplayedObject>>& savedObj)
{
  QString fileName = QFileDialog::getSaveFileName( this, tr("Save Document"), QDir::currentPath(),
    tr("Documents (*.bin)") );

 //  std::istream stream(fileName);
}
}
