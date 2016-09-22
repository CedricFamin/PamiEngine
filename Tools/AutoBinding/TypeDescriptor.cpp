#include "TypeDescriptor.h"

namespace AutoBinding
{
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    FieldDescriptor::FieldDescriptor(std::string const & name, std::string const & type)
        : m_name(name)
        , m_type(type)
    {

    }

    std::string const & FieldDescriptor::Name() const
    {
        return m_name;
    }

    std::string const & FieldDescriptor::Type() const
    {
        return m_type;
    }

    //-------------------------------------------------------------
    //-------------------------------------------------------------
    TypeDescriptor::TypeDescriptor(std::string const & name)
        : m_name(name)
    {

    }

    TypeDescriptor::TypeDescriptor(TypeDescriptor const & other)
        : m_name(other.m_name)
    {
        for (auto it : other.m_fields)
        {
            AddField(new FieldDescriptor(*it));
        }
    }

    TypeDescriptor::~TypeDescriptor()
    {
        for (auto it : m_fields)
        {
            delete it;
        }
    }

    TypeDescriptor & TypeDescriptor::operator=(TypeDescriptor const & other)
    {
        m_name = other.m_name;
        for (auto it : other.m_fields)
        {
            AddField(new FieldDescriptor(*it));
        }
        return *this;
    }

    void TypeDescriptor::AddField(FieldDescriptor * field)
    {
        m_fields.push_back(field);
    }

    std::string TypeDescriptor::ImportCode() const
    {
        std::string result;

        for (auto field : m_fields)
        {
            result += "\t\tobj->" + field->Name() + " = Read<" + field->Type() + ">(state);\n";
        }
        return result;
    }

    std::string const & TypeDescriptor::Name() const
    {
        return m_name;
    }
}
