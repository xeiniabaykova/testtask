#include "FileIO.h"
#include "Editor/DisplayedObject.h"

#include "Serializer/Serializer.h"


namespace Editor {
//------------------------------------------------------------------------------
// \ru создается диалоговое окно открытия файла
// ---
void FileIO::Open( std::vector<std::shared_ptr<Math::Curve>>& inputObj )
{
  QString fileName = QFileDialog::getOpenFileName( this, tr("Open Document"), QDir::currentPath(),
    tr( "All files (*.*)") );

  if( fileName.isNull() )
  {
    qDebug( fileName.toUtf8() );
  }
  else
  {
    Serializer::Serializer serializer;
    std::vector<std::shared_ptr<Math::Curve>> curveVector = serializer.ReadCurves( fileName.toStdString() );
    if ( curveVector.size() > 0 )
        inputObj =  curveVector;
  }
}


//------------------------------------------------------------------------------
// \ru создается диалоговое окно сохранения файла
// ---
void FileIO::Save( const std::vector<std::shared_ptr<Math::Curve>>& savedObj )
{
  QString fileName = QFileDialog::getSaveFileName( this, tr("Save Document"), QDir::currentPath(),
    tr("Documents (*.bin)") );

  if( fileName.isNull() )
  {
    qDebug( fileName.toUtf8() );
  }
  else
  {
    Serializer::Serializer serializer;
    serializer.WriteCurves( savedObj, fileName.toStdString() );
  }


}
}
