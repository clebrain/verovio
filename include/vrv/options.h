/////////////////////////////////////////////////////////////////////////////
// Name:        options.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OPTIONS_H__
#define __VRV_OPTIONS_H__

#ifdef CUSTOM_VEROVIO_OPTIONS
#include "custom_options.h"
#else

#include <cstddef>
#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attalternates.h"
#include "atttypes.h"
#include "smufl.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

//----------------------------------------------------------------------------

namespace vrv {

class OptionGrp;

//----------------------------------------------------------------------------
// Default scaling (%) and spacing (units) values
//----------------------------------------------------------------------------

#define DEFAULT_SCALE 100
#define MIN_SCALE 1
#define MAX_SCALE 1000

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

// the space between each lyric line in units
#define TEMP_LYRIC_LINE_SPACE 5.0
// the key signature spacing factor
#define TEMP_KEYSIG_STEP 0.4
// the key signature spacing factor for natural (usually slighly larger)
#define TEMP_KEYSIG_NATURAL_STEP 0.6

//----------------------------------------------------------------------------
// Option defines
//----------------------------------------------------------------------------

enum option_BREAKS { BREAKS_none = 0, BREAKS_auto, BREAKS_line, BREAKS_smart, BREAKS_encoded };

enum option_CONDENSE { CONDENSE_none = 0, CONDENSE_auto, CONDENSE_all, CONDENSE_encoded };

enum option_ELISION {
    ELISION_regular = SMUFL_E551_lyricsElision,
    ELISION_narrow = SMUFL_E550_lyricsElisionNarrow,
    ELISION_wide = SMUFL_E552_lyricsElisionWide,
    ELISION_unicode = UNICODE_UNDERTIE
};

enum option_FONT_FALLBACK { FONT_FALLBACK_Leipzig = 0, FONT_FALLBACK_Bravura };

enum option_FOOTER { FOOTER_none = 0, FOOTER_auto, FOOTER_encoded, FOOTER_always };

enum option_HEADER { HEADER_none = 0, HEADER_auto, HEADER_encoded };

enum option_MULTIRESTSTYLE {
    MULTIRESTSTYLE_auto = 0,
    MULTIRESTSTYLE_default,
    MULTIRESTSTYLE_block,
    MULTIRESTSTYLE_symbols
};

enum option_SYSTEMDIVIDER { SYSTEMDIVIDER_none = 0, SYSTEMDIVIDER_auto, SYSTEMDIVIDER_left, SYSTEMDIVIDER_left_right };

enum option_SMUFLTEXTFONT { SMUFLTEXTFONT_embedded = 0, SMUFLTEXTFONT_linked, SMUFLTEXTFONT_none };

class OptionDbl;
class OptionInt;
class OptionIntMap;
class OptionString;
class OptionArray;
class OptionBool;
class OptionJson;

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------

enum class OptionsCategory { None, Base, General, Layout, Mensural, Margins, Midi, Selectors, Full };

#ifdef RUST_LIBRARY

struct OptionStringView {
    const char *str;
    size_t length;
};

#endif

/**
 * This class is a base class of each styling parameter
 */
class Option {
public:
    // constructors and destructors
    Option()
    {
        m_shortOption = 0;
        m_isCmdOnly = false;
    }
    virtual ~Option() {}
    virtual void CopyTo(Option *option) = 0;

    void SetKey(const std::string &key) { m_key = key; }
    std::string GetKey() const { return m_key; }

    virtual bool SetValueBool(bool value);
    virtual bool SetValueDbl(double value);
    virtual bool SetValueArray(const std::vector<std::string> &values);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const = 0;
    virtual std::string GetDefaultStrValue() const = 0;

    virtual void Reset() = 0;
    virtual bool IsSet() const = 0;

    void SetInfo(const std::string &title, const std::string &description);
    std::string GetTitle() const { return m_title; }
    std::string GetDescription() const { return m_description; }

    void SetShortOption(char shortOption, bool isCmdOnly);
    char GetShortOption() const { return m_shortOption; }
    bool IsCmdOnly() const { return m_isCmdOnly; }
    virtual bool IsArgumentRequired() const { return true; }

#ifdef RUST_LIBRARY

    bool SetValueArray(const OptionStringView *values, size_t numValues)
    {
        std::vector<std::string> convertedValues;
        for (size_t i = 0; i < numValues; ++i) {
            convertedValues.push_back(std::string(values[i].str, values[i].length));
        }
        return SetValueArray(convertedValues);
    }

    OptionDbl *AsDbl();
    OptionInt *AsInt();
    OptionIntMap *AsIntMap();
    OptionString *AsString();
    OptionArray *AsArray();
    OptionBool *AsBool();
    OptionJson *AsJson();

    const OptionDbl *AsDbl() const;
    const OptionInt *AsInt() const;
    const OptionIntMap *AsIntMap() const;
    const OptionString *AsString() const;
    const OptionArray *AsArray() const;
    const OptionBool *AsBool() const;
    const OptionJson *AsJson() const;

#endif

    /**
     * Return a JSON object for the option
     */
    jsonxx::Object ToJson() const;

public:
    //----------------//
    // Static members //
    //----------------//

