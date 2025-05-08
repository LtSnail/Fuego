#include "ServiceLocator.h"

template <>
Fuego::ServiceLocator& Fuego::singleton<Fuego::ServiceLocator>::instance()
{
    static ServiceLocator inst;
    return inst;
}