// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "AppExp.h"

using namespace std;

QString TestUsage()
{
    return QString();
}

struct Item
{
    int64_t itemId;
    int64_t deviceGroupId;
    std::string name;

    Item(int64_t itemId_, int64_t deviceGroupId_, const char* pName)
        : itemId(itemId_)
        , deviceGroupId(deviceGroupId_)
        , name(pName)
    {
    }
};

template<class T>
struct DeviceGroupData
{
    typedef std::map<int64_t, std::shared_ptr<T>> Map;
    Map                     itemMap;
    std::map<int64_t, Map>  deviceGroupMap;

    DeviceGroupData() = default;

    void add(int64_t deviceGroupId, int64_t itemId, std::shared_ptr<T>& item)
    {
        itemMap.emplace(itemId, item);

        auto it = deviceGroupMap.find(deviceGroupId);
        if (it == deviceGroupMap.end())
        {
            Map items{ { itemId, item } };
            deviceGroupMap.emplace(deviceGroupId, move(items));
        }
        else
        {
            it->second.emplace(itemId, item);
        }
    }

    std::shared_ptr<T> remove(int64_t deviceGroupId, int64_t itemId)
    {
        std::shared_ptr<T> item;

        if (auto it = deviceGroupMap.find(deviceGroupId); it != deviceGroupMap.end())
        {
            it->second.erase(itemId);
        }

        if (auto it = itemMap.find(itemId); it != itemMap.end())
        {
            item = move(it->second);
            itemMap.erase(it);
        }

        return item;
    }
};


int AppExp(QStringList arguments)
{
    DeviceGroupData<Item> data;

    auto item1 = make_shared<Item>(1, 5, "One");
    data.add(1, 5, item1);

    auto item2 = make_shared<Item>(1, 6, "Two");
    data.add(1, 6, item2);

    auto item3 = data.remove(1, 5);

    return 0;
}
