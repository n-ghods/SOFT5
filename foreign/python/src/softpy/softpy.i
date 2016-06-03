/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * SWIG interface file for the python bindings.
 *
 * We use numpy for interfacing arrays.  See
 * http://docs.scipy.org/doc/numpy-1.10.0/reference/swig.interface-file.html
 * for how to use numpy.i.
 *
 * The numpy.i file itself is downloaded from
 * https://github.com/numpy/numpy/blame/master/tools/swig/numpy.i
 */

%define DOCSTRING
"""softpy -- Python bindings to SOFT5

softpy provides bindings to most of the SOFT C API using SWIG.  Based
on entity descriptions, softpy makes it very easy to create and
instanciate Python classes for these entities.  The main mechanism for
this, is the entity() factory function, described in the next section.


Quick introduction
==================

Creating new entities
---------------------
The easiest way to create an entity is to use the entity() factory
function, which returns an entity class based on a metadata
description (as a JSON-formatted string):

    >>> metadata = open('person.json')  # fetch metadata from file
    >>> Person = softpy.entity(metadata)

This class expose all the metadata information, e.g.:

    >>> Person.name
    'Person'
    >>> Person.version
    '0.2.4'
    >>> Person.namespace
    'http://sintef.no/meta/soft'
    >>> Person.dimensions
    ['number_of_skills']
    >>> Person.property_names
    ['name', 'age', 'skills']

Instances of this class will have the entity properties available as
attributes:

    >>> jack = Person()
    >>> jack.name
    Uninitialized

These attributes can either be initialized manually:

    >>> jack.name = 'Jack Daniels'
    >>> jack.age = 42
    >>> jack.skills = ['tasting', 'Python', 'C']

or assigned by the creator:

    >>> jack = Person(name='Jack Daniels', age=42,
    ...               skills=['tasting', 'Python', 'C'])

or loaded from a storage:

   >>> jack = Person(uuid='39fad2c3-edd9-408d-93a0-924e4817a35e',
   ...               driver='hdf5', uri='archieve.h5')


Converting existing classes to entities
---------------------------------------
If you have an existing set of classes that you want to turn into SOFT
entities, you normally just have to inherit from an autogenerated entity.
Lets say that you have a class

   >>> class Account(object):
   ...     def __init__(self, owner)
   ...         self.owner = owner
   ...         self.credit = 0.0

with a set of additional methods for different kind of transactions.  Lets
say that you have created 'account.json' defining ``Account`` as an entity
with properties `owner` and `credit`.  Then, create your base class

   >>> BaseAccount = softpy.entity(open('account.json'))

and inherit from it:

   >>> class Account(BaseAccount)
   ...     def __init__(self, owner)
   ...         super(Account, self).__init__(owner)
   ...         self.owner = owner
   ...         self.credit = 0.0

without changing anything else.  You are now ready to use SOFT to store
and share your Account instances:

   >>> account = Account(owner='Per Gynt')
   >>> with softpy.Storage(driver='hdf5', uri='mydata.h5') as storage:
   ...     storage.save(account)

If you have attributes that cannot be stored as SOFT properties (e.g.
a dict) you can add setter and getter methods that maps the attribute
to something that can be stored by SOFT.  E.g. if the Accound class
above also contains an attribute `transactions` which is a dict
mapping dates to amounts, you could define the methods:

    >>>     def get_transactions(self):
    ...         string_list = ['%s:%f' % (k, v) for k, v in
    ...                        self.transaction.items()]
    ...         return string_list

    >>>     def set_transactions(self, string_list):
    ...         for s in string_list:
    ...             date, amount = s.split(':')
    ...             self.transactions[date] = float(amount)

If your class has getters and setters with another naming convention,
you can override softpy_get_property() and softpy_set_property().


Other classes
-------------
Below is a list of other usefull classes exposing the SOFT:

  * Storage     : For connecting to a data storage.
  * Collection  : A special entity containing a set of entities and
                  relations between them.


Some technical details about entities
=====================================
The C-level handles to entities are instances of ``softpy.entity_t``.  If
an object has an attribute named '__soft_entity__' that refers to such an
instance, the object itself will be treated as an entity by all the high-
level classes in softpy.

The entities created with the entity() factory function inherits from
BaseEntity, which defines a set of useful methods, all starting with
'soft_' to avoid name conflicts with entity properties and subclass
methods (note that the properties `name`, `version` etc of the
generated classes will not be inherited by the instances.  The
instances could use the soft_get_meta_name(), etc..  methods to obtain
the same information).
"""
%enddef

%module(docstring=DOCSTRING) softpy



/**********************************************
 ** C code included in the wrapper
 **********************************************/
%{
#include <stdint.h>
#include <stdbool.h>

#include "softc.h"

#define SWIG_FILE_WITH_INIT  // tell numpy that we initialize it in %init

  /* Globals */
  softc_t *softc = NULL;     // a global pointer to a softc instance
  //PyObject *SoftPyError = PyErr_NewException("_softpy.SoftPyError", NULL, NULL);

  /* Wrapper for softc_cleanup() */
  void cleanup(void) {
    softc_cleanup(softc);
    softc = NULL;
  }

  /* Initialize softc calling softc_init() with argc and argv obtained from
   * the sys module */
  void init(void) {
    int i, argc;
    char **argv;
    PyObject *sys, *sys_dict, *argv_obj, **items;
    sys = PyImport_ImportModule("sys");  // new ref
    sys_dict = PyModule_GetDict(sys);    // borrowed ref
    argv_obj = PyMapping_GetItemString(sys_dict, "argv");  // new ref
    argc = PySequence_Length(argv_obj);
    argv = malloc(argc * sizeof(char *));
    items = calloc(argc, sizeof(PyObject *));
    for (i=0; i<argc; i++) {
      items[i] = PySequence_GetItem(argv_obj, i);  // new ref
  #if PY_MAJOR_VERSION <= 2
      argv[i] = PyBytes_AsString(items[i]);  // borrowed memory
  #else
      argv[i] = PyUnicode_AsUTF8(items[i]);  // borrowed memory
  #endif
    }
    softc = softc_init(argc, argv);
    // Cleanup, can the strings pointed to by argv be free()ed?
    //for (i=0; i<argc; i++) if (items[i]) Py_DECREF(items[i]);
    free(items);
    free(argv);
    Py_DECREF(argv_obj);
    Py_DECREF(sys);
  }


  /* Returns the content of the python string object `o` as a newly
     malloc'ed string. NULL is returned on error. */
  char *pystring(PyObject *o)
  {
    char *s, *str=NULL;
    if (PyString_Check(o)) {
      if (!(str = PyString_AsString(o))) return NULL;
      if ((s = strdup(str))) return s;
      PyErr_SetString(PyExc_MemoryError, "");
    } else if (PyBytes_Check(o)) {
      if (!(str = PyBytes_AsString(o))) return NULL;
      if ((s = strdup(str))) return s;
      PyErr_SetString(PyExc_MemoryError, "");
    } else if (PyUnicode_Check(o)) {
      PyObject *bytes = PyUnicode_AsUTF8String(o);
      if (bytes) {
	str = PyBytes_AS_STRING(bytes);
	Py_DECREF(bytes);
	if (!str) return NULL;
	if ((s = strdup(str))) return s;
	PyErr_SetString(PyExc_MemoryError, "");
      }
    } else {
      PyErr_SetString(PyExc_TypeError,"must be string or unicode");
    }
    return NULL;
  }

%}


/**********************************************
 ** Module initialisation
 **********************************************/
%init %{
  /* Initialize numpy */
  import_array();

  /* Initialize softc */
  init();
%}


/**********************************************
 ** Typemaps
 **********************************************/
/* Generic typemaps */
%include <typemaps.i>
%include <exception.i>
%include "numpy.i"  // slightly changed to fit out needs, search for "XXX"

%include "softpy-typemaps.i"




/**********************************************
 ** Declare functions to wrap
 **********************************************/

/* Remove the softc_ prefix from the python bindings */
%feature("autodoc","2");
%feature("keyword");
%rename("%(strip:[softc_])s") "";
%include <stdint.i>


/*
 * softc
 */
void   init();  // called automatically in module initialisation...
void   cleanup();

%feature("docstring", "\
Returns the number of available storage drivers.") softc_storage_driver_count;
int    softc_storage_driver_count();

%feature("docstring", "\
Returns a list with storage driver names.") softc_get_storage_drivers;
char **softc_get_storage_drivers();

%feature("docstring", "Returns a new unique uuid.") softc_uuidgen;
char  *softc_uuidgen();


/*
 * entity
 */
%include "softpy-entity.i"


/*
 * datamodel
 */
%include "softpy-datamodel.i"


/*
 * storage
 */
softc_storage_t * softc_storage_create (const char *driver, const char *uri, const char *options=NULL);
void              softc_storage_free (softc_storage_t *);
void              softc_storage_load (softc_storage_t *, void *entity);
void              softc_storage_save (softc_storage_t *, void *entity);
softc_storage_strategy_t * softc_storage_get_storage_strategy(softc_storage_t *);
void softc_storage_free_storage_strategy(softc_storage_strategy_t *strategy);



/*
 * storage strategy
 */
softc_datamodel_t * softc_storage_strategy_get_datamodel(softc_storage_strategy_t *);
void softc_storage_strategy_free_datamodel(softc_datamodel_t *datamodel);
void                softc_storage_strategy_store(softc_storage_strategy_t *, const softc_datamodel_t *);
void                softc_storage_strategy_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void                softc_storage_strategy_start_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);
void                softc_storage_strategy_end_retrieve(softc_storage_strategy_t *, softc_datamodel_t *);



/*
 * collection
 */
/* Questions:
 *   - What is the difference between softc_collection_create()
 *     and softc_collection_create_ext().  What about namespace?
 *
 *   - The char *id argument of softc_collection_create() is not used
 *
 *   - Is the ***dimensions arg. a pointer to a newly allocated
 *     NULL-terminated array of string pointers?
 */
//%include softpy-collection.i
softc_collection_s * softc_collection_create(const char *id=NULL);
//softc_collection_s * softc_collection_create_ext(const char *name, const char *version);
void                 softc_collection_free(softc_collection_s *self);
void                 softc_collection_register_entity(softc_collection_s *self, const char *label, const softc_entity_t *entity);
void                 softc_collection_add_dim(softc_collection_s *self, const char *label, const char *description);
void                 softc_collection_connection(softc_collection_s *self, const char *subject, const char *predicate, const char *object);
size_t               softc_collection_num_entities(softc_collection_s *self);
size_t               softc_collection_num_dims(softc_collection_s *self);
size_t               softc_collection_num_relations(softc_collection_s *self);
size_t               softc_collection_num_dim_maps(softc_collection_s *self);
void                 softc_collection_get_dimensions(softc_collection_s *self, char ***ARGOUT_NULLTERM_STRING_LIST);


/**********************************************
 ** Python code
 **********************************************/
%include "softpy-python.i"
