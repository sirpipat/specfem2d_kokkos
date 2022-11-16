#include "../include/kokkos_abstractions.h"
#include "../include/material.h"
#include "../include/mesh.h"
#include "../include/params.h"
#include "../include/read_mesh_database.h"
#include "../include/specfem_mpi.h"
#include "yaml-cpp/yaml.h"
#include <Kokkos_Core.hpp>
#include <stdexcept>
#include <string>
#include <vector>
// Specfem2d driver

//-----------------------------------------------------------------
// config parser routines
struct config {
  std::string database_filename;
};

void operator>>(YAML::Node &Node, config &config) {
  config.database_filename = Node["database_file"].as<std::string>();
}

config get_node_config(std::string config_file, specfem::MPI *mpi) {
  // read specfem config file
  config config{};
  YAML::Node yaml = YAML::LoadFile(config_file);
  YAML::Node Node = yaml["databases"];
  assert(Node.IsSequence());
  if (Node.size() != mpi->get_size()) {
    std::ostringstream message;
    message << "Specfem configuration file generated with " << Node.size()
            << " number of processors. Current run is with nproc = "
            << mpi->get_size()
            << " Please run the code with nprocs = " << Node.size();
    throw std::runtime_error(message.str());
  }
  for (auto N : Node) {
    if (N["processor"].as<int>() == mpi->get_rank()) {
      N >> config;
      return config;
    }
  }

  throw std::runtime_error("Could not process yaml file");

  // Dummy return type. Should never reach here.
  return config;
}
//-----------------------------------------------------------------

int main(int argc, char **argv) {

  // Initialize MPI
  specfem::MPI *mpi = new specfem::MPI(&argc, &argv);
  // Initialize Kokkos
  Kokkos::initialize(argc, argv);
  {

    std::string config_file = "../DATA/specfem_config.yaml";

    config config = get_node_config(config_file, mpi);

    specfem::mesh mesh{};
    std::vector<specfem::material> materials;
    specfem::parameters params;

    mpi->cout("\n\n\n================Reading database "
              "file=====================\n\n\n");

    try {
      IO::read_mesh_database(config.database_filename, mesh, params, materials,
                             mpi);
    } catch (std::runtime_error &e) {
      std::cout << e.what() << std::endl;
      mpi->exit();
    }

    mpi->cout("\n\n\n================Done Reading Database "
              "file=====================\n\n\n");
  }

  // Finalize Kokkos
  Kokkos::finalize();
  // Finalize MPI
  delete mpi;

  return 0;
}
