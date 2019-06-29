#include "linker/cpp/include/linkerinter.h"
#include "linker/cpp/include/linkerobject.h"

using namespace LinkerInter;

Code::Code()
{
}

Code::~Code()
{
}

const std::string
Code::gen() const
{
    return "";
}

size_t
Code::size() const
{
    return 0;
}

const Code *
Code::reduce(LinkerObject::SymTable *sym,
             int16_t offset) const
{
    return this;
}

Offset::Offset(const std::string &offset) :
    m_offset(offset)
{
}

Offset::~Offset()
{
}

const std::string
Offset::gen() const
{
    int16_t offset = std::stoi(m_offset);
    return std::string((char *) &offset, sizeof (offset));
}

const Code *
Offset::reduce(LinkerObject::SymTable *symtab,
               int16_t offset) const
{
    return new Offset(m_offset);
}

size_t
Offset::size() const
{
    return sizeof (int16_t);
}


ByteCode::ByteCode(const std::string bytes) :
    Code(), m_bytes(bytes)
{
}

ByteCode::~ByteCode()
{
}

const std::string
ByteCode::gen() const
{
    return m_bytes;
}

const Code *
ByteCode::reduce(LinkerObject::SymTable *sym,
                 int16_t offset) const
{
    return new ByteCode(m_bytes);
}

size_t
ByteCode::size() const
{
    return m_bytes.size();
}

Sym::Sym(const std::string &id) :
    Code(), m_id(id)
{
}

Sym::~Sym()
{
}

const std::string
Sym::gen() const
{
    return "";
}

const Code *
Sym::reduce(LinkerObject::SymTable *sym,
            int16_t offset) const
{
    offset = sym->get_sym(this)->offset() - offset;
    return new Offset(std::string((char *) &offset, sizeof (offset)));
}

const std::string &
Sym::id() const
{
    return m_id;
}

size_t
Sym::size() const
{
    return sizeof (int16_t);
}

SymCode::SymCode(const Code *code, const Sym *sym) :
    m_code(code), m_sym(sym)
{
}

SymCode::~SymCode()
{
    delete m_code;
    delete m_sym;
}

const std::string
SymCode::gen() const
{
    return m_code->gen() + m_sym->gen();
}

const Code *
SymCode::reduce(LinkerObject::SymTable *symtab,
                int16_t offset) const
{
    const Code *pref = m_code->reduce(symtab, offset);
    offset += m_code->size();
    const Code *post = m_sym->reduce(symtab, offset);
    offset += m_sym->size();
    return new CodeSeq(pref, post);
}

size_t
SymCode::size() const
{
    return m_code->size() + m_sym->size();
}

CodeSeq::CodeSeq(const Code *pref_code, const Code *post_code) :
    m_pref_code(pref_code), m_post_code(post_code)
{
}

CodeSeq::~CodeSeq()
{
    delete m_pref_code;
    delete m_post_code;
}

const std::string
CodeSeq::gen() const
{
    return m_pref_code->gen() + m_post_code->gen();
}

const Code *
CodeSeq::reduce(LinkerObject::SymTable *symtab,
                int16_t offset) const
{
    const Code *pref = m_pref_code->reduce(symtab, offset);
    offset += m_pref_code->size();
    const Code *post = m_post_code->reduce(symtab, offset);
    offset += m_post_code->size();
    return new CodeSeq(pref, post);
}

size_t
CodeSeq::size() const
{
    return m_pref_code->size() + m_post_code->size();
}

CodeTmpSeq::CodeTmpSeq(const Code *pref_code,
                       const Code *post_code) :
    CodeSeq(pref_code, post_code)
{
}

CodeTmpSeq::~CodeTmpSeq()
{
}

const Code *
CodeTmpSeq::reduce(LinkerObject::SymTable *symtab,
                   int16_t offset) const
{
    const Code *code_seq = CodeSeq::reduce(symtab,offset);
    delete this;
    return code_seq;
}
