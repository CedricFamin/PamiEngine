#pragma once

#include <string>

namespace RTTI
{
    class Base
    {
    public:
        Base(std::string const & name);

        std::string const & ClassName() { return m_name; }

    protected:
        std::string m_name;
    };
}
