#include "doondelete.h"

DoOnDelete::DoOnDelete(std::function<void()> func)
{
    this->func=func;
    on=true;
}

DoOnDelete::~DoOnDelete()
{
    if(on)
    {
        func();
    }
}
