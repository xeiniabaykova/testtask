#include "FileIO.h"



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
void FileIO::Save()
{
  QString filename = QFileDialog::getSaveFileName( this, tr("Save Document"), QDir::currentPath(),
    tr("Documents (*.txt)") );

  if( !filename.isNull() )
  {
    qDebug( filename.toUtf8() );
  }

}
