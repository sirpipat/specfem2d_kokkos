#ifndef _SPECFEM_KERNELS_IMPL_INTERFACE_KERNELS_HPP
#define _SPECFEM_KERNELS_IMPL_INTERFACE_KERNELS_HPP

#include "compute/interface.hpp"
#include "coupled_interface/interface.hpp"
#include "enumerations/dimension.hpp"
#include "enumerations/medium.hpp"
#include "enumerations/simulation.hpp"

namespace specfem {
namespace kernels {
namespace impl {
template <specfem::wavefield::type WavefieldType,
          specfem::dimension::type DimensionType,
          specfem::element::medium_tag MediumTag>
class interface_kernels;

template <specfem::wavefield::type WavefieldType,
          specfem::dimension::type DimensionType>
class interface_kernels<WavefieldType, DimensionType,
                        specfem::element::medium_tag::elastic> {
public:
  interface_kernels(const specfem::compute::assembly &assembly)
      : elastic_acoustic_interface(assembly) {}

  inline void compute_coupling() {
    elastic_acoustic_interface.compute_coupling();
  }

private:
  specfem::coupled_interface::coupled_interface<
      WavefieldType, DimensionType, specfem::element::medium_tag::elastic,
      specfem::element::medium_tag::acoustic>
      elastic_acoustic_interface;
};

template <specfem::wavefield::type WavefieldType,
          specfem::dimension::type DimensionType>
class interface_kernels<WavefieldType, DimensionType,
                        specfem::element::medium_tag::acoustic> {
public:
  interface_kernels(const specfem::compute::assembly &assembly)
      : acoustic_elastic_interface(assembly) {}

  inline void compute_coupling() {
    acoustic_elastic_interface.compute_coupling();
  }

private:
  specfem::coupled_interface::coupled_interface<
      WavefieldType, DimensionType, specfem::element::medium_tag::acoustic,
      specfem::element::medium_tag::elastic>
      acoustic_elastic_interface;
};

} // namespace impl
} // namespace kernels
} // namespace specfem

#endif /* _SPECFEM_KERNELS_IMPL_INTERFACE_KERNELS_HPP */
