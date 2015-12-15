#include <QCoreApplication>
#include <QDebug>
#include <soft.h>
#include "softc-private.h"

struct _softc_t *softc_private_init(int argc, char *argv[])
{
  struct _softc_t *ret;  
  ret = (struct _softc_t*) malloc (sizeof (struct _softc_t));
  ret->app = 0;
  soft::init(argc, argv);
  return ret;
}

int softc_private_registered_storage_driver_count()
{
  return soft::storageDriverCount();
}

char **softc_private_get_storage_drivers()
{
  char **retval = (char**)malloc(sizeof(char*) * soft::storageDriverCount() + 1);
  int i = 0;
  for (auto &driver : soft::registeredStorageDrivers()) {
    retval[i] = (char*)malloc (sizeof(char*) * driver.length());
    strcpy(retval[i], driver.data());
    retval[++i] = NULL;	   
  }
  return retval;
}
