/**
    @file
    @author  Alexander Sherikov
    @author  Jan Michalczyk
    @copyright 2019 Alexander Sherikov. Licensed under the Apache License, Version 2.0.
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/


#ifndef H_EIGENUT_MISC
#define H_EIGENUT_MISC

#include <vector>


/**
 * @page eigenut eigenut
 *
 * @anchor eigenut_casting_hack
 *
 * @note Since an attempt to write to a temporary object may result in a
 * compilation error, it is not possible to use the result of matrix.block() as
 * an output parameter of a function. However, there is a workaround: the
 * parameter can be declared as const and then casted to non-const as suggested
 * in Eigen's documentation:
 * https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html.
 * This hack is useful when it is necessary to handle matrix blocks and
 * matrices identically.
 */


/**
 * @defgroup eigenut eigenut
 * @brief Utility functions based on Eigen.
 */

/// @ingroup eigenut
namespace eigenut
{
    inline void getRandomPositiveDefinititeMatrix(Eigen::MatrixXd &M, const std::size_t size)
    {
        M.setRandom(size, size);
        M = M.transpose()*M + Eigen::MatrixXd::Identity(size, size);
    }


    /**
     * @brief Unset matrix (initialize to NaN)
     *
     * @tparam t_Derived matrix data type
     *
     * @param[in,out] matrix
     */
    template <typename t_Derived>
        void unsetMatrix (Eigen::DenseBase< t_Derived > &matrix)
    {
        if (std::numeric_limits<eigenut::DefaultScalar>::has_quiet_NaN)
        {
            matrix.setConstant(std::numeric_limits<eigenut::DefaultScalar>::quiet_NaN());
        }
        else
        {
            matrix.setZero();
        }
    }


    /**
     * @brief result = A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[out] result
     * @param[in] A
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENUT_VISIBILITY_ATTRIBUTE
            getATA( Eigen::PlainObjectBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A)
    {
        std::ptrdiff_t num_el = A.cols();
        result.resize(num_el,num_el);
        for (std::ptrdiff_t i = 0; i < num_el; ++i)
        {
            result.block(i, i, num_el-i, 1).noalias() = A.transpose().bottomRows(num_el-i) * A.col(i);
        }
    }


    /**
     * @brief result.diagonalBlock() = A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[out] result
     * @param[in] A
     * @param[in] offset    offset of A in result
     * @param[in] num_el    size of result
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENUT_VISIBILITY_ATTRIBUTE
            getATA( Eigen::PlainObjectBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A,
                    const std::ptrdiff_t offset,
                    const std::ptrdiff_t num_el)
    {
        std::ptrdiff_t A_num_col = A.cols();
        result.resize(num_el, num_el);
        result.template triangularView<Eigen::Lower>().setZero();
        for (std::ptrdiff_t i = 0; i < A_num_col; ++i)
        {
            result.block(   offset,
                            offset,
                            A_num_col,
                            A_num_col).block(i, i, A_num_col-i, 1).noalias() = A.transpose().bottomRows(A_num_col-i) * A.col(i);
        }
    }


    /**
     * @brief result += A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[in,out] result
     * @param[in] A
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENUT_VISIBILITY_ATTRIBUTE
            addATA( Eigen::DenseBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A)
    {
        std::ptrdiff_t num_el = A.cols();
        for (std::ptrdiff_t i = 0; i < num_el; ++i)
        {
            result.block(i, i, num_el-i, 1).noalias() += A.transpose().bottomRows(num_el-i) * A.col(i);
        }
    }


    /**
     * @brief result.diagonalBlock() += A^T * A
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput   Eigen parameter
     *
     * @param[in,out] result
     * @param[in] A
     * @param[in] offset    offset of A in result
     *
     * @attention Only the left lower triangular part of the result is initialized.
     */
    template<class t_DerivedOutput, class t_DerivedInput>
        void EIGENUT_VISIBILITY_ATTRIBUTE
            addATA( Eigen::DenseBase<t_DerivedOutput> &result,
                    const Eigen::DenseBase<t_DerivedInput> &A,
                    const std::ptrdiff_t offset)
    {
        std::ptrdiff_t A_num_col = A.cols();
        for (std::ptrdiff_t i = 0; i < A_num_col; ++i)
        {
            result.block(   offset,
                            offset,
                            A_num_col,
                            A_num_col).block(i, i, A_num_col-i, 1).noalias() += A.transpose().bottomRows(A_num_col-i) * A.col(i);
        }
    }



    /**
     * @brief Converts left lower triangular matrix to a symmetric matrix.
     *
     * @tparam t_Derived Eigen parameter
     *
     * @param[in,out] matrix
     */
    template<class t_Derived>
        void EIGENUT_VISIBILITY_ATTRIBUTE
            convertLLTtoSymmetric(Eigen::PlainObjectBase<t_Derived> &matrix)
    {
        std::ptrdiff_t num_el = matrix.cols();
        for (std::ptrdiff_t i = 0; i < num_el-1; ++i)
        {
            matrix.block(i, i+1, 1, num_el-i-1) = matrix.transpose().block(i, i+1, 1, num_el-i-1);
        }
    }


    /**
     * @brief Transform the input positions given as a concatenated set of 2d/3d
     * vectors, given M = [v1, v2, ...], returns M_new = [R*v1 + t, R*v2 + t, ...]
     *
     * @tparam t_DerivedMatrix      Eigen parameter
     * @tparam t_DerivedRotation    Eigen parameter
     * @tparam t_DerivedTranslation Eigen parameter
     *
     * @param[in] matrix matrix containing vectors (M)
     * @param[in] rotation rotation matrix         (R)
     * @param[in] translation translation vector   (t)
     *
     * @return matrix of transformed vectors
     */
    template <class t_DerivedMatrix, class t_DerivedRotation, class t_DerivedTranslation>
        inline Eigen::Matrix<   typename Eigen::MatrixBase<t_DerivedMatrix>::Scalar,
                                Eigen::MatrixBase<t_DerivedMatrix>::RowsAtCompileTime,
                                Eigen::MatrixBase<t_DerivedMatrix>::ColsAtCompileTime>
        EIGENUT_VISIBILITY_ATTRIBUTE
            transform(  const Eigen::MatrixBase<t_DerivedMatrix>        &matrix,
                        const Eigen::MatrixBase<t_DerivedRotation>      &rotation,
                        const Eigen::MatrixBase<t_DerivedTranslation>   &translation)
    {
        return ((rotation*matrix).colwise() + translation);
    }



    /**
     * @brief Create a diagonal matrix consisting of the input matrices
     *
     * @tparam t_Scalar         Eigen parameter (input)
     * @tparam t_rows           Eigen parameter (input)
     * @tparam t_cols           Eigen parameter (input)
     * @tparam t_flags          Eigen parameter (input)
     *
     * @param[in] input_matrices vector of smaller matrices to be placed into a large block diagonal matrix
     *
     * @return matrix with each input element as a block in the diagonal
     */
    template<   typename t_Scalar,
                int t_rows,
                int t_cols,
                int t_flags>
        EIGENUT_DYNAMIC_MATRIX(t_Scalar)
        EIGENUT_VISIBILITY_ATTRIBUTE
            makeBlockDiagonal(const std::vector<Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > &input_matrices)
    {
        switch (input_matrices.size())
        {
            case 0:
                return (EIGENUT_DYNAMIC_MATRIX(t_Scalar)());

            case 1:
                return (input_matrices[0]);

            default:
                // Initialize the output diagonal matrix
                std::ptrdiff_t row_size = 0;
                std::ptrdiff_t col_size = 0;

                for(std::size_t i = 0; i < input_matrices.size(); ++i)
                {
                    row_size += input_matrices[i].rows();
                    col_size += input_matrices[i].cols();
                }
                EIGENUT_DYNAMIC_MATRIX(t_Scalar) output;
                output.setZero(row_size, col_size);

                // Add in the input matrices
                std::ptrdiff_t cumulative_row = 0;
                std::ptrdiff_t cumulative_col = 0;

                for(std::size_t i = 0; i < input_matrices.size(); ++i)
                {
                    int current_num_rows = input_matrices[i].rows();
                    int current_num_cols = input_matrices[i].cols();

                    output.block(cumulative_row, cumulative_col, current_num_rows, current_num_cols) = input_matrices[i];

                    cumulative_row += current_num_rows;
                    cumulative_col += current_num_cols;
                }
                return output;
        }
    }



    /**
     * @brief Create a diagonal matrix replicating the input matrix
     *
     * @tparam t_DerivedInput   Eigen parameter (input)
     *
     * @param[in] input_matrix      matrix to be replicated into block diagonal
     * @param[in] num_copies        determines the output size
     *
     * @return matrix with each input element as a block in the diagonal
     */
    template<class t_DerivedInput>
        EIGENUT_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar)
        EIGENUT_VISIBILITY_ATTRIBUTE
            makeBlockDiagonal(  const Eigen::DenseBase<t_DerivedInput> &input_matrix,
                                const std::ptrdiff_t num_copies)
    {
        switch (num_copies)
        {
            case 0:
                return (EIGENUT_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar)());

            case 1:
                return (input_matrix);

            default:
                // Initialize the output diagonal matrix
                EIGENUT_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_DerivedInput>::Scalar) output;

                output.setZero(num_copies * input_matrix.rows(),
                               num_copies * input_matrix.cols());

                // Replicate the input matrix into block diagonal form
                for(std::ptrdiff_t i = 0; i < num_copies; ++i)
                {
                    output.block(i*input_matrix.rows(), i*input_matrix.cols(),
                                 input_matrix.rows(), input_matrix.cols()) = input_matrix;
                }
                return (output);
        }
    }



    /**
     * @brief Concatenate matrices vertically, [A;B;C; ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_Scalar         Eigen parameter (input)
     * @tparam t_rows           Eigen parameter (input)
     * @tparam t_cols           Eigen parameter (input)
     * @tparam t_flags          Eigen parameter (input)
     *
     * @param[out] result    result of concatenation
     * @param[in] matrices  matrices
     */
    template<   class t_DerivedOutput,
                typename t_Scalar,
                int t_rows,
                int t_cols,
                int t_flags>
        void  EIGENUT_VISIBILITY_ATTRIBUTE
            concatenateMatricesVertically(  Eigen::PlainObjectBase<t_DerivedOutput> &result,
                                            const std::vector<Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > &matrices)
    {
        std::vector< Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> > nonempty_matrices  = matrices;
        std::size_t                  number_of_matrices = nonempty_matrices.size();

        if (number_of_matrices > 0)
        {
            std::ptrdiff_t total_number_of_rows = 0;
            std::ptrdiff_t number_of_cols = 0;

            for(std::size_t i = 0; i < number_of_matrices; ++i)
            {
                if(!nonempty_matrices[i].size())
                {
                    nonempty_matrices.erase(nonempty_matrices.begin() + i);
                    --number_of_matrices;
                }
            }

            if(!number_of_matrices)
            {
                result.resize(0, 0);
                return;
            }

            number_of_cols = nonempty_matrices[0].cols();
            for (std::size_t i = 0; i < number_of_matrices; ++i)
            {
                EIGENUT_ASSERT(number_of_cols == nonempty_matrices[i].cols(), "Inconsistent size of input matrices.");
                total_number_of_rows += nonempty_matrices[i].rows();
            }

            result.resize(total_number_of_rows, number_of_cols);

            std::ptrdiff_t row_index = 0;
            for (std::size_t i = 0; i < number_of_matrices; ++i)
            {
                result.block(row_index, 0, nonempty_matrices[i].rows(), number_of_cols) = nonempty_matrices[i];
                row_index += nonempty_matrices[i].rows();
            }
        }
        else
        {
            result.resize(0, 0);
        }
    }


    /**
     * @brief Concatenate matrices horizontally, [A B C ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput1  Eigen parameter
     * @tparam t_DerivedInput2  Eigen parameter
     *
     * @param[out] result    result of concatenation
     * @param[in] matrix1
     * @param[in] matrix2
     */
    template<   class t_DerivedOutput,
                class t_DerivedInput1,
                class t_DerivedInput2>
        void  EIGENUT_VISIBILITY_ATTRIBUTE
            concatenateMatricesHorizontally(
                Eigen::PlainObjectBase<t_DerivedOutput> &result,
                const Eigen::DenseBase<t_DerivedInput1> &matrix1,
                const Eigen::DenseBase<t_DerivedInput2> &matrix2)
    {
        if (matrix1.rows() == 0)
        {
            result = matrix2;
        }
        else
        {
            if (matrix2.rows() == 0)
            {
                result = matrix1;
            }
            else
            {
                std::ptrdiff_t  number_of_rows = matrix1.rows();
                std::ptrdiff_t  number_of_cols = matrix1.cols() + matrix2.cols();

                EIGENUT_ASSERT(number_of_rows == matrix2.rows(), "Inconsistent size of input matrices.");

                result.resize(number_of_rows, number_of_cols);
                result << matrix1, matrix2;
            }
        }
    }



    /**
     * @brief Concatenate matrices horizontally, [A B C ...]
     *
     * @tparam t_DerivedOutput  Eigen parameter
     * @tparam t_DerivedInput1  Eigen parameter
     * @tparam t_DerivedInput2  Eigen parameter
     * @tparam t_DerivedInput3  Eigen parameter
     *
     * @param[out] result    result of concatenation
     * @param[in] matrix1
     * @param[in] matrix2
     * @param[in] matrix3
     */
    template<   class t_DerivedOutput,
                class t_DerivedInput1,
                class t_DerivedInput2,
                class t_DerivedInput3>
        void  EIGENUT_VISIBILITY_ATTRIBUTE
            concatenateMatricesHorizontally(
                Eigen::PlainObjectBase<t_DerivedOutput> &result,
                const Eigen::DenseBase<t_DerivedInput1> &matrix1,
                const Eigen::DenseBase<t_DerivedInput2> &matrix2,
                const Eigen::DenseBase<t_DerivedInput3> &matrix3)
    {
        if (matrix1.rows() == 0)
        {
            concatenateMatricesHorizontally(result, matrix2, matrix3);
        }
        else
        {
            if (matrix2.rows() == 0)
            {
                concatenateMatricesHorizontally(result, matrix1, matrix3);
            }
            else
            {
                if (matrix3.rows() == 0)
                {
                    concatenateMatricesHorizontally(result, matrix1, matrix2);
                }
                else
                {
                    std::ptrdiff_t  number_of_rows = matrix1.rows();
                    std::ptrdiff_t  number_of_cols = matrix1.cols() + matrix2.cols() + matrix3.cols();

                    EIGENUT_ASSERT(  (number_of_rows == matrix2.rows())
                                        && (number_of_rows == matrix3.rows()),
                                        "Inconsistent size of input matrices.");

                    result.resize(number_of_rows, number_of_cols);
                    result << matrix1, matrix2, matrix3;
                }
            }
        }
    }



    /**
     * @brief Remove a row with the specified index.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in,out] matrix        matrix
     * @param[in] row_to_remove index of a row
     *
     * Based on
     * http://stackoverflow.com/questions/13290395/how-to-remove-a-certain-row-or-column-while-using-eigen-library-c
     */
    template<class t_Derived>
        void  EIGENUT_VISIBILITY_ATTRIBUTE
            removeRow(  Eigen::PlainObjectBase<t_Derived> & matrix,
                        const std::ptrdiff_t row_to_remove)
    {
        EIGENUT_ASSERT(row_to_remove <= matrix.rows(), "The index of a removed row is greater than the size of the matrix.");

        std::ptrdiff_t number_of_rows = matrix.rows()-1;
        std::ptrdiff_t number_of_cols = matrix.cols();

        if( row_to_remove < number_of_rows )
        {
            matrix.block(row_to_remove, 0, number_of_rows - row_to_remove, number_of_cols) =
                matrix.block(row_to_remove + 1, 0 , number_of_rows - row_to_remove, number_of_cols);
        }

        matrix.conservativeResize(number_of_rows, number_of_cols);
    }



    /**
     * @brief Remove a column with the specified index.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in,out] matrix            matrix
     * @param[in] column_to_remove  index of a column
     *
     * Based on
     * http://stackoverflow.com/questions/13290395/how-to-remove-a-certain-row-or-column-while-using-eigen-library-c
     */
    template<class t_Derived>
        void  EIGENUT_VISIBILITY_ATTRIBUTE
            removeColumn(   Eigen::PlainObjectBase<t_Derived> & matrix,
                            const std::ptrdiff_t column_to_remove)
    {
        EIGENUT_ASSERT(column_to_remove <= matrix.rows(), "The index of a removed column is greater than the size of the matrix.");

        std::ptrdiff_t number_of_rows = matrix.rows();
        std::ptrdiff_t number_of_cols = matrix.cols()-1;

        if( column_to_remove < number_of_cols )
        {
            matrix.block(0, column_to_remove, number_of_rows, number_of_cols - column_to_remove) =
                matrix.block(0, column_to_remove + 1 ,number_of_rows, number_of_cols - column_to_remove);
        }

        matrix.conservativeResize(number_of_rows, number_of_cols);
    }


    /**
     * @brief Custom Kronecker product: blocks of the input matrix are treated as
     * single elements.
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] input input matrix
     * @param[in] input_block_rows  rows in a block
     * @param[in] input_block_cols  cols in a block
     * @param[in] identity_size     size of the identity matrix used in the product
     *
     * @return result of the product
     */
    template<class t_Derived>
        EIGENUT_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_Derived>::Scalar)
        EIGENUT_VISIBILITY_ATTRIBUTE
            multiplyBlockKroneckerIdentity (const Eigen::DenseBase<t_Derived> & input,
                                            const std::size_t                   input_block_rows,
                                            const std::size_t                   input_block_cols,
                                            const std::size_t                   identity_size)
    {
        EIGENUT_ASSERT(input.rows() % input_block_rows == 0, "Wrong block size.");
        EIGENUT_ASSERT(input.cols() % input_block_cols == 0, "Wrong block size.");

        EIGENUT_DYNAMIC_MATRIX(typename Eigen::DenseBase<t_Derived>::Scalar)  output;

        std::size_t     num_blocks_vert = input.rows() / input_block_rows;
        std::size_t     num_blocks_hor  = input.cols() / input_block_cols;

        std::size_t     output_block_rows = identity_size*input_block_rows;
        std::size_t     output_block_cols = identity_size*input_block_cols;

        output.setZero(identity_size * input.rows(),
                       identity_size * input.cols());

        for (std::size_t i = 0; i < num_blocks_vert; ++i)
        {
            for (std::size_t j = 0; j < num_blocks_hor; ++j)
            {
                for(std::size_t k = 0; k < identity_size; ++k)
                {
                    output.block(   i*output_block_rows + k*input_block_rows,
                                    j*output_block_cols + k*input_block_cols,
                                    input_block_rows,
                                    input_block_cols)
                        = input.block(i*input_block_rows, j*input_block_cols, input_block_rows, input_block_cols);
                }
            }
        }

        return(output);
    }


    // ===========================================================================
    // Selections
    // ===========================================================================


    /**
     * @brief Select rows from a matrix, in Matlab notation the result is
     * M(first:step:end, :).
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] matrix    input matrix
     * @param[in] row_step  each 'row_step' is selected from the input matrix
     * @param[in] first_row starting from 'first_row'
     *
     * @return Matrix consisting of selected rows.
     */
    template<class t_Derived>
        inline Eigen::Map<  const EIGENUT_DYNAMIC_MATRIX(typename Eigen::PlainObjectBase<t_Derived>::Scalar),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
        EIGENUT_VISIBILITY_ATTRIBUTE
            selectRows( const Eigen::PlainObjectBase<t_Derived> &matrix,
                        const std::size_t row_step,
                        const std::size_t first_row = 0)
    {
        return (EIGENUT_DYNAMIC_MATRIX(typename Eigen::PlainObjectBase<t_Derived>::Scalar)::Map(
                    matrix.data() + first_row,
                    ceil( static_cast<double> (matrix.rows() - first_row)/row_step),
                    matrix.cols(),
                    Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(matrix.rows(), row_step)));
    }


    /**
     * @brief Selection matrix
     */
    class EIGENUT_VISIBILITY_ATTRIBUTE SelectionMatrix
    {
        template<class t_Derived>
        friend Eigen::Map<  const EIGENUT_DYNAMIC_MATRIX( typename Eigen::PlainObjectBase<t_Derived>::Scalar ),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
            operator*(   const SelectionMatrix &,
                         const Eigen::PlainObjectBase<t_Derived> &);


        private:
            std::size_t step_size_;
            std::size_t first_index_;

        public:
            /**
             * @brief Constructore
             *
             * @param[in] step_size     step size of selection: 3 -- each third element
             * @param[in] first_index   index of the first element for selection
             */
            SelectionMatrix( const std::size_t step_size,
                             const std::size_t first_index)
            {
                step_size_   = step_size;
                first_index_ = first_index;
            }
    };


    /**
     * @brief Multiply selection matrix by a generic Eigen matrix (select rows)
     *
     * @tparam t_Derived  Eigen parameter
     *
     * @param[in] selector
     * @param[in] matrix
     *
     * @return selected rows
     */
    template<class t_Derived>
        inline Eigen::Map<  const EIGENUT_DYNAMIC_MATRIX( typename Eigen::PlainObjectBase<t_Derived>::Scalar ),
                            Eigen::Unaligned,
                            Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> >
        EIGENUT_VISIBILITY_ATTRIBUTE
            operator*(  const SelectionMatrix                   & selector,
                        const Eigen::PlainObjectBase<t_Derived> & matrix)
    {
        return(selectRows(matrix, selector.step_size_, selector.first_index_));
    }


    #ifdef EIGENUT_ENABLE_EIGENTYPE_DETECTION
        #include <boost/utility/enable_if.hpp>
        #include <boost/mpl/has_xxx.hpp>
        #include <boost/mpl/and.hpp>

        namespace detail
        {
            BOOST_MPL_HAS_XXX_TRAIT_DEF(Scalar)
            BOOST_MPL_HAS_XXX_TRAIT_DEF(Index)
            BOOST_MPL_HAS_XXX_TRAIT_DEF(StorageKind)
        }


        /**
         * Traits for checking if T is indeed an Eigen Type
         * @tparam T any Type
         *
         * Example Usage:
         * is_eigen_type<int>::value // evaluates to false
         * is_eigen_type<int>::type // evaluates to false_type
         * is_eigen_type<Eigen::Vector2d>::value // evaluates to true
         * is_eigen_type<Eigen::Vector2d>::type // true_type
         *
         * @see https://forum.kde.org/viewtopic.php?f=74&t=121280
         */
        template<typename T>
            struct is_eigen_type : boost::mpl::and_<detail::has_Scalar<T>,
                                                    detail::has_Index<T>,
                                                    detail::has_StorageKind<T> >
        {
        };

        #define EIGENUT_EIGENTYPE_ENABLER_TYPE(DataType)  \
            const typename boost::enable_if_c< eigenut::is_eigen_type<DataType>::value >::type
        #define EIGENUT_EIGENTYPE_DISABLER_TYPE(DataType) \
            const typename boost::enable_if_c< !(eigenut::is_eigen_type<DataType>::value) >::type
    #endif
} // eigenut

#endif
