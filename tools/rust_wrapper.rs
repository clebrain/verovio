/////////////////////////////////////////////////////////////////////////////
// Name:        rust_wrapper.rs
// Author:      Chanjung Kim
// Created:     29/03/2023
// Copyright (c) Authors and others. All rights reserved.
//
// This wrapper is considered one of the library header files mentioned in
// the GNU Lesser General Public License document.
/////////////////////////////////////////////////////////////////////////////

use autocxx::prelude::*;

include_cpp! {
    #include "vrv.h"
    #include "toolkit.h"
    #include "options.h"
    safety!(unsafe)
    generate!("vrv::SetLogInterceptor")
    generate!("vrv::Toolkit")
    generate_pod!("vrv::OptionStringView")
    generate!("vrv::Options")
    generate!("vrv::OptionsMemberOffsets")
    generate_pod!("vrv::MapOfStrOptionsWrapperPair")
    generate!("vrv::MapOfStrOptionsWrapper")
    generate!("vrv::OptionsCategory")
    generate!("vrv::OptionGrp")
    generate!("vrv::Option")
    generate!("vrv::OptionDbl")
    generate!("vrv::OptionInt")
    generate!("vrv::OptionIntMap")
    generate!("vrv::OptionString")
    generate!("vrv::OptionArray")
    generate!("vrv::OptionBool")
    exclude_utilities!()
    exclude_impls!()
}

// Must be aligned with macro constants in vrvdef.h
pub const VERSION_MAJOR: i32 = 4;
pub const VERSION_MINOR: i32 = 2;
pub const VERSION_REVISION: i32 = 0;
pub const VERSION_DEV: bool = false;

// Must be aligned with enum constants in toolkitdef.h
pub const LOG_OFF: i32 = 0;
pub const LOG_ERROR: i32 = 1;
pub const LOG_WARNING: i32 = 2;
pub const LOG_INFO: i32 = 3;
pub const LOG_DEBUG: i32 = 4;
