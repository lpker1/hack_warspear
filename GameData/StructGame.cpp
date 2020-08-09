#include "BaseGame.h"
#include "StructGame.h"
#include "debug.h"
#include "const.h"

void EntityBase::attack(DWORD monsterPtr)
{
    try
    {
        DWORD selfPtr = this->basePtr;
        log_debug("will attack %p, %p\n", selfPtr, monsterPtr);
        __asm
        {
            mov eax, monsterPtr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, 0x0073F1E0
            CALL eax
        }
    }
    catch (...)
    {
    }
}

void EntityBase::searchDead()
{
    try
    {
        /*
        a = imm.readLong(0x96ea30)
        a = imm.readLong(a + 0x10)
        a = imm.readLong(a + 0x34)
        a = imm.readLong(a + 0x38c)
        a = imm.readLong(a + 0xe14)
        a = imm.readLong(a + 0x78)
        a = imm.readLong(a)
        a = imm.readLong(a + 0x8)*/
        DWORD tmp_ptr = *(DWORD*)(BASE_DEAD_SEARCH_ADDR);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x10);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x34);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x38c);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0xe14);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x78);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x8);
        
        DWORD selfPtr = this->basePtr;
        log_debug("will searchDead %p\n", selfPtr);
        __asm
        {
            mov eax, tmp_ptr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, 0x0073F0C0
            CALL eax
        }
    }
    catch (...)
    {
    }
}


void EntityMgr::readTree(DWORD node)
{
    if (node == NULL)
    {
        return;
    }

    DWORD left = *(DWORD*)(node + 0x4);
    DWORD right = *(DWORD*)(node + 0x8);
    DWORD value = *(DWORD*)(node + 0x10);
    DWORD ptr = *(DWORD*)(node + 0x14);
    DWORD hp = *(DWORD*)(ptr + 0xf4);
    DWORD type = *(DWORD*)(ptr + 0x4);

    log_debug("hp:%d, ptr:%p\n", hp, (void*)ptr);
    readTree(left);
    readTree(right);

    EntityBase::EntityBasePtr entity(new EntityBase(ptr, hp, type));
    this->entities.insert(std::make_pair(value, entity));
}

EntityBase::EntityBasePtr EntityMgr::getEntityByHp(DWORD hp)
{
    for (auto ptr : this->entities)
    {
        if (ptr.second->HP == hp)
        {
            return EntityBase::EntityBasePtr(ptr.second);
        }
    }

    return EntityBase::EntityBasePtr();
}

EntityBase::EntityBasePtr EntityMgr::getEntityByType(DWORD type)
{
    for (auto ptr : this->entities)
    {
        if (ptr.second->type == type)
        {
            return EntityBase::EntityBasePtr(ptr.second);
        }
    }
    return EntityBase::EntityBasePtr();
}

void EntityMgr::getData()
{
    try
    {
        DWORD tmpVal = *(DWORD *)BASE_MONSTER_LIST_ROOT;
        tmpVal = *(DWORD*)(tmpVal + 0x10);
        tmpVal = *(DWORD*)(tmpVal + 0x30);
        tmpVal = *(DWORD*)(tmpVal);
        this->entities.clear();
        readTree(tmpVal);
    }
    catch (...) 
    {
        log_debug("error happened!\n");
    }
}

void EntityMgr::test()
{
    try
    {
        auto avatar = this->getEntityByType(EntityType::SELF);
        log_debug("avatar :%d\n", avatar->HP);
        avatar->searchDead();
    }
    catch (...)
    {
        log_debug("error happened!\n");
    }
}

HWND getGameWndHandle()
{
    try
    {
        return *(HWND*)BASE_HANDLE_ADDR;
    }
    catch (...)
    {
        return NULL;
    }
}