//
// Created by tobias on 12.02.17.
//

#ifndef VULKANTEST_NONCOPYABLE_H
#define VULKANTEST_NONCOPYABLE_H

namespace helper
{

    class NonCopyable
    {
    protected:
        NonCopyable () = default;
        ~NonCopyable () = default;

        NonCopyable (NonCopyable const &) = delete;
        void operator= (NonCopyable const &x) = delete;
    };

}

#endif //VULKANTEST_NONCOPYABLE_H
