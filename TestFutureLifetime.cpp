// Copyright 2024 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "TestFutureLifetime.h"

#include "Map.h"
#include "MapTypes.h"
#include "MapQuickView.h"
#include "Viewpoint.h"

#include <QFuture>

using namespace Esri::ArcGISRuntime;


MyClass::MyClass()
{
  qDebug() << "MyClass::MyClass()";
}

MyClass::~MyClass()
{
  qDebug() << "~MyClass::MyClass()";
}

TestFutureLifetime::TestFutureLifetime(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(BasemapStyle::ArcGISStreets, this))
{
  auto myObject = std::make_shared<MyClass>();
  myObject->m_future = QtFuture::connect(this, &TestFutureLifetime::mapViewClicked).then(
  [myObject]()
  {
    qDebug() << "lambda executed";
  });
}

TestFutureLifetime::~TestFutureLifetime()
{
}

MapQuickView* TestFutureLifetime::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void TestFutureLifetime::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
  {
    return;
  }

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  connect(m_mapView, &MapQuickView::mouseClicked, this, &TestFutureLifetime::mapViewClicked);

  emit mapViewChanged();
}
