#ifndef SOFTC_H
#define SOFTC_H

#include "softc-entity.h"
#include "softc-utils.h"
#include "softc-collection.h"
#include "softc-macros.h"

typedef struct _softc_t softc_t;

SOFTC_BEGIN_DECLS

softc_t * softc_init(int argc, char *argv[]);
int       softc_storage_driver_count();
char   ** softc_get_storage_drivers();

SOFTC_END_DECLS

#endif 
