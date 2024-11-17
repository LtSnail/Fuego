#pragma once


#include "CoreLibConcepts.h"

#include <Core.h>


namespace Fuego
{
    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    class FUEGO_API SharedPtr
    {};
}
