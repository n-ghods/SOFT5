#ifndef IDATAMODEL_H_DEF
#define IDATAMODEL_H_DEF

#include <cstddef>
#include "softns.h"

SOFT_BEGIN_NAMESPACE

class IDataModel
{
public:
   IDataModel();
   virtual ~IDataModel() = 0;

   virtual IDataModel* createModel() = 0;
   virtual void appendInt32(const char *, int) = 0;
   virtual void appendDouble(const char *, double) = 0;
   virtual void appendBool(const char*, bool value) = 0;
   virtual void appendDoubleArray(const char *, size_t length, const double *value) = 0;
   virtual void appendArray(const char*, IDataModel *array) = 0;
   virtual void appendModel(const char*, IDataModel *model) = 0;
};

SOFT_END_NAMESPACE

#endif // IDATAMODEL_H_DEF