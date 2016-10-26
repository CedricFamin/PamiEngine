#pragma once

#include <string>
#include <vector>

namespace AutoBinding
{
    class FieldDescriptor
    {
    public:
        FieldDescriptor(std::string const & name, std::string const & type);

        std::string const & Name() const;
        std::string const & Type() const;
    private:
        std::string m_name;
        std::string m_type;
    };

    class TypeDescriptor
    {
    public:
        TypeDescriptor(std::string const & name);
        TypeDescriptor(TypeDescriptor const & other);

        ~TypeDescriptor();

        TypeDescriptor & operator=(TypeDescriptor const & other);

        void AddField(FieldDescriptor * field);
        std::string ImportCode() const;
        std::string const & Name() const;

        std::vector<FieldDescriptor*> const & Fields() const { return m_fields; }
    private:
        std::vector<FieldDescriptor*> m_fields;
        std::string m_name;
    };
}
