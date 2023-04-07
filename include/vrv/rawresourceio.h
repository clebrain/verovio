/////////////////////////////////////////////////////////////////////////////
// Name:        rawresourcesio.h
// Author:      Chanjung Kim
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RAW_RESOURCE_IO_H__
#define __VRV_RAW_RESOURCE_IO_H__

#include "resourceio.h"

namespace vrv {

//----------------------------------------------------------------------------
// RawResourceIO
//----------------------------------------------------------------------------

/**
 * This class implements ResourceIO using function pointers.
 */
class RawResourceIO final : public ResourceIO {
public:
    struct FunctionTable final {
        void (*m_destructor)(void *context) noexcept;
        bool (*m_queryFont)(void *context, const std::string &fontName, std::string &output) noexcept;
        bool (*m_queryCssFont)(void *context, const std::string &fontName, std::string &output) noexcept;
        bool (*m_queryTextFont)(void *context, const std::string &fontName, std::string &output) noexcept;
        bool (*m_queryGlyph)(
            void *context, const std::string &fontName, const std::string &codePoint, std::string &output) noexcept;
    };

public:
    RawResourceIO(FunctionTable *table, void *context);

    virtual ~RawResourceIO() final override;

    virtual std::optional<std::string> QueryFont(const std::string &fontName) final override;

    virtual std::optional<std::string> QueryCssFont(const std::string &fontName) final override;

    virtual std::optional<std::string> QueryTextFont(const std::string &fontName) final override;

    virtual std::optional<std::string> QueryGlyph(
        const std::string &fontName, const std::string &codePoint) final override;

private:
    FunctionTable *m_table;
    void *m_context;
};

} // namespace vrv

#endif