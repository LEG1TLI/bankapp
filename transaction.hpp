#pragma once 

#include <string>

namespace bank
{
    struct Transaction 
    {
        std::string type;
        double amount = 0.0;
    };
} 