#pragma once

#include <vector>

#include "Clang.h"

#include "TypeDescriptor.h"

namespace AutoBinding
{
    class CheckRTTINamespace : public clang::ast_matchers::MatchFinder::MatchCallback
    {
    public:
        CheckRTTINamespace();

        void run(const clang::ast_matchers::MatchFinder::MatchResult & result) override;
        std::vector<TypeDescriptor> const & RegisteredClass() const;
    private:
        std::vector<TypeDescriptor> _classes;
    };
}