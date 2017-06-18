#ifndef GRAPH_GLOBAL_H
#define GRAPH_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef GRAPH_LIB
# define GRAPH_EXPORT Q_DECL_EXPORT
#else
# define GRAPH_EXPORT Q_DECL_IMPORT
#endif

#endif // GRAPH_GLOBAL_H
