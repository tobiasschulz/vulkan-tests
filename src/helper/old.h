//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_OLD_H
#define VULKANTEST_OLD_H


namespace vk
{

    template<typename T>
    class ptr
    {
    public:
        ptr () : ptr ([] (T, VkAllocationCallbacks *) {})
        {}

        ptr (std::function<void (T, VkAllocationCallbacks *)> deletef)
        {
            this->deleter = [=] (T obj) { deletef (obj, nullptr); };
        }

        ptr (const ptr<VkInstance> &instance, std::function<void (VkInstance, T, VkAllocationCallbacks * )> deletef)
        {
            this->deleter = [&instance, deletef] (T obj) { deletef (instance, obj, nullptr); };
        }

        ptr (const ptr<VkDevice> &device, std::function<void (VkDevice, T, VkAllocationCallbacks * )> deletef)
        {
            this->deleter = [&device, deletef] (T obj) { deletef (device, obj, nullptr); };
        }

        ~ptr ()
        {
            cleanup ();
        }

        const T *operator& () const
        {
            return &object;
        }

        T *replace ()
        {
            cleanup ();
            return &object;
        }

        operator T () const
        {
            return object;
        }

        void operator= (T rhs)
        {
            if (rhs != object) {
                cleanup ();
                object = rhs;
            }
        }

        template<typename V>
        bool operator== (V rhs)
        {
            return object == T (rhs);
        }

    private:
        T object{VK_NULL_HANDLE};
        std::function<void (T)> deleter;

        void cleanup ()
        {
            if (object != VK_NULL_HANDLE) {
                deleter (object);
            }
            object = VK_NULL_HANDLE;
        }
    };
}


#endif //VULKANTEST_OLD_H
