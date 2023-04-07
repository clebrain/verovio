/////////////////////////////////////////////////////////////////////////////
// Name:        rawresourcesio.cpp
// Author:      Chanjung Kim
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rawresourceio.h"

//----------------------------------------------------------------------------

#include <optional>
#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// RawResourceIO
//----------------------------------------------------------------------------

RawResourceIO::RawResourceIO(const FunctionTable *table, void *context) : m_table{ table }, m_context{ context } {}

RawResourceIO::~RawResourceIO()
{
    m_table->m_destructor(m_context);
}

std::optional<std::string> RawResourceIO::QueryFont(const std::string &fontName)
{
    std::string output;
    if (!m_table->m_queryFont(m_context, fontName, output)) {
        return std::nullopt;
    }
    return output;
}

std::optional<std::string> RawResourceIO::QueryCssFont(const std::string &fontName)
{
    std::string output;
    if (!m_table->m_queryCssFont(m_context, fontName, output)) {
        return std::nullopt;
    }
    return output;
}

std::optional<std::string> RawResourceIO::QueryTextFont(const std::string &fontName)
{
    std::string output;
    if (!m_table->m_queryTextFont(m_context, fontName, output)) {
        return std::nullopt;
    }
    return output;
}

std::optional<std::string> RawResourceIO::QueryGlyph(const std::string &fontName, const std::string &codePoint)
{
    std::string output;
    if (!m_table->m_queryGlyph(m_context, fontName, codePoint, output)) {
        return std::nullopt;
    }
    return output;
}

std::optional<std::string> RawResourceIO::QueryFooter()
{
    std::string output;
    if (!m_table->m_queryFooter(m_context, output)) {
        return std::nullopt;
    }
    return output;
}

} // namespace vrv