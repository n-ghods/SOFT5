#include <QtCore>
#include <cstring> // for std::memcpy
#include <cassert>
#include <storage.h>
#include <softtypes.h>
#include "remarc.h"
#include "common.h"
#include "reference.hxx" // autogenerated file

Remarc :: Remarc(soft::Collection *collection, QString const &remarcPath, QObject *parent)
  : QObject(parent)
  , collection(collection)
  , remarcPath(remarcPath)
{
  process = new QProcess(this);
  connect(process, SIGNAL(started()), this, SLOT(started()));
  connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(exited()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));

}

Remarc :: ~Remarc()
{}

void Remarc :: run()
{
  auto dftPath = dynamic_cast<soft::Reference const*>(collection->findInstance("dftPath"));
  assert(nullptr != dftPath);
  process->setWorkingDirectory(remarcPath);
  process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
  process->start(QString("./rmc-vasp.sh %1").arg(QString::fromStdString(dftPath->uri)));
}

void Remarc :: readStdout()
{
  QTextStream(stdout) << process->readAllStandardOutput();
}

void Remarc :: readStderr()
{
  QTextStream(stdout) << process->readAllStandardError();
}

// Candidate for util functions in Soft?
static soft::StdBlob toStdBlob(QByteArray const &bytes)
{
  soft::StdBlob blob(bytes.size());
  std::memcpy(blob.data(), bytes.constData(), bytes.size());
  return blob;
}

// Candidate for util functions in Soft?
static QByteArray sha1(QString const &filename) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    return QByteArray();
  }
  QByteArray buffer = file.readAll();
  QCryptographicHash hash(QCryptographicHash::Sha1);
  hash.addData(buffer.data(), buffer.length());
  return hash.result();
}

void Remarc :: exited()
{
  soft::Storage storage(const_global_driver, const_global_uri, const_global_options);

  // Warning - ownership of this entity is lost to the collection
  auto surfaceChemkinRef = new soft::Reference();
  QFileInfo fileInfo(QDir(remarcPath), "SurfaceChemkin.inp");
  assert(fileInfo.exists()); // Check if the file really exists

  surfaceChemkinRef->uri          = fileInfo.absoluteFilePath().toStdString();
  surfaceChemkinRef->created      = fileInfo.created().toString("dd-mm-yyyy").toStdString();
  surfaceChemkinRef->owner        = fileInfo.owner().toStdString();
  surfaceChemkinRef->lastModified = fileInfo.lastModified().toString("dd-mm-yyyy").toStdString();
  surfaceChemkinRef->sha1         = toStdBlob(sha1(fileInfo.absoluteFilePath()));
  collection->attachEntity("surfaceChemkinRef", surfaceChemkinRef);
  storage.save(surfaceChemkinRef);

  emit finished();
}
  
void Remarc :: started()
{
  qDebug() << "started";
}
