#ifndef DOONDELETE_H
#define DOONDELETE_H


class DoOnDelete
{
public:
    DoOnDelete(std::function<void()> func);
    ~DoOnDelete();
    std::function<void()> func;
    bool on;
};

#endif // DOONDELETE_H
