#ifndef PARAMSSERVER_GLOBAL_H
#define PARAMSSERVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PARAMSSERV_LIBRARY)
# define PARAMSSERVERSHARED_EXPORT Q_DECL_EXPORT
#else
# define PARAMSSERVERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PARAMSSERVER_GLOBAL_H
