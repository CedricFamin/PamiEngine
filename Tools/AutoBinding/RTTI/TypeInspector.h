#include <string>

#include "Clang.h"

#include "TypeDescriptor.h"

namespace AutoBinding
{
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    class TypeInspectorResult
    {
    public:
        virtual std::string exec(FieldDescriptor * fd) = 0;
    };

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    class TypeInspectorLeaf : public TypeInspectorResult
    {
    public:
        TypeInspectorLeaf(std::string const & name);
        virtual std::string exec(FieldDescriptor * fd) override;
    private:
        std::string m_name;
    };

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    class TypeInspectorPointer : public TypeInspectorResult
    {
    public:
        TypeInspectorPointer(clang::Type const * type);
        virtual std::string exec(FieldDescriptor * fd) override;
    private:
        clang::Type const * m_type;
    };

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    class TypeInspectorClass : public TypeInspectorResult
    {
    public:
        TypeInspectorClass(clang::Type const * type);
        virtual std::string exec(FieldDescriptor * fd) override;
    private:
        clang::Type const * m_type;
    };

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    std::string typeInspector(clang::Type const * type, FieldDescriptor *  fd);
}