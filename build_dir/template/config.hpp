#pragma once

#include <boost/filesystem/path.hpp>

namespace config
{
    static boost::filesystem::path const installation("/home/atraore/gpi/try_gpi/gspc-gromovwitten/install_dir");
    static boost::filesystem::path const sing_path("/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-4.4.0p2-k7rgdkzo5prqsvxjckejvcdvxgjr64bk");
    static boost::filesystem::path const flint_path("/usr/local");

    inline boost::filesystem::path const& singularLibrary()
    {
        static boost::filesystem::path const library
        (sing_path / "lib" / "libSingular.so"
        );

        return library;
    }

    inline boost::filesystem::path const& flintLibrary()
    {
        static boost::filesystem::path const library
        (flint_path / "lib" / "libflint.so"
        );

        return library;
    }

    inline boost::filesystem::path const& implementation()
    {
        static boost::filesystem::path const library(
            installation / "libexec" / "workflow" / "libtemplate_implementation.so"
        );
        return library;
    }

}
