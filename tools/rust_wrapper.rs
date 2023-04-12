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
    #include "vrvdef.h"
    #include "toolkit.h"
    safety!(unsafe)
    generate!("vrv::Toolkit")
    exclude_utilities!()
}

// Must be aligned with macro constants in vrvdef.h
pub const VERSION_MAJOR: i32 = 3;
pub const VERSION_MINOR: i32 = 15;
pub const VERSION_REVISION: i32 = 0;
pub const VERSION_DEV: bool = false;
