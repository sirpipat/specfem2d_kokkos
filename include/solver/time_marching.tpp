#ifndef _TIME_MARCHING_TPP
#define _TIME_MARCHING_TPP

#include "domain/interface.hpp"
#include "solver.hpp"
#include "time_marching.hpp"
#include "timescheme/interface.hpp"
#include <Kokkos_Core.hpp>

template <typename qp_type>
void specfem::solver::time_marching<qp_type>::run() {

  auto *it = this->it;
  auto acoustic_domain = this->acoustic_domain;
  auto elastic_domain = this->elastic_domain;

  const int nstep = it->get_max_timestep();

  auto acoustic_field = acoustic_domain.get_field();
  auto acoustic_field_dot = acoustic_domain.get_field_dot();
  auto acoustic_field_dot_dot = acoustic_domain.get_field_dot_dot();

  auto elastic_field = elastic_domain.get_field();
  auto elastic_field_dot = elastic_domain.get_field_dot();
  auto elastic_field_dot_dot = elastic_domain.get_field_dot_dot();

  while (it->status()) {
    int istep = it->get_timestep();

    type_real timeval = it->get_time();

    Kokkos::Profiling::pushRegion("Stiffness calculation");
    it->apply_predictor_phase(acoustic_field, acoustic_field_dot, acoustic_field_dot_dot);

    acoustic_domain.compute_stiffness_interaction();
    acoustic_elastic_interface.compute_coupling();
    acoustic_domain.compute_source_interaction(timeval);
    acoustic_domain.divide_mass_matrix();

    it->apply_corrector_phase(acoustic_field, acoustic_field_dot, acoustic_field_dot_dot);

    it->apply_predictor_phase(elastic_field, elastic_field_dot, elastic_field_dot_dot);

    elastic_domain.compute_stiffness_interaction();
    elastic_acoustic_interface.compute_coupling();
    elastic_domain.compute_source_interaction(timeval);
    elastic_domain.divide_mass_matrix();

    it->apply_corrector_phase(elastic_field, elastic_field_dot, elastic_field_dot_dot);

    if (it->compute_seismogram()) {
      int isig_step = it->get_seismogram_step();
      acoustic_domain.compute_seismogram(isig_step);
      elastic_domain.compute_seismogram(isig_step);
      it->increment_seismogram_step();
    }
    Kokkos::Profiling::popRegion();

    if (istep % 10 == 0) {
      std::cout << "Progress : executed " << istep << " steps of " << nstep
                << " steps" << std::endl;
    }

    it->increment_time();
  }

  std::cout << std::endl;

  return;
}

#endif
