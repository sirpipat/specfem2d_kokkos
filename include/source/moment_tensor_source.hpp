#ifndef _MOMENT_TENSOR_SOURCE_HPP
#define _MOMENT_TENSOR_SOURCE_HPP

#include "compute/compute_mesh.hpp"
#include "compute/compute_partial_derivatives.hpp"
#include "compute/properties/properties.hpp"
#include "constants.hpp"
#include "enumerations/specfem_enums.hpp"
#include "kokkos_abstractions.h"
#include "quadrature/interface.hpp"
#include "source.hpp"
#include "source_time_function/interface.hpp"
#include "specfem_mpi/interface.hpp"
#include "specfem_setup.hpp"
#include "utilities/interface.hpp"
#include "yaml-cpp/yaml.h"
#include <Kokkos_Core.hpp>

namespace specfem {
namespace sources {
/**
 * @brief Moment-tensor source
 *
 */
class moment_tensor : public source {

public:
  /**
   * @brief Default source constructor
   *
   */
  moment_tensor(){};
  /**
   * @brief Construct a new moment tensor force object
   *
   * @param moment_tensor a moment_tensor data holder read from source file
   * written in .yml format
   */
  moment_tensor(YAML::Node &Node, const type_real dt)
      : Mxx(Node["Mxx"].as<type_real>()), Mzz(Node["Mzz"].as<type_real>()),
        Mxz(Node["Mxz"].as<type_real>()), specfem::sources::source(Node, dt){};
  /**
   * @brief User output
   *
   */
  std::string print() const override;

  void compute_source_array(
      const specfem::compute::mesh &mesh,
      const specfem::compute::partial_derivatives &partial_derivatives,
      const specfem::compute::properties &properties,
      specfem::kokkos::HostView3d<type_real> source_array) override;

private:
  type_real Mxx; ///< Mxx for the source
  type_real Mxz; ///< Mxz for the source
  type_real Mzz; ///< Mzz for the source
};
} // namespace sources
} // namespace specfem

#endif
