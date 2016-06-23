#include <QtCore>
#include "externalmodel.h"

SOFT_BEGIN_NAMESPACE

class ExternalModel :: Private
{
  friend class ExternalModel;
  Private()
  {}

  QJsonObject jsonObject;
};

ExternalModel :: ExternalModel()
  : IDataModel()
  , d (new ExternalModel::Private())
{}

ExternalModel :: ~ExternalModel()
{
  delete d;
}

IDataModel *ExternalModel :: createModel()
{
  return new ExternalModel;
}

bool ExternalModel :: appendDimension (const char *, StdUInt)
{
  return false;
}

bool ExternalModel :: appendVariant (const char *key, StdVariant const &value)
{
  switch (value.type()) {
  case soft::StdTypes::String:
    d->jsonObject.insert(key, QJsonValue(QString::fromStdString(value.get<StdString>())));
    break;
  case soft::StdTypes::Int:
  case soft::StdTypes::UInt:
  case soft::StdTypes::Int8:
  case soft::StdTypes::UInt8:
  case soft::StdTypes::Int64:
  case soft::StdTypes::UInt64:
    d->jsonObject.insert(key, QJsonValue(value.toInt()));
    break;
  case soft::StdTypes::Double:
    d->jsonObject.insert(key, QJsonValue(value.get<StdDouble>()));
    break;
  case soft::StdTypes::DoubleArray:
    QJsonArray array;
    auto v = value.get<StdDoubleArray>();
    for (StdDoubleArray::iterator it = v.begin(); it != v.end(); ++it) {
      array.append(QJsonValue(*it));
    }
    d->jsonObject.insert(key, array);
    break;   
  }
  return true;
}

bool ExternalModel :: appendString     (const char* key, const std::string &value)
{
  QJsonValue jsonValue(QString::fromStdString(value));
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendInt8       (const char *key, int8_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendUInt8      (const char *key, uint8_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendInt16      (const char *key, int16_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendUInt16     (const char *key, uint16_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendInt32      (const char *key, int32_t value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendUInt32     (const char *key, uint32_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendInt64      (const char *key, int64_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendUInt64     (const char *key, uint64_t value)
{
  QJsonValue jsonValue((int)value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendFloat      (const char *key, float value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendDouble     (const char *key, double value)
{
  QJsonValue jsonValue(value);
  return (d->jsonObject.insert(key, jsonValue) != d->jsonObject.end());
}

bool ExternalModel :: appendBool       (const char *, bool)
{}

bool ExternalModel :: appendInt32Array (const char *, const std::vector<int32_t> &)
{}

bool ExternalModel :: appendDoubleArray(const char *key, const std::vector<double> &value)
{
  QJsonArray jsonArray;
  for (auto &v: value) {
    jsonArray.append(QJsonValue(v));
  }
  d->jsonObject.insert(key, jsonArray);
}

bool ExternalModel :: appendByteArray  (const char *, const std::vector<unsigned char> &)
{}

bool ExternalModel :: appendStringArray(const char *, const std::vector<std::string> &)
{}

bool ExternalModel :: appendArray      (const char *, const IDataModel *)
{}

bool ExternalModel :: appendModel      (const char *, IDataModel *)
{}

bool ExternalModel :: getDimension (const char *, StdUInt &) const
{
  return false;
}

bool ExternalModel :: getVariant       (const char *, StdVariant &) const
{}

bool ExternalModel :: getString (const char *key, std::string &str) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isString()) return false;
  auto qstr = (*it).toString();
  str.resize(qstr.length());
  str = qstr.toStdString();

  return true;    
}

bool ExternalModel :: getInt8          (const char *, int8_t &) const
{}

bool ExternalModel :: getUInt8         (const char *, uint8_t &) const
{}

bool ExternalModel :: getInt16         (const char *, int16_t &) const
{}

bool ExternalModel :: getUInt16        (const char *, uint16_t &) const
{}

bool ExternalModel :: getInt32         (const char *, int32_t &) const
{}

bool ExternalModel :: getUInt32        (const char *, uint32_t &) const
{}

bool ExternalModel :: getInt64         (const char *, int64_t &) const
{}

bool ExternalModel :: getUInt64        (const char *, uint64_t &) const
{}

bool ExternalModel :: getFloat         (const char *, float &) const
{}

bool ExternalModel :: getDouble        (const char *key, double &value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;

  if (!(*it).isDouble()) return false;
  value = (*it).toDouble();
  return true;
}

bool ExternalModel :: getBool          (const char *, bool &) const
{}

bool ExternalModel :: getInt32Array    (const char *, std::vector<int32_t> &) const
{}

bool ExternalModel :: getDoubleArray   (const char *key, std::vector<double> &value) const
{
  auto it = d->jsonObject.find(key);
  if (it == d->jsonObject.end()) return false;
  if (!(*it).isArray()) return false;
  QJsonArray array = (*it).toArray();
  value.resize(array.size());

  auto array_it = array.constBegin();
  auto value_it = value.begin();
  while (array_it !=array.constEnd() && value_it != value.end()) {
    (*value_it++) = (*array_it++).toDouble();
  }

  return true;
}

bool ExternalModel :: getByteArray     (const char *, std::vector<unsigned char> &) const
{}

bool ExternalModel :: getStringArray(const char *, std::vector<std::string> &) const
{}

bool ExternalModel :: getArray         (const char *, IDataModel *) const
{}

IDataModel* ExternalModel :: getModel(const char *) const 
{}


const QJsonObject *ExternalModel :: json() const
{
  return &d->jsonObject;
}

void ExternalModel :: setJson(QJsonObject const &obj)
{
  d->jsonObject = obj;
}

bool ExternalModel :: appendDoubleArray2D (const char *, const std::vector<std::vector<double> >&) 
{
  return false;
}

bool ExternalModel :: appendDoubleArray3D (const char *, const std::vector<std::vector<std::vector<double> > >&)
{
  return false;
}

bool ExternalModel :: getDoubleArray2D (const char *, std::vector<std::vector<double> > &) const
{
  return false;
}

bool ExternalModel :: getDoubleArray3D (const char *, std::vector<std::vector<std::vector<double> > > &) const
{
  return false;
}

SOFT_END_NAMESPACE
