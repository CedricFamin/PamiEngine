#include <iostream>

#include "RTTIClassCollector.h"

#include "TypeDescriptor.h"
#include "TypeInspector.h"

namespace AutoBinding
{
    
    CheckRTTINamespace::CheckRTTINamespace()
    {
    }

    void CheckRTTINamespace::run(const clang::ast_matchers::MatchFinder::MatchResult & result)
    {
        const clang::NamespaceDecl* nmspace = result.Nodes.getNodeAs<clang::NamespaceDecl>("RTTINamespace");
        if (nmspace)
        {
            auto it = nmspace->decls_begin();
            for (; it != nmspace->decls_end(); ++it)
            {
                if (it->getKind() == clang::Decl::CXXRecord)
                {
                    clang::CXXRecordDecl * recordDecl = llvm::cast<clang::CXXRecordDecl>(*it);
                    TypeDescriptor type(recordDecl->getName().data());

                    auto fieldIt = recordDecl->field_begin();
                    for (; fieldIt != recordDecl->field_end(); ++fieldIt)
                    {
                        clang::FieldDecl * field = llvm::cast<clang::FieldDecl>(*fieldIt);
                        clang::QualType qualType = field->getType();

                        FieldDescriptor * fieldDescriptor = new FieldDescriptor(field->getName().data(), typeInspector(qualType.getTypePtr(), nullptr));
                        type.AddField(fieldDescriptor);
                    }

                    _classes.push_back(type);
                }
            }
        }
    }

    std::vector<TypeDescriptor> const & CheckRTTINamespace::RegisteredClass() const { return _classes; }
}