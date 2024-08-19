#pragma once

#include "compute/assembly/assembly.hpp"
#include "enumerations/dimension.hpp"
#include "enumerations/medium.hpp"

namespace specfem {
namespace coupled_interface {

namespace impl {

template <specfem::dimension::type DimensionType,
          specfem::element::medium_tag SelfMedium,
          specfem::element::medium_tag CoupledMedium>
class coupled_interface;

template <specfem::dimension::type DimensionType>
class coupled_interface<DimensionType, specfem::element::medium_tag::acoustic,
                        specfem::element::medium_tag::elastic> {
public:
  using CoupledPointFieldType =
      specfem::point::field<DimensionType,
                            specfem::element::medium_tag::elastic, true, false,
                            false, false, false>;
  using SelfPointFieldType =
      specfem::point::field<DimensionType,
                            specfem::element::medium_tag::acoustic, false,
                            false, true, false, false>;
};

template <specfem::dimension::type DimensionType>
class coupled_interface<DimensionType, specfem::element::medium_tag::elastic,
                        specfem::element::medium_tag::acoustic> {
public:
  using CoupledPointFieldType =
      specfem::point::field<DimensionType,
                            specfem::element::medium_tag::acoustic, false,
                            false, true, false, false>;
  using SelfPointFieldType =
      specfem::point::field<DimensionType,
                            specfem::element::medium_tag::elastic, false, false,
                            true, false, false>;
};

} // namespace impl

template <specfem::wavefield::type WavefieldType,
          specfem::dimension::type DimensionType,
          specfem::element::medium_tag SelfMedium,
          specfem::element::medium_tag CoupledMedium>
class coupled_interface {
public:
  using CoupledPointFieldType =
      typename impl::coupled_interface<DimensionType, SelfMedium,
                                       CoupledMedium>::CoupledPointFieldType;

  using SelfPointFieldType =
      typename impl::coupled_interface<DimensionType, SelfMedium,
                                       CoupledMedium>::SelfPointFieldType;

  static_assert(SelfMedium != CoupledMedium,
                "Error: self_medium cannot be equal to coupled_medium");

  static_assert(((SelfMedium == specfem::element::medium_tag::acoustic &&
                  CoupledMedium == specfem::element::medium_tag::elastic) ||
                 (SelfMedium == specfem::element::medium_tag::elastic &&
                  CoupledMedium == specfem::element::medium_tag::acoustic)),
                "Only acoustic-elastic coupling is supported at the moment.");

  coupled_interface(const specfem::compute::assembly &assembly);

  void compute_coupling();

private:
  int nedges;  ///< Number of edges in the interface.
  int npoints; ///< Number of quadrature points in the interface.
  specfem::compute::interface_container<SelfMedium, CoupledMedium>
      interface_data; ///< Struct containing the coupling information.
  specfem::compute::simulation_field<WavefieldType> field;
};
} // namespace coupled_interface
} // namespace specfem

// #ifndef _COUPLED_INTERFACE_HPP_
// #define _COUPLED_INTERFACE_HPP_

// #include "compute/interface.hpp"
// #include "enumerations/interface.hpp"
// #include "impl/edge/interface.hpp"
// #include "kokkos_abstractions.h"
// #include "specfem_setup.hpp"

// namespace specfem {
// namespace coupled_interface {
// /**
//  * @brief Class to compute the coupling between two domains.
//  *
//  * @tparam self_domain_type Primary domain of the interface.
//  * @tparam coupled_domain_type Coupled domain of the interface.
//  */
// template <specfem::wavefield::type WavefieldType,
//           specfem::dimension::type DimensionType,
//           specfem::element::medium_tag SelfMedium,
//           specfem::element::medium_tag CoupledMedium>
// class coupled_interface {
// public:
//   static_assert(SelfMedium != CoupledMedium,
//                 "Error: self_medium cannot be equal to coupled_medium");

//   static_assert(((SelfMedium == specfem::element::medium_tag::acoustic &&
//                   CoupledMedium == specfem::element::medium_tag::elastic) ||
//                  (SelfMedium == specfem::element::medium_tag::elastic &&
//                   CoupledMedium == specfem::element::medium_tag::acoustic)),
//                 "Only acoustic-elastic coupling is supported at the
//                 moment.");

//   coupled_interface(const specfem::compute::assembly &assembly);

//   void compute_coupling();

// private:
//   using EdgeType =
//       specfem::coupled_interface::impl::edges::edge<DimensionType,
//       SelfMedium,
//                                                     CoupledMedium>;
//   using CoupledPointFieldType = typename EdgeType::CoupledPointFieldType;
//   using SelfPointFieldType = typename EdgeType::SelfPointFieldType;

//   int nedges; ///< Number of edges in the interface.
//   specfem::compute::points points;
//   specfem::compute::quadrature quadrature;
//   specfem::compute::partial_derivatives partial_derivatives;
//   specfem::compute::simulation_field<WavefieldType> field;
//   specfem::compute::interface_container<SelfMedium, CoupledMedium>
//       interface_data; ///< Struct containing the coupling information.
//   EdgeType edge;      ///< Edge class to implement coupling physics
// };
// } // namespace coupled_interface
// } // namespace specfem
// #endif // _COUPLED_INTERFACES_HPP_
