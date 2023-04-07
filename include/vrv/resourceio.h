/////////////////////////////////////////////////////////////////////////////
// Name:        resourcesio.h
// Author:      Chanjung Kim
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESOURCE_IO_H__
#define __VRV_RESOURCE_IO_H__

#include <optional>
#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// ResourceIO
//----------------------------------------------------------------------------

/**
 * This class is called by Resources when loading fonts.
 */

class ResourceIO {
public:
    virtual ~ResourceIO() = default;
    virtual std::optional<std::string> QueryFont(const std::string &fontName) = 0;
    virtual std::optional<std::string> QueryCssFont(const std::string &fontName) = 0;
    virtual std::optional<std::string> QueryTextFont(const std::string &fontName) = 0;
    virtual std::optional<std::string> QueryGlyph(const std::string &fontName, const std::string &codePoint) = 0;
    virtual std::optional<std::string> QueryFooter() = 0;
};

} // namespace vrv

#endif