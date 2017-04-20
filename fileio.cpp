#include "FileIO.h"

void FileIO::Open()
{
  QFileDialog::getOpenFileName( this, tr("Open Document"), QDir::currentPath(),
                                tr("Document files (All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );

  QString filename = QFileDialog::getOpenFileName(
    this,
    tr("Open Document"),
    QDir::currentPath(),
    tr("All files (*.*)") );
  if( !filename.isNull() )
  {
    qDebug( filename.toUtf8() );
  }
}

void FileIO::Save()
{
  QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Document"),
        QDir::currentPath(),
        tr("Documents (*.txt)") );
  if( !filename.isNull() )
  {
    qDebug( filename.toUtf8() );
  }

}
