/**
    @file
    @author  Alexander Sherikov
    @copyright 2019 Alexander Sherikov. Licensed under the Apache License, Version 2.0.
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#ifndef H_EIGENUT_ALL
#define H_EIGENUT_ALL

/**
 * @brief This namespace contains various functions operating on Eigen
 * matrices and vectors.
 */
namespace eigenut
{
}

#include <Eigen/Dense>

#include "cpput/config.h"
#include "cpput/exception.h"
#include "cpput/visibility.h"

#define EIGENUT_ASSERT(condition, message)      CPPUT_ASSERT(condition, message)
#define EIGENUT_VISIBILITY_ATTRIBUTE            CPPUT_LOCAL
//#define EIGENUT_ENABLE_EIGENTYPE_DETECTION

#include "types.h"
#include "misc.h"
#include "cross_product.h"
#include "blockmatrix_base.h"
#include "blockmatrix_kronecker.h"
#include "blockmatrix.h"

#endif
