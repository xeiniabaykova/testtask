#include "CreatorHandler.h"
#include "Math/GeomPolyline.h"
#include "Math/Line.h"
#include "Math/NurbsCurve.h"
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QInputDialog>

#include "Math/Ellipse.h"

namespace Editor {

CreatorHandler::CreatorHandler( int theNumExpectedPoits, TypeCurve theType ):
  numExpectedPoits            ( theNumExpectedPoits ),
  type                        ( theType )
{
}

//-----------------------------------------------------------------------------
//  Добавить точку с экрана в массив точек.
// ---
void CreatorHandler::AddPointFromScreen( Math::Point point )
{
  points.push_back( point );
}


//-----------------------------------------------------------------------------
//   Проверить, достаточно точек на экране выбрано для построения кривой.
// ---
bool CreatorHandler::IsSufficientNum() const
{
  return ( points.size() == numExpectedPoits );
}


//-----------------------------------------------------------------------------
//   Создать кривую по точкам в зависимости от типа.
// ---
std::shared_ptr<Math::Curve> CreatorHandler::Create()
{
  switch( type )
  {
    case CreateLine:
    {
      return std::make_shared<Math::Line>( points );
      break;
    }
    case CreateEllipse:
    {
      return std::make_shared<Math::Ellipse>( points );
      break;
    }
    case CreatePolyline:
    {
      return std::make_shared<Math::GeomPolyline>( points );
       break;
    }
    case CreateNURBS:
    {
      std::vector<double> weights;
      for ( size_t i = 0; i < points.size(); i++ )
        weights.push_back( 1.0 );

      int degree = 3;
      std::vector<double> nodes;
      bool isClosed = false;
      AddInformationNurbs( isClosed, degree ) ;
      return std::make_shared<Math::NurbsCurve>( points, weights, nodes, isClosed, degree );
      break;
    }
  }
  return nullptr;
}


//-----------------------------------------------------------------------------
//  Очистить массив точек, полученных с экрана.
// ---
void CreatorHandler::ClearPoints()
{
  points.clear();
}


//-----------------------------------------------------------------------------
//   Вызвать меню для получения дополнительной информации о нурбс - кривой.(степень, замкнутость)
// ---
void CreatorHandler::AddInformationNurbs( bool& isClosed, int& degree )
{
  QDialog * d = new QDialog();
  QVBoxLayout * vbox = new QVBoxLayout();
  QLabel* Closed = new QLabel( "&Closed:");
  QLabel* DegreeLabel = new QLabel( "&Degree:");
  QLineEdit * closedEdit = new QLineEdit();
  closedEdit->setPlaceholderText( QString("0") );
  closedEdit->setText( QString("0") );
  QLineEdit * degreeEdit = new QLineEdit();
  isClosed = 0;
  degree = 3;
  degreeEdit->setPlaceholderText( QString("3") );
  degreeEdit->setText( QString("3") );
  QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok| QDialogButtonBox::Cancel );

  QObject::connect( buttonBox, SIGNAL(accepted()), d, SLOT(accept()) );
  QObject::connect( buttonBox, SIGNAL(rejected()), d, SLOT(reject()) );
  Closed->setBuddy( Closed );
  DegreeLabel->setBuddy( DegreeLabel );

  vbox->addWidget( Closed );
  vbox->addWidget( closedEdit );
  vbox->addWidget( DegreeLabel );
  vbox->addWidget( degreeEdit );

  vbox->addWidget( buttonBox );

  d->setLayout( vbox );

  int result = d->exec();

  if ( result == QDialog::Accepted )
  {
    isClosed = closedEdit->text().toInt();
    degree = degreeEdit->text().toInt();
  }
}
}
