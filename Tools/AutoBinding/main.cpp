#include <memory>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Clang.h"

#include "Jinja2Cpp/Jinja2CppLight.h"
#include "RTTI/TypeDescriptor.h"
#include "RTTI/TypeInspector.h"
#include "RTTI/RTTIClassCollector.h"


static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv)
{
    clang::tooling::CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    clang::ast_matchers::MatchFinder Finder;

    AutoBinding::CheckRTTINamespace rttiNamespaceInspector;
    Finder.addMatcher(clang::ast_matchers::namespaceDecl(clang::ast_matchers::hasName("RTTI")).bind("RTTINamespace"), &rttiNamespaceInspector);

    Tool.run(clang::tooling::newFrontendActionFactory(&Finder).get());

    std::string templateString;
    {
        std::ifstream t("Template/Binding.template.cpp");
        t.seekg(0, std::ios::end);
        templateString.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        templateString.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    }
    
    std::vector<std::string> const & sourcePath = OptionsParser.getSourcePathList();
    assert(sourcePath.size() == 1);
    std::string pathToSource = sourcePath[0];

    for (auto classDescriptor : rttiNamespaceInspector.RegisteredClass())
    {
        Jinja2CppLight::Template tpl(templateString);
        tpl.setValue("definitionFile", "todo.h");
        tpl.setValue("className", classDescriptor.Name());
        tpl.setValue("importCode", classDescriptor.ImportCode());
        std::ofstream file;
        file.open(classDescriptor.Name() + ".binding.cpp", std::ios_base::trunc | std::ios_base::out);
        file << tpl.render();
        file.close();
    }
    system("PAUSE");
    return 0;
}