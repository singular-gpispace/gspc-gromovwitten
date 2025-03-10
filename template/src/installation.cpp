#include <installation.hpp>
#include <util-generic/executable_path.hpp>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <cstdlib> // for std::getenv

#ifndef SP_INSTALL_PATH
#define SP_INSTALL_PATH "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/install_dir"
#endif

namespace singular_template
{
  const std::filesystem::path SP_INSTALL_PATH_OBJ(SP_INSTALL_PATH);

  namespace
  {
    void check(const std::filesystem::path& path, bool condition, const std::string& message)
    {
      if (!condition)
      {
        throw std::logic_error(path.string() + " " + message + ": Installation incomplete!?");
      }
    }

    void check_is_directory(const std::filesystem::path& path)
    {
      check(path, std::filesystem::is_directory(path), "is not a directory");
    }

    void check_is_file(const std::filesystem::path& path)
    {
      check(path, std::filesystem::exists(path), "does not exist");
      check(path, std::filesystem::is_regular_file(path), "is not a regular file");
    } 

    std::filesystem::path gspc_home(const std::filesystem::path& gspc_path)
    {
      return gspc_path;
    }

    std::filesystem::path workflow_path(const std::filesystem::path& installation_path)
    {
      return installation_path / "libexec" / "workflow";
    }

    std::filesystem::path workflow_all_file(const std::filesystem::path& installation_path)
    {
      return workflow_path(installation_path) / "template.pnet";
    }
  }

  // Default constructor using the defined SP_INSTALL_PATH.
  installation::installation()
    : installation(SP_INSTALL_PATH_OBJ)
  {
  }

  // Constructor that reads the GSPC_HOME environment variable.
  installation::installation(const std::filesystem::path& ip)
    : _path(ip)
  {
    const char* gspc_env = std::getenv("GSPC_HOME");
    if (!gspc_env)
    {
      throw std::runtime_error("GSPC_HOME environment variable is not set!");
    }
    _gspc_path = std::filesystem::path(gspc_env);

    std::cout << "Using GSPC_HOME (inside installation.cpp): " << _gspc_path << std::endl;
    
    if (!std::filesystem::exists(_gspc_path / "gspc_version")) {
      std::cerr << "ERROR: GSPC version file not found at " << (_gspc_path / "gspc_version") << std::endl;
      throw std::runtime_error("GSPC version file missing!");
    } 

    check_is_directory(_gspc_path);
  }

  // Constructor that accepts both installation and GSPC paths.
  installation::installation(const std::filesystem::path& ip, const std::filesystem::path& gp)
    : _path(ip), _gspc_path(gp)
  {
    std::cout << "Using GSPC_HOME (inside installation.cpp): " << _gspc_path << std::endl;
    std::cout << "Using path ip (inside installation.cpp): " << _path << std::endl;

    if (!std::filesystem::exists(_gspc_path / "gspc_version"))
    {
      std::cerr << "ERROR: GSPC version file not found at " << (_gspc_path / "gspc_version") << std::endl;
    }

    check_is_directory(gspc_home(_gspc_path));
    check_is_directory(workflow_path(_path));
    check_is_file(workflow_all());
  } 

  std::filesystem::path installation::workflow_all() const
  {
    return workflow_all_file(_path);
  }

  std::filesystem::path installation::workflow_dir() const
  {
    return workflow_path(_path);
  }

  gspc::installation installation::gspc_installation(boost::program_options::variables_map& vm) const
  {
    gspc::set_gspc_home(vm, gspc_home(_gspc_path));
    gspc::set_application_search_path(vm, workflow_path(_path));

    return gspc::installation(vm);
  }
}
