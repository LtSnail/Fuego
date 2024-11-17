#pragma once


#include "CoreLibConcepts.h"

#include <Engine/Fuego/Core.h>


namespace Fuego
{
    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    class FUEGO_API SharedPtr
    {};
}
