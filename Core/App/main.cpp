#include <vector>
#include <string>
#include <iostream>
#include <chrono>

#include "RTTI/RTTIBase.h"
#include "Utility/TaskScheduler/TaskScheduler.h"
#include "Utility/TaskScheduler/Task.h"


#include "luajit-2.0/lua.hpp"

namespace RTTI
{
    template <typename T>
    struct reversion_wrapper { T& iterable; };

    template <typename T>
    auto begin(reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

    template <typename T>
    auto end(reversion_wrapper<T> w) { return std::rend(w.iterable); }

    template <typename T>
    reversion_wrapper<T> reverse(T&& iterable) { return{ iterable }; }

    class Cluster : public RTTI::Base
    {
    public:
        Cluster(std::string const & name) : RTTI::Base(name) {}
        virtual void Init() {}
        virtual void Exec() {}
        virtual void DeInit() {}
    };

    class ClusterList : public Cluster
    {
    public:
        ClusterList() : Cluster("ClusterList")
        {

        }

        virtual void Init()
        {
            for (auto& cluster : SubClusters)
            {
                std::cout << "[Cluster] Init " << cluster->ClassName() << std::endl;
                cluster->Init();
            }
        }

        virtual void Exec()
        {
            for (auto& cluster : SubClusters)
            {
                cluster->Exec();
            }
        }

        virtual void DeInit()
        {
            for (auto& cluster : reverse(SubClusters))
            {
                std::cout << "[Cluster] DeInit " << cluster->ClassName() << std::endl;
                cluster->DeInit();
            }
        }

        void AddCluster(std::shared_ptr<Cluster> subCluster)
        {
            SubClusters.push_back(subCluster);
        }
    private:
        std::vector<std::shared_ptr<Cluster>> SubClusters;
    };

    class LuaVmCluster : public Cluster
    {
    public:
        LuaVmCluster() : Cluster("LuaVmCluster") { }

        virtual void Init()
        {
            m_luaState = luaL_newstate();
            if (m_luaState == nullptr)
            {
                std::cerr << "Spark2: lua_newstate() failed to create lua_State." << std::endl;
            }

            luaL_openlibs(m_luaState);
        }

    private:
       lua_State* m_luaState;
    };

    class TaskSchedulerCluster : public Cluster
    {
    public:
        TaskSchedulerCluster() : Cluster("TaskSchedulerCluster"), m_scheduler(nullptr) { }
        
        virtual void Init() override
        {
            m_scheduler = std::make_shared<TaskScheduler>(16);
            m_scheduler->Start();
            taskSchedulerService = std::make_unique<EnableService<TaskScheduler>>(m_scheduler);
            
        }

        virtual void Exec() override
        {
            m_scheduler->MainLoop();
        }

        virtual void DeInit() override
        {
            m_scheduler->Stop();
            taskSchedulerService = nullptr;
        }

    private:
        std::shared_ptr<TaskScheduler> m_scheduler;
        std::unique_ptr<EnableService<TaskScheduler>> taskSchedulerService;
    };
}

void LoadCluster(Task::Task::TaskData & parData)
{
    RTTI::Cluster * cluster = parData.Payload.Get<RTTI::Cluster>();
    assert(cluster != nullptr);
    cluster->Exec();
}

template<class... Ts> struct ServiceContainer {};

namespace Detail
{
    template<typename V, typename T, class...Ts>
    class ServiceContainerGetSetHelper
    {
    public:
        static std::shared_ptr<V> Get(ServiceContainer<T, Ts...> & container)
        {
            return ServiceContainerGetSetHelper<V, Ts...>::Get(container);
        }

        static void Set(ServiceContainer<T, Ts...> & container, std::shared_ptr<V> service)
        {
            ServiceContainerGetSetHelper<V, Ts...>::Set(container, service);
        }
    };

    template<typename T, class...Ts>
    class ServiceContainerGetSetHelper<T, T, Ts...>
    {
    public:
        static std::shared_ptr<T> Get(ServiceContainer<T, Ts...> & container)
        {
            return container.m_service;
        }
        static void Set(ServiceContainer<T, Ts...> & container, std::shared_ptr<T> service)
        {
            container.m_service = service;
        }
    };
};

template<class T, class... Ts>
struct ServiceContainer<T, Ts...> : ServiceContainer<Ts...>
{
    std::shared_ptr<T> m_service;

    template<typename V>
    std::shared_ptr<V> Get()
    {
        return Detail::ServiceContainerGetSetHelper<V, T, Ts...>::Get(*this);
    }

    template<typename V>
    void Set(std::shared_ptr<V> service)
    {
        Detail::ServiceContainerGetSetHelper<V, T, Ts...>::Set(*this, service);
    }
};

template<class... _Types>
class ServiceLocator
{
public:
    ServiceLocator() {}

    template<typename T> std::shared_ptr<T> Get()
    {
        return std::get<std::shared_ptr<T>>(serviceContainer);
    }

    template<typename T> void Store(std::shared_ptr<T> service)
    {
        std::get<std::shared_ptr<T>>(serviceContainer) = service;
    }

private:
    std::tuple<std::shared_ptr<_Types>...> serviceContainer;
};

class Foo {};
class Bar {};
class Toto {};
class Toti {};

template<typename..._Types, typename T>
ServiceLocator<_Types..., T> EnableService(ServiceLocator<_Types...> & const sc, std::shared_ptr<T> service)
{
    ServiceLocator<_Types..., T> newSc;
    newSc.Store<T>(service)
    return newSc;
}
int main()
{
    ServiceLocator<> sc;
    sc = EnableService(sc, std::string("test"));
    sc = EnableService(sc, int(5));
    
    
    sc.Store(std::make_shared<int>(5));
    sc.Store(std::make_shared<std::string>("test"));

    std::cout << *sc.Get<std::string>().get() << std::endl;
    RTTI::ClusterList initCluster;
    initCluster.AddCluster(std::make_shared<RTTI::TaskSchedulerCluster>());
    initCluster.AddCluster(std::make_shared<RTTI::LuaVmCluster>());

    initCluster.Init();
    initCluster.Exec();
    initCluster.DeInit();

    system("PAUSE");
    return 0;
}