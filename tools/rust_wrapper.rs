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
    #include "toolkit.h"
    safety!(unsafe)
    generate!("vrv::Toolkit")
    exclude_utilities!()
}
