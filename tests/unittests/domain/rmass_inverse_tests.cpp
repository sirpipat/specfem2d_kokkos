#include "../../../include/compute.h"
#include "../../../include/domain.h"
#include "../../../include/material.h"
#include "../../../include/mesh.h"
#include "../../../include/parameter_parser.h"
#include "../../../include/quadrature.h"
#include "../../../include/read_sources.h"
#include "../../../include/utils.h"
#include "../Kokkos_Environment.hpp"
#include "../MPI_environment.hpp"
#include "../utilities/include/compare_array.h"
#include "yaml-cpp/yaml.h"

// ----- Parse test config ------------- //

struct test_config {
  std::string specfem_config, solutions_file, sources_file, database_file;
};

void operator>>(const YAML::Node &Node, test_config &test_config) {
  test_config.specfem_config = Node["specfem_config"].as<std::string>();
  test_config.solutions_file = Node["solutions_file"].as<std::string>();
  test_config.sources_file = Node["sources_file"].as<std::string>();
  test_config.database_file = Node["database_file"].as<std::string>();

  return;
}

test_config parse_test_config(std::string test_configuration_file,
                              specfem::MPI::MPI *mpi) {

  YAML::Node yaml = YAML::LoadFile(test_configuration_file);
  const YAML::Node &tests = yaml["Tests"];
  const YAML::Node &serial = tests["serial"];

  test_config test_config;
  if (mpi->get_size() == 1) {
    serial >> test_config;
  }

  return test_config;
}

// ------------------------------------- //

TEST(DOMAIN_TESTS, rmass_inverse_elastic_test) {
  std::string config_filename =
      "../../../tests/unittests/domain/test_config.yaml";

  specfem::MPI::MPI *mpi = MPIEnvironment::mpi_;

  test_config test_config = parse_test_config(config_filename, mpi);

  const std::string parameter_file = test_config.specfem_config;
  const std::string sources_file = test_config.sources_file;
  const std::string database_file = test_config.database_file;

  specfem::runtime_configuration::setup setup(parameter_file);

  // Set up GLL quadrature points
  auto [gllx, gllz] = setup.instantiate_quadrature();

  // Read mesh generated MESHFEM
  std::vector<specfem::material *> materials;
  specfem::mesh mesh(database_file, materials, mpi);

  // Read sources
  //    if start time is not explicitly specified then t0 is determined using
  //    source frequencies and time shift
  auto [sources, t0] = specfem::read_sources(sources_file, mpi);

  // Generate compute structs to be used by the solver
  specfem::compute::compute compute(mesh.coorg, mesh.material_ind.knods, gllx,
                                    gllz);
  specfem::compute::partial_derivatives partial_derivatives(
      mesh.coorg, mesh.material_ind.knods, gllx, gllz);
  specfem::compute::properties material_properties(mesh.material_ind.kmato,
                                                   materials, mesh.nspec,
                                                   gllx.get_N(), gllz.get_N());

  // Locate the sources
  for (auto &source : sources)
    source->locate(compute.ibool, compute.coordinates.coord, gllx.get_hxi(),
                   gllz.get_hxi(), mesh.nproc, mesh.coorg,
                   mesh.material_ind.knods, mesh.npgeo,
                   material_properties.ispec_type, mpi);

  // User output
  for (auto &source : sources) {
    if (mpi->main_proc())
      std::cout << *source << std::endl;
  }

  // Update solver intialization time
  setup.update_t0(t0);

  // Update solver intialization time
  setup.update_t0(t0);

  // Instantiate the solver and timescheme
  auto it = setup.instantiate_solver();

  // User output
  if (mpi->main_proc())
    std::cout << *it << std::endl;

  // Setup solver compute struct
  specfem::compute::sources compute_sources(sources, gllx, gllz, mpi);

  // Instantiate domain classes
  const int nglob = specfem::utilities::compute_nglob(compute.ibool);

  specfem::Domain::Domain *domains = new specfem::Domain::Elastic(
      ndim, nglob, &compute, &material_properties, &partial_derivatives,
      &compute_sources, &gllx, &gllz);

  EXPECT_NO_THROW(specfem::testing::test_array(
      domains->get_rmass_inverse(), test_config.solutions_file, nglob, ndim));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new MPIEnvironment);
  ::testing::AddGlobalTestEnvironment(new KokkosEnvironment);
  return RUN_ALL_TESTS();
}