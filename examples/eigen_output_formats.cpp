/**
    @file
    @author  Alexander Sherikov

    @brief Eigen supports custom output formats of matrices, but the methods
    for application of the formats are not very convenient. Ideally, I would
    like to have something like
        'stream << format << matrix1 << matrix2'
    instead of
        'stream << matrix1.format() << matrix2.format()'.
    This can be easily implemented using locale facets as demonstrated below.
    The problem of this approach lies in the necessity to define a static id of
    the facet class, therefore it is unlikely to be accepted in Eigen. At the
    same time I cannot use this approach in my code, since I have to define
    'operator<<', which is already defined in Eigen (src/Core/IO.h). The code
    presented below is compilable only if definition of '<<' is removed from
    IO.h header.
*/



#include "Eigen/Core"
#include <locale>
#include <iostream>
#include <limits>
#include <iomanip>

namespace Eigen
{
    class OutputFormatFacet : public std::locale::facet
    {
        private:
            IOFormat         output_format_;


        public:
            static std::locale::id  id;


            OutputFormatFacet(  const IOFormat  & output_format,
                                std::size_t     refs = 0)    :     std::locale::facet(refs)
            {
                output_format_ = output_format;
            }


            const IOFormat & getFormat() const
            {
                return (output_format_);
            }
    };


    std::locale::id     OutputFormatFacet::id;


    std::ostream & operator<< (std::ostream & s, const IOFormat& output_format)
    {
        std::locale L (std::cout.getloc(),
                       new OutputFormatFacet(output_format));
        s.imbue(L);
        return (s);
    }


    template<typename Derived> std::ostream & operator<< (std::ostream & s, const DenseBase<Derived> & m)
    {
        if ( std::has_facet<OutputFormatFacet>(s.getloc()) )
        {
            return internal::print_matrix(s, m.eval(), std::use_facet<OutputFormatFacet>(s.getloc()).getFormat());
        }
        else
        {
            return internal::print_matrix(s, m.eval(), EIGEN_DEFAULT_IO_FORMAT);
        }
    }
}


int main()
{
    Eigen::IOFormat format1(Eigen::FullPrecision, 0, ", ", ";\n", "", "", "[", "];\n");
    Eigen::IOFormat format2(5, 0, ", ", "\n", "|", "|");

    Eigen::MatrixXd matrix = Eigen::MatrixXd::Random(2,4);

    std::cout << "============================" << std::endl;
    std::cout << "Default format"               << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << matrix << std::endl << std::endl;


    std::cout << "============================" << std::endl;
    std::cout << "Matlab format"                << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << format1 << matrix << std::endl << std::endl;


    std::cout << "============================" << std::endl;
    std::cout << "Still Matlab format"          << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << matrix << std::endl << std::endl;


    std::cout << "============================" << std::endl;
    std::cout << "Some other format"           << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << format2;
    std::cout << matrix << std::endl << std::endl;


    std::cout << "============================" << std::endl;
    std::cout << "Back to the default format"   << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << EIGEN_DEFAULT_IO_FORMAT;
    std::cout << matrix << std::endl << std::endl;

    return (0);
}