    /**
     * Static maps used my OptionIntMap objects. Set in OptIntMap::Init
     */
    static const std::map<int, std::string> s_breaks;
    static const std::map<int, std::string> s_condense;
    static const std::map<int, std::string> s_elision;
    static const std::map<int, std::string> s_fontFallback;
    static const std::map<int, std::string> s_footer;
    static const std::map<int, std::string> s_header;
    static const std::map<int, std::string> s_multiRestStyle;
    static const std::map<int, std::string> s_pedalStyle;
    static const std::map<int, std::string> s_systemDivider;
    static const std::map<int, std::string> s_smuflTextFont;

protected:
    std::string m_title;
    std::string m_description;

private:
    std::string m_key;
    /* the character for a short option - not set (0) by default) */
    char m_shortOption;
    /* a flag indicating that the option is available only on the command line */
    bool m_isCmdOnly;
};

//----------------------------------------------------------------------------
// OptionBool
//----------------------------------------------------------------------------

/**
 * This class is for boolean styling params
 */
class OptionBool : public Option {
public:
    // constructors and destructors
    OptionBool() { m_defaultValue = false; }
    virtual ~OptionBool() {}
    void CopyTo(Option *option) override;
    void Init(bool defaultValue);

    bool SetValueBool(bool value) override;
    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    bool GetValue() const { return m_value; }
    bool GetDefault() const { return m_defaultValue; }
    bool SetValue(bool value);

    bool IsArgumentRequired() const override { return false; }

private:
    //
public:
    //
private:
    bool m_value;
    bool m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionDbl
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionDbl : public Option {
public:
    // constructors and destructors
    OptionDbl()
    {
        m_defaultValue = 0.0;
        m_minValue = 0.0;
        m_maxValue = 0.0;
        m_definitionFactor = false;
    }
    virtual ~OptionDbl() {}
    void CopyTo(Option *option) override;
    void Init(double defaultValue, double minValue, double maxValue, bool definitionFactor = false);

    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    double GetValue() const;
    double GetUnfactoredValue() const;
    double GetDefault() const { return m_defaultValue; }
    double GetMin() const { return m_minValue; }
    double GetMax() const { return m_maxValue; }
    bool SetValue(double value);

private:
    //
public:
    //
private:
    double m_value;
    double m_defaultValue;
    double m_minValue;
    double m_maxValue;
    bool m_definitionFactor;
};

//----------------------------------------------------------------------------
// OptionInt
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionInt : public Option {
public:
    // constructors and destructors
    OptionInt()
    {
        m_defaultValue = 0;
        m_minValue = 0;
        m_maxValue = 0;
        m_definitionFactor = false;
    }
    virtual ~OptionInt() {}
    void CopyTo(Option *option) override;
    void Init(int defaultValue, int minValue, int maxValue, bool definitionFactor = false);

    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    int GetValue() const;
    int GetUnfactoredValue() const;
    int GetDefault() const { return m_defaultValue; }
    int GetMin() const { return m_minValue; }
    int GetMax() const { return m_maxValue; }
    bool SetValue(int value);

private:
    //
public:
    //
private:
    int m_value;
    int m_defaultValue;
    int m_minValue;
    int m_maxValue;
    bool m_definitionFactor;
};

//----------------------------------------------------------------------------
// OptionString
//----------------------------------------------------------------------------

/**
 * This class is for string styling params
 */
class OptionString : public Option {
public:
    // constructors and destructors
    OptionString() {}
    virtual ~OptionString() {}
    void CopyTo(Option *option) override;
    void Init(const std::string &defaultValue);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override { return m_value; }
    std::string GetDefaultStrValue() const override { return m_defaultValue; }

    std::string GetValue() const { return m_value; }
    std::string GetDefault() const { return m_defaultValue; }

    void Reset() override;
    bool IsSet() const override;

private:
    //
public:
    //
private:
    std::string m_value;
    std::string m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionArray
//----------------------------------------------------------------------------

/**
 * This class is for array (string) styling params
 */
class OptionArray : public Option {
public:
    // constructors and destructors
    OptionArray() {}
    virtual ~OptionArray() {}
    void CopyTo(Option *option) override;
    void Init();

    bool SetValueArray(const std::vector<std::string> &values) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    std::vector<std::string> GetValue() const { return m_values; }
    std::vector<std::string> GetDefault() const { return m_defaultValues; }
    bool SetValue(std::vector<std::string> const &values);

    void Reset() override;
    bool IsSet() const override;

#ifdef RUST_LIBRARY

    bool SetValue(const OptionStringView *values, size_t numValues)
    {
        std::vector<std::string> convertedValues;
        for (size_t i = 0; i < numValues; ++i) {
            convertedValues.push_back(std::string(values[i].str, values[i].length));
        }
        return SetValue(convertedValues);
    }

#endif

private:
    std::string GetStr(const std::vector<std::string> &values) const;

public:
    //
private:
    std::vector<std::string> m_values;
    std::vector<std::string> m_defaultValues;
};

//----------------------------------------------------------------------------
// OptionIntMap
//----------------------------------------------------------------------------

/**
 * This class is for map break options
 */
class OptionIntMap : public Option {
public:
    // constructors and destructors
    OptionIntMap();
    virtual ~OptionIntMap() {}
    void CopyTo(Option *option) override;
    void Init(int defaultValue, const std::map<int, std::string> *values);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    int GetValue() const { return m_value; }
    int GetDefault() const { return m_defaultValue; }
    bool SetValue(int value);

    std::vector<std::string> GetStrValues(bool withoutDefault) const;
    std::string GetStrValuesAsStr(bool withoutDefault) const;

