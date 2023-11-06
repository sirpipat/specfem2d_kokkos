#ifndef _ENUMS_BOUNDARY_CONDITIONS_NONE_HPP_
#define _ENUMS_BOUNDARY_CONDITIONS_NONE_HPP_

#include "compute/interface.hpp"
#include "enumerations/dimension.hpp"
#include "enumerations/quadrature.hpp"
#include "enumerations/specfem_enums.hpp"
#include <Kokkos_Core.hpp>

namespace specfem {
namespace enums {
namespace boundary_conditions {

template <typename dim, typename medium, typename qp_type> class none {

public:
  /**
   * @name Typedefs
   *
   */
  ///@{
  /**
   * @brief Medium type of the boundary.
   *
   */
  using medium_type = medium;
  /**
   * @brief Dimension of the boundary.
   *
   */
  using dimension = dim;
  /**
   * @brief Quadrature points object to define the quadrature points either at
   * compile time or run time.
   *
   */
  using quadrature_points_type = qp_type;
  ///@}

  constexpr static specfem::enums::element::boundary_tag value =
      specfem::enums::element::boundary_tag::none; ///< boundary tag

  none(){};

  none(const specfem::compute::boundaries &boundary_conditions,
       const quadrature_points_type &quadrature_points){};

  KOKKOS_INLINE_FUNCTION
  void enforce_gradient(
      const int &ielement, const int &xz,
      typename dimension::template array_type<type_real> &df_dx,
      typename dimension::template array_type<type_real> &df_dz) const {};

  KOKKOS_INLINE_FUNCTION
  void enforce_stress(
      const int &ielement, const int &xz,
      typename dimension::template array_type<type_real> &stress_integrand_xi,
      typename dimension::template array_type<type_real>
          &stress_integrand_xgamma) const {};

  KOKKOS_INLINE_FUNCTION
  void enforce_traction(const int &ielement, const int &xz,
                        typename dimension::template array_type<type_real>
                            &field_dot_dot) const {};
};

} // namespace boundary_conditions
} // namespace enums
} // namespace specfem

#endif /* _ENUMS_BOUNDARY_CONDITIONS_NONE_HPP_ */
