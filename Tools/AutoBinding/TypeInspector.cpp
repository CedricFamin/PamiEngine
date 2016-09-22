#include "TypeInspector.h"

namespace AutoBinding
{
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    TypeInspectorLeaf::TypeInspectorLeaf(std::string const & name)
        : m_name(name)
    {
    }

    std::string TypeInspectorLeaf::exec(FieldDescriptor * fd)
    {
        return m_name;
    }
    
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    TypeInspectorPointer::TypeInspectorPointer(clang::Type const * type, FieldDescriptor * fd)
        : m_type(type), m_fd(fd)
    {
    }

    std::string TypeInspectorPointer::exec(FieldDescriptor * fd)
    {
        return typeInspector(m_type->getPointeeOrArrayElementType(), m_fd) + std::string(" *");
    }
    
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    TypeInspectorClass::TypeInspectorClass(clang::Type const * type, FieldDescriptor * fd)
        : m_type(type), m_fd(fd)
    {
    }

    std::string TypeInspectorClass::exec(FieldDescriptor * fd)
    {

        clang::CXXRecordDecl * record = m_type->getAsCXXRecordDecl();
        if (record->isInStdNamespace())
        {

            std::string type = "std::";
            type += record->getName();
            if (record->getTemplateSpecializationKind() != clang::TSK_Undeclared)
            {
                type += "<";
                clang::ClassTemplateSpecializationDecl const * tpl = static_cast<clang::ClassTemplateSpecializationDecl *>(record);
                clang::TemplateArgumentList const & argList = tpl->getTemplateArgs();
                for (int i = 0; i < argList.size(); ++i)
                {
                    if (i > 0)
                    {
                        type += ", ";
                    }
                    clang::TemplateArgument const & argument = argList.get(i);
                    llvm::raw_string_ostream stream(type);
                    argument.dump(stream);
                }
                type += ">";
            }
            return type;
        }

        return "unknown";
    }

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    typedef bool (clang::Type::*CheckTest)() const;
    typedef TypeInspectorResult* CheckResult;
    typedef std::vector<std::pair<CheckTest, CheckResult>> Checks;

    void AddChecks(Checks & checks, CheckTest test, CheckResult result)
    {
        checks.push_back(std::make_pair(test, result));
    }

    void AddLeafCheck(Checks& checks, CheckTest test, std::string const & result)
    {
        AddChecks(checks, test, new TypeInspectorLeaf(result));
    }

    std::string typeInspector(clang::Type const * type, FieldDescriptor *  fd)
    {
        Checks checks;

        AddLeafCheck(checks, &clang::Type::isCharType, "Char");
        AddLeafCheck(checks, &clang::Type::isBooleanType, "Bool");
        AddLeafCheck(checks, &clang::Type::isEnumeralType, "Enum");
        AddLeafCheck(checks, &clang::Type::isIntegerType, "Int");
        AddChecks(checks, &clang::Type::isPointerType, new TypeInspectorPointer(type, fd));
        AddChecks(checks, &clang::Type::isRecordType, new TypeInspectorClass(type, fd));
        for (auto it : checks)
        {
            if ((type->*it.first)())
            {
                return it.second->exec(fd);
            }
        }
        return std::string();
    }
}