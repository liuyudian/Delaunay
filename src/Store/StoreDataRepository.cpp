/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StoreDataRepository.h"


/***********************************************************************************************************************
* Static class members
***********************************************************************************************************************/
StoreDataRepository *StoreDataRepository::instance = nullptr;


/***********************************************************************************************************************
* Function definitions
***********************************************************************************************************************/
StoreDataRepository *StoreDataRepository::getInstance()
{
    if (instance == nullptr)
    {
        instance = new StoreDataRepository();
    }

    return instance;
}


StoreDataRepository::StoreDataRepository() : data(nullptr)
{
    instance = new StoreDataRepository();
}


void StoreDataRepository::deleteInstance()
{
    delete instance;
    instance = nullptr;
}
