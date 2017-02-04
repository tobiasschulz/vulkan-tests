#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vk
{

template <typename T>
class vk_ptr
{
  public:
    vk_ptr() : vk_ptr([](T, VkAllocationCallbacks *) {}) {}

    vk_ptr(std::function<void(T, VkAllocationCallbacks *)> deletef)
    {
        this->deleter = [=](T obj) { deletef(obj, nullptr); };
    }

    vk_ptr(const vk_ptr<VkInstance> &instance, std::function<void(VkInstance, T, VkAllocationCallbacks *)> deletef)
    {
        this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
    }

    vk_ptr(const vk_ptr<VkDevice> &device, std::function<void(VkDevice, T, VkAllocationCallbacks *)> deletef)
    {
        this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
    }

    ~vk_ptr()
    {
        cleanup();
    }

    const T *operator&() const
    {
        return &object;
    }

    T *replace()
    {
        cleanup();
        return &object;
    }

    operator T() const
    {
        return object;
    }

    void operator=(T rhs)
    {
        if (rhs != object)
        {
            cleanup();
            object = rhs;
        }
    }

    template <typename V>
    bool operator==(V rhs)
    {
        return object == T(rhs);
    }

  private:
    T object{VK_NULL_HANDLE};
    std::function<void(T)> deleter;

    void cleanup()
    {
        if (object != VK_NULL_HANDLE)
        {
            deleter(object);
        }
        object = VK_NULL_HANDLE;
    }
};
}