    void Reset() override;
    bool IsSet() const override;

private:
    //
public:
    //
private:
    const std::map<int, std::string> *m_values;
    int m_value;
    int m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionStaffrel
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionStaffrel : public Option {
public:
    // constructors and destructors
    OptionStaffrel(){};
    virtual ~OptionStaffrel(){};
    void CopyTo(Option *option) override;
    // Alternate type style cannot have a restricted list of possible values
    void Init(data_STAFFREL defaultValue);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    // For alternate types return a reference to the value
    // Alternatively we can have a values vector for each sub-type
    const data_STAFFREL *GetValueAlternate() const { return &m_value; }
    const data_STAFFREL *GetDefaultAlernate() const { return &m_defaultValue; }

private:
    //
public:
    //
private:
    data_STAFFREL m_value;
    data_STAFFREL m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionJson
//----------------------------------------------------------------------------

/// Distinguish whether Json is passed directly or should be read from file
enum class JsonSource { String, FilePath };

/**
 * This class is for Json input params
 */

class OptionJson : public Option {
    using JsonPath = std::vector<std::reference_wrapper<jsonxx::Value>>;

public:
    /**
     * @name Constructor, destructor and initialization
     */
    ///@{
    OptionJson() = default;
    virtual ~OptionJson() = default;
    void CopyTo(Option *option) override;
    void Init(JsonSource source, const std::string &defaultValue);
    ///@}

    /**
     * Member access
     */
    JsonSource GetSource() const;
    jsonxx::Object GetValue(bool getDefault = false) const;

    /**
     * Interface methods: accessing values as string
     */
    ///@{
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;
    ///@}

    /**
     * Accessing values as json node path
     */
    ///@{
    bool HasValue(const std::vector<std::string> &jsonNodePath) const;
    int GetIntValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    double GetDblValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    std::string GetStrValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    bool UpdateNodeValue(const std::vector<std::string> &jsonNodePath, const std::string &value);
    ///@}

    /**
     * Accessing all keys
     */
    ///@{
    std::set<std::string> GetKeys() const;
    std::set<std::string> GetKeysByNode(const std::string &nodeName, std::list<std::string> &jsonNodePath) const;
    ///@}

protected:
    JsonPath StringPath2NodePath(const jsonxx::Object &obj, const std::vector<std::string> &jsonNodePath) const;

    // Find node by recursively processing all elements within. Only nodes of OBJECT_ type are processed
    const jsonxx::Object *FindNodeByName(
        const jsonxx::Object &obj, const std::string &jsonNodeName, std::list<std::string> &jsonNodePath) const;

    /// Read json from string or file
    bool ReadJson(jsonxx::Object &output, const std::string &input) const;

private:
    JsonSource m_source = JsonSource::String;

    jsonxx::Object m_values;
    jsonxx::Object m_defaultValues;
};

//----------------------------------------------------------------------------
// OptionGrp
//----------------------------------------------------------------------------

/**
 * This class is a base class of each styling parameter
 */
class OptionGrp {
public:
    // constructors and destructors
    OptionGrp() {}
    virtual ~OptionGrp() {}

    void SetLabel(const std::string &label, const std::string &id)
    {
        m_label = label;
        m_id = id;
    }
    std::string GetLabel() const { return m_label; }
    std::string GetId() const { return m_id; }

    void SetCategory(OptionsCategory category) { m_category = category; }
    OptionsCategory GetCategory() const { return m_category; }

    void AddOption(Option *option) { m_options.push_back(option); }

#ifdef RUST_LIBRARY

    const void *GetTransmutedOptions() const { return &m_options; }

#endif

    const std::vector<Option *> *GetOptions() const { return &m_options; }

public:
    //
protected:
    std::string m_id;
    std::string m_label;
    std::vector<Option *> m_options;
    OptionsCategory m_category = OptionsCategory::None;
};

#ifdef RUST_LIBRARY

struct MapOfStrOptionsWrapperPair {
    void *key;
    void *value;
};

//----------------------------------------------------------------------------
// MapOfStrOptionsWrapper
//----------------------------------------------------------------------------

class MapOfStrOptionsWrapper {
public:
    MapOfStrOptionsWrapper(const MapOfStrOptions &map)
        : m_begin{ map.begin() }, m_end{ map.end() }, m_numElementsRemaining{ map.size() }
    {
    }

    MapOfStrOptionsWrapperPair GetNext()
    {
        if (m_begin == m_end) {
            return { nullptr, nullptr };
        }

        auto &[key, value] = *(m_begin++);
        --m_numElementsRemaining;
        return { const_cast<std::string *>(&key), value };
    }

    size_t GetNumElementsRemaining() const { return m_numElementsRemaining; }

private:
    MapOfStrOptions::const_iterator m_begin;
    MapOfStrOptions::const_iterator m_end;
    size_t m_numElementsRemaining;
};

#endif

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

/**
 * This class contains the document styling parameters.
 */
class Options {
public:
    // constructors and destructors
    Options();
    virtual ~Options();

    Options(const Options &options);
    Options &operator=(const Options &options);

#ifdef RUST_LIBRARY

    MapOfStrOptionsWrapper GetWrappedItems() const { return MapOfStrOptionsWrapper(m_items); }

    void *GetWrappedItem(const std::string &key) const
    {
        MapOfStrOptions::const_iterator it = m_items.find(key);
        if (it != m_items.end()) {
            return it->second;
        }
        return nullptr;
    }

    const void *GetTransmutedGrps() const { return &m_grps; }

    const void *GetTransmutedBaseOptions() const;

#endif

    const MapOfStrOptions *GetItems() const { return &m_items; }

    const std::vector<OptionGrp *> *GetGrps() const { return &m_grps; }

    jsonxx::Object GetBaseOptGrp();

    const std::vector<Option *> *GetBaseOptions() const;

    // post processing of parameters
    void Sync();

private:
    void Register(Option *option, const std::string &key, OptionGrp *grp);

public:
    /**
     * Comments in implementation file options.cpp
     */
    OptionGrp m_baseOptions;

    // These options are only given for documentation - except for m_scale
    // They are ordered by short option alphabetical order
    OptionBool m_standardOutput;
    OptionString m_help;
    OptionBool m_allPages;
    OptionString m_inputFrom;
    OptionString m_logLevel;
    OptionString m_outfile;
    OptionInt m_page;
    OptionString m_resourcePath;
    OptionInt m_scale;
    OptionString m_outputTo;
    OptionBool m_version;
    OptionInt m_xmlIdSeed;

    /**
     * General
     */
    OptionGrp m_general;

    OptionBool m_adjustPageHeight;
    OptionBool m_adjustPageWidth;
    OptionIntMap m_breaks;
    OptionDbl m_breaksSmartSb;
    OptionIntMap m_condense;
    OptionBool m_condenseFirstPage;
    OptionBool m_condenseNotLastSystem;
    OptionBool m_condenseTempoPages;
    OptionBool m_evenNoteSpacing;
    OptionString m_expand;
    OptionIntMap m_footer;
    OptionIntMap m_header;
    OptionBool m_humType;
    OptionBool m_incip;
    OptionBool m_justifyVertically;
    OptionBool m_landscape;
    OptionDbl m_minLastJustification;
    OptionBool m_mmOutput;
    OptionBool m_moveScoreDefinitionToStaff;
    OptionBool m_neumeAsNote;
    OptionBool m_noJustification;
    OptionBool m_openControlEvents;
    OptionBool m_outputFormatRaw;
    OptionInt m_outputIndent;
    OptionBool m_outputIndentTab;
    OptionBool m_outputSmuflXmlEntities;
    OptionInt m_pageHeight;
    OptionInt m_pageMarginBottom;
    OptionInt m_pageMarginLeft;
    OptionInt m_pageMarginRight;
    OptionInt m_pageMarginTop;
    OptionInt m_pageWidth;
    OptionIntMap m_pedalStyle;
    OptionBool m_preserveAnalyticalMarkup;
    OptionBool m_removeIds;
    OptionBool m_scaleToPageSize;
    OptionBool m_setLocale;
    OptionBool m_showRuntime;
    OptionBool m_shrinkToFit;
    OptionIntMap m_smuflTextFont;
    OptionBool m_staccatoCenter;
    OptionBool m_svgBoundingBoxes;
    OptionString m_svgCss;
    OptionBool m_svgViewBox;
    OptionBool m_svgHtml5;
    OptionBool m_svgFormatRaw;
    OptionBool m_svgRemoveXlink;
    OptionArray m_svgAdditionalAttribute;
    OptionDbl m_unit;
    OptionBool m_useFacsimile;
    OptionBool m_usePgFooterForAll;
    OptionBool m_usePgHeaderForAll;
    OptionBool m_useBraceGlyph;
    OptionBool m_xmlIdChecksum;

    /**
     * General layout
     */
    OptionGrp m_generalLayout;

    OptionDbl m_barLineSeparation;
    OptionDbl m_barLineWidth;
    OptionBool m_beamFrenchStyle;
    OptionInt m_beamMaxSlope;
    OptionBool m_beamMixedPreserve;
    OptionDbl m_beamMixedStemMin;
    OptionDbl m_bracketThickness;
    OptionBool m_breaksNoWidow;
    OptionDbl m_dashedBarLineDashLength;
    OptionDbl m_dashedBarLineGapLength;
    OptionDbl m_dynamDist;
    OptionBool m_dynamSingleGlyphs;
    OptionJson m_engravingDefaults;
    OptionJson m_engravingDefaultsFile;
    OptionDbl m_extenderLineMinSpace;
    OptionDbl m_fingeringScale;
    OptionString m_font;
    OptionArray m_fontAddCustom;
    OptionIntMap m_fontFallback;
    OptionBool m_fontLoadAll;
    OptionDbl m_graceFactor;
    OptionBool m_graceRhythmAlign;
    OptionBool m_graceRightAlign;
    OptionDbl m_hairpinSize;
    OptionDbl m_hairpinThickness;
    OptionArray m_handwrittenFont;
    OptionDbl m_harmDist;
    OptionDbl m_justificationBraceGroup;
    OptionDbl m_justificationBracketGroup;
    OptionDbl m_justificationStaff;
    OptionDbl m_justificationSystem;
    OptionDbl m_justificationMaxVertical;
    OptionDbl m_ledgerLineThickness;
    OptionDbl m_ledgerLineExtension;
    OptionIntMap m_lyricElision;
    OptionDbl m_lyricLineThickness;
    OptionBool m_lyricNoStartHyphen;
    OptionDbl m_lyricSize;
    OptionDbl m_lyricTopMinMargin;
    OptionBool m_lyricVerseCollapse;
    OptionDbl m_lyricWordSpace;
    OptionInt m_measureMinWidth;
    OptionInt m_mnumInterval;
    OptionIntMap m_multiRestStyle;
    OptionDbl m_multiRestThickness;
    OptionBool m_octaveAlternativeSymbols;
    OptionDbl m_octaveLineThickness;
    OptionBool m_octaveNoSpanningParentheses;
    OptionDbl m_pedalLineThickness;
    OptionDbl m_repeatBarLineDotSeparation;
    OptionDbl m_repeatEndingLineThickness;
    OptionDbl m_slurCurveFactor;
    OptionDbl m_slurEndpointFlexibility;
    OptionDbl m_slurEndpointThickness;
    OptionDbl m_slurMargin;
    OptionInt m_slurMaxSlope;
    OptionDbl m_slurMidpointThickness;
    OptionDbl m_slurSymmetry;
    OptionInt m_spacingBraceGroup;
    OptionInt m_spacingBracketGroup;
    OptionBool m_spacingDurDetection;
    OptionDbl m_spacingLinear;
    OptionDbl m_spacingNonLinear;
    OptionInt m_spacingStaff;
    OptionInt m_spacingSystem;
    OptionDbl m_staffLineWidth;
    OptionDbl m_stemWidth;
    OptionDbl m_subBracketThickness;
    OptionIntMap m_systemDivider;
    OptionInt m_systemMaxPerPage;
    OptionDbl m_textEnclosureThickness;
    OptionDbl m_thickBarlineThickness;
    OptionDbl m_tieEndpointThickness;
    OptionDbl m_tieMidpointThickness;
    OptionDbl m_tieMinLength;
    OptionBool m_tupletAngledOnBeams;
    OptionDbl m_tupletBracketThickness;
    OptionBool m_tupletNumHead;

    /**
     * Selectors
     */
    OptionGrp m_selectors;

    OptionArray m_appXPathQuery;
    OptionArray m_choiceXPathQuery;
    OptionBool m_loadSelectedMdivOnly;
    OptionBool m_mdivAll;
    OptionString m_mdivXPathQuery;
    OptionArray m_substXPathQuery;
    OptionString m_transpose;
    OptionJson m_transposeMdiv;
    OptionBool m_transposeSelectedOnly;
    OptionBool m_transposeToSoundingPitch;

    /**
     * Element margins
     */
    OptionGrp m_elementMargins;

    OptionDbl m_defaultBottomMargin;
    OptionDbl m_defaultLeftMargin;
    OptionDbl m_defaultRightMargin;
    OptionDbl m_defaultTopMargin;
    //
    OptionDbl m_bottomMarginArtic;
    OptionDbl m_bottomMarginHarm;
    OptionDbl m_bottomMarginOctave;
    OptionDbl m_bottomMarginPgHead;
    //
    OptionDbl m_leftMarginAccid;
    OptionDbl m_leftMarginBarLine;
    OptionDbl m_leftMarginBeatRpt;
    OptionDbl m_leftMarginChord;
    OptionDbl m_leftMarginClef;
    OptionDbl m_leftMarginKeySig;
    OptionDbl m_leftMarginLeftBarLine;
    OptionDbl m_leftMarginMensur;
    OptionDbl m_leftMarginMeterSig;
    OptionDbl m_leftMarginMRest;
    OptionDbl m_leftMarginMRpt2;
    OptionDbl m_leftMarginMultiRest;
    OptionDbl m_leftMarginMultiRpt;
    OptionDbl m_leftMarginNote;
    OptionDbl m_leftMarginRest;
    OptionDbl m_leftMarginRightBarLine;
    OptionDbl m_leftMarginTabDurSym;
    //
    OptionDbl m_rightMarginAccid;
    OptionDbl m_rightMarginBarLine;
    OptionDbl m_rightMarginBeatRpt;
    OptionDbl m_rightMarginChord;
    OptionDbl m_rightMarginClef;
    OptionDbl m_rightMarginKeySig;
    OptionDbl m_rightMarginLeftBarLine;
    OptionDbl m_rightMarginMensur;
    OptionDbl m_rightMarginMeterSig;
    OptionDbl m_rightMarginMRest;
    OptionDbl m_rightMarginMRpt2;
    OptionDbl m_rightMarginMultiRest;
    OptionDbl m_rightMarginMultiRpt;
    OptionDbl m_rightMarginNote;
    OptionDbl m_rightMarginRest;
    OptionDbl m_rightMarginRightBarLine;
    OptionDbl m_rightMarginTabDurSym;
    //
    OptionDbl m_topMarginArtic;
    OptionDbl m_topMarginHarm;
    OptionDbl m_topMarginPgFooter;

    /**
     * Midi
     */
    OptionGrp m_midi;

    OptionBool m_midiNoCue;
    OptionDbl m_midiTempoAdjustment;

    /**
     * Mensural
     */
    OptionGrp m_mensural;

    OptionBool m_ligatureAsBracket;
    OptionBool m_mensuralToMeasure;

    /**
     * Deprecated options
     */
    OptionGrp m_deprecated;

private:
    /** The array of style parameters */
    MapOfStrOptions m_items;

    std::vector<OptionGrp *> m_grps;
};

// Must be synced with Options
enum OptionsMemberOffsets {
    m_baseOptions = offsetof(Options, m_baseOptions),
    m_standardOutput = offsetof(Options, m_standardOutput),
    m_help = offsetof(Options, m_help),
    m_allPages = offsetof(Options, m_allPages),
    m_inputFrom = offsetof(Options, m_inputFrom),
    m_logLevel = offsetof(Options, m_logLevel),
    m_outfile = offsetof(Options, m_outfile),
    m_page = offsetof(Options, m_page),
    m_resourcePath = offsetof(Options, m_resourcePath),
    m_scale = offsetof(Options, m_scale),
    m_outputTo = offsetof(Options, m_outputTo),
    m_version = offsetof(Options, m_version),
    m_xmlIdSeed = offsetof(Options, m_xmlIdSeed),
    m_general = offsetof(Options, m_general),
    m_adjustPageHeight = offsetof(Options, m_adjustPageHeight),
    m_adjustPageWidth = offsetof(Options, m_adjustPageWidth),
    m_breaks = offsetof(Options, m_breaks),
    m_breaksSmartSb = offsetof(Options, m_breaksSmartSb),
    m_condense = offsetof(Options, m_condense),
    m_condenseFirstPage = offsetof(Options, m_condenseFirstPage),
    m_condenseNotLastSystem = offsetof(Options, m_condenseNotLastSystem),
    m_condenseTempoPages = offsetof(Options, m_condenseTempoPages),
    m_evenNoteSpacing = offsetof(Options, m_evenNoteSpacing),
    m_expand = offsetof(Options, m_expand),
    m_footer = offsetof(Options, m_footer),
    m_header = offsetof(Options, m_header),
    m_humType = offsetof(Options, m_humType),
    m_incip = offsetof(Options, m_incip),
    m_justifyVertically = offsetof(Options, m_justifyVertically),
    m_landscape = offsetof(Options, m_landscape),
    m_ligatureAsBracket = offsetof(Options, m_ligatureAsBracket),
    m_mensuralToMeasure = offsetof(Options, m_mensuralToMeasure),
    m_minLastJustification = offsetof(Options, m_minLastJustification),
    m_mmOutput = offsetof(Options, m_mmOutput),
    m_moveScoreDefinitionToStaff = offsetof(Options, m_moveScoreDefinitionToStaff),
    m_neumeAsNote = offsetof(Options, m_neumeAsNote),
    m_noJustification = offsetof(Options, m_noJustification),
    m_openControlEvents = offsetof(Options, m_openControlEvents),
    m_outputFormatRaw = offsetof(Options, m_outputFormatRaw),
    m_outputIndent = offsetof(Options, m_outputIndent),
    m_outputIndentTab = offsetof(Options, m_outputIndentTab),
    m_outputSmuflXmlEntities = offsetof(Options, m_outputSmuflXmlEntities),
    m_pageHeight = offsetof(Options, m_pageHeight),
    m_pageMarginBottom = offsetof(Options, m_pageMarginBottom),
    m_pageMarginLeft = offsetof(Options, m_pageMarginLeft),
    m_pageMarginRight = offsetof(Options, m_pageMarginRight),
    m_pageMarginTop = offsetof(Options, m_pageMarginTop),
    m_pageWidth = offsetof(Options, m_pageWidth),
    m_pedalStyle = offsetof(Options, m_pedalStyle),
    m_preserveAnalyticalMarkup = offsetof(Options, m_preserveAnalyticalMarkup),
    m_removeIds = offsetof(Options, m_removeIds),
    m_scaleToPageSize = offsetof(Options, m_scaleToPageSize),
    m_showRuntime = offsetof(Options, m_showRuntime),
    m_shrinkToFit = offsetof(Options, m_shrinkToFit),
    m_smuflTextFont = offsetof(Options, m_smuflTextFont),
    m_staccatoCenter = offsetof(Options, m_staccatoCenter),
    m_svgBoundingBoxes = offsetof(Options, m_svgBoundingBoxes),
    m_svgCss = offsetof(Options, m_svgCss),
    m_svgViewBox = offsetof(Options, m_svgViewBox),
    m_svgHtml5 = offsetof(Options, m_svgHtml5),
    m_svgFormatRaw = offsetof(Options, m_svgFormatRaw),
    m_svgRemoveXlink = offsetof(Options, m_svgRemoveXlink),
    m_svgAdditionalAttribute = offsetof(Options, m_svgAdditionalAttribute),
    m_unit = offsetof(Options, m_unit),
    m_useFacsimile = offsetof(Options, m_useFacsimile),
    m_usePgFooterForAll = offsetof(Options, m_usePgFooterForAll),
    m_usePgHeaderForAll = offsetof(Options, m_usePgHeaderForAll),
    m_useBraceGlyph = offsetof(Options, m_useBraceGlyph),
    m_xmlIdChecksum = offsetof(Options, m_xmlIdChecksum),
    m_generalLayout = offsetof(Options, m_generalLayout),
    m_barLineSeparation = offsetof(Options, m_barLineSeparation),
    m_barLineWidth = offsetof(Options, m_barLineWidth),
    m_beamFrenchStyle = offsetof(Options, m_beamFrenchStyle),
    m_beamMaxSlope = offsetof(Options, m_beamMaxSlope),
    m_beamMixedPreserve = offsetof(Options, m_beamMixedPreserve),
    m_beamMixedStemMin = offsetof(Options, m_beamMixedStemMin),
    m_bracketThickness = offsetof(Options, m_bracketThickness),
    m_breaksNoWidow = offsetof(Options, m_breaksNoWidow),
    m_dashedBarLineDashLength = offsetof(Options, m_dashedBarLineDashLength),
    m_dashedBarLineGapLength = offsetof(Options, m_dashedBarLineGapLength),
    m_dynamDist = offsetof(Options, m_dynamDist),
    m_dynamSingleGlyphs = offsetof(Options, m_dynamSingleGlyphs),
    m_engravingDefaults = offsetof(Options, m_engravingDefaults),
    m_engravingDefaultsFile = offsetof(Options, m_engravingDefaultsFile),
    m_extenderLineMinSpace = offsetof(Options, m_extenderLineMinSpace),
    m_fingeringScale = offsetof(Options, m_fingeringScale),
    m_font = offsetof(Options, m_font),
    m_graceFactor = offsetof(Options, m_graceFactor),
    m_graceRhythmAlign = offsetof(Options, m_graceRhythmAlign),
    m_graceRightAlign = offsetof(Options, m_graceRightAlign),
    m_hairpinSize = offsetof(Options, m_hairpinSize),
    m_hairpinThickness = offsetof(Options, m_hairpinThickness),
    m_handwrittenFont = offsetof(Options, m_handwrittenFont),
    m_harmDist = offsetof(Options, m_harmDist),
    m_justificationBraceGroup = offsetof(Options, m_justificationBraceGroup),
    m_justificationBracketGroup = offsetof(Options, m_justificationBracketGroup),
    m_justificationStaff = offsetof(Options, m_justificationStaff),
    m_justificationSystem = offsetof(Options, m_justificationSystem),
    m_justificationMaxVertical = offsetof(Options, m_justificationMaxVertical),
    m_ledgerLineThickness = offsetof(Options, m_ledgerLineThickness),
    m_ledgerLineExtension = offsetof(Options, m_ledgerLineExtension),
    m_lyricElision = offsetof(Options, m_lyricElision),
    m_lyricLineThickness = offsetof(Options, m_lyricLineThickness),
    m_lyricNoStartHyphen = offsetof(Options, m_lyricNoStartHyphen),
    m_lyricSize = offsetof(Options, m_lyricSize),
    m_lyricTopMinMargin = offsetof(Options, m_lyricTopMinMargin),
    m_lyricVerseCollapse = offsetof(Options, m_lyricVerseCollapse),
    m_lyricWordSpace = offsetof(Options, m_lyricWordSpace),
    m_measureMinWidth = offsetof(Options, m_measureMinWidth),
    m_mnumInterval = offsetof(Options, m_mnumInterval),
    m_multiRestStyle = offsetof(Options, m_multiRestStyle),
    m_multiRestThickness = offsetof(Options, m_multiRestThickness),
    m_octaveAlternativeSymbols = offsetof(Options, m_octaveAlternativeSymbols),
    m_octaveLineThickness = offsetof(Options, m_octaveLineThickness),
    m_octaveNoSpanningParentheses = offsetof(Options, m_octaveNoSpanningParentheses),
    m_pedalLineThickness = offsetof(Options, m_pedalLineThickness),
    m_repeatBarLineDotSeparation = offsetof(Options, m_repeatBarLineDotSeparation),
    m_repeatEndingLineThickness = offsetof(Options, m_repeatEndingLineThickness),
    m_slurCurveFactor = offsetof(Options, m_slurCurveFactor),
    m_slurEndpointFlexibility = offsetof(Options, m_slurEndpointFlexibility),
    m_slurEndpointThickness = offsetof(Options, m_slurEndpointThickness),
    m_slurMargin = offsetof(Options, m_slurMargin),
    m_slurMaxSlope = offsetof(Options, m_slurMaxSlope),
    m_slurMidpointThickness = offsetof(Options, m_slurMidpointThickness),
    m_slurSymmetry = offsetof(Options, m_slurSymmetry),
    m_spacingBraceGroup = offsetof(Options, m_spacingBraceGroup),
    m_spacingBracketGroup = offsetof(Options, m_spacingBracketGroup),
    m_spacingDurDetection = offsetof(Options, m_spacingDurDetection),
    m_spacingLinear = offsetof(Options, m_spacingLinear),
    m_spacingNonLinear = offsetof(Options, m_spacingNonLinear),
    m_spacingStaff = offsetof(Options, m_spacingStaff),
    m_spacingSystem = offsetof(Options, m_spacingSystem),
    m_staffLineWidth = offsetof(Options, m_staffLineWidth),
    m_stemWidth = offsetof(Options, m_stemWidth),
    m_subBracketThickness = offsetof(Options, m_subBracketThickness),
    m_systemDivider = offsetof(Options, m_systemDivider),
    m_systemMaxPerPage = offsetof(Options, m_systemMaxPerPage),
    m_textEnclosureThickness = offsetof(Options, m_textEnclosureThickness),
    m_thickBarlineThickness = offsetof(Options, m_thickBarlineThickness),
    m_tieEndpointThickness = offsetof(Options, m_tieEndpointThickness),
    m_tieMidpointThickness = offsetof(Options, m_tieMidpointThickness),
    m_tieMinLength = offsetof(Options, m_tieMinLength),
    m_tupletAngledOnBeams = offsetof(Options, m_tupletAngledOnBeams),
    m_tupletBracketThickness = offsetof(Options, m_tupletBracketThickness),
    m_tupletNumHead = offsetof(Options, m_tupletNumHead),
    m_selectors = offsetof(Options, m_selectors),
    m_appXPathQuery = offsetof(Options, m_appXPathQuery),
    m_choiceXPathQuery = offsetof(Options, m_choiceXPathQuery),
    m_loadSelectedMdivOnly = offsetof(Options, m_loadSelectedMdivOnly),
    m_mdivAll = offsetof(Options, m_mdivAll),
    m_mdivXPathQuery = offsetof(Options, m_mdivXPathQuery),
    m_substXPathQuery = offsetof(Options, m_substXPathQuery),
    m_transpose = offsetof(Options, m_transpose),
    m_transposeMdiv = offsetof(Options, m_transposeMdiv),
    m_transposeSelectedOnly = offsetof(Options, m_transposeSelectedOnly),
    m_transposeToSoundingPitch = offsetof(Options, m_transposeToSoundingPitch),
    m_elementMargins = offsetof(Options, m_elementMargins),
    m_defaultBottomMargin = offsetof(Options, m_defaultBottomMargin),
    m_defaultLeftMargin = offsetof(Options, m_defaultLeftMargin),
    m_defaultRightMargin = offsetof(Options, m_defaultRightMargin),
    m_defaultTopMargin = offsetof(Options, m_defaultTopMargin),
    m_bottomMarginArtic = offsetof(Options, m_bottomMarginArtic),
    m_bottomMarginHarm = offsetof(Options, m_bottomMarginHarm),
    m_bottomMarginOctave = offsetof(Options, m_bottomMarginOctave),
    m_bottomMarginPgHead = offsetof(Options, m_bottomMarginPgHead),
    m_leftMarginAccid = offsetof(Options, m_leftMarginAccid),
    m_leftMarginBarLine = offsetof(Options, m_leftMarginBarLine),
    m_leftMarginBeatRpt = offsetof(Options, m_leftMarginBeatRpt),
    m_leftMarginChord = offsetof(Options, m_leftMarginChord),
    m_leftMarginClef = offsetof(Options, m_leftMarginClef),
    m_leftMarginKeySig = offsetof(Options, m_leftMarginKeySig),
    m_leftMarginLeftBarLine = offsetof(Options, m_leftMarginLeftBarLine),
    m_leftMarginMensur = offsetof(Options, m_leftMarginMensur),
    m_leftMarginMeterSig = offsetof(Options, m_leftMarginMeterSig),
    m_leftMarginMRest = offsetof(Options, m_leftMarginMRest),
    m_leftMarginMRpt2 = offsetof(Options, m_leftMarginMRpt2),
    m_leftMarginMultiRest = offsetof(Options, m_leftMarginMultiRest),
    m_leftMarginMultiRpt = offsetof(Options, m_leftMarginMultiRpt),
    m_leftMarginNote = offsetof(Options, m_leftMarginNote),
    m_leftMarginRest = offsetof(Options, m_leftMarginRest),
    m_leftMarginRightBarLine = offsetof(Options, m_leftMarginRightBarLine),
    m_leftMarginTabDurSym = offsetof(Options, m_leftMarginTabDurSym),
    m_rightMarginAccid = offsetof(Options, m_rightMarginAccid),
    m_rightMarginBarLine = offsetof(Options, m_rightMarginBarLine),
    m_rightMarginBeatRpt = offsetof(Options, m_rightMarginBeatRpt),
    m_rightMarginChord = offsetof(Options, m_rightMarginChord),
    m_rightMarginClef = offsetof(Options, m_rightMarginClef),
    m_rightMarginKeySig = offsetof(Options, m_rightMarginKeySig),
    m_rightMarginLeftBarLine = offsetof(Options, m_rightMarginLeftBarLine),
    m_rightMarginMensur = offsetof(Options, m_rightMarginMensur),
    m_rightMarginMeterSig = offsetof(Options, m_rightMarginMeterSig),
    m_rightMarginMRest = offsetof(Options, m_rightMarginMRest),
    m_rightMarginMRpt2 = offsetof(Options, m_rightMarginMRpt2),
    m_rightMarginMultiRest = offsetof(Options, m_rightMarginMultiRest),
    m_rightMarginMultiRpt = offsetof(Options, m_rightMarginMultiRpt),
    m_rightMarginNote = offsetof(Options, m_rightMarginNote),
    m_rightMarginRest = offsetof(Options, m_rightMarginRest),
    m_rightMarginRightBarLine = offsetof(Options, m_rightMarginRightBarLine),
    m_rightMarginTabDurSym = offsetof(Options, m_rightMarginTabDurSym),
    m_topMarginArtic = offsetof(Options, m_topMarginArtic),
    m_topMarginHarm = offsetof(Options, m_topMarginHarm),
    m_topMarginPgFooter = offsetof(Options, m_topMarginPgFooter),
    m_midi = offsetof(Options, m_midi),
    m_midiNoCue = offsetof(Options, m_midiNoCue),
    m_midiTempoAdjustment = offsetof(Options, m_midiTempoAdjustment),
    m_deprecated = offsetof(Options, m_deprecated),
};

} // namespace vrv

#endif // CUSTOM_VEROVIO_OPTIONS

#endif // __VRV_DEF_H__
